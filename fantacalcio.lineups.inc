<?php 
/**
*
* @file fantacalcio.formazioni.inc
*
*/

/********************/
/* menu        */
/********************/
function _fantacalcio_formazioni() {
  if (user_access("access fantacalcio")) $items[] = l("Visualizza", "formazioni/view");
  if (user_access("inserisci formazione")) $items[] = l("Inserisci", "formazioni/insert");
  if (user_access("inserisci formazione")) $items[] = l("Multe", "formazioni/multe");
  if (user_access("amministra formazioni")) $items[] = l("Amministra", "formazioni/admin");
  return theme_item_list($items);
}

/************************/
/* visualizzazione    */
/************************/
function _fantacalcio_formazioni_view($c_name, $round, $t_id) {

  $js_mobile = "function change_round(competition, c_id, base_url){
	var roundCompetition = document.getElementById('round_' + c_id);
	var url = base_url + 'formazioni/view/' + competition + '/' + roundCompetition.value;
	$.mobile.changePage( url , true );
    }";

  if (is_mobile())
    drupal_add_js($js_mobile, 'inline', 'footer');

  $args = array("c_name" => $c_name, "round" => $round, "t_id" => $t_id);
  return make_competition_tabs("get_formazione", $args, array('lineups' => 1));
}

function _fantacalcio_formazioni_box($c_name, $round, $t_id) {
  $args = array("c_name" => $c_name, "round" => $round, "t_id" => $t_id);
  $args['c_id'] = get_competition_id($args['c_name']);
  return show_formazione($args);
}

function get_formazione($args = array()) {

  $c_name = $args["c_name"];
  $t_id = $args["t_id"];
  $round = $args["round"];

  $c_id = get_competition_id(strtolower($c_name));
  $args["c_id"] = $c_id;
  $round = (!empty ($round)) ? $round : get_last_round_formazione($c_id);

  $out .= make_columns('show_formazione', 'formazioni/view/' . $c_name . '/' . $round, $args, 'filter_teams_formazioni', 'choose_formazioni_round_form');

  return $out;

}

function show_formazione($args = array()) {

  $teams = get_teams();
  $competitions = get_competitions();

  $t_id = $args['t_id'];
  $c_name = $competitions[$args['c_id']]->name;
  $c_id = get_competition_id($c_name);
  $round = (!empty ($args['round'])) ? $args['round'] : get_last_round_formazione($c_id);
  $players = get_players(get_round_by_competition_round($round, $c_id));

  $out = "";

  #squadra trovata
  if (array_key_exists($t_id, $teams)) {
    $round_label = get_round_label(get_match($round, $t_id, $c_id));  
    drupal_set_title(filter_xss($competitions[$c_id]->name . " - " . $round_label . " - ". $teams[$t_id]->name));

    $out .= "<h3 class='team_name'>" . $teams[$t_id]->name . "</h3><br>";

    $out .= "<div class='shirt'><img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/shirts/" . $teams[$t_id]->shirt . "'></div>";

    $formazione = array();
    $titolari = array();
    $riserve = array();
    $sql = "SELECT * FROM {fanta_lineups} 
            WHERE t_id = '%d'
            AND c_id = '%d' 
            AND round = '%d'
			AND position > 0";
    $result = db_query($sql, $t_id, $c_id, $round);
    if (db_affected_rows($result) > 0) {
      while ($row = db_fetch_object($result)) {
        $pl_id = $row->pl_id;
        $formazione[$pl_id] = $row;
        $posizioni[$row->position][$pl_id] = $row;
        if ($row->position == 1)
          $titolari[$pl_id] = $row;
        elseif (in_array($row->position, array(
          2,
          3
        ))) $riserve[$pl_id] = $row;
      }

      ksort($posizioni);

      #ordino i titolari(ruolo, nome)
      $roles = array();
      $names = array();

      foreach ($titolari as $titolare) {
        $modulo[$players[$titolare->pl_id]->role]++;
        $roles[] = $players[$titolare->pl_id]->role;
        $names[] = $players[$titolare->pl_id]->name;
      }

      array_multisort($roles, SORT_ASC, $names, SORT_ASC, $titolari);

      #ordino le riserve(ruolo, posizione, nome)
      #ordino i titolari(ruolo, nome)
      $roles = array();
      $names = array();

      foreach ($riserve as $riserva) {
        $roles[] = $players[$riserva->pl_id]->role;
        $names[] = $players[$riserva->pl_id]->name;
      }

      array_multisort($roles, SORT_ASC, $names, SORT_ASC, $riserve);

      if ($modulo) {
        ksort($modulo);

        unset($modulo[0]);
        $out .= "<p><strong>" . implode(" - ", $modulo) . "</strong></p>";
      }
      
      //mostra l'ordine dei rigoristi se è previsto il passaggio del turno
      $show_penalties = is_passaggio_turno($c_id, $round);

      #titolari
      $header_titolari = array(array("data" => "Titolari", "colspan" => 5));
      $rows_titolari = show_position($titolari, $players, $show_penalties);

      #riserve
      $header_riserve = array(array("data" => "Riserve", "colspan" => 5));
      $rows_riserve = show_position($riserve, $players);

      $out .= theme_table($header_titolari, $rows_titolari) . theme_table($header_riserve, $rows_riserve);

    } 
    else $out .= "<p>Formazione non trovata</p>";

  } 
  
  else $out .= "<h3>Squadra inesistente</h3><br>";

  return $out;
}

function show_position($position, $players, $show_penalties = FALSE) {

  $out = "";

  if ($position) {  
    foreach ($position as $pl_id => $player) {
      $roles[] = $players[$player->pl_id]->role;
      $names[] = $players[$player->pl_id]->name;
      $positions[] = $player->position;
    }

    array_multisort($roles, $positions, $names, $position);

    foreach ($position as $p_id => $player) {
      $rows[] = array(
              "data" => array(
                      array("data" => convert_role($players[$player->pl_id]->role), "class" => "role"),
                      array("data" => $players[$player->pl_id]->name, "class" => "name"),
                      ucwords(convert_teams($players[$player->pl_id]->team)),
                      l("<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/stats.png'>", "scheda/giocatore/" . $player->pl_id, array(
                                                                "html" => "TRUE", 
                                                                "attributes" => array(
                                                                  "class" => "thickbox",
                                                                  "rel" => "lightframe")
                                                                  )),
                      ($show_penalties) ? "<span title='Ordine rigoristi'>(" . ($player->penalty_position+12) . ")</span>" : ""
                      ),
              "class" => "role_" . $players[$player->pl_id]->role
              );

    }
  }
  return $rows;
}

/************************/
/* END visualizzazione  */
/************************/

/************************/
/* inserimento      */
/************************/

function fantacalcio_formazione_insert_start_user($competition_name = '') {
  $t_id = get_team_id_by_user($user->uid);
  return fantacalcio_formazione_insert_start($competition_name, $t_id, TRUE);
}

function fantacalcio_formazione_insert_start_admin($competition_name = '', $t_id) {
  return fantacalcio_formazione_insert_start($competition_name, $t_id, TRUE);
}

