<?php
/**
* @file fantacalcio.inc
* 
*/

function fantacalcio_team_form($form_state) {

  $team = get_team_data();
  $action = ($team) ? 'update' : 'insert';
  $title = ($team) ? 'Modifica squadra - ' . $team['name'] : 'Crea squadra';
  drupal_set_title(filter_xss($title));

  $form['title'] = array(
    '#title' => check_plain($title)
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Nome squadra',
    '#default_value' => check_plain($team['name']),
    '#required' => TRUE,

  );
  
  $form['coach'] = array(
    '#type' => 'textfield',
    '#title' => 'Allenatore',
    '#default_value' => check_plain($team['coach']),

  );

  $form['stadium'] = array(
    '#type' => 'textfield',
    '#title' => 'Stadio',
    '#default_value' => check_plain($team['stadium']),

  );

  $form['shirt'] = array(
    '#type' => 'fieldset',
    '#title' => 'Divisa',
    '#ahah' => array(
      'wrapper' => 'prova',
        // default so not really needed, but included for clarity
      'method' => 'replace',
      'event' => 'change',
      'path' => 'js/divisa.js',

    ),

  );

  $form['shirt']['type'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => 'solo bordi',
      1 => 'righe verticali',
      2 => 'righe orizzontali',
      3 => 'banda diagonale'
    ),
    '#title' => 'Tipo',
    '#default_value' => check_plain(substr($team['shirt'], -9, 1)), '#ahah' => array(
    'wrapper' => 'shirt_preview',
      // default so not really needed, but included for clarity
    'method' => 'replace',
    'event' => 'change',
    'path' => 'team/js',

  ));

  $form['shirt']['color_1'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => 'Bianco',
      1 => 'Nero',
      2 => 'Rosso',
      3 => 'Blu',
      4 => 'Verde',
      5 => 'Giallo'
    ),
    '#title' => 'Colore 1',
    '#default_value' => check_plain(substr($team['shirt'], -7, 1)), '#ahah' => array(
    'wrapper' => 'shirt_preview',
      // default so not really needed, but included for clarity
    'method' => 'replace',
    'event' => 'change',
    'path' => 'team/js',

  ));

  $form['shirt']['color_2'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => 'Bianco',
      1 => 'Nero',
      2 => 'Rosso',
      3 => 'Blu',
      4 => 'Verde',
      5 => 'Giallo'
    ),
    '#title' => 'Colore 2',
    '#default_value' => check_plain(substr($team['shirt'], -5, 1)), '#ahah' => array(
    'wrapper' => 'shirt_preview',
      // default so not really needed, but included for clarity
    'method' => 'replace',
    'event' => 'change',
    'path' => 'team/js',

  ));

  $form['shirt']['preview'] = array(
    '#prefix' => "<div id='shirt_preview' style='float:left;'>",
    '#suffix' => "</div>",
    '#type' => 'markup',
    '#value' => "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/shirts/" . $team['shirt'] . "'>"
  );

  $form['t_id'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($team['t_id']),

  );

  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($action),

  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Ok'
  );

  $form['clear'] = array(
    '#type' => 'submit',
    '#value' => 'Annulla',
    '#validate' => array(
      'fantacalcio_squadra_form_clear'
    ),

  );

  return $form;
}

function fantacalcio_team_form_ahah() {
  $type = $_POST['type'];
  $color_1 = $_POST['color_1'];
  $color_2 = $_POST['color_2'];

  $output = "<img src='" . base_path() . drupal_get_path('module', 'fantacalcio') . "/images/shirts/shirt_" . $type . "_" . $color_1 . "_" . $color_2 . ".png' >";
  print drupal_to_js(array(
    'data' => $output,
    'status' => true
  ));
  exit();
}

function fantacalcio_team_manager() {
  return drupal_get_form('fantacalcio_team_form');
}

function fantacalcio_team_form_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function fantacalcio_team_form_validate($form, &$form_state) {
  $name = $form_state['values']['name'];
  if (!$name)
    form_set_error('name', 'Inserisci un nome per la squadra');
}

