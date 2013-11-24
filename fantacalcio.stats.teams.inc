<?php

/**
 * @file fantastats.teams.inc
 *
 */

function dettagli_squadra($t_id = '') {

  $teams = array();
  $sql = "SELECT * FROM {fanta_teams} WHERE active = 1 ORDER BY name";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $teams[$row->t_id] = $row->name;
  }

  if (!empty($t_id) && team_exists($t_id)) {
    $out .= scheda_squadra($team);
  }

  return $out;
}

function scheda_squadra($team) {

  $out = "<br><h2>" . $team->name . "</h2><br>";
  $out .= "<p><img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/shirts/" . $team->shirt . "'></p>";
  $rows[] = array("<h4>" . t("Allenatore") . "</h4>", $team->coach);
  $rows[] = array("<h4>" . t("Stadio") . "</h4>", $team->stadium);
  $rows[] = array("<h4>" . t("Trofei") . "</h4>", theme_team_history($team->history));

  return theme_table(array(), $rows);
}

function team_stats($t_id) {

  #pt max, pt min, pt medio, gol medio, pt, vinte, perse , pareggiate, striscia pos, striscia neg,
  $max_pt = 0; $min_pt = 1000; $tot_pt = 0; $tot_gol = 0; $rounds = 0;
  $sql = "SELECT * FROM {fanta_matches} " .
      "WHERE (t1_id = '%d' OR t2_id = '%d') " .
      "AND played = 1 " .
      "AND g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = '%d')";
  $result = db_query($sql, $t_id, $t_id, variable_get("fantacalcio_main_competition", 1));
  while ($row = db_fetch_array($result)) {
    if ($t_id == $row['t1_id']) {
      $pt = $row['tot_1'];
      $gol = $row['goals_1'];
    }
    if ($t_id == $row['t2_id']) {
      $pt = $row['tot_2'];
      $gol = $row['goals_2'];
    }

    $tot_pt += $pt;
    $tot_gol += $gol;

    #punteggio massimo
    if ($max_pt < $pt) {
      $max_pt = $pt;
      $round_max = $row['round'];
    }

    #punteggio minimo
    if ($min_pt > $pt) {
      $min_pt = $pt;
      $round_min = $row['round'];
    }
    $rounds++;
  }

  $position = get_team_position($t_id);

  $header = array();
  $rows[] = array("Posizione", $position["position"] . "&deg; (" . $position["point"] . " punti)");
  $rows[] = array("Distacco dal " . $position["team_gap"] . "&deg;", $position["gap"]);
  $rows[] = array("Punteggio Massimo", $max_pt . " (" . $round_max . "&ordf; giornata)");
  $rows[] = array("Punteggio Minimo", $min_pt . " (" . $round_min . "&ordf; giornata)");
  
  if ($rounds) {
    $rows[] = array("Media Punti", round($tot_pt / $rounds, 2));
    $rows[] = array("Media Gol", round($tot_gol / $rounds, 2));
  }

  $round_stop = get_last_competition_round(variable_get("fantacalcio_main_competition", 1));

  $charts_links = array(l("Classifica", "statistiche/grafici/chart/" . $t_id . "/standing/1/" . $round_stop, array("attributes" => array("class" => "thickbox", "rel" => "lightframe[" . $t_id . "|width:770;height:450]", "title" => "Classifica"))), 
                        l("Goal fatti", "statistiche/grafici/chart/" . $t_id . "/goals/1/" . $round_stop, array("attributes" => array("class" => "thickbox", "rel" => "lightframe[" . $t_id . "|width:770;height:450]", "title" => "Goal fatti"))),
                        l("Punti fatti", "statistiche/grafici/chart/" . $t_id . "/points/1/" . $round_stop, array("attributes" => array("class" => "thickbox", "rel" => "lightframe[" . $t_id . "|width:770;height:450]", "title" => "Punti fatti")))
                       );

  return theme_table($header, $rows) . "" .  theme_item_list($charts_links, NULL, "ul", array("class" => "float")) . "<div class=clear></div>";
}

function choose_t_id_form($form_state, $teams, $t_id) {

  $form['team'] = array("#type" => "fieldset");

  $form['team']['t_id'] = array(
            "#title" => "Squadra",
            "#type" => "select",
            "#options" => $teams,
            "#default_value" => $t_id
            );

  $form['team']['submit'] = array(
            "#type" => "submit",
            "#value" => t("Ok"),
            );

  return $form;

}

function choose_t_id_form_submit($form, $form_state) {

  $t_id = $form_state['values']['t_id'];

  drupal_goto("statistiche/squadra/" . $t_id);
}

function get_dettagli_squadra($t_id = '') {
  $args = array("t_id" => $t_id);
  $out .= make_columns('dettagli_squadra', 'statistiche/squadra', $args);
  return $out;
}

function get_team_position($t_id) {

  $sql = "SELECT g.g_id 
      FROM {fanta_teams_groups} t, {fanta_groups} g
      WHERE g.g_id = t.g_id
      AND t.t_id = '%d'
      AND g.c_id = '%d'";
  $g_id = db_result(db_query($sql, $t_id, variable_get("fantacalcio_main_competition", 1)));

  $sql= "SELECT * FROM {fanta_matches} WHERE g_id = '%d' AND played = 1";
  $result = db_query($sql, $g_id);
  while ($row = db_fetch_object($result)) {
    $matches[$row->m_id] = $row;
  }

  if ($matches) {
    foreach ($matches as $m_id => $match) {
      switch ($match->winner_id) {
        case $match->t1_id: 
          $points[$match->t1_id] += 3; 
          break;
        case $match->t2_id: 
          $points[$match->t2_id] += 3; 
          break;
        case -1: 
          $points[$match->t1_id] += 1; 
          $points[$match->t2_id] += 1; 
          break;
      }
    }

    $max_point = 0; $t_id_max_point = 0;
    foreach ($points as $team_id => $point) {
      if ($t_id == $team_id) $position['point'] = $point;

      if ($max_point < $point) {
        $max_point = $point;
        $t_id_max_point = $team_id;
      }
    }

    rsort($points);
    $points_teams = array_flip($points);

    if ($position["point"] == $max_point) {
      $position["gap"] = $position["point"] - $points[1];
      $position["team_gap"] = 2;
    }
    else {
      $position["gap"] = $points[0] - $position["point"];
      $position["team_gap"] = 1;
    }

    $position["position"] = $points_teams[$position["point"]] +1;

    return $position;
  }
}