function fantacalcio_formazione_insert_start($competition_name = '', $t_id = NULL, $is_admin = FALSE) {

  global $user;

  $competitions = get_competitions();
  $teams = get_teams();

  $t_id = ($t_id != null) ? $t_id : get_team_id_by_user($user->uid);

  if (empty ($competition_name) || !is_competition_name($competition_name)) {
  
    #controllo quante formazioni vanno inserite per la giornata
    $vote_round = get_last_votes() or 0;
    $vote_round++;
    $sql = "SELECT * FROM {fanta_rounds_competitions} WHERE round = '%d'";
    $result = db_query($sql, $vote_round);
    while ($row = db_fetch_array($result)) {
      $sqla = "SELECT * FROM {fanta_matches} " .
          "WHERE g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = '%d') " .
          "AND round = '%d' " .
          "AND (t1_id = '%d' OR t2_id = '%d')";
      $resulta = db_query($sqla, $row['c_id'], $row['competition_round'], $t_id, $t_id);
      while ($rowa = db_fetch_array($resulta)) {
        $formazioni[$rowa['g_id']] = $rowa['round'];
      }
    }

    if (count($formazioni) > 1) {
      $out = "<p>Inserire una formazione per ogni partita</p>";
      foreach ($formazioni as $g_id => $round) {
        $c_id = get_cid_by_gid($g_id);
        $competition = $competitions[$c_id];

        $insert = check_formazione_insert($t_id, $c_id, $round);
        $opponent = get_opponent($t_id, $c_id, $round);
        $match = get_match($round, $t_id, $c_id);
        $opponent_link = l($teams[$opponent]->name, "formazioni/view/" . $competition->name . "/" . $round . "/" . $opponent, array("attributes" => array("title" => "Formazione " . $teams[$opponent]->name)) );

        $round_label = get_round_label($match);

        $rows[$c_id][] = "<a href='" . base_path() . arg(0) . "/" . arg(1) . "/" . $competition->name . "'>" . $competition->name . " - " . $round_label . "</a>";
        $rows[$c_id][] = "<span class='insert'>" . $insert . "</span>";
        $rows[$c_id][] = "<span class='opponent'>" . $teams[$t_id]->name . " - " . $opponent_link . "</span>";
      }

      $out .= "<div style='width: 70%;'>" . theme_table(array(), $rows, array()) . "</div>";

      return $out;
    }
    elseif (count($formazioni) == 1) {
      foreach ($formazioni as $c_id => $round) $competition_name = $competitions[$c_id]->name;

      //return insert_formazione($competition_name);
      return fantacalcio_insert_formazione(get_competition_name(variable_get("fantacalcio_main_competition", 1)), $t_id, $is_admin);
    }
    else return fantacalcio_insert_formazione(get_competition_name(variable_get("fantacalcio_main_competition", 1)), $t_id, $is_admin);
  }

  else return fantacalcio_insert_formazione($competition_name, $t_id, $is_admin);

}

//fantacalcio_formazione_insert_start
function fantacalcio_insert_formazione($competition_name, $t_id, $is_admin = FALSE) {
  global $user;

  $c_id = (competition_exists(get_competition_id($competition_name))) ? get_competition_id($competition_name) : variable_get("fantacalcio_main_competition", 1);
  $round = get_next_round_formazione($c_id);
  $match = get_match($round, $t_id, $c_id);
  $round_label = get_round_label($match);

  $out = "";
  
  $ts_match = get_timestamp_match($c_id, $round);

  #formazione inserita in orario
  if ($ts_match > time() || $is_admin) {

    $out .= "\n<h3>" . ucwords($competition_name) . "</h3>";
    $out .= "\n<h4>" . $round_label . "</h4>";

    $args = array(
      't_id' => $t_id,
      'c_id' => $c_id,
      'round' => $round,
    );

    $formazione_form = drupal_get_form('fantacalcio_formazione_form', $args);

    $args_confirm = get_args_confirm($args);

    $out .= confirm_formazione($args_confirm);
      
    $out .= "\n<div id='div-formazione'>";
    $out .= "\n<div id='header-formazione'>";
	
    $out .= "\n<div class='clear' style='height:30px;'></div>";
	
    $out .= "<div data-role='popup' id='popup_validate'>" . show_line_up_check(fantacalcio_formazione_check(get_line_up($c_id, $t_id, $round))) . "</div>";
    		  
    $out .= "\n<div id='_formazione_form'>" . $formazione_form . "</div>";
          
    $out .= "</div>";

    $out .= "\n<div class='clear' style='height:10px;'></div>";

    $out .= "<div class='ui-grid-a'>";
	
    if (is_mobile()) {
	  $out .= "<div class='ui-block-a'>" 
                  . "<a href='#popup_validate' data-rel='popup' data-theme=c data-role='button' data-icon='plus' data-inline='true' data-mini='true' data-iconpos='notext'>" 
                  . t('check') 
                  . "</a>"
                  . "</div>";
	}
		  
    //link formazione avversario
    $out .= "\n<div class='ui-block-b'>"
          . l("Formazione avversario", "formazioni/box/" . get_competition_name($c_id) . "/" . $round . "/" . get_opponent($t_id, $c_id, $round), array("html" => TRUE, "attributes" => array("rel" => "lightframe[box|width:500px;]")))
          . "</div>";
	
	$out .= "</div>";

    $out .= "\n<div class='clear' style='height:30px;'></div>";

    $out .= make_formazione($t_id, $c_id, $round);

    $out .= "\n<div class='clear' style='height:10px;'></div>";
      
    $out .= "\n</div>";
    
  }
  else {
    fantacalcio_formazione_insert_players('old', $t_id, $c_id, $round);
    header("location: " . base_path() . "formazioni/view/" . get_competition_name($c_id) . "/" . $round . "/" . $t_id);
  }
  
  return $out;

}

function fantacalcio_formazione_form($form_state, $args) {

  global $user;

  $checks = fantacalcio_formazione_check(get_line_up($args["c_id"], $args["t_id"], $args["round"]));

  //form redirect (rigoristi se previsto il passaggio del turno)
  if (is_passaggio_turno($args['c_id'], $args['round']) )
    $form['#redirect'] = 'formazioni/insert/' . get_competition_name($args['c_id']) . '/penalties' ;
  else $form['#redirect'] = 'formazioni/view/' . get_competition_name($args['c_id']) . '/' . $args['round'] . '/' . $args['t_id'] ;

  if ($args['status'] == 1)
    $disabled = FALSE;
  else
    $disabled = TRUE;

  $form['t_id'] = array(
    '#type' => 'hidden',
    '#value' => $args['t_id']
  );

  $form['c_id'] = array(
    '#type' => 'hidden',
    '#value' => $args['c_id']
  );

  $form['round'] = array(
    '#type' => 'hidden',
    '#value' => $args['round']
  );

  $form['line_up'] = array(
    '#type' => 'hidden',
    '#value' => json_encode(get_line_up($args["c_id"], $args["t_id"], $args["round"])),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Invia',
    '#disabled' => ($checks['number_1'] == 1 && $checks['module_1'] == 1 && $checks['number_2_3'] == 1 && $checks['module_2_3'] == 1),
    '#validate' => array('fantacalcio_formazione_form_validate'),
    '#id' => "line_up_submit",
	'#prefix' => "<div class='ui-grid-a'><div class='ui-block-a'>",
	'#suffix' => "</div>",
  );

  $form['clear'] = array(
    '#type' => 'submit',
    '#value' => 'Cancella',
    '#validate' => array(
      'fantacalcio_formazione_form_clear'
    ),
    '#attributes' => array(
      'onclick' => "return confirm_reset_formazione()"
    ),
	'#prefix' => "<div class='ui-block-b'>",
	'#suffix' => "</div></div>",
  );

  return $form;
}