function fantacalcio_team_form_submit($form, &$form_state) {

  global $user;

  $team_name = $form_state['values']['name'];
  $team_coach = $form_state['values']['coach'];
  $team_stadium = $form_state['values']['stadium'];
  $team_shirt = "shirt_" . $form_state['values']['type'] . "_" . $form_state['values']['color_1'] . "_" . $form_state['values']['color_2'] . ".png";
  $t_id = $form_state['values']['t_id'];
  $action = $form_state['values']['action'];

  if ($action == 'update') {
    $sql = "UPDATE {fanta_teams} " .
    "SET name = '%s', " .
    "coach = '%s', " .
    "stadium = '%s', " .
    "shirt = '%s' " .
    "WHERE t_id = '%d'";
    $result = db_query($sql, $team_name, $team_coach, $team_stadium, $team_shirt, $t_id);
    return drupal_set_message('Squadra modificata con successo');
  }
  elseif ($action == 'insert') {
    $sql = "INSERT INTO {fanta_teams} (name, coach, stadium, shirt, active, uid) " .
    "VALUES ('%s', '%s', '%s', '%s', 1, '%d')";
    $result = db_query($sql, $team_name, $team_coach, $team_stadium, $team_shirt, $user->uid);
    return drupal_set_message('Squadra creata con successo');
  }
}

function fantacalcio_get_title_team() {

  $team = get_team_data();

  if (count($team) == 0)
    return 'Crea Squadra';
  else
    return 'Modifica Squadra';
}

/****************/
/* squadre      */
/****************/

function fantacalcio_show_teams($t_id = '') {

  if (!empty($t_id) && team_exists($t_id))
    $args = array(
      't_id' => $t_id
    );
  $out .= make_columns('show_team', 'squadre', $args);
  
  return $out;

}

function show_team($args) {
  $t_id = $args['t_id'];
  $teams = get_teams();
  
    $js_web = "$(function() {
        $('#tabs').tabs({fxFade: true});
        });";

    $js_mobile = "$(document).on('pageinit', function() {
        $('.tab-button_" . $t_id . "').click(function() { 
          $('.tab').fadeOut('fast');
          var div_id = $(this).attr('href');
          $('.tab-button').removeClass('ui-btn-active');
          var a_id = $(this);
          $(div_id).fadeIn('fast', 'swing', function(){ $(a_id).addClass('ui-btn-active');});
        });
        });";

    drupal_add_js( (is_mobile() ? $js_mobile : $js_web), 'inline', "footer");

  $tabs = array(
                array("name" => "Rosa", "function_name" => "show_rosa"),
				array("name" => "Dettagli", "function_name" => "dettagli_squadra"), 
                array("name" => "Statistiche", "function_name" => "team_stats"),
                array("name" => "Partite", "function_name" => "show_team_matches"),
               );

  drupal_set_title(check_plain("Squadre - " . $teams[$t_id]->name) );

  $out = "<h3>" . $teams[$t_id]->name . "</h3><br>";

  $out .= "<div class='shirt'><img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/shirts/" . $teams[$t_id]->shirt . "'></div>";

  $out .= "\n<div class='tabs' id='tabs' data-role=navbar>" .
      "\n<ul>";

  $i = 0;
  foreach ($tabs as $tab_id => $tab) {      
    $out .= "\n<li><a data-theme=d class='tab-button_" . $t_id . " ". ($i == 0 ? "ui-btn-active" : "") ."' href='#g_" . $t_id . "_" . $tab_id . "'><span>" . $tab['name'] . "</span></a></li>";
    $i++;
  }
  $out .= "\n</ul>";
  $out .= (is_mobile()) ? "\n</div>" : "";

  $i = 0;
  foreach ($tabs as $tab_id => $tab) {
    $out .= "\n<div class=tab style='display: " . ($i == 0 ? 'block' : 'none') . "' id='g_" . $t_id . "_" . $tab_id . "'>"
        . $tab['function_name']($t_id)
        ."\n</div>";
    $i++;
  }
  $out .= (!is_mobile()) ? "\n</div>" : "";

  return $out;
}

