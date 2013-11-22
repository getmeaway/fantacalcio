<?php 

/**
 * @file fantastats.images.inc
 *
 */

function statistiche_grafici() {

  $teams = get_teams_names();
  asort($teams);
  
  $rounds = get_all_rounds_played();
  
  return "<p>Scegli una o più squadre</p>"
      . drupal_get_form("form_grafici", $teams, $rounds);
}

function form_grafici($form_state, $teams, $rounds) {
  
  if($rounds)
  $form['round_start'] = array(
            "#type" => "select", 
            "#title" => "Giornata inizio",
            '#options' => $rounds
            );

  if($rounds)
    $form['round_stop'] = array(
            "#type" => "select", 
            "#title" => "Giornata fine",
            "#default_value" => max($rounds),
            '#options' => $rounds
            );
            
  $form['type'] = array(
            "#type" => "select", 
            "#title" => "Tipo statistica",
            '#options' => array("standing" => "Classifica",
                      "goals" => "Goal fatti", 
                      "points" => "Punti fatti", ),
            );

  $form['teams'] = array(
            "#type" => "checkboxes",
            "#options" => $teams,
            "#title" => "Squadre"
            );

  $form['submit'] = array(
            "#type" => "submit",
            "#value" => "Ok",
            );
            
  return $form;
}

function form_grafici_validate($form, &$form_state) {
  if ($form_state["values"]["round_start"] == $form_state["values"]["round_stop"])
    form_set_error("", "Scegliere due giornate diverse");
    
  $n = 0;
  foreach ($form_state['values']['teams'] as $key => $value) {
    if ($value > 0) $n++;
  }
    
  if ($n == 0)  form_set_error("", "Scegliere almeno una squadra");

}

function form_grafici_submit($form, &$form_state) {
      
  $teams_ids = array();
  foreach ($form_state['values']['teams'] as $key => $value) {
    if ($value > 0) array_push($teams_ids, $key);
  }
  $team_list = implode("-", $teams_ids);
  
  $type = $form_state["values"]["type"];
  $round_start = $form_state["values"]["round_start"];
  $round_stop = $form_state["values"]["round_stop"];
  
  drupal_goto("statistiche/grafici/" . $team_list . "/" . $type . "/" . $round_start . "/" . $round_stop);
}

function statistiche_crea_grafico($teams_list = '', $type = "", $round_start = 1, $round_stop = 1) {
  
  $out = "";
  
  $teams = get_teams();
  
  if ($teams) {
    $out .= "<ul>";
    foreach (explode("-", $teams_list) as $t_id) {
      $out .= "<li>" . $teams[$t_id]->name . "</li>";
    }  
    $out .= "</ul>";
  }
  
  $type_text = get_type_text($type);

  //data from DB
  $data = chart_get_data($teams_list, $type, $round_start, $round_stop);
  
  $out .= "<p>" . $type_text . " (Giornate: $round_start - $round_stop)</p>";

  $js = "
      google.load(\"visualization\", \"1\", {packages:[\"corechart\"], callback:drawChart});
 
      function drawChart() {

        // Create the data table.
        var data = new google.visualization.DataTable();
        data.addColumn('string', 'Giornate');";

  foreach(explode("-", $teams_list) as $t_id) {
    $js .= "\ndata.addColumn('number', '" . $teams[$t_id]->name . "');";
  }

  for($i = $round_start; $i <= $round_stop; $i++) {
    $js .= "\ndata.addRow(['" . $i . "'";
    foreach($data as $t_id => $team_data) {
      $js .= ", " . $team_data[$i];
    }
    $js .= "]);";
  }

  $js .= "
        // Set chart options
        var options = {
                       'width':'100%',
                       'height':300,
                       'background': 'transparent' 
                      };

        // Instantiate and draw our chart, passing in some options.
        var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
        chart.draw(data, options);
      }
  ";

  drupal_add_js(drupal_get_path("module", "fantacalcio") . "/js/jsapi.js");
  drupal_add_js($js, 'inline');

  $out .= "<div id='chart_div'></div>";
  
  return $out;
}

function get_type_text($type) {
  switch ($type) {
    case "standing": 
      return "Classifica";
    case "goals": 
      return "Goal fatti";
    case "points": 
      return "Punti fatti";  
  }
}

function chart_get_data($teams, $type, $round_start, $round_stop) {
  
  $summing_types = array("standing");
  
  $teams_list = explode("-", $teams);

  foreach ($teams_list as $t_id) {
    if ($t_id) {
      $sum = 0;
      $sql = chart_create_query($t_id, $type, $round_start, $round_stop, variable_get("fantacalcio_main_competition", 1));
      $result = db_query($sql);
      while ($row = db_fetch_array($result)) {
        if (in_array($type, $summing_types)) {
          $value = 0;
          if ($row['field'] == $t_id) $value = 3;
          elseif ($row['field'] == -1)$value = 1;
          else $value = 0;

          $data[$t_id][$row['round']] = $sum + $value;
          
          $sum = $data[$t_id][$row['round']];
        }
        else {
          $data[$t_id][$row['round']] = $row['field'];
        }
      }
    } 
  }
  
  if (in_array($type, $summing_types)) {
    foreach ($data as $t_id => $data_t_id) {
      
      foreach ($data_t_id as $round => $data_round ) {
        if ($round < $round_start || $round > $round_stop) unset($data[$t_id][$round]);
      }
    }
  }
  return $data;
}

function chart_create_query($t_id, $type, $round_start, $round_stop, $c_id) {
  switch ($type) {
    case "standing": 
      $sql = "SELECT winner_id AS field, round FROM {fanta_matches}
          WHERE t1_id = $t_id 
          AND round <= $round_stop
          AND g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = $c_id)
          UNION 
          SELECT winner_id AS field, round FROM {fanta_matches} 
          WHERE t2_id = $t_id 
          AND round <= $round_stop
          AND g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = $c_id)          
          ORDER BY round";
      break;
    case "goals": 
      $sql = "SELECT goals_1 AS field, round FROM {fanta_matches} 
          WHERE t1_id = $t_id 
          AND round >= $round_start 
          AND round <= $round_stop
          AND g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = $c_id)
          UNION 
          SELECT goals_2 AS field, round FROM {fanta_matches} 
          WHERE t2_id = $t_id
          AND round >= $round_start 
          AND round <= $round_stop
          AND g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = $c_id)
          ORDER BY round";
      break;
    case "points": 
      $sql = "SELECT tot_1 AS field, round FROM {fanta_matches} 
          WHERE t1_id = $t_id 
          AND round >= $round_start 
          AND round <= $round_stop 
          AND g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = $c_id)
          UNION 
          SELECT tot_2 AS field, round FROM {fanta_matches} 
          WHERE t2_id = $t_id
          AND round >= $round_start 
          AND round <= $round_stop
          AND g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = $c_id)
          ORDER BY round";
      break;
  } 
  return $sql;
}