function fantacalcio_formazione_form_submit($form, & $form_state) {
  $t_id = $form_state['values']['t_id'];
  $c_id = $form_state['values']['c_id'];
  $round = $form_state['values']['round'];

  $ts_match = get_timestamp_match($c_id, $round);

  #formazione inviata in orario
  if ($ts_match > time()) {

    $line_up = json_decode($form_state['clicked_button']['#post']['line_up']);

    //cancello la vecchia formazione
    $sql = "DELETE FROM {fanta_lineups} WHERE t_id = '%d' AND c_id = '%d' AND round = '%d'";
    db_query($sql, $t_id, $c_id, $round);

    foreach($line_up as $pl_id => $player) {
      //insert
      if ($player->position > 0) {
        $sql = "INSERT INTO {fanta_lineups} (t_id, pl_id, c_id, round, position, timestamp, uid) VALUES ('%d', '%d', '%d', '%d', '%d', '%d', '%d')";
        db_query($sql, $t_id, $pl_id, $c_id, $round, $player->position, $timestamp, $user->uid);
      }
    }

    fantacalcio_formazione_insert_players('new', $t_id, $c_id, $round);
    drupal_set_message('Formazione inserita correttamente.');    
  }
  #formazione inviata in ritardo -> giornata precedente
  else {
    fantacalcio_formazione_insert_players('old', $t_id, $c_id, $round);
    drupal_set_message('Tempo scaduto. Userai la formazione della giornata precedente');
  }

  return;
}

function fantacalcio_formazione_form_clear($form, &$form_state) {

  #cancella la formazione inserita
  $sql = "DELETE FROM {fanta_lineups} " .
  "WHERE c_id = '%d' " .
  "AND round = '%d' " .
  "AND t_id = '%d'";
  $result = db_query($sql, $form_state['values']['c_id'], $form_state['values']['round'], $form_state['values']['t_id']);

  $sql = "DELETE FROM {fanta_lineups_inserts} " .
      "WHERE c_id = '%d' " .
      "AND round = '%d' " .
      "AND t_id = '%d'";
  $result = db_query($sql, $form_state['values']['c_id'], $form_state['values']['round'], $form_state['values']['t_id']);

  drupal_set_message('Formazione cancellata');
  $teams = get_teams();
  watchdog('fantacalcio', '@team: Formazione cancellata', array('@team' => $teams[$form_state['values']['t_id']]->name), WATCHDOG_NOTICE);
  return $form_state['rebuild'] = TRUE;
}

function fantacalcio_formazione_form_validate($form, &$form_state) {

  $t_id = $form_state['values']['t_id'];
  $c_id = $form_state['values']['c_id'];
  $round = $form_state['values']['round'];

  $line_up = json_decode($form_state['clicked_button']['#post']['line_up']);
  //print_r($line_up);die();

  //check giocatori in rosa
  $check_players_in_team = true;
  foreach($line_up as $pl_id => $player) {
    $sql = "SELECT * FROM {fanta_squads} WHERE pl_id = '%d' ANd t_id = '%d' AND in_team = 1";
    $result = db_query($sql, $pl_id, $t_id);
    if (db_affected_rows($result) == 0) {
        form_set_error("", t("Errore nella formazione"));
    }
  }

  if ($check_players_in_team) {
    //check ruolo giocatori
    $check_players_role = true;
    foreach($line_up as $pl_id => $player) {
      $sql = "SELECT * FROM {fanta_players} WHERE pl_id = '%d'";
      $result = db_query($sql, $pl_id);
      while($row = db_fetch_object($result)) {
        if ($row->role != $player->role)
          form_set_error("", t("Errore nella formazione"));
      }
    }

    if ($check_players_role) {
      //check modulo
      $checks = fantacalcio_formazione_check($line_up);//print_r($line_up); print_r($checks);die();

      if (!$checks['number_1'] && $checks['number_1'] != 1)
        form_set_error("", t("Errore nella formazione: Numero titolari errato - Numero titolari consentito: 11"));
      if (!$checks['module_1'] && $checks['module_1'] != 1)
        form_set_error("", t("Errore nella formazione: Modulo titolari errato - Moduli titolari consentiti: " . variable_get("fantacalcio_moduli_titolari", "")));    
      if (!$checks['number_2_3'] && $checks['number_2_3'] != 1)
        form_set_error("", t("Errore nella formazione: Numero riserve errato - Numero riserve consentite: 7"));
      if (!$checks['module_2_3'] && $checks['module_2_3'] != 1)
        form_set_error("", t("Errore nella formazione: Modulo riserve errato - Moduli riserve consentiti: " . variable_get("fantacalcio_moduli_riserve", "")));    
    }
    else {
      //error
      form_set_error("", t("Errore nella formazione"));
    }
  }
  else {
    //error
    form_set_error("", t("Errore nella formazione"));
  }
}

function make_formazione($t_id, $c_id, $round, $admin = FALSE) {
  if (is_mobile()) 
    return make_formazione_mobile($t_id, $c_id, $round, $admin);
  else 
    return make_formazione_web($t_id, $c_id, $round, $admin);
}