function show_rosa($t_id) {

  $credits = variable_get("fantacalcio_credits", 350);

  $players = get_players();

  $rose = array(); $spesa = 0;
  $sql = "SELECT * FROM {fanta_squads} WHERE t_id = '%d' ";
  $result = db_query($sql, $t_id);
  while ($row = db_fetch_array($result)) {
    $rose[] = $row;
  }

  if ($rose) {
  
    $header = array("", "Nome", "Squadra", "Costo", "");
    foreach ($rose as $rosa) {
      $roles[] = $players[$rosa['pl_id']]->role;
      $names[] = $players[$rosa['pl_id']]->name;
    }

    array_multisort($roles, SORT_ASC, $names, SORT_ASC, $rose);

    foreach ($rose as $rosa) {

    if ($rosa["in_team"] == 1) {
      
        $rows[] = array("data" => array(
                array("data" => convert_role($players[$rosa['pl_id']]->role), "class" => "role"),
                array("data" => $players[$rosa['pl_id']]->name, "class" => "name"),
                ucwords(convert_teams($players[$rosa['pl_id']]->team)),
                $rosa['cost'],
                l("<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/stats.png'>", 
                  "scheda/giocatore/" . $rosa['pl_id'], 
                  array("html" => "true", "attributes" => array("class" => "thickbox", "rel" => "lightframe", "data-role" => "button", "data-mini" => "true")))
                  ),
                "class" => "role_" . $players[$rosa['pl_id']]->role);
      }

      $recupero = ($rosa["in_team"] == -1) ? ceil($rosa["cost"] / 2) : 0;
      $spesa += $rosa["cost"] - $recupero;
    }
    
    $sql = "SELECT SUM(money) FROM {fanta_squad_changes} WHERE t1_id = '%d' AND active = 1";
    $result = db_query($sql, $t_id);
    $money_out = db_result($result);
    
    $sql = "SELECT SUM(money) FROM {fanta_squad_changes} WHERE t2_id = '%d' AND active = 1";
    $result = db_query($sql, $t_id);
    $money_in = db_result($result);
    
    $spesa = $spesa - $money_in + $money_out;

    $out .= theme_table($header, $rows, array("class" => "rosa"));

    $rows_credits[] = array("Crediti", $credits);
    $rows_credits[] = array("Spesi", $spesa);
    $rows_credits[] = array("Rimasti", "<strong>" . ($credits - $spesa) . "</strong>");

    $out .= theme_table(array(), $rows_credits);
  }
  else $out .= "Rosa non ancora presente";

  return $out;
}

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

function fantacalcio_show_teams_() {

  $teams = get_teams();
  $users = get_users();

  foreach ($teams as $team)
    $teams_names[] = ucwords($team->name);

  array_multisort($teams_names, SORT_ASC, $teams, SORT_DESC);

  if ($teams) {

    $header = array(
      array(
        "data" => "",
        "colspan" => 3
      ),
      "Nome",
      "Allenatore",
      "Stadio",
      "User"
    );

    foreach ($teams as $t_id => $team) {
      $rows[] = array(
        "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/shirts/" . $team->shirt . "'>",
        theme_team_last_year($team->last_year),
        array("data" => theme_team_history($team->history), "width" => 40 ),
        $team->name,
        $team->coach,
        $team->stadium,
        l($users[$teams[$t_id]->uid]->name, "user/" . $teams[$t_id]->uid)
      );
    }
  }
  return theme_table($header, $rows);
}

function show_team_matches($t_id) {
  $teams = get_teams();

  $sql = "SELECT * FROM {fanta_matches} 
          WHERE (t1_id = '%d' OR t2_id = '%d')
          AND g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = 1)
          ORDER BY round";
  $result = db_query($sql, $t_id, $t_id);
  while($row = db_fetch_object($result)){
    $rows[$row->m_id][] = $row->round;// . "&ordf; Giornata";
//    $rows[$row->m_id][] = ($row->date > 0) ? date("d-m", $row->date) : "";
//    $rows[$row->m_id][] = ($row->date > 0) ? date("H:i", $row->date) : "";
    $rows[$row->m_id][] = $teams[$row->t1_id]->name . " - " . $teams[$row->t2_id]->name;
    $rows[$row->m_id][] = ($row->played == 1) ? $row->goals_1 . " - " . $row->goals_2 : "";
    
    if ($row->round == get_next_competition_round(get_cid_by_gid($row->g_id)) || $row->played == 1) {
      $rows[$row->m_id][] = l("<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/details.png'>", 
                "scheda/partita/" . $row->m_id, 
                array("html" => "true", "attributes" => array("class" => "_thickbox", "rel" => "lightframe['matches_" . $t_id . "'|width:970px;]")));
    }
    else $rows[$row->m_id][] = array("data" => "&nbsp;", "colspan" => 1);
  }

  return theme_table(array("Giornata", "Partita", "Risultati", ""), $rows);
}

function theme_team_history($history) {

  if ($history) {
    $data = unserialize($history);
    $out = "";  
    $out = "<ul class='team-history'>";
    if ($data) {
      foreach ($data as $elem) {
        foreach ($elem as $key => $value) 
          $out .= "<li>" . $key . " <img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . variable_get($value, "") . "'></li>";
      }
    }
    $out .= "</ul>";
    return $out;
  }  
}

function theme_team_last_year($last_year) {
  if ($last_year) {
    $data = unserialize($last_year);
    $out = "";  
    foreach ($data as $key => $value)
      $out .= "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . variable_get($value, "") . "'>";

    return $out;
  }
}