function make_formazione_web($t_id, $c_id, $round, $admin = FALSE) {

  global $user;

  drupal_add_css('sites/all/plugins/interface/interface.css', 'themes', 'all', TRUE);
  drupal_add_js('sites/all/plugins/interface/jquery.js', 'footer');
  drupal_add_js('sites/all/plugins/interface/interface.js', 'footer');
  drupal_add_js('sites/all/plugins/interface/compressed/idrag.js', 'footer');
  drupal_add_js('sites/all/plugins/interface/compressed/idrop.js', 'footer');
  drupal_add_js('sites/all/plugins/interface/compressed/isortables.js', 'footer');
  drupal_add_js('sites/all/modules/fantacalcio/js/lineup_insert.js', 'footer');

  $players = get_players(get_round_by_competition_round($round, $c_id));

  $formazione = array();
  $titolari = array();
  $riserve_1 = array();
  $riserve_2 = array();
  $sql = "SELECT * FROM {fanta_lineups} " .
  "WHERE round = '%d' " .
  "AND c_id = '%d' " .
  "AND t_id = '%d' " .
  "AND position != 0";
  $result = db_query($sql, $round, $c_id, $t_id);
  while ($row = db_fetch_array($result)) {
    $formazione[$row['pl_id']] = $row;
    if ($row['position'] == 1)
      array_push($titolari, $row);
    elseif ($row['position'] == 2) array_push($riserve_1, $row);
    elseif ($row['position'] >= 3) array_push($riserve_2, $row);
  }

  $sql = "SELECT * FROM {fanta_squads} WHERE t_id = '%d' AND in_team = 1";
  $result = db_query($sql, $t_id);
  while ($row = db_fetch_array($result)) {
    $rose[] = $row;
  }

  if ($rose) {
  foreach ($rose as $rosa) {
    $roles[] = $players[$rosa['pl_id']]->role;
    $names[] = $players[$rosa['pl_id']]->name;
  }

  array_multisort($roles, SORT_ASC, $names, SORT_ASC, $rose);

  #rosa
  $i = 0;
  foreach ($rose as $player) {
    if (!array_key_exists($player['pl_id'], $formazione)) {
      $pl_id = $player['pl_id'];
      $rosa[$pl_id] = $player;

      $class = "role_" . $players[$pl_id]->role;

      $out_rosa .= "\n<div  class='groupItem' id='$pl_id' name='" . $players[$pl_id]->role . "_" . $c_id . "_" . $t_id . "'>" .
      "\n<div  class='itemHeader'>\n" ;

      $out_rosa .= "\n<table width='100%'>" .
      "\n<tr class='$class'>" .
      "\n<td class='role' width='10%'>" . convert_role($players[$pl_id]->role) . "</td>" .
      "\n<td width='50%'>" . $players[$pl_id]->name . "</td>" .
      "\n<td width='35%'>" . ucwords(strtolower($players[$pl_id]->team)) . "</td>" .
      "\n<td><a rel='lightframe[" . $t_id . "|width:770px;]' href='" . base_path() . "scheda/giocatore/" . $pl_id . "' title='Dettagli giocatore'>" .
      "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/stats.png'>" .
      "</a></td>".
      "\n</tr>" .
      "\n</table>";

        $out_rosa .= "\n</div>"
          # . "\n<div class='itemContent'>\n"
          #   . "\n</div>"
           . "\n</div>";

      $i++;
    }
  }

  #titolari
  $roles = array(); $names = array();
  foreach ($titolari as $titolare) {
    $roles[] = $players[$titolare['pl_id']]->role;
    $names[] = $players[$titolare['pl_id']]->name;
  }

  array_multisort($roles, SORT_ASC, $names, SORT_ASC, $titolari);

  foreach ($titolari as $titolare) {
    $pl_id = $titolare['pl_id'];
    $class = "role_" . $players[$pl_id]->role;

    $out_titolari .= "\n<div  class='groupItem' id='$pl_id' name='" . $players[$pl_id]->role . "_" . $c_id . "_" . $t_id . "'>" .
      "\n<div  class='itemHeader'>\n";
    
    $out_titolari .= "\n<table width='100%'>" .
    "\n<tr class='$class'>" .
    "\n<td class='role' width='10%'>" . convert_role($players[$pl_id]->role) . "</td>" .
    "\n<td width='50%'>" . $players[$pl_id]->name . "</td>" .
    "\n<td width='35%'>" . ucwords(strtolower($players[$pl_id]->team)) . "</td>" .
    "\n<td width='5%' style='padding-left:5px;'>".
    "\n<td><a rel='lightframe[" . $t_id . "|width:770px;]' href='" . base_path() . "scheda/giocatore/" . $pl_id . "' title='Dettagli giocatore'>" .
    "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/stats.png'>" .
    "</a></td>".
    "\n</td>" .
    "\n</tr>" .
    "\n</table>";
    
    $out_titolari .= "\n</div>"
      # . "\n<div class='itemContent'>\n"
      #   . "\n</div>"
       . "\n</div>";

  }

  #riserve
  $riserve = array_merge($riserve_1, $riserve_2);

  $roles = array(); $names = array(); $positions = array();
  foreach ($riserve as $riserva) {
    $roles[] = $players[$riserva['pl_id']]->role;
    $names[] = $players[$riserva['pl_id']]->name;
    $positions[] = $riserva['position'];
  }

  array_multisort($roles, SORT_ASC, $positions, SORT_ASC, $names, SORT_ASC, $riserve);

  foreach ($riserve as $riserva) {
    $pl_id = $riserva['pl_id'];
    $class = "role_" . $players[$pl_id]->role;

    $out_riserve .= "\n<div  class='groupItem' id='$pl_id' name='" . $players[$pl_id]->role . "_" . $c_id . "_" . $t_id . "'>" ;
    
    $out_riserve .= "\n<div  class='itemHeader'>\n".
    "\n<table width='100%'>" .
    "\n<tr class='$class'>" .
    "\n<td class='role' width='10%'>" . convert_role($players[$pl_id]->role) . "</td>" .
    "\n<td width='50%'>" . $players[$pl_id]->name . "</td>" .
    "\n<td width='35%'>" . ucwords(strtolower($players[$pl_id]->team)) . "</td>" .
    "\n<td width='5%' style='padding-left:5px;'>".
    "\n<td><a rel='lightframe[" . $t_id . "|width:770px;]' href='" . base_path() . "scheda/giocatore/" . $pl_id . "' title='Dettagli giocatore'>" .
    "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/stats.png'>" .
    "</a></td>".
    "\n</td>" .
    "\n</tr>" .
    "\n</table></div>";

    $out_riserve .= "\n</div>";

  }


  $div_class = ($admin) ? "groupWrapperAdmin" : "groupWrapper";

  $out = "<div id='formazione-web'>";

  #rosa
  $out .= "<div id='pos_0' class='" . $div_class . "'>" .
    $out_rosa .
    "</div>";

  #titolari
  $out .= "<div id='pos_1' class='" . $div_class . "'>" .
    "<h4>Titolari</h4>" .
    $out_titolari .
    "</div>";

  #riserve
  $out .= "<div id='pos_2' class='" . $div_class . "'>" .
    "<h4>Riserve</h4>" .
    $out_riserve .
    "</div>";
  }
  
  $out .= "</div>";
  
  return $out;
}

function make_formazione_mobile($t_id, $c_id, $round, $admin = FALSE) {

  global $user;

  drupal_add_js('sites/all/modules/fantacalcio/js/lineup_insert.js', null, "footer");

  $players = get_players(get_round_by_competition_round($round, $c_id));

  $formazione = array();
  $titolari = array();
  $riserve_1 = array();
  $riserve_2 = array();
  $sql = "SELECT * FROM {fanta_lineups} 
        WHERE round = '%d' 
        AND c_id = '%d' 
        AND t_id = '%d' ";
  $result = db_query($sql, $round, $c_id, $t_id);
  while ($row = db_fetch_array($result)) {
    $formazione[$row['pl_id']] = $row;
    if ($row['position'] == 1)
      array_push($titolari, $row);
    elseif ($row['position'] == 2) array_push($riserve_1, $row);
    elseif ($row['position'] >= 3) array_push($riserve_2, $row);
  }

  $sql = "SELECT * FROM {fanta_squads} WHERE t_id = '%d' AND in_team = 1";
  $result = db_query($sql, $t_id);
  while ($row = db_fetch_array($result)) {
    $rose[] = $row;
  }

  if ($rose) {
    foreach ($rose as $rosa) {
      $roles[] = $players[$rosa['pl_id']]->role;
      $names[] = $players[$rosa['pl_id']]->name;
    }

    array_multisort($roles, SORT_ASC, $names, SORT_ASC, $rose);
  
    #rosa
    foreach ($rose as $player) {
      $pl_id = $player['pl_id'];
      $rosa[$pl_id] = $player;

      $class = "role_" . $players[$pl_id]->role;
            
      $rows[$pl_id] = array("class" => $class = "role_" . $players[$pl_id]->role,
                "data" => array(
                      array("data" => convert_role($players[$pl_id]->role), "class" => "role", "width" => "7%"),
                      array("data" => $players[$pl_id]->name, "width" => "40%"),
                      array("data" => ucwords(strtolower($players[$pl_id]->team)), "width" => "25%"),
                      array("data" => position_button($pl_id, $t_id, $c_id, $players[$pl_id]->role, $formazione[$pl_id]['position']), "width" => "25%", "id" => "button_" . $pl_id, "class" => "button"),
                      array("data" => l("<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/stats.png'>", 
                              "scheda/giocatore/" . $pl_id,
                              array("html" => TRUE, "title" => "Dettagli giocatore", "attributes" => array("class" => "thickbox", "rel" => "lightframe", "data-role" => "button", "data-mini" => "true"))), "width" => "7%")
                      ));
    }

  }
  
  return "<div id='formazione-mobile'>" . theme_table(array(), $rows) . "</div>";
}

function fantacalcio_formazione_insert_players($type = 'new', $t_id, $c_id, $round, $other_c_id = '', $other_round = '', $status = 1) {

  global $user;

  $status = ($status + 1 ) % 2;

  if ($type == 'new') {
    #registrazione formazione
    $sql = "INSERT INTO {fanta_lineups_inserts} (t_id, c_id, round, status, timestamp) VALUES ('%d', '%d', '%d', 1, '%d')";
    $result = db_query($sql, $t_id, $c_id, $round, time());
    $teams = get_teams();
    watchdog('fantacalcio', '@team: Formazione inserita correttamente', array('@team' => $teams[$t_id]->name), WATCHDOG_NOTICE);
  }
  elseif ($type == 'old') {

    if (fantacalcio_formazione_exists($t_id, $c_id, $round)) {
      drupal_set_message("Hai già inserito la formazione");
    }
    else{
      #registrazione formazione
      $sql = "INSERT INTO {fanta_lineups_inserts} (t_id, c_id, round, status, timestamp) VALUES ('%d', '%d', '%d', 0, '%d')";
      $result = db_query($sql, $t_id, $c_id, $round, time());

      #inserimento formazione giornata precedente
      $sql = "SELECT * FROM {fanta_lineups} WHERE c_id = '%d' AND t_id='%d' AND round = '%d'";
      $result = db_query($sql, $c_id, $t_id, $round -1);
      while ($row = db_fetch_array($result)) {
        $pl_id = $row['pl_id'];
        $position = $row['position'];
        $sqla = "INSERT INTO {fanta_lineups} (pl_id, c_id, t_id, round, position, uid, timestamp) VALUES " .
        "(" . $pl_id . ", " . $c_id . ", " . $t_id . ", " . $round . ", " . $position . "," . $user->uid . "," . time() . ")";
        $resulta = db_query($sqla);
      }
      drupal_set_message("Userai la formazione della giornata precedente");
      $teams = get_teams();
      watchdog('fantacalcio', '@team: Formazione inserita in ritardo', array('@team' => $teams[$t_id]->name), WATCHDOG_NOTICE);
    }
  }
  elseif ($type == 'confirm') {

    #registrazione formazione
    $sql = "INSERT INTO {fanta_lineups_inserts} (t_id, c_id, round, status, timestamp) VALUES ('%d', '%d', '%d', '%d', '%d')";
    $result = db_query($sql, $t_id, $c_id, $round, $status, time());

    #inserimento formazione giornata precedente
    $sql = "DELETE FROM {fanta_lineups} " .
        "WHERE c_id = '%d' " .
        "AND round = '%d' " .
        "AND t_id = '%d'";
    $result = db_query($sql, $c_id, $round, $t_id);

    $sql = "SELECT * FROM {fanta_lineups} WHERE c_id = '%d' AND t_id='%d' AND round = '%d'";
    $result = db_query($sql, $other_c_id, $t_id, $other_round);
    while ($row = db_fetch_array($result)) {
      $pl_id = $row['pl_id'];
      $position = $row['position'];
      $sqla = "INSERT INTO {fanta_lineups} (pl_id, c_id, t_id, round, position, uid, timestamp) VALUES " .
        "(" . $pl_id . ", " . $c_id . ", " . $t_id . ", " . $round . ", " . $position . "," . $user->uid . "," . time() . ")";
      $resulta = db_query($sqla);
    }
    drupal_set_message("Formazione confermata");
    $teams = get_teams();
    watchdog('fantacalcio', '@team: Formazione confermata', array('@team' => $teams[$t_id]->name), WATCHDOG_NOTICE);
  }
}

function confirm_formazione($args) {

  $t_id = $args["t_id"];
  $round = $args["round"];
  $c_id = $args["c_id"];

  if (!isset($args["other_c_id"]) or !isset($args["other_round"])) {

    $sql = "SELECT round FROM {fanta_rounds_competitions} WHERE c_id = '%d' AND competition_round = '%d'";
    $result = db_query($sql, $c_id, $round);
    $vote_round = db_result($result);
    $sql = "SELECT * FROM {fanta_rounds_competitions} WHERE round = '%d' AND c_id <> '%d'";
    $result = db_query($sql, $vote_round, $c_id);
    while ($row = db_fetch_array($result)) {
      $other_c_id = $row['c_id'];
      $other_round = $row['competition_round'];
    }
  }
  else {
    $other_c_id = $args["other_c_id"];
    $other_round = $args["other_round"];
  }
  
  $sql = "SELECT * FROM {fanta_lineups_inserts} " .
    "WHERE c_id = '%d' " .
    "AND round = '%d' " .
    "AND t_id = '%d'";
  $result = db_query($sql, $other_c_id, $other_round, $t_id);
  if (db_affected_rows($result) > 0) {
    $args = array(  "t_id" => $t_id,
            "other_c_id" => $other_c_id,
            "other_round" => $other_round,
            "other_competition_name" => get_competition_name($other_c_id),
            "c_id" => $c_id,
            "competition_name" => get_competition_name($c_id),
            "round" => $round);

    if (arg(0) == "formazioni" && arg(1) == "admin") $args["is_admin"] = 1;

    return "<div class='formazione-confirm'>"
       . "\n<div style='float:left;'>Vuoi utilizzare la stessa formazione che hai gi&agrave; inserito per la partita di "
      . $args['other_competition_name'] . " (" . $args['other_round'] . "&ordf; Giornata) ?</div>"
       . "\n<div style='float:left; margin-left: 20px;'>"
      . drupal_get_form("confirm_formazione_form", $args)
       . "</div><div class='clear'></div></div>";
  }
  else return "";

}

function get_args_confirm($args) {

  $t_id = $args['t_id'];
  $round = $args['round'];
  $c_id = $args['c_id'];
  
  $new_args = $args;

  $sql = "SELECT c.round 
      FROM {fanta_rounds} r, {fanta_rounds_competitions} c
      WHERE c.round = r.round
      AND c.c_id = '%d' 
      AND c.competition_round = '%d'";
  $result = db_query($sql, $c_id, $round);
  $vote_round = db_result($result);

  $sql = "SELECT *
      FROM {fanta_rounds} r, {fanta_rounds_competitions} c
      WHERE c.round = r.round
      AND c.c_id != '%d' 
      AND c.round = '%d'
      ORDER BY c_id DESC";
  $result = db_query($sql, $c_id, $vote_round);
  while ($row = db_fetch_array($result)) {
    $other_c_id = $row['c_id'];
    $other_round = $row['competition_round'];
  }

  $sql = "SELECT * FROM {fanta_lineups_inserts} " .
      "WHERE c_id = '%d' " .
      "AND round = '%d' " .
      "AND t_id = '%d'";
  $result = db_query($sql, $other_c_id, $other_round, $t_id);

  if (db_affected_rows($result) > 0) {
    $new_args['other_c_id'] = $other_c_id;
    $new_args['other_round'] = $other_round;
  }
  else {
    $new_args['other_c_id'] = $c_id;
    $new_args['other_round'] = $round - 1;
  }

  if (arg(0) == "formazione" && arg(1) == "admin")
    $new_args['is_admin'] = 1;
  
  return $new_args;
}

function get_opponent($t_id, $c_id, $round) {

  $g_id = get_group_team($c_id, $t_id);

  $sql = "SELECT * FROM {fanta_matches} " .
      "WHERE g_id = '%d' " .
      "AND round = '%d' " .
      "AND (t1_id = '%d' OR t2_id = '%d')";
  $result = db_query($sql, $g_id, $round, $t_id, $t_id );
  while ($row = db_fetch_object($result)) {
    if ($t_id == $row->t1_id) return $row->t2_id;
    elseif ($t_id == $row->t2_id) return $row->t1_id;
  }
}


function confirm_formazione_form($form_state, $args) {

  //form redirect (rigoristi se previsto il passaggio del turno)
  if (is_passaggio_turno($args['c_id'], $args['round']) )
    $form['#redirect'] = 'formazioni/insert/' . get_competition_name($args['c_id']) . '/penalties' ;
  else $form['#redirect'] = 'formazioni/view/' . get_competition_name($args['c_id']) . '/' . $args['round'] . '/' . $args['t_id'] ;
  
  $form['other_c_id'] = array("#type" => "hidden", "#value" => $args['other_c_id']);
  $form['other_round'] = array("#type" => "hidden", "#value" => $args['other_round']);
  $form['c_id'] = array("#type" => "hidden", "#value" => $args['c_id']);
  $form['round'] = array("#type" => "hidden", "#value" => $args['round']);
  $form['t_id'] = array("#type" => "hidden", "#value" => $args['t_id']);

  if (isset($args['is_admin']) && $args['is_admin'] == 1) {
    $form['multa'] = array("#type" => "checkbox", "#title" => "Multa", "#attributes" => array("data-theme" => "e"));
  }
  else $form['multa'] = array("#type" => "hidden", "#value" => 0);

  $form['confirm']['submit'] = array(
              "#type" => "submit",
              "#value" => "Ok",
              "#title" => "conferma la formazione inserita",
              '#attributes' => array('onclick' => "return confirm_conferma_formazione()"),
              );

  return $form;

}

function confirm_formazione_form_submit($form, $form_state) {
  fantacalcio_formazione_insert_players('confirm', $form_state['values']['t_id'], $form_state['values']['c_id'], $form_state['values']['round'], $form_state['values']['other_c_id'], $form_state['values']['other_round'], $form_state['values']['multa']);
}

/****************************/
/* END inserimento      */
/****************************/

/****************************/
/* rigoristi        */
/****************************/

function fantacalcio_insert_penalties($competition_name) {
  global $user;

  $t_id = get_team_id_by_user($user->uid);
  $c_id = get_competition_id($competition_name);
  $round = get_next_round_formazione($c_id);
  
  return drupal_get_form('insert_penalties_form', $t_id, $c_id, $round);
}

function insert_penalties_form(&$form_state, $t_id, $c_id, $round) {
  $i = 1;
  //titolari da ordinare
  $sql = "SELECT * 
      FROM {fanta_lineups} f, {fanta_players} p, {fanta_players_teams} t, {fanta_real_teams} r
      WHERE f.pl_id = p.pl_id
      AND p.pl_id = t.pl_id
      AND t.rt_id = r.rt_id
      AND f.t_id = '%d' 
      AND f.c_id = '%d' 
      AND f.round = '%d'
      AND t.round = '%d'
      AND position = 1
      ORDER BY penalty_position";
  $result = db_query($sql, $t_id, $c_id, $round, get_round_by_competition_round($round, $c_id));
  while ($row = db_fetch_object($result)) {
    //create a partial table row containing the data from the table
    $data = array(
      convert_role($row->role),
      $row->name,
      ucwords($row->long_name),
      //$row->penalty_position
    );
   
    //add our static "row" data into a form value
    $form['rows'][$row->pl_id]['data']=array(
                   '#type' => 'value',
                   '#value' => $data
                   );
   
    //now create the weight form element.  
    //NOTE how we add the id into the element key
    $form['rows'][$row->pl_id]['weight-' . $row->pl_id]=array(
      '#type' => 'weight',
      '#default_value' => $row->penalty_position,
      '#delta' => 11,
      //add a specific class in here - we need this later
      '#attributes' => array('class' => 'weight'),
    );
    $i++;
  }
  
  $form['#redirect'] = 'formazioni/view/' . get_competition_name($c_id) . '/' . $round . '/' . $t_id ;
  
  $form['t_id'] = array(
    '#type' => 'hidden',
    '#value' => $t_id,
  );
  
  $form['c_id'] = array(
    '#type' => 'hidden',
    '#value' => $c_id,
  );
  
  $form['round'] = array(
    '#type' => 'hidden',
    '#value' => $round,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Conferma'),
  );
  
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Annulla'),
    '#validate' => array(
      'insert_penalties_form_clear'
    )
  );

  return $form;
}

function insert_penalties_form_submit($form, &$form_state) {
  foreach ($form_state['values'] as $key => $data) {
    //we are only interested in weight elements
    if (substr($key, 0, 6) == 'weight') {
      //cunningly we have the DB id of the row in the element name
      //print_r ("<pre>".($data) . "</pre>");
      $pl_id = str_replace('weight-', '', $key);
      $sql = "UPDATE {fanta_lineups} 
          SET penalty_position = %d 
          WHERE pl_id = %d 
          AND t_id = %d 
          AND c_id = %d 
          AND round = %d";
      db_query($sql, $data, $pl_id, $form_state['values']['t_id'], $form_state['values']['c_id'], $form_state['values']['round']);    
    }
  }  
  
  drupal_set_message("Rigoristi ordinati");
}

function insert_penalties_form_clear($form, &$form_state) {
  return $form_state['rebuild'] = TRUE;
}

function theme_insert_penalties_form($form) {
  //loop through each "row" in the table array
  foreach ($form['rows'] as $id => $row) {
    
    //we are only interested in numeric keys
    if (intval($id)) { 
      $this_row = $row['data']['#value'];

      //Add the weight field to the row
      $this_row[] = drupal_render($form['rows'][$id]['weight-' . $id]);

      //Add the row to the array of rows
      $table_rows[] = array('data' => $this_row, 'class' => 'draggable');
    }
  }

  //Make sure the header count matches the column count
  $header = array(
    "Ruolo", "Nome", "Squadra", ""
  );

  $output = theme('table', $header, $table_rows, array('id' => 'penalty-position-table'));
  $output .= drupal_render($form);

  // Call add_tabledrag to add and setup the JS for us
  // The key thing here is the first param - the table ID
  // and the 4th param, the class of the form item which holds the weight
  drupal_add_tabledrag('penalty-position-table', 'order', 'sibling', 'weight');      

  return $output;
}
/********************************/
/* END  rigoristi        */
/********************************/

/********************************/
/* multe            */
/********************************/
function fantacalcio_show_multe() {

  $out = '';
  $multe = array();

  $sql = "SELECT * FROM {fanta_rounds_competitions}";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $vote_rounds[$row->c_id][$row->competition_round] = $row->round;
  }

  $sql = "SELECT * FROM {fanta_lineups_inserts} WHERE status = 0";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $t_id = $row->t_id;
    $competition_round = $row->round;
    $c_id = $row->c_id;

    $round = $vote_rounds[$c_id][$competition_round];

    $multe[$t_id][$round] += ($row->status == 0) ? 1 : 0;
  }

  $num_multe = array(); $tot_multe = array();

  if ($multe) {

    $header[] = "Squadra";
    $header[] = "Giornate";

    $teams = get_teams();

    foreach ($teams as $t_id => $team) {
      $key = ucwords($team->name);
    }

    $tot_multe = 0;

    foreach ($multe as $t_id => $multe_team) {

      $key = ucwords($teams[$t_id]->name);
      $rows[$key][] = $key;

      $multe_rounds = "";

      foreach ($multe_team as $round => $multa) {
        
        if ($multa != 0) {
          $num_multe[$t_id]++;
          $multe_rounds .= $round . ", ";
        }
        
        $last_user_round = $round;
      }

      $rows[$key][] = substr($multe_rounds, 0, -2);

      $tot_multa[$t_id] = ($num_multe[$t_id]);

    }
    
    foreach ($tot_multa as $t_id => $multa) {
      $key = ucwords($teams[$t_id]->name);
      $multa_team = fantacalcio_calcola_multe($multa);
      $rows[$key][] = $multa_team . " &euro;";
      $tot_multe += $multa_team;
    }

    $header[] = "Totale";

    ksort($rows);

    $rows['totale'] = array(
              array(
                "data" => "Totale",
                "colspan" => 2,
                "style" => "font-weight: bold;"
                ),
              array(
                "data" => $tot_multe . " &euro;",
                "style" => "font-weight: bold;"
                )
              );

    return theme_table($header, $rows);
  }
  else return "Nessuna multa";
}

function fantacalcio_calcola_multe($num_multe) {
  #calcola la multa totale per ciascuna squadra
  return $num_multe * ($num_multe +1) / 2;
}

/********************************/
/* END multe          */
/********************************/

/********************/
/* utility          */
/********************/
function choose_formazioni_round_form($form_state, $c_id, $round) {
  $round_formazioni = get_round_formazioni($c_id);

  #$competition_name = get_competition_name($c_id);

  #$out = "\n<h4>" . drupal_get_form("choose_formazioni_round_form", $c_id, $round_formazioni, $arg_round) . "</h4>"; 

  #return $out;

  if ($round_formazioni) {

    $form['round'] = array(
      '#type' => 'select',
      '#options' => $round_formazioni,
      '#default_value' => $round,
      '#attributes' => array("data-theme" => "c", "data-mini" => "false", "data-inline" => "true", "onchange" => "this.form.submit()")
    );

    $form['c_id'] = array(
      '#type' => 'hidden',
      '#default_value' => $c_id,
      '#value' => $c_id,
    );


    $form['submit'] = array(
      '#type' => 'submit',
      '#attributes' => array("class" => "hide")
    );
  
    return $form;

  }
}

function choose_formazioni_round_form_submit($form, &$form_state) {

  $c_id = $form_state['values']['c_id'];
  $round = $form_state['values']['round'];

  drupal_goto("formazioni/view/" . get_competition_name($c_id) . "/" . $round);
}

function filter_teams_formazioni($teams = array(), $args = array()) {

  $c_id = (isset($args['c_id']) && !empty($args['c_id'])) ? $args['c_id'] : variable_get("fantacalcio_main_competition", 1);

  $round = (isset($args['round']) && !empty($args['round'])) ? $args['round'] : get_last_round_formazione($c_id);

  $sql = "SELECT DISTINCT t_id FROM {fanta_lineups} 
      WHERE c_id = '%d' 
      AND round = '%d'
	  AND position > 0";
  $result = db_query($sql, $c_id, $round);
  $teams_ok = array();
  while ($row = db_fetch_array($result)) {
    $teams_ok[] = $row['t_id'];
  }

  foreach ($teams as $t_id => $team) {
    if (!in_array($t_id, $teams_ok))
      unset($teams[$t_id]);
  }
  return $teams;
}

function get_round_formazioni($c_id) {
  //giornate della competizione
  $competition_rounds = get_competition_rounds($c_id);

  //filtro le giornate (solo quelle con formazioni)
  $sql = "SELECT DISTINCT f.round, r.round_label FROM {fanta_lineups} f, {fanta_rounds_competitions} r 
          WHERE r.competition_round = f.round 
          AND f.c_id = r.c_id
          AND f.c_id = '%d'";
  $result = db_query($sql, $c_id);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->round] = filter_xss((!empty($row->round_label) ? $row->round_label : $row->round . "a Giornata"));
  }

  if ($rounds) {
    return $rounds;
  }
  else
    return array();
}

function get_line_up($c_id, $t_id, $round){
  $line_up = array();

  $sql = "SELECT * FROM {fanta_squads} r, {fanta_players} p 
          WHERE r.pl_id = p.pl_id
          AND r.t_id = '%d'
          AND r.in_team = 1
          ORDER BY p.role";
  $result = db_query($sql, $t_id);
  while ($row = db_fetch_object($result)) {
    $line_up[$row->pl_id] = array("role" => $row->role, "position" => "0");
  }

  $sql = "SELECT * FROM {fanta_lineups} f, {fanta_players} p 
          WHERE f.pl_id = p.pl_id
          AND f.c_id = '%d' AND f.t_id = '%d' AND f.round = '%d'
          ORDER BY f.position, p.role";
  $result = db_query($sql, $c_id, $t_id, $round);
  while ($row = db_fetch_object($result)) {
    $line_up[$row->pl_id] = (Object) array("role" => $row->role, "position" => $row->position);
  }

  return $line_up;
}

function position_button($pl_id, $t_id, $c_id, $role, $position = "") {
  $positions = array("Tribuna", "Titolare", "Riserva 1", "Riserva 2");
  $position = ($position == "") ? 0 : $position;
  $position = ($position > 3) ? 3 : $position;
  
  $display = array("none", "none", "none", "none");
  $display[$position] = "block";
  
  $out = "";
  $out .= "<a id='" . $pl_id . "_0' data-role=button data-mini='true' data-iconpos='notext' class='tribuna' onclick='changePosition(" . $pl_id . ", " . $t_id . ", " . $c_id . ", " . $role . ", 1)'  style='display: " . $display[0] . "'>Tribuna</a>";
  $out .= "<a id='" . $pl_id . "_1' data-role=button data-mini='true' data-iconpos='notext' class='titolare' onclick='changePosition(" . $pl_id . ", " . $t_id . ", " . $c_id . ", " . $role . ", 2)'  style='display: " . $display[1] . "'>Titolare</a>";
  $out .= "<a id='" . $pl_id . "_2' data-role=button data-mini='true' data-iconpos='notext' class='riserva1' onclick='changePosition(" . $pl_id . ", " . $t_id . ", " . $c_id . ", " . $role . ", 3)' style='display: " . $display[2] . "'>Riserva 1</a>";
  $out .= "<a id='" . $pl_id . "_3' data-role=button data-mini='true' data-iconpos='notext' class='riserva2' onclick='changePosition(" . $pl_id . ", " . $t_id . ", " . $c_id . ", " . $role . ", 0)' style='display: " . $display[3] . "'>Riserva 2</a>";
  
  return $out;
}

function fantacalcio_formazione_exists($t_id, $c_id, $round) {

  $sql = "SELECT * FROM {fanta_lineups} " .
      "WHERE t_id = '%d' " .
      "AND c_id = '%d' " .
      "AND round = '%d'";
  $result = db_query($sql, $t_id, $c_id, $round);
  if (db_affected_rows($result) > 0) return TRUE;
  else return FALSE;

}

function fantacalcio_formazione_check($line_up){
    //massima posizione
	$max_position = 0;

    foreach($line_up as $pl_id => $player) {
		if ($player->position > $maxPosition)
			$max_position = $player->position;
	}

	//inizializzo gli array
	$positions = array();
	for($j = 0; $j <= $max_position; $j++){
		$positions[$j] = array(0, 0, 0, 0);
	}

	//conto gli elementi (per posizione e per ruolo)
	foreach($line_up as $pl_id => $player) {
		$currPosition = $player->position;
		$currRole = $player->role;
		$positions[$currPosition][$currRole] = $positions[$currPosition][$currRole] + 1;
	}
		
	//moduli consentiti e numero giocatori
	$check_number_1 = TRUE; 
    $check_module_1 = TRUE; 
    $check_number_2_3 = TRUE; 
    $check_module_2_3 = TRUE; 
	$modules_1 = array(array(1, 3, 4, 3), array(1, 3, 5, 2), array(1, 4, 3, 3), array(1, 4, 4, 2), array(1, 4, 5, 1), array(1, 5, 3, 2), array(1, 5, 4, 1), array(1, 6, 3, 1)); //TODO prenderli da variable_get
	$modules_2 = array(array(1, 1, 1, 1));
	$modules_3 = array(array(0, 1, 1, 1));
	
	//verifico titolari
	if ($positions[1] != null ) {
		//numero titolari
		$number_1 = $positions[1][0] + $positions[1][1] + $positions[1][2] + $positions[1][3];
		if($number_1 != 11)
          $check_number_1 = FALSE;

		//modulo titolari
		$module_1 = array($positions[1][0], $positions[1][1], $positions[1][2], $positions[1][3]);
		if(!in_array($module_1, $modules_1))
          $check_module_1 = FALSE;
	}
	else {
	  $check_number_1 = FALSE; 
      $check_module_1 = FALSE; 
	}

	//verifico riserve
	if ($positions[2] != null && $positions[3] != null) {
		//numero riserve
		$number_2_3 = $positions[2][0] + $positions[2][1] + $positions[2][2] + $positions[3][3] + $positions[3][0] + $positions[3][1] + $positions[3][2] + $positions[3][3];
		if($number_2_3 != 7)
          $check_number_2_3 = FALSE;

		//modulo riserve
		$module_2 = array($positions[2][0], $positions[2][1], $positions[2][2], $positions[2][3]);
		$module_3 = array($positions[3][0], $positions[3][1], $positions[3][2], $positions[3][3]);
		if(!in_array($module_2, $modules_2) || !in_array($module_3, $modules_3)) 
          $check_module_2_3 = FALSE;
	}
	else {
	  $check_number_2_3 = FALSE; 
      $check_module_2_3 = FALSE; 
	}
	
  return array('number_1' => $check_number_1, 'module_1' => $check_module_1, 'number_2_3' => $check_number_2_3, 'module_2_3' => $check_module_2_3);
}

function check_formazione_insert($t_id, $c_id, $round) {
  $sql = "SELECT i_id FROM {fanta_lineups_inserts} " .
      "WHERE t_id = '%d' " .
      "AND c_id = '%d' " .
      "AND round = '%d'";
  $result = db_query($sql, $t_id, $c_id, $round );
  if (db_result($result))
    return "<img src='" . base_path() . drupal_get_path('module', 'fantacalcio') . "/images/ok.png' title='Formazione già inserita'>";
  else 
    return "<img src='" . base_path() . drupal_get_path('module', 'fantacalcio') . "/images/no.png' title='Formazione non inserita'>";
}

function show_line_up_check($checks) {

  if (is_mobile()) {
    $out = "<ul data-role='listview' data-theme='d'>
			<li><img id='check_number_1' src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($checks["number_1"]) . "' class='ui-li-icon'>Numero Titolari</li>
			<li><img id='check_module_1' src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($checks["module_1"]) . "' class='ui-li-icon'>Modulo Titolari</li>
			<li><img id='check_number_2_3' src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($checks["number_2_3"]) . "' class='ui-li-icon'>Numero Riserve</li>
			<li><img id='check_module_2_3' src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($checks["module_2_3"]) . "' class='ui-li-icon'>Modulo Riserve</li>
			<li data-role='list-divider'>" . t("Legenda") . "</li>
			<li><img class='legenda legenda-titolare ui-li-icon' src=''>" . t("Titolare") . "</li>
			<li><img class='legenda legenda-riserva-1 ui-li-icon' src=''>" . t("Prima riserva") . "</li>
			<li><img class='legenda legenda-riserva-2 ui-li-icon' src=''>" . t("Seconda riserva") . "</li>
			<li><img class='legenda legenda-tribuna ui-li-icon' src=''>" . t("Tribuna") . "</li>
		</ul>";
		
    return $out;
  }
  else {
    $rows = array();
    $rows[0] = array("data" => array(t("Numero Titolari"), "<img id='check_number_1' src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($checks["number_1"]) . "'>", 
                                   t("Numero Riserve"),  "<img id='check_number_2_3' src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($checks["number_2_3"]) . "'>" ));
    $rows[1] = array("data" => array(t("Modulo Titolari"), "<img id='check_module_1' src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($checks["module_1"]) . "'>", 
                                   t("Modulo Riserve"), "<img id='check_module_2_3' src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($checks["module_2_3"]) . "'>" ));					   
								   
    return theme_table(array(), $rows);
  }
}

function _get_image_check($bool) {
  return "<img src='" . drupal_get_path("module", "fantacalcio") . "images/" . ($bool ? "yes.png" : "no.png") . "'>";
}
/********************/
/* END utility      */
/********************/
