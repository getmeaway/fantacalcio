<?php

/**
  * @file fantacalcio.risultati.inc
*/

/************************/
/* GESTIONE RISULTATI  */
/************************/

function fantacalcio_risultati($round = '', $action = '') {

  $round = (is_numeric($round)) ? $round : get_last_votes() + 1;

  $out = '';

  $order = 1;

  $actions = array(
    'giornata' => array(
      'name' => 'Scegli giornata',
      'function' => 'scegli_giornata',
      'order' => $order++
    ),
    'formazioni' => array(
      'name' => 'Controlla Formazioni',
      'function' => 'controlla_formazioni',
      'order' => $order++
    ),
    'voti' => array(
      'name' => 'Carica Voti',
      'function' => 'upload_votes',
      'order' => $order++
    ),
    'titolari' => array(
      'name' => 'Trova titolari',
      'function' => 'trova_titolari',
      'order' => $order++
    ),
    'modificatori' => array(
      'name' => 'Modificatori',
      'function' => 'calcola_modificatori',
      'order' => $order++
    ),
    'totali' => array(
      'name' => 'Calcola totali',
      'function' => 'calcola_totali',
      'order' => $order++
    ),

    'passaggio' => array(
      'name' => 'Passaggio Turno',
      'function' => 'passaggio_turno',
      'order' => $order++
    ),
    
    'news' => array(
      'name' => 'Inserisci news',
      'function' => 'crea_news',
      'order' => $order++
    ),
  );
    
  if (module_exists('bets'))
    $actions['scommesse'] = array(
      'name' => 'Risultati scommesse',
      'function' => 'calcola_scommesse',
      'order' => $order++
    );

  if (module_exists('fantasfide'))
    $actions['sfide'] = array(
      'name' => 'Aggiorna sfide',
      'function' => 'aggiorna_sfide',
      'order' => $order++
    );

  if (module_exists('simplenews'))
    $actions['newsletter'] = array(
      'name' => 'Invia newsletter',
      'function' => 'send_newsletter',
      'order' => $order++
    );

  $out = "\n<div id='actions'>";
  $out .= get_actions_list($actions, $action, $round);
  $out .= "\n</div>";

  if (!empty ($action) && array_key_exists($action, $actions)) {
    $function = "fantacalcio_" . $actions[$action]['function'];

    $out .= "\n<div id='data'>";
    $out .= $function ($round);
    $out .= "\n</div>";

  }

  return $out;

}

function get_actions_list($actions, $action = '', $round) {

  foreach ($actions as $act) {
    $order[] = $act["order"];
  }

  array_multisort($order, $actions);

  $this_order = $actions[$action]['order'];

  $out .= "\n<ul>";
  foreach ($actions as $key => $act) {
    if ($key == $action) {
      $out .= "\n<li class='selected'>" . $act['name'] . "</li>";
    }
    elseif ($act['order'] == ($this_order + 1)) $out .= "\n<li><a href='" . base_path() . arg(0) . "/" . $round . "/" . $key . "'>" . $act['name'] . "</a></li>";
    elseif ($act['order'] < ($this_order)) $out .= "\n<li class='done'>" . $act['name'] . "</li>";
    else
      $out .= "\n<li>" . $act['name'] . "</li>";
  }

  $out .= "\n</ul>";

  return $out;
}


function fantacalcio_risultati_calcolati() {

  $leagues = get_competitions();
  $calcolati = 0;

  foreach ($competitions as $c_id => $competition) {

    $last_round = get_last_formazioni($c_id);

    $n_played = get_competition_played($c_id);

    if ($n_played == $last_round)$calcolati++;
  }
  if ($calcolati == 0)
    return FALSE;
  else
    return TRUE;

}

/************************/
/* scegli giornata      */
/************************/

function fantacalcio_scegli_giornata($round) {
	drupal_set_title(filter_xss('Risultati ' . $round . '&ordf; giornata'));
	return drupal_get_form('risultati_choose_round_form', $round);
}

function risultati_choose_round_form($form_state, $round) {
  $rounds = get_rounds_list_with_status();

  $form['round'] = array(
    '#type' => 'select',
    '#default_value' => $round,
	'#options' => $rounds,
    '#title' => t('Giornata'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Scegli')
  );

  return $form;
}

function risultati_choose_round_form_submit($form, &$form_state) {
  $round = $form_state["values"]["round"];
  drupal_goto('risultati/' . $round . '/giornata');
}

/************************/
/* END scegli giornata  */
/************************/

/************************/
/* formazioni      */
/************************/

function fantacalcio_controlla_formazioni($vote_round) {

  drupal_set_title(filter_xss('Risultati ' . $vote_round . '&ordf; giornata'));
  
  $teams = get_teams();
  $competitions = get_competitions();

  $formazioni = array();
  foreach ($competitions as $c_id => $competition) {
    $result = db_query("SELECT c.competition_round, r.date 
              FROM {fanta_rounds} r, {fanta_rounds_competitions} c
              WHERE c.round = r.round
              AND c.c_id = '%d' 
              AND c.round = '%d'", $c_id, $vote_round);
    while ($row = db_fetch_object($result)) {
      $last_round = $row->competition_round;
      $ts_round = $row->date;
    }
    
    if (db_affected_rows($result) > 0) {
    
      $formazioni = array();

      $sql = "SELECT * FROM {fanta_matches} " .
          "WHERE g_id IN (SELECT g_id FROM {fanta_groups WHERE c_id = '%d') " .
          "AND round = '%d'";
      $result = db_query($sql, $c_id, $last_round);
      while ($row = db_fetch_object($result)) {
        $competition_teams[$c_id][] = $row->t1_id;
        $competition_teams[$c_id][] = $row->t2_id;
      }

      $sql = "SELECT * FROM {fanta_lineups} f
          INNER JOIN {fanta_lineups_inserts} i
          ON f.c_id = i.c_id
          AND f.round = i.round
          AND f.t_id = i.t_id
          WHERE  f.c_id = '%d' 
          AND f.round = '%d'
          AND i.i_id = (SELECT MAX(i_id) FROM {fanta_lineups_inserts} WHERE c_id = '%d' AND round = '%d' AND t_id = f.t_id)";
      $result = db_query($sql, $c_id, $last_round, $c_id, $last_round);
      while ($row = db_fetch_object($result)) {
        $formazioni[$row->t_id][] = $row;
      }
      
      $rows = array();
      
      if ($competition_teams[$c_id]) {
        
        $out  .= "<h3>" . $competitions[$c_id]->name . "</h3>";

        foreach ($competition_teams[$c_id] as $t_id) {

          $formazione = $formazioni[$t_id];

          $modulo = array();

          if ($formazione) {

            foreach ($formazione as $player) {
              if ($player->position > 0) $modulo[$player->position]++;
            }

            if ($modulo == array(1 => 11, 2 => 4, 3 => 3))  $status = "<span style='color: green; font-weight:bold;'>OK</span>";
            else $status = "<span style='color: red; font-weight:bold;'>NO</span>";
            ksort($modulo);
            
            $ts = ($player->timestamp < $ts_round ) ? date('d-m-Y H:i:s', $player->timestamp) : "<span style='color: red; font-weight:bold;'>" . date('d-m-Y H:i:s', $player->timestamp) . "</span>";

            $rows[$t_id] = array(l($teams[$t_id]->name, "formazioni/admin/" . $t_id), implode(" - ", $modulo), $status, $ts);
          }
          else $rows[$t_id] = array(l($teams[$t_id]->name, "formazioni/admin/" . $t_id), array("data" => "<span style='color: red; font-weight:bold;'>NON INSERITA</span>", "colspan" => 2));

        }

        $out .= theme_table(array(), $rows);
      }
    }
  }

  return $out;
}


function fantacalcio_admin_formazione($t_id = "", $competition_name = '') {

  if (!team_exists($t_id)) return choose_team_formazione_admin();

  else {
    drupal_set_title(filter_xss("Amministra formazioni - " . get_team_name($t_id) ));
    return fantacalcio_formazione_insert_start_admin($competition_name, $t_id);
  }
}

function fantacalcio_admin_penalties($t_id = "", $competition_name = '') {

  if (!team_exists($t_id)) return choose_team_formazione_admin_penalties();

  else return fantacalcio_formazione_admin_penalties($competition_name, $t_id);
}

function choose_team_formazione_admin() {
  $out = '<br/>' . l("Amministra rigoristi &raquo;", "formazioni/admin/penalties", array("html" => TRUE)) . '<br/><br/>';

  $teams = get_teams();

  foreach ($teams as $t_id => $team) {
    $team_names[] = $team->name;
  }

  array_multisort($team_names, $teams);

  $out .= "<ul>";
  foreach ($teams as $t_id => $team) {
    $out .= "<li>" . l($teams[$t_id]->name, "formazioni/admin/" . $teams[$t_id]->t_id) . "</li>";
  }
  $out .= "</ul>";

  return $out;
}

function choose_team_formazione_admin_penalties() {
  $out = '<br/>' . l("Amministra formazioni &raquo;", "formazioni/admin", array("html" => TRUE)) . '<br/><br/>';

  $teams = get_teams();

  foreach ($teams as $t_id => $team) {
    $team_names[] = $team->name;
  }

  array_multisort($team_names, $teams);

  $out .= "<ul>";
  foreach ($teams as $t_id => $team) {
    $out .= "<li>" . l($teams[$t_id]->name, "formazioni/admin/penalties/" . $teams[$t_id]->t_id) . "</li>";
  }
  $out .= "</ul>";

  return $out;
}

function fantacalcio_insert_formazione_admin($competition_name = '', $t_id) {

  global $user;

  $competitions = get_competitions();

  if (empty ($competition_name) || !is_competition_name($competition_name)) {
    #controllo quante formazioni vanno inserite per la giornata
    $vote_round = get_last_votes() or 0;
    $vote_round++;
    $sql = "SELECT * FROM {fanta_rounds_competitions} WHERE round = '%d'";
    $result = db_query($sql, $vote_round);
    while ($row = db_fetch_array($result)) {
      $sqla = "SELECT * FROM {fanta_matches} " .
          "WHERE g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id =  '%d') " .
          "AND round = '%d' " .
          "AND (t1_id = '%d' OR t2_id = '%d')";
      $resulta = db_query($sqla, $row['c_id'], $row['competition_round'], $t_id, $t_id);
      while ($rowa = db_fetch_array($resulta)) {
        $formazioni[$row['c_id']] = $rowa['round'];
      }
    }

    if (count($formazioni) > 1) {
    
      $teams = get_teams();
      $out = "<h3>" . $teams[$t_id]->name . "</h3>";

      $out .= "<p>Inserire una formazione per ogni partita</p>";

      $out .= "<ul>";
      foreach ($formazioni as $c_id => $round) {
        $competition = $competitions[$c_id];
        $out .= "\n<li><a href='" . base_path() . "formazioni/admin/" . $t_id . "/" . $competition->name . "'>" . $competition->name . " (" . $round . "&ordf; Giornata)</a></li>";
      }
      $out .= "</ul>";
      return $out;
    }
    elseif (count($formazioni) == 1) {
    
      foreach ($formazioni as $c_id => $round) $competition_name = $competitions[$c_id]->name;

      return insert_formazione_admin($competition_name, $t_id);
    }
  }

  else {
  
    return insert_formazione_admin($competition_name, $t_id);
  }

}

function fantacalcio_formazione_admin_penalties  ($competition_name = '', $t_id) {

  global $user;

  $competitions = get_competitions();

  if (empty ($competition_name) || !is_competition_name($competition_name)) {
    #controllo quante formazioni vanno inserite per la giornata
    $vote_round = get_last_votes() or 0;
    $vote_round++;
    $sql = "SELECT * FROM {fanta_rounds_competitions} WHERE round = '%d' AND next = 1";
    $result = db_query($sql, $vote_round);
    while ($row = db_fetch_array($result)) {
      $sqla = "SELECT * FROM {fanta_matches} " .
          "WHERE g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id =  '%d') " .
          "AND round = '%d' " .
          "AND (t1_id = '%d' OR t2_id = '%d')";
      $resulta = db_query($sqla, $row['c_id'], $row['competition_round'], $t_id, $t_id);
      while ($rowa = db_fetch_array($resulta)) {
        $penalties[$row['c_id']] = $rowa['round'];
      }
    }

    if (count($penalties) > 1) {
    
      $teams = get_teams();
      $out = "<h3>" . $teams[$t_id]->name . "</h3>";

      $out .= "<p>Inserire una formazione per ogni partita</p>";

      $out .= "<ul>";
      foreach ($penalties as $c_id => $round) {
        $competition = $competitions[$c_id];
        $out .= "\n<li><a href='" . base_path() . "formazioni/admin/penalties/" . $t_id . "/" . $competition->name . "'>" . $competition->name . " (" . $round . "&ordf; Giornata)</a></li>";
      }
      $out .= "</ul>";
      return $out;
    }
    elseif (count($penalties) == 1) {
    
      foreach ($penalties as $c_id => $round) $competition_name = $competitions[$c_id]->name;

      //return insert_formazione_admin($competition_name, $t_id);
      return drupal_get_form('insert_penalties_form', $t_id, $c_id, $round);
    }
    
    else {
      return "La squadra non deve inserire rigoristi<br>" . l("Indietro", "formazioni/admin/penalties");
    }
  }

  else {
  
    //return insert_formazione_admin($competition_name, $t_id);
    return drupal_get_form('insert_penalties_form', $t_id, $c_id, $round);
  }

}


function insert_formazione_admin($competition_name, $t_id) {

  $c_id = get_competition_id($competition_name);
  $round = get_next_round_formazione($c_id);
  $teams = get_teams();

  $out = '';

  $ts_match = get_timestamp_match($c_id, $round);

  $out .= "\n<h3>" . ucwords($competition_name) . "</h3>";
  $out .= "\n<h4>" . $round . "&#7491; giornata</h4>";
  $out .= "\n<h3>" . $teams[$t_id]->name . "</h3>";

  $args = array(
    't_id' => $t_id,
    'c_id' => $c_id,
    'round' => $round,
  );

  $formazione_form = drupal_get_form('fantacalcio_formazione_form', $args);

  $args_confirm = get_args_confirm($args);

  $out .= confirm_formazione($args_confirm);

  $validate = show_line_up_check(fantacalcio_formazione_check(get_line_up($c_id, $t_id, $round)));

  $out .= "\n<div id='validate'>" . $validate . "</div>";



  $out .= "\n<div id='formazione_form'>" . $formazione_form . "</div>";

  $out .= "\n<div class='clear' style='height:10px;'></div>";

  $out .= "\n<div id='formazione'>" .
      make_formazione($t_id, $c_id, $round, TRUE) .
      "\n</div>";

  return $out;

}

/************************/
/* END formazioni    */
/************************/


/********************/
/* voti        */
/********************/

function fantacalcio_load_voti($round) {
  drupal_set_title(filter_xss('Risultati ' . $round . '&ordf; giornata'));

  $providers = get_vote_providers();
  $votes = get_votes_html_data($round, $providers);

  foreach($providers as $p_id => $provider) {
    insert_votes($votes[$p_id], $round, $p_id);
  }  
  return "";
}

function fantacalcio_upload_votes($round) {
  drupal_set_title(filter_xss('Risultati ' . $round . '&ordf; giornata'));

  if(variable_get('fantacalcio_votes_url', '') != "")
    $out = "<p>".l("Voti", variable_get('fantacalcio_votes_url', ''))."</p>";
	
  return $out.drupal_get_form('votes_upload_form', $round);
}


function votes_upload_form($form_state, $round) {

  //$form['#redirect'] = 'risultati/titolari';
  $last_round = 0;
  $teams_serieA = get_teams_serieA();
  $sql = "SELECT round FROM {fanta_rounds} WHERE status = 1 ORDER BY round";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->round] = $row->round;
    $last_round = $row->round;
  }
  $rounds[$last_round + 1] = $last_round + 1;
  
  $form["voto_default_1"] = array(
    "#type" => "fieldset",
    "#collapsed" => TRUE,
    "#collapsible" => TRUE,
    "#title" => "Voto default #1",
  );

  $form["voto_default_1"]["teams_default_1"] = array(
    "#type" => "checkboxes",
    "#title" => "Squadre",
    "#default_value" => array(),
    "#description" => "Squadre che non hanno giocato nella giornata (Voto default: " . variable_get("fantacalcio_default_vote_1", "6") . ")",
    "#options" => $teams_serieA,
  );
  
  $form["voto_default_2"] = array(
    "#type" => "fieldset",
    "#collapsed" => TRUE,
    "#collapsible" => TRUE,
    "#title" => "Voto default #2",
  );

  $form["voto_default_2"]["teams_default_2"] = array(
    "#type" => "checkboxes",
    "#title" => "Squadre",
    "#default_value" => array(),
    "#description" => "Squadre che non hanno giocato nella giornata (Voto default: " . variable_get("fantacalcio_default_vote_2", "6") . ")",
    "#options" => $teams_serieA,
  );

  $form['provider'] = array(
    '#type' => 'checkboxes',
	'#options' => array(1 => "Milano", 2 => "Roma", 3 => "Napoli"),
	"#default_value" => array(1, 2, 3),
    '#title' => t('Provider'),
  );

  $form['round'] = array(
    '#type' => 'hidden',
    '#default_value' => $round,
	'#value' => $round,
    '#title' => t('Giornata'),
  );

  $form["file"] = array(
    "#type" => "fieldset",
    "#collapsed" => TRUE,
    "#collapsible" => TRUE,
    "#title" => t("File di testo esterno"),
  );

  $form["file"]['text'] = array(
    '#type' => 'textarea',
    '#rows' => 20,
    '#cols' => 80,
    '#title' => t('File'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Carica'
  ));

  $form['clear'] = array(
    '#type' => 'submit',
    '#value' => 'Annulla',
    '#validate' => array(
      'upload_form_clear'
    ),

  );
  return $form;
}

function votes_upload_form_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function votes_upload_form_submit($form, &$form_state) {

  $text = $form_state["values"]["text"];
  $round = $form_state["values"]["round"];

  $votes = get_votes_html_data($round, $form_state["values"]["provider"]);

  foreach($form_state["values"]["provider"] as $provider) {
    $out = insert_votes($votes[$provider], $round, $provider);
    insert_default_votes($form_state["values"]["teams_default_1"], $round, 1, $provider);
    insert_default_votes($form_state["values"]["teams_default_2"], $round, 2, $provider);  
  }
    
  update_players($round + 1);
  
  //risultati provvisori
  db_query("UPDATE {fanta_rounds} SET status = -1 WHERE round = '%d'", $round);
  
  drupal_set_message('Voti caricati');
  drupal_set_message(check_plain($out));
}

function parse_votes_text($text, $round, $provider) {

  //spezzo il file in righe
  $file_righe = explode("\n", $text);
  $query_values = array();

  //n° righe
  $count_1 = count($file_righe);

  //inserisco ogni riga come un nuovo record della tabella 'fanta_votes'
  foreach ($file_righe as $key => $riga) {
    $riga = trim($riga);
    if (is_numeric(substr($riga, 0, 1))) {
      //separo i campi della riga con il carattere \t "tab"
      $elems = explode(chr(9), $riga);

      //se il ruolo è P,D,C,A => ok 
      if (in_array($elems[1], array("P", "D", "C", "A"))) {
        foreach ($elems as $id_elem => $elem) {
          $elem = trim($elem);
        }
        //controllo se ha preso SV
        if (!is_numeric($elems[3])) $elems[3] = strtr($elems[3], ',', '.');

        //giocatori senza voti ma con bonus e/o malus
        if (!is_numeric($elems[3]) || $elems[3] == 0) {
          //portiere SV
          if ($elems[1] == 'P') {
            $elems[3] = 6;
          }
          //non portiere con bonus/malus
          elseif (($elems[4] + $elems[5] + $elems[6] + $elems[7] + $elems[8] + $elems[9] + $elems[10] + $elems[11] + $elems[12] + $elems[13]) != 0) {
            $elems[3] = 6;
          } 
          // non inserire 
          else {
            unset($file_righe[$key]);
          }

        }

        //elimino il ruolo del giocatore dal voto
        if (isset ($file_righe[$key]))
          unset($elems[1]);
          
        //sommo assist da azione con assist da fermo
        //$elems[12] = $elems[12] + $elems[13];
  
        //elimino nome, assist da fermo, gol_vittoria e gol_pareggio
        unset($elems[2]);
        unset($elems[13]);
        unset($elems[14]);
        unset($elems[15]);        
        
        //trasformo il voto a un solo decimale
        $elems[3] = (strlen($elems[3]) == 4) ? substr($elems[3], 0, 3) : $elems[3];
        //echo $elems[3]." - ";

        if (isset ($file_righe[$key])) {
          $query_values[$key] = implode(',', $elems);
          $votes[$elems[0]][$provider] = array("vote" => $elems[3], "gol_fatto" => $elems[4], "gol_subito" => $elems[5], "rig_parato" => $elems[6], "rig_sbagliato" => $elems[7], "rig_fatto" => $elems[8], "autogol" => $elems[9], "amm" => $elems[10], "esp" => $elems[11], "assist" => $elems[12], "assist_fermo" => $elems[13] );
        }
      }
    }
    else unset($file_righe[$key]);
  }

  sort($query_values);
  array_unique($query_values);

  $tot = count($file_righe);

}

function insert_votes($votes, $round, $provider) {
  //VOTI della GIORNATA
  $sql = "DELETE FROM {fanta_votes} WHERE round = '%d'";

  if ($provider > 0) 
    $sql = "DELETE FROM {fanta_votes} WHERE round = '%d' AND provider = '%d'";

  $result = db_query($sql, $round, $provider);

  $sql = "INSERT INTO {fanta_votes} (round, pl_id, provider, vote, goals_for, goals_against, saved_penalties, missed_penalties, penalty_goals, own_goals, yellow_cards, red_cards, assists, win_goals, draw_goals) VALUES ";
  foreach ($votes as $pl_id => $vote) {
      $sql .= "\n($round, " . $pl_id . ", " . $provider . ", " . $vote["vote"] . ", " . $vote["goals_for"] . ", " . $vote["goals_against"] . ", " 
           . $vote["saved_penalties"] . ", " . $vote["missed_penalties"] . ", " . $vote["penalty_goals"] . ", " . $vote["own_goals"] . ", " . $vote["yellow_cards"] . ", " 
           . $vote["red_cards"] . ", " . $vote["assists"] . ", " . $vote["win_goals"] . ", " . $vote["draw_goals"] . "),";
  }
  $sql = substr($sql, 0, -1) . ";";
  $result = db_query($sql);

  //formula per calcolare totale
  $formula = " (vote " 
        . "+ (goals_for * " . variable_get("fantacalcio_points_goals_for" , "3") . ") "
        . "+ (penalty_goals * " . variable_get("fantacalcio_points_penalty_goals" , "3") . ") "
        . "+ (assists * " . variable_get("fantacalcio_points_assists" , "1") . ") "
        . "+ (saved_penalties  * " . variable_get("fantacalcio_points_saved_penalties" , "3") . ") "
        . "+ (goals_against * " . variable_get("fantacalcio_points_goals_against" , "-1") . ") "
        . "+ (red_cards * " . variable_get("fantacalcio_points_red_card" , "-1") . ") "
        . "+ (yellow_cards * " . variable_get("fantacalcio_points_yellow_card" , "-0.5") . ") "
        . "+ (own_goals * " . variable_get("fantacalcio_points_own_goal" , "-2") . ") "
        . "+ (missed_penalties * " . variable_get("fantacalcio_points_missed_penalties" , "-3") . ") "
        . "+ (draw_goals * " . variable_get("fantacalcio_points_draw_goals" , "1") . ") "
        . "+ (win_goals * " . variable_get("fantacalcio_points_win_goals" , "3") . ") "
        . ") ";
  $sql = "UPDATE {fanta_votes} SET total = " . $formula . " WHERE round = '%d'";
  if ($provider > 0) 
    $sql = "UPDATE {fanta_votes} SET total = " . $formula . " WHERE round = '%d' AND provider = '%d'";
  db_query($sql, $round, $provider);
  
  return $tot . " Voti Inseriti";

}

function insert_default_votes($teams = array(), $round, $number_default, $provider_id) {

  if ($teams) {
    $checked_teams = array();
    foreach ($teams as $t_id => $value) {
      if ($value != "0")  array_push($checked_teams, $value);

    }

    if ($checked_teams) {
      $sqla = "INSERT INTO {fanta_votes} (pl_id, provider, round, total, vote) VALUES ";

      $message = "Voto di default #" . $number_default . " (" . variable_get('fantacalcio_default_vote_' . $number_default , '6') . ") per i giocatori di";

      $team_sql = "";
      foreach ($checked_teams as $team) {
        $team_sql .= "'" . $team . "',";
        $message .= " " . $team . ",";
      }
      $team_sql = substr($team_sql, 0, -1);
      $message = substr($message, 0 , -1);
      drupal_set_message(check_plain($message));

      $sql = "SELECT DISTINCT pl_id FROM {fanta_players_teams} WHERE rt_id in (" . $team_sql . ") AND round = '%d'";
      $result = db_query($sql, ($round - 1)) ;
      while ($row = db_fetch_array($result)) {
        $sqla .= "\n(" . $row["pl_id"] . ", " . $provider_id . ", " . $round . ", " . variable_get('fantacalcio_default_vote_' . $number_default , '6') . ", " . variable_get('fantacalcio_default_vote_' . $number_default, '6') . "),";
      }
      $sqla = substr($sqla, 0, -1);

      $resulta = db_query($sqla);
    }
  }
}

function update_players($round) {
  $i = 0;
  $sqla = "INSERT INTO {fanta_players_teams} (pl_id, round, rt_id, active) VALUES";

  //cancello i giocatori della giornata per evitare doppioni
  $sql = "DELETE FROM {fanta_players_teams} WHERE round = '%d'";
  db_query($sql, $round );
  
  $sql = "SELECT DISTINCT pl_id, rt_id as team, active FROM {fanta_players_teams} WHERE round = '%d'";
  $result = db_query($sql, ($round - 1) );
  while ($row = db_fetch_object($result)) {
    $sqla .= " (" . $row->pl_id . ", " . $round . ", '" . $row->team . "', " . $row->active . "),";
    $i++;
  }
  $sqla = substr($sqla, 0, -1) ;
  db_query($sqla);
  $message = "Giocatori aggiornati: " . $i . " (Giornata n° " . $round . ")";
  drupal_set_message(check_plain($message));
}

function get_teams_serieA() {

  $sql = "SELECT rt_id, long_name FROM {fanta_real_teams} ORDER BY long_name";
  $result = db_query($sql);
  while ($row = db_fetch_array($result)) {
    $teams[$row["rt_id"]] = ucwords($row["long_name"]);
  }
  return $teams;
}

/********************/
/* END voti      */
/********************/



/************************/
/* calcola modificatori  */
/************************/

function fantacalcio_calcola_modificatori($vote_round) {
  drupal_set_title(filter_xss('Risultati ' . $vote_round . '&ordf; giornata'));
  
  if (
    variable_get('fantacalcio_modificatore_portiere', '0') 
    || variable_get('fantacalcio_modificatore_difesa', '0') 
    || variable_get('fantacalcio_modificatore_centrocampo', '0')
    || variable_get('fantacalcio_modificatore_attacco', '0')
    ) {

    $out = "";

    //$vote_round = get_last_votes();
    $teams = get_teams();
    $votes = get_votes($vote_round);
    $competitions = get_competitions();

    $matches = get_matches_vote_round($vote_round);
    
    foreach ($matches as $m_id => $match) {
      $t1_id = $match->t1_id;
      $t2_id = $match->t2_id;
      $competition_round = $match->round;
      $c_id = get_cid_by_gid($match->g_id);
      
      if (variable_get('fantacalcio_modificatore_portiere', '0')) {
        $mod_portiere_1 = fantacalcio_calcola_modificatori_portiere($t1_id, $c_id, $vote_round, $competition_round, variable_get("fantacalcio_votes_provider", 1));
        $mod_portiere_2 = fantacalcio_calcola_modificatori_portiere($t2_id, $c_id, $vote_round, $competition_round, variable_get("fantacalcio_votes_provider", 1));
      }
      else {
        $mod_portiere_1 = 0;
        $mod_portiere_2 = 0;
      }

      if (variable_get('fantacalcio_modificatore_difesa', '0')) {
        $mod_difesa_1 = fantacalcio_calcola_modificatori_difesa($t1_id, $c_id, $vote_round, $competition_round, variable_get("fantacalcio_votes_provider", 1));
        $mod_difesa_2 = fantacalcio_calcola_modificatori_difesa($t2_id, $c_id, $vote_round, $competition_round, variable_get("fantacalcio_votes_provider", 1));
      }
      else {
        $mod_difesa_1 = 0;
        $mod_difesa_2 = 0;
      }
      
      if (variable_get('fantacalcio_modificatore_centrocampo', '0')) {
        $mod_centrocampo = fantacalcio_calcola_modificatori_centrocampo($t1_id, $t2_id, $c_id, $vote_round, $competition_round, variable_get("fantacalcio_votes_provider", 1));
        $mod_centrocampo_1 = $mod_centrocampo[1];
        $mod_centrocampo_2 = $mod_centrocampo[2];
      }
      else {
        $mod_centrocampo_1 = 0;
        $mod_centrocampo_2 = 0;
      }
      
      if (variable_get('fantacalcio_modificatore_attacco', '0')) {
        $mod_attacco_1 = fantacalcio_calcola_modificatori_attacco($t1_id, $c_id, $vote_round, $competition_round, variable_get("fantacalcio_votes_provider", 1));
        $mod_attacco_2 = fantacalcio_calcola_modificatori_attacco($t2_id, $c_id, $vote_round, $competition_round, variable_get("fantacalcio_votes_provider", 1));
      }
      else {
        $mod_attacco_1 = 0;
        $mod_attacco_2 = 0;
      }

      $sql = "UPDATE {fanta_matches} SET 
          mod_por_1 = '%f',
          mod_por_2 = '%f',
          mod_dif_1 = '%f',
          mod_dif_2 = '%f',
          mod_centr_1 = '%f',
          mod_centr_2 = '%f',
          mod_att_1 = '%f',
          mod_att_2 = '%f'
          WHERE m_id = '%d'";
      $result = db_query($sql, $mod_portiere_1, $mod_portiere_2, $mod_difesa_1, $mod_difesa_2, $mod_centrocampo_1, $mod_centrocampo_2, $mod_attacco_1, $mod_attacco_2, $match->m_id);
      
    }

    $sqlx = "SELECT * FROM {fanta_rounds_competitions} WHERE round = '%d'";
    $resultx = db_query($sqlx, $vote_round);
    while ($rowx = db_fetch_array($resultx)) {
      $c_id = $rowx['c_id'];
      $competition_round = $rowx['competition_round'];

      $sql = "SELECT * FROM {fanta_matches} " .
          "WHERE g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id =  '%d') " .
          "AND round = '%d' ";
      $result = db_query($sql, $c_id, $competition_round);
      $out .= "<h3>" . check_plain($competitions[$c_id]->name) . "</h3>";

      $header = array("Squadra", "Portiere", "Difesa", "Centrocampo", "Attacco");
      $rows = array();

      while ($row = db_fetch_array($result)) {
        $rows[$row['m_id'] . "_1"][] = $teams[$row['t1_id']]->name;
        $rows[$row['m_id'] . "_1"][] = $row['mod_por_1'];
        $rows[$row['m_id'] . "_1"][] = $row['mod_dif_1'];
        $rows[$row['m_id'] . "_1"][] = $row['mod_centr_1'];
        $rows[$row['m_id'] . "_1"][] = $row['mod_att_1'];
        $rows[$row['m_id'] . "_2"][] = $teams[$row['t2_id']]->name;//squadra 2
        $rows[$row['m_id'] . "_2"][] = $row['mod_por_2'];
        $rows[$row['m_id'] . "_2"][] = $row['mod_dif_2'];
        $rows[$row['m_id'] . "_2"][] = $row['mod_centr_2'];
        $rows[$row['m_id'] . "_2"][] = $row['mod_att_2'];
        
        $rows[$row['m_id'] . "_3"][] = array("data" => "<hr>", "colspan" => 5);//separatore
      }

      $out .= theme_table($header, $rows);

    }
  }
  else $out = "No modificatori";
  
  return $out;

}

function fantacalcio_calcola_modificatori_portiere($t_id, $c_id, $vote_round, $competition_round, $votes_provider) {
  $sql = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
      "WHERE f.pl_id = p.pl_id " .
      "AND p.role = 0 " .
      "AND f.c_id = '%d' " .
      "AND f.round = '%d' " .
      "AND f.t_id = '%d' " .
      "AND f.has_played = 1";
  $result = db_query($sql, $c_id, $competition_round, $t_id);
  while ($row = db_fetch_array($result)) {
    $pl_id = $row['pl_id'];
  }
  
  if ($pl_id) {
    $sql = "SELECT * FROM {fanta_votes} " .
    "WHERE pl_id = '%d' " .
    "AND round = '%d' " . 
	"AND provider = '%d'";
    $result = db_query($sql, $pl_id, $vote_round, $votes_provider);
    while ($row = db_fetch_object($result)) {
      if ($row->rig_parato == 0 && $row->vote > 6) return $row->vote - 6;
      else return 0;
    }
  }
  else return 0;
}

function fantacalcio_calcola_modificatori_difesa($t_id, $c_id, $vote_round, $competition_round, $votes_provider) {

  $dif_ids = array();
  $n_dif_titolari = 0;
  $n_dif_entrati = 0;
  $sql = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
        "WHERE f.pl_id = p.pl_id " .
        "AND p.role = 1 " .
        "AND f.c_id = '%d' " .
        "AND f.round = '%d'" .
        "AND f.t_id = '%d'";
        
  $result = db_query($sql, $c_id, $competition_round, $t_id);
  while ($row = db_fetch_array($result)) {

    if ($row['has_played'] == 1) {
      array_push($dif_ids, $row['pl_id']);
      $n_dif_entrati++;
    }

    if ($row['position'] == 1)
      $n_dif_titolari++;

  }

  $elenco_dif_ids = implode(',', $dif_ids);

  $tot_difesa = 0;
  $sql = "SELECT DISTINCT pl_id, vote FROM {fanta_votes} " .
      "WHERE pl_id IN ($elenco_dif_ids) " .
      "AND round = '%d' " . 
	  "AND provider = '%d'";
  $result = db_query($sql, $vote_round, $votes_provider);
  while ($row = db_fetch_array($result)) {
    $tot_difesa += $row['vote'];
  }

  $tot_difesa += 5 * ($n_dif_titolari - $n_dif_entrati);
  $avg_difesa = round($tot_difesa / $n_dif_titolari, 2);

  $mod_difesa = 27 - $n_dif_titolari - floor($avg_difesa * 4);

  #echo "$t_id : $tot_difesa : $avg_difesa : $n_dif_titolari : $mod_difesa <br>";

  return $mod_difesa;
}

function fantacalcio_calcola_modificatori_centrocampo($t1_id, $t2_id, $c_id, $vote_round, $competition_round, $votes_provider) {

  $centr_t1_ids = array();
  $n_centr_t1_titolari = 0;
  $n_centr_t1_entrati = 0;
  $centr_t2_ids = array();
  $n_centr_t2_titolari = 0;
  $n_centr_t2_entrati = 0;

  #team 1
  $sql = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
    "WHERE f.pl_id = p.pl_id " .
    "AND p.role = 2 " .
    "AND f.c_id = '%d' " .
    "AND f.round = '%d'" .
    "AND f.t_id = '%d'";
  $result = db_query($sql, $c_id, $competition_round, $t1_id);
  while ($row = db_fetch_array($result)) {
    if ($row['has_played'] == 1) {
      array_push($centr_t1_ids, $row['pl_id']);
      $n_centr_t1_entrati++;
    }

    if ($row['position'] == 1)
      $n_centr_t1_titolari++;

  }

  #team 2
  $sql = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
    "WHERE f.pl_id = p.pl_id " .
    "AND p.role = 2 " .
    "AND f.c_id = '%d' " .
    "AND f.round = '%d'" .
    "AND f.t_id = '%d'";
  $result = db_query($sql, $c_id, $competition_round, $t2_id);
  while ($row = db_fetch_array($result)) {
    if ($row['has_played'] == 1) {
      array_push($centr_t2_ids, $row['pl_id']);
      $n_centr_t2_entrati++;
    }

    if ($row['position'] == 1)
      $n_centr_t2_titolari++;

  }

  $elenco_centr_t1_ids = implode(',', $centr_t1_ids);
  $elenco_centr_t2_ids = implode(',', $centr_t2_ids);

  $tot_centr_t1 = 0;
  $tot_centr_t2 = 0;

  #voti team 1
  $sql = "SELECT DISTINCT pl_id, vote  FROM {fanta_votes} " .
    "WHERE pl_id IN ($elenco_centr_t1_ids) " .
    "AND round = '%d' " . 
	"AND provider = '%d'";
  $result = db_query($sql, $vote_round, $votes_provider);
  while ($row = db_fetch_array($result)) {
    $tot_centr_t1 += $row['vote'];
  }

  #voti team 2
  $sql = "SELECT DISTINCT pl_id, vote  FROM {fanta_votes} " .
    "WHERE pl_id IN ($elenco_centr_t2_ids) " .
    "AND round = '%d' " . 
    "AND provider = '%d'";
  $result = db_query($sql, $vote_round, $votes_provider);
  while ($row = db_fetch_array($result)) {
    $tot_centr_t2 += $row['vote'];
  }

  $tot_centr_t1 += 5 * ($n_centr_t1_titolari - $n_centr_t1_entrati);
  $tot_centr_t2 += 5 * ($n_centr_t2_titolari - $n_centr_t2_entrati);

  if ($n_centr_t1_titolari > $n_centr_t2_titolari)
    $tot_centr_t2 += 5 * ($n_centr_t1_titolari - $n_centr_t2_titolari);

  if ($n_centr_t2_titolari > $n_centr_t1_titolari)
    $tot_centr_t1 += 5 * ($n_centr_t2_titolari - $n_centr_t1_titolari);

  $scarto = $tot_centr_t1 - $tot_centr_t2;

  $scarto = floor(abs($scarto)) / 2;

  if ($tot_centr_t1 > $tot_centr_t2)
    $mod_centrocampo = array(
      1 => $scarto,
      2 => 0 - $scarto
    );

  if ($tot_centr_t1 < $tot_centr_t2)
    $mod_centrocampo = array(
      1 => 0 - $scarto,
      2 => $scarto
    );

  if ($tot_centr_t1 == $tot_centr_t2)
    $mod_centrocampo = array(
      1 => 0,
      2 => 0
    );

  return $mod_centrocampo;

}

function fantacalcio_calcola_modificatori_attacco($t2_id, $c_id, $vote_round, $competition_round, $votes_provider) {

  $mod_att = 0;

  $sql = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
  "WHERE f.pl_id = p.pl_id " .
  "AND p.role = 3" .
  "AND f.c_id = '%d' " .
  "AND f.round = '%d' " .
  "AND f.t_id = '%d' " .
  "AND f.has_played = 1";
  $result = db_query($sql, $c_id, $competition_round, $t_id);
  while ($row = db_fetch_array($result)) {
    $pl_id = $row['pl_id'];
  
    if ($pl_id) {
      $sql = "SELECT * FROM {fanta_votes} " .
      "WHERE pl_id = '%d' " .
      "AND round = '%d' " . 
      "AND provider = '%d'";
      $result = db_query($sql, $pl_id, $vote_round, $votes_provider);
      while ($row = db_fetch_object($result)) {
        if ($row->assist == 0 && $row->gol_fatto == 0 && $row->vote > 6) $mod_att += ($row->vote - 6);
      }
    }
  }
  
  return $mod_att;
}

/************************/
/* END modificatori    */
/************************/

/************************/
/* titolari        */
/************************/

function fantacalcio_trova_titolari($vote_round) {
  drupal_set_title(filter_xss('Risultati ' . $vote_round . '&ordf; giornata'));

  $out = "";

//  $vote_round = get_last_votes();

  $teams = get_teams();
  $votes = get_votes($vote_round);
  $competitions = get_competitions();

  $pl_votes = array();
  foreach ($votes as $vote)
    $pl_votes[] = $vote->pl_id;

  $elenco_pl_ids = implode(',', $pl_votes);
  
  $sqlx = "SELECT * FROM {fanta_rounds_competitions} " .
      "WHERE round = '%d'";
  $resultx = db_query($sqlx, $vote_round);
  while ($row = db_fetch_array($resultx)) {

    $c_id = $row['c_id'];
    $competition_round = $row['competition_round'];
        
    $out .= "<h3>" . $competitions[$c_id]->name . "</h3>";
    
    //resetto i valori 
    $sql = "UPDATE {fanta_lineups} 
      SET has_played = 0 
      WHERE round = '%d' 
      AND c_id = '%d'";
    $result = db_query($sql, $competition_round, $c_id);

    #titolari con voto
    $sql = "UPDATE {fanta_lineups} " .
      "SET has_played = 1 " .
      "WHERE position = 1 " .
      "AND round = '%d' " .
      "AND c_id = '%d'" .
      "AND pl_id IN ($elenco_pl_ids)";
      //echo $sql;
    $result = db_query($sql, $competition_round, $c_id);
    
    switch (variable_get("fantacalcio_riserve_fisse", 0) ) {
      case 0: //riserve libere
        fantacalcio_get_titolari_riserve_libere($pl_votes, $competition_round, $c_id);
        break;
      case 1: //riserve fisse
        fantacalcio_get_titolari_riserve_fisse($pl_votes, $competition_round, $c_id);
        break;
      default:
        fantacalcio_get_titolari_riserve_fisse($pl_votes, $competition_round, $c_id);
    }

    #report
    $sql = "SELECT count(*) AS n, t_id FROM {fanta_lineups} " .
          "WHERE has_played = 1 " .
          "AND c_id = '%d' " .
          "AND round = '%d'" .
          "GROUP BY t_id";
    $result = db_query($sql, $c_id, $competition_round);
    $played = array();
    $i = 0;
    while ($row = db_fetch_array($result)) {
      $i++;
      $played[$i]['t_id'] = $row['t_id'];
      $played[$i]['n'] = $row['n'];
    }

    $players = get_players();

    $sql = "SELECT * FROM {fanta_lineups} " .
        "WHERE c_id = '%d' " .
        "AND round = '%d'";
    $result = db_query($sql, $c_id, $competition_round);
    $formazioni = array();
    while ($row = db_fetch_object($result)) {

      $role = $players[$row->pl_id]->role;

      if ($row->position == 1)  $formazioni[$row->t_id]["titolari"][$role]++;
      if ($row->has_played == 1) $formazioni[$row->t_id]["played"][$role]++;
    }

    #riepilogo titolari squadre
    $header = array("Squadra", "N&deg; Titolari", "Modulo Titolari", "Modulo Formazione");

    $rows = array();
    foreach ($formazioni as $key => $value) {
      $n_titolari = array_sum($formazioni[$key]["played"]);

      $style = ($n_titolari == 11) ? "" : "font-weight: bold; color: red;";

      ksort($formazioni[$key]["played"]);
      ksort($formazioni[$key]["titolari"]);

      $rows[$key][] = $teams[$key]->name;
      $rows[$key][] = array("data" => $n_titolari, "style" => $style);
      $rows[$key][] = implode("-", $formazioni[$key]["played"]);
      $rows[$key][] = implode("-", $formazioni[$key]["titolari"]);
    }
    $out .= theme_table($header, $rows);

  }

  return $out;

}

function fantacalcio_get_titolari_riserve_fisse($pl_votes, $competition_round, $c_id) {
  #titolari senza voto da sostituire
  $sql = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
      "WHERE p.pl_id = f.pl_id " .
      "AND f.has_played = 0 " .
      "AND f.position = 1 " .
      "AND f.round = '%d' " .
      "AND f.c_id = '%d'";
  $result = db_query($sql, $competition_round, $c_id);
  while ($row = db_fetch_array($result)) {

    $role = $row['role'];
    $t_id = $row['t_id'];
        
    #1a riserva dello stesso ruolo
    $sqla = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
        "WHERE p.pl_id = f.pl_id " .
        "AND f.position = 2 " .
        "AND f.has_played = 0 " .
        "AND p.role = '%d' " .
        "AND f.round = '%d' " .
        "AND f.c_id = '%d' " .
        "AND f.t_id = '%d' ";
    $resulta = db_query($sqla, $role, $competition_round, $c_id, $t_id);

    if (db_affected_rows($resulta) > 0) {

      while ($rowa = db_fetch_array($resulta)) {
        $pl_id = $rowa['pl_id'];
        
        #se con il voto entra la prima riserva
        if (in_array($pl_id, $pl_votes)) {
          $sqlb = "UPDATE {fanta_lineups} set has_played = 1 " .
            "WHERE pl_id = '%d' " .
            "AND t_id = '%d' " .
            "AND c_id = '%d' " .
            "AND round = '%d'";
          $resultb = db_query($sqlb, $pl_id, $t_id, $c_id, $competition_round);
        }
        #se senza voto cerco la seconda riserva
        else {
          $sqlb = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
            "WHERE p.pl_id = f.pl_id " .
            "AND f.position = 3 " .
            "AND f.has_played = 0 " .
            "AND p.role = '%d' " .
            "AND f.round = '%d' " .
            "AND f.c_id = '%d' " .
            "AND f.t_id = '%d' ";
          $resultb = db_query($sqlb, $role, $competition_round, $c_id, $t_id);
          while ($rowb = db_fetch_array($resultb)) {
            $pl_id = $rowb['pl_id'];
            if (in_array($pl_id, $pl_votes)) {
              $sqlc = "UPDATE {fanta_lineups} set has_played = 1 " .
                "WHERE pl_id = '%d' " .
                "AND t_id = '%d' " .
                "AND c_id = '%d' " .
                "AND round = '%d'";
              $resultc = db_query($sqlc, $pl_id, $t_id, $c_id, $competition_round);
            }
          }
        }
      }
    }

    #nessuna prima riserva ancora non entrata
    else {
      $sqlb = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
            "WHERE p.pl_id = f.pl_id " .
            "AND f.position = 3 " .
            "AND f.has_played = 0 " .
            "AND p.role = '%d' " .
            "AND f.round = '%d' " .
            "AND f.c_id = '%d' " .
            "AND f.t_id = '%d' ";
      $resultb = db_query($sqlb, $role, $competition_round, $c_id, $t_id);
      while ($rowb = db_fetch_array($resultb)) {
        $pl_id = $rowb['pl_id'];
        if (in_array($pl_id, $pl_votes)) {
          $sqlc = "UPDATE {fanta_lineups} set has_played = 1 " .
                "WHERE pl_id = '%d' " .
                "AND t_id = '%d' " .
                "AND c_id = '%d' " .
                "AND round = '%d'";
          $resultc = db_query($sqlc, $pl_id, $t_id, $c_id, $competition_round);
        }
      }
    }

  }
}

//DA CAMBIARE
function fantacalcio_get_titolari_riserve_libere($pl_votes, $competition_round, $c_id) {
  #titolari senza voto da sostituire
  $sql = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
  "WHERE p.pl_id = f.pl_id " .
  "AND f.has_played = 0 " .
  "AND f.position = 1 " .
  "AND f.round = '%d' " .
  "AND f.c_id = '%d'";
  $result = db_query($sql, $competition_round, $c_id);
  while ($row = db_fetch_array($result)) {

    $role = $row['role'];
    $t_id = $row['t_id'];

    #1a riserva dello stesso ruolo
    $sqla = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
    "WHERE p.pl_id = f.pl_id " .
    "AND f.position = 2 " .
    "AND f.has_played = 0 " .
    "AND p.role = '%d' " .
    "AND f.round = '%d' " .
    "AND f.c_id = '%d' " .
    "AND f.t_id = '%d' ";
    $resulta = db_query($sqla, $role, $competition_round, $c_id, $t_id);//OK

    //prima riserva non ancora entrata
    if (db_affected_rows($resulta) > 0) {

      while ($rowa = db_fetch_array($resulta)) {
        $pl_id = $rowa['pl_id'];

        #se con il voto entra la prima riserva
        if (in_array($pl_id, $pl_votes)) {
          $sqlb = "UPDATE {fanta_lineups} set has_played = 1 " .
          "WHERE pl_id = '%d' " .
          "AND t_id = '%d' " .
          "AND c_id = '%d' " .
          "AND round = '%d'";
          $resultb = db_query($sqlb, $pl_id, $t_id, $c_id, $competition_round);
        }
        //OK
        
        #se senza voto cerco la seconda riserva
        else {
          $da_far_entrare[$role]++;
        
          /*
          $sqlb = "SELECT * FROM {fanta_lineups} f, {fanta_players} p " .
          "WHERE p.pl_id = f.pl_id " .
          "AND f.position = 3 " .
          "AND f.has_played = 0 " .
          "AND p.role = '%d' " .
          "AND f.round = '%d' " .
          "AND f.c_id = '%d' " .
          "AND f.t_id = '%d' ";
          $resultb = db_query($sqlb, $role, $competition_round, $c_id, $t_id);
          while ($rowb = db_fetch_array($resultb)) {
            $pl_id = $rowb['pl_id'];
            if (in_array($pl_id, $pl_votes)) {
              $sqlc = "UPDATE {fanta_lineups} set has_played = 1 " .
              "WHERE pl_id = '%d' " .
              "AND t_id = '%d' " .
              "AND c_id = '%d' " .
              "AND round = '%d'";
              $resultc = db_query($sqlc, $pl_id, $t_id, $c_id, $league_round);
            }
          }
          */
        }
      }
    }

    #nessuna prima riserva ancora non entrata
    else {
      
    }

  }
}

//controllo nuovo modulo
function new_modulo_ok($modulo, $role_in, $role_out) {
  $new_modulo = $modulo;
  $new_modulo[$role_in]++;
  $new_modulo[$role_out]--;
  
  $array_moduli = array(
          array(1, 3, 4, 3),
          array(1, 3, 5, 2),
          array(1, 4, 3, 3),
          array(1, 4, 4, 2),
          array(1, 4, 5, 1),
          array(1, 5, 3, 2),
          array(1, 5, 4, 1),
          array(1, 6, 3, 1)
          );
          
  return in_array($modulo, $array_moduli);
}
/************************/
/* END titolari      */
/************************/



/************************/
/* totali        */
/************************/

function fantacalcio_calcola_totali($vote_round) {
  drupal_set_title(filter_xss('Risultati ' . $vote_round . '&ordf; giornata'));

//  $vote_round = get_last_votes();
  $teams = get_teams();
  $votes = get_votes($vote_round);
  $competitions = get_competitions();

  $matches = array();
  foreach ($competitions as $c_id => $competition) {
    $competition_round = get_last_competition_round($c_id);
    $matches_competitions = get_round_matches($competition_round, '', $c_id);

    $matches = array_merge($matches, $matches_competitions);
  }

  $out = '';

  $header = array("Comp", "Team1", "Voti", "Mod Dif", "Mod Centr", "+", "Tot", "Gol", "Team1", "Voti", "Mod Dif", "Mod Centr", "+", "Tot", "Gol", "W");

  foreach ($matches as $m_id => $match) {
    $t1_id = $match->t1_id;
    $t2_id = $match->t2_id;
    $competition_round = $match->round;
    $c_id = get_cid_by_gid($match->g_id);

    $mod_por_1 = $match->mod_por_1;
    $mod_por_2 = $match->mod_por_2;
    $mod_dif_1 = $match->mod_dif_1;
    $mod_dif_2 = $match->mod_dif_2;
    $mod_centr_1 = $match->mod_centr_1;
    $mod_centr_2 = $match->mod_centr_2;
    $mod_att_1 = $match->mod_att_1;
    $mod_att_2 = $match->mod_att_2;
    
    $bonus_t1 = $match->bonus_t1;
    $bonus_t2 = $match->bonus_t2;

    $tot_voti_1 = get_totale($t1_id, $competition_round, $vote_round, $c_id, variable_get("fantacalcio_votes_provider", 1));
    $tot_voti_2 = get_totale($t2_id, $competition_round, $vote_round, $c_id, variable_get("fantacalcio_votes_provider", 1));

    $tot_1 = $tot_voti_1 + $mod_por_1 + $mod_dif_2 + $mod_centr_1 + $mod_att_1 + $bonus_t1;
    $tot_2 = $tot_voti_2 + $mod_por_2 + $mod_dif_1 + $mod_centr_2 + $mod_att_2 + $bonus_t2;

    $goals_1 = floor(($tot_1 -60) / 6);
    $goals_2 = floor(($tot_2 -60) / 6);
    $goals_1 = ($goals_1 >= 0) ? $goals_1 : 0;
    $goals_2 = ($goals_2 >= 0) ? $goals_2 : 0;
    
    //vittoria con scarto
    if (variable_get('fantacalcio_scarto', '0') && variable_get('fantacalcio_scarto_punti', '0') > 0) {
      if ( ($goals_1 == $goals_2) && ($tot_1 - $tot_2) > variable_get('fantacalcio_scarto_punti', '0') ) $goals_1++;
      if ( ($goals_1 == $goals_2) && ($tot_2 - $tot_1) > variable_get('fantacalcio_scarto_punti', '0') ) $goals_2++;
    }

    $winner_id = ($goals_1 > $goals_2) ? $t1_id : $t2_id;
    $winner_id = ($goals_1 == $goals_2) ? -1 : $winner_id;

    //aggiorno partite
    $sql = "UPDATE {fanta_matches} SET 
        pt_1 = '%f', 
        pt_2 = '%f', 
        tot_1 = '%f', 
        tot_2 = '%f', 
        goals_1 = '%d', 
        goals_2 = '%d', 
        played = 1, 
        winner_id = '%d' 
        WHERE m_id = '%d'";
    $result = db_query($sql, $tot_voti_1, $tot_voti_2, $tot_1, $tot_2, $goals_1, $goals_2, $winner_id, $match->m_id);
    
  }

  $sqlx = "SELECT * FROM {fanta_rounds_competitions} WHERE round = '%d'";
  $resultx = db_query($sqlx, $vote_round);
  while ($rowx = db_fetch_array($resultx)) {
    $c_id = $rowx['c_id'];
    $competition_round = $rowx['competition_round'];

    $sql = "SELECT * FROM {fanta_matches} " .
          "WHERE g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id =  '%d') " .
          "AND round = '%d' ";
    $result = db_query($sql, $c_id, $competition_round);
    $out .= "<h3>" . check_plain($competitions[$c_id]->name) . "</h3>";

    $header = array("Squadra", "Punti", "Totale", "Goal", "Vincitore");
    $rows = array();

    while ($row = db_fetch_array($result)) {
      $rows[$row['m_id'] . "_1"][] = $teams[$row['t1_id']]->name;
      $rows[$row['m_id'] . "_1"][] = $row['pt_1'];
      $rows[$row['m_id'] . "_1"][] = $row['tot_1'];
      $rows[$row['m_id'] . "_1"][] = $row['goals_1'];
      //$rows[$row['m_id'] . "_1"][] = $row['mod_att_1'];
      $rows[$row['m_id'] . "_2"][] = $teams[$row['t2_id']]->name;//squadra 2
      $rows[$row['m_id'] . "_2"][] = $row['pt_2'];
      $rows[$row['m_id'] . "_2"][] = $row['tot_2'];
      $rows[$row['m_id'] . "_2"][] = $row['goals_2'];
      $rows[$row['m_id'] . "_2"][] = ($row['winner_id'] == -1) ? " - " : $teams[$row['winner_id']]->name;
        
      $rows[$row['m_id'] . "_3"][] = array("data" => "<hr>", "colspan" => 5);//separatore
    }
    $out .= theme_table($header, $rows);

  }

  return $out;
}

function get_totale($t_id, $competition_round, $vote_round, $c_id) {

  $total = 0;
  $sql = "SELECT DISTINCT v.pl_id, v.total FROM {fanta_votes} v, {fanta_lineups} f 
      WHERE f.pl_id = v.pl_id 
      AND f.round = '%d' 
      AND v.round = '%d' 
      AND f.t_id = '%d'
      AND f.c_id = '%d'
      AND f.has_played = 1 
	  AND v.provider = '%d'";
  $result = db_query($sql, $competition_round, $vote_round, $t_id, $c_id, variable_get("fantacalcio_votes_provider", 1));
  while ($row = db_fetch_array($result)) {
    $total += $row['total'];
  }
  return $total;
}


/********************/
/* passaggio turno  */
/********************/

function fantacalcio_passaggio_turno($vote_round) {
  drupal_set_title(filter_xss('Risultati ' . $vote_round . '&ordf; giornata'));

  $teams = get_teams();

  $matches = get_matches_next($vote_round);

  if ($matches) {
  $header = array("Label", "Team 1", "Team 2", "Goal 1", "Goal 2", "OT 1", "OT 2", "Penalty 1", "Penalty 2", "Vincente");

  $rows = array();
  foreach ($matches as $label => $matches_label) {
    $rows[$label][] = $label;
    
    //sommo i goals della squadra (andata + ritorno)
    foreach ($matches_label as $m_id => $match) {
      $goals[$match->t1_id]["h"] += $match->goals_1;
      $goals[$match->t2_id]["a"] += $match->goals_2;

      $t1 = $match->t1_id;
      $t2 = $match->t2_id;
    }
    
    /////table
    
    //squadre
    $rows[$label][] = $teams[$match->t1_id]->name;
    $rows[$label][] = $teams[$match->t2_id]->name;

    //gol (tempi regolamentari)
    $rows[$label][] = "<b>" . array_sum($goals[$t1]) . "</b> (" . $goals[$t1]["h"] . "+" . $goals[$t1]["a"] . ")";
    $rows[$label][] = "<b>" . array_sum($goals[$t2]) . "</b> (" . $goals[$t2]["h"] . "+" . $goals[$t2]["a"] . ")";
    
    //tempi regolamentari in parità --> tempi supplementari
    if (array_sum($goals[$t1]) == array_sum($goals[$t2]) && ( ($goals[$t1]["a"] == $goals[$t2]["a"]) || count($matches_label) == 1)) {
            
      $overtime = get_overtime($match, variable_get("fantacalcio_votes_provider", 1)); //array(0=>array(t_id, pt_ot, goals_ot), 1=>array(t_id, pt_ot, goals_ot))
      
      //tempi supplementari
      $rows[$label][] = "<b>" . $overtime[0]['goals_ot'] . "</b> (" . $overtime[0]['pt_ot'] . ")";
      $rows[$label][] = "<b>" . $overtime[1]['goals_ot'] . "</b> (" . $overtime[1]['pt_ot'] . ")";
      
      //tempi supplementari in parità --> rigori
      if ($overtime[0]['goals_ot'] == $overtime[1]['goals_ot']) {
        $penalties = get_penalties($match);//ar(0=>ar(t_id, goals_penalties=>array(i, gol)),1=>ar(t_id,goals_penalties=>array(i=>gol)))      
        $last_penalty_index = get_penalties_last_index($penalties);
        //rigori
        $goals_penalties_1 = get_goals_penalties($penalties[0]['goals_penalties'], $last_penalty_index);
        $goals_penalties_2 = get_goals_penalties($penalties[1]['goals_penalties'], $last_penalty_index);
        $rows[$label][] = "<b>" . $goals_penalties_1 . "/" . $last_penalty_index . "</b>";
        $rows[$label][] = "<b>" . $goals_penalties_2 . "/" . $last_penalty_index . "</b>";
        
        update_goals_penalties($match, $goals_penalties_1, $goals_penalties_2);
      }
      else {
        //rigori
        $rows[$label][] = "-";
        $rows[$label][] = "-";
      }      
    }
    else {
      //tempi supplementari
      $rows[$label][] = "-";
      $rows[$label][] = "-";
      
      //rigori
      $rows[$label][] = "-";
      $rows[$label][] = "-";
    }
    
    //winner
    ////team 1
    if (array_sum($goals[$t1]) > array_sum($goals[$t2]) ) $next[$label] = $t1;

    ////team 2
    elseif (array_sum($goals[$t1]) < array_sum($goals[$t2]) ) $next[$label] = $t2;

    ////draw
    elseif (array_sum($goals[$t1]) == array_sum($goals[$t2]) ) {
      //winner is home team 
      if ($goals[$t1]["a"] > $goals[$t2]["a"]) $next[$label] = $t1;
      //winner is away team
      elseif ($goals[$t1]["a"] < $goals[$t2]["a"]) $next[$label] = $t2;
      //draw ==> overtime
      elseif ($goals[$t1]["a"] == $goals[$t2]["a"]) $next[$label] = get_winner_overtime($overtime, $penalties);
    }
    
    $rows[$label][] =($next[$label] >= 0) ? array("data" => $teams[$next[$label]]->name, "style" => "font-weight:bold;") : array("data" => "**", "style" => "color: red;");
  }

  //inserisco il vincitore nella partita successiva
  foreach ($next as $label => $t_id) {
    if ($t_id >= 0) {
      $sql = "UPDATE {fanta_matches} SET t1_id = '%d' WHERE t1_label = '%s'";
      $result = db_query($sql, $t_id, $label);
      $sql = "UPDATE {fanta_matches} SET t2_id = '%d' WHERE t2_label = '%s'";
      $result = db_query($sql, $t_id, $label);
    }
  }
  return theme_table($header, $rows);
  }
  else 
    return t("Passaggio turno non previsto in questa giornata");

}

function get_overtime($match, $votes_provider) {

  $vote_round = get_round_by_match(get_competition_group($match->g_id), $match->round);
  $votes = get_votes($vote_round);
  
  $players = get_players();
  
  //voti riserve non entrate
  $riserve_team_1 = get_riserve_overtime($match, $match->t1_id, $votes_provider);
  $riserve_team_2 = get_riserve_overtime($match, $match->t2_id, $votes_provider);
  
  #if ($match->t2_id == 7) {echo "<pre>"; print_r($riserve_team_1);die("a");}
  
  foreach ($riserve_team_1 as $pl_id => $player) {
    $tot_1 += $votes[$pl_id]->tot;
  }
  
  foreach ($riserve_team_2 as $pl_id => $player) {
    $tot_2 += $votes[$pl_id]->tot;
  }
  
  //1° goal: 20pts; poi un goal ogni 4pts
  $goals_ot_1 = floor(($tot_1 - 16) / 4);
  $goals_ot_2 = floor(($tot_2 - 16) / 4);
  
  $goals_ot_1 = ($goals_ot_1 < 0 ) ? 0 : $goals_ot_1;
  $goals_ot_2 = ($goals_ot_2 < 0 ) ? 0 : $goals_ot_2;
  
  $sql = "UPDATE {fanta_matches} SET 
        pt_ot_1 = " . $tot_1 . ", 
        pt_ot_2 = " . $tot_2 . ", 
        goals_ot_1 = " . $goals_ot_1 . ", 
        goals_ot_2 = " . $goals_ot_2 . " 
        WHERE m_id = " . $match->m_id;
  db_query($sql);
  
  return array(0 => array('t_id' => $match->t1_id, 'pt_ot' => $tot_1, 'goals_ot' => $goals_ot_1), 
          1 => array('t_id' => $match->t1_id, 'pt_ot' => $tot_2, 'goals_ot' => $goals_ot_2));
}

function get_penalties($match) {
  //elenco rigoristi (in ordine)
  $penalties_players_1 = get_penalties_team($match, $match->t1_id);
  $penalties_players_2 = get_penalties_team($match, $match->t2_id);
    
  foreach ($penalties_players_1 as $penalty_position => $penalties_player) {
    $goals_penalties_1[$penalty_position] = array("pl_id" => $penalties_player->pl_id, 'penalty' => $penalties_player->penalty);
  }
  foreach ($penalties_players_2 as $penalty_position => $penalties_player) {
    $goals_penalties_2[$penalty_position] = array("pl_id" => $penalties_player->pl_id, 'penalty' => $penalties_player->penalty);
  }
  
  return array(0 => array('t_id' => $match->t1_id, 'goals_penalties' => $goals_penalties_1), 
          1 => array('t_id' => $match->t2_id, 'goals_penalties' => $goals_penalties_2));    
}

function get_winner_overtime($overtime, $penalties) {
  if ($overtime[0]['goals_ot'] > $overtime[1]['goals_ot']) 
    return $overtime[0]['t_id'];
  elseif ($overtime[1]['goals_ot'] > $overtime[0]['goals_ot']) 
    return $overtime[1]['t_id'];
  if ($overtime[0]['goals_ot'] == $overtime[1]['goals_ot']) 
    return get_winner_penalties($penalties);
}

function get_winner_penalties($penalties) {
  $goals_1 = 0; $goals_2 = 0;
  for ($i = 1; $i <= 5; $i++) {
    $goals_1 += $penalties[0]['goals_penalties'][$i]['penalty'];
    $goals_2 += $penalties[1]['goals_penalties'][$i]['penalty'];
  }
    
  if ($goals_1 > $goals_2)
    return $penalties[0]['t_id'];
  elseif ($goals_2 > $goals_1)
    return $penalties[1]['t_id'];
  elseif ($goals_2 == $goals_1)
    return get_winner_penalties_oltranza($penalties, 6);  
}

function get_winner_penalties_oltranza($penalties, $start) {
  //rigori a oltranza
  if ($start == 18) return 0; //ferma la ricorsione
    
  if ($penalties[0]['goals_penalties'][$start]['penalty'] > $penalties[1]['goals_penalties'][$start]['penalty']) 
    return $penalties[0]['t_id'];
  if ($penalties[1]['goals_penalties'][$start]['penalty'] > $penalties[0]['goals_penalties'][$start]['penalty']) 
    return $penalties[1]['t_id'];
  if ($penalties[0]['goals_penalties'][$start]['penalty'] == $penalties[1]['goals_penalties'][$start]['penalty']) 
    return get_winner_penalties_oltranza($penalties, ($start+1));
}

function get_penalties_last_index($penalties) {
  $goals_1 = 0; $goals_2 = 0;
  for ($i = 1; $i <= 5; $i++) {
    $goals_1 += $penalties[0]['goals_penalties'][$i]['penalty'];
    $goals_2 += $penalties[1]['goals_penalties'][$i]['penalty'];
  }
  
  if ($goals_1 > $goals_2)
    return 5;
  elseif ($goals_2 > $goals_1)
    return 5;
  elseif ($goals_2 == $goals_1)
    return get_penalties_last_index_oltranza($penalties, 6);  
}

function get_penalties_last_index_oltranza($penalties, $start) {
  //rigori a oltranza
  if ($start == 18) return 0; //ferma la ricorsione
    
  if ($penalties[0]['goals_penalties'][$start]['penalty'] > $penalties[1]['goals_penalties'][$start]['penalty']) 
    return $start;
  if ($penalties[1]['goals_penalties'][$start]['penalty'] > $penalties[0]['goals_penalties'][$start]['penalty']) 
    return $start;
  if ($penalties[0]['goals_penalties'][$start]['penalty'] == $penalties[1]['goals_penalties'][$start]['penalty']) 
    return get_penalties_last_index_oltranza($penalties, ($start+1));
}

function get_goals_penalties($penalties_team, $last_penalty_index) {
  $goals_penalties = 0;
  for ($i = 1; $i <= $last_penalty_index; $i++)
    $goals_penalties += $penalties_team[$i]['penalty'];
    
  return $goals_penalties;
}

function update_goals_penalties($match, $goals_penalties_1, $goals_penalties_2) {
  $sql = "UPDATE {fanta_matches} SET penalties_1 = " . $goals_penalties_1 . ", penalties_2 = " . $goals_penalties_2 . " 
      WHERE m_id = " . $match->m_id;
  $result = db_query($sql);
}

function get_riserve_overtime($match, $t_id, $votes_provider) {

  $roles = array(0, 0, 0);
  
  //riserve non entrate
  $sql = "SELECT f.pl_id, f.round, f.t_id, f.c_id, p.role FROM {fanta_lineups} f, {fanta_players} p, {fanta_votes} v
      WHERE f.pl_id = p.pl_id
      AND f.pl_id = v.pl_id
      AND f.c_id = '%d' 
      AND f.round = '%d' 
      AND f.t_id = '%d'
      AND f.position >= 2 
      AND f.has_played = 0 
      AND p.role != 0
      AND v.round = '%d'
	  AND v.provider = '%d'
      ORDER BY position";
  $result = db_query($sql, get_cid_by_gid($match->g_id), $match->round, $t_id, get_round_by_competition_round($match->round, get_cid_by_gid($match->g_id)), $votes_provider);
  while ($row = db_fetch_object($result)) {
    $roles[$row->role]++;
    if ($roles[$row->role] == 1) {
      $riserve[$row->pl_id] = $row;
      $sql_a = "UPDATE {fanta_lineups} SET has_played = 2 
            WHERE pl_id = '%d' 
            AND t_id = '%d' 
            AND round = '%d' 
            AND c_id = '%d'";
      $result_a = db_query($sql_a, $row->pl_id, $t_id, $row->round, $row->c_id);
    }
  }
    
  //riserve e voti
  return $riserve;
}

function get_penalties_team($match, $t_id, $votes_provider) {

  $vote_round = get_round_by_match(get_cid_by_gid($match->g_id), $match->round);
  $votes = get_votes($vote_round);
  $i = 1;
  //rigoristi ordinati
  $sql = "SELECT * FROM {fanta_lineups} f, {fanta_votes} v
      WHERE f.pl_id = v.pl_id
      AND f.c_id = '%d' 
      AND v.round = '%d'
      AND f.round = '%d' 
      AND f.t_id = '%d'
	  AND v.provider = '%d'
      ORDER BY penalty_position";
  $result = db_query($sql, get_cid_by_gid($match->g_id), $match->round, $match->round, $t_id, $votes_provider);
  while ($row = db_fetch_object($result)) {
    $penalties[$i] = $row;
    $penalties[$i]->penalty = ($row->vote >= 6) ? 1 : 0;
    $i++;
  }
  
  return $penalties;
}

function get_matches_vote_round($vote_round) {

  $matches = array();

  $sql = "SELECT * FROM {fanta_rounds_competitions} WHERE round = '%d'";
  $result = db_query($sql, $vote_round);
  while ($row = db_fetch_array($result)) {
    $c_id = $row['c_id'];
    $competition_round = $row['competition_round'];
    $matches_competitions = get_round_matches($competition_round, '', $c_id);
    $matches = array_merge($matches, $matches_competitions);

  }

  return $matches;
}

function get_matches_next($vote_round) {

  $matches = get_matches_vote_round($vote_round);
  
  $sql = "SELECT match_label, c.next
      FROM {fanta_matches} m, {fanta_rounds_competitions} c 
      WHERE m.round = c.competition_round 
      AND c.c_id = (SELECT c_id FROM {fanta_groups} WHERE g_id = m.g_id)
      AND c.round = '%d'";
  $result = db_query($sql, $vote_round);
  while ($row = db_fetch_object($result)) {
    if ($row->match_label != "" && $row->next == 1)
      $match_labels[] = "'" . $row->match_label . "'";
  }
  
  if ($match_labels) {
    $sql = "SELECT * FROM {fanta_matches} WHERE match_label IN (" . implode(",", $match_labels) . ")";
    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
      $matches_next[$row->match_label][$row->m_id] = $row;
    }
  }
  return $matches_next;
}

/************************/
/* END passaggio turno  */
/************************/

/************************/
/* news          */
/************************/

function fantacalcio_crea_news($vote_round) {
  drupal_set_title(filter_xss('Risultati ' . $vote_round . '&ordf; giornata'));
 
  global $user;
  
  $competitions = get_competitions();

  $sql = "SELECT * FROM {fanta_rounds_competitions} WHERE round = '%d'";
  $result = db_query($sql, $vote_round);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->c_id] = (!empty($row->round_label)) ? $row->round_label : $row->competition_round . "&ordf; giornata";
  }
  
  $body = "";
  foreach ($rounds as $c_id => $round) {
     $body .= $round . " " . $competitions[$c_id]->name . ", ";
  }
  
  $body = substr($body, 0, -2) . ": Pronti i risultati";
  
  $title = "Risultati " . $vote_round . "&ordf; Giornata";

  $news = (object) array('title'=> $title, 'body'=> $body, 'type' => 'news', 'uid' => $user->uid, 'language' => 'it');
  node_save($news);
  
  //rimuovi "risultati provvisori"
  db_query("UPDATE {fanta_rounds} SET status = 1 WHERE round = '%d'", $vote_round);
  
  return "News creata";    
}
/************************/
/* END news        */
/************************/


/************************/
/* newsletter      */
/************************/

function fantacalcio_send_newsletter($vote_round) {
  drupal_set_title(filter_xss('Risultati ' . $vote_round . '&ordf; giornata'));
 
  global $user;
  
  $sql = "SELECT * FROM {fanta_rounds_competitions} WHERE round = '%d' AND c_id IN (SELECT c_id FROM {fanta_competitions} WHERE newsletter = 1)";
  $result = db_query($sql, $vote_round);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->c_id] = $row;
  }
  
  //newsletters
 //TODO //Administation
  $tids = array(1 => 13, 2 => 16);
  
  foreach ($rounds as $c_id => $round) {

    $tid = $tids[$c_id];

    $body = newsletter_create_body($c_id, $round->competition_round);
    $title = "Riepilogo " . (!empty($round->round_label) ? $round->round_label : $round->competition_round . "a Giornata");

    //create node
    $newsletter = array();
    $newsletter = (object) array('title'=> $title, 'body'=> $body, 'type' => 'simplenews', 'uid' => $user->uid, 'language' => 'it');

    //taxonomy
    $newsletter->taxonomy[$tid] = taxonomy_get_term($tid);
    $newsletter->simplenews = array('tid' => $tid, 's_format' => 'html', 'send' => SIMPLENEWS_COMMAND_SEND_NOW);

    node_save($newsletter);

    module_invoke('simplenews', 'simplenews_nodeapi', $newsletter, 'insert');

  }
  
  return "Newsletter inviate";  
}

function newsletter_create_body($c_id, $round) {
  
  $body = competition_situation($c_id, $round); 
  
  return $body;
}

function newsletter_create_title($tid, $round) {
  
  $sql = "SELECT name FROM {term_data} WHERE tid = '%d'";
  $result = db_query($sql, $tid);
  $title = "[" . db_result($result) . "] " . $round . "a Giornata";
  
  return $title;
}

function competition_situation($c_id, $round) {

  $sql = "SELECT * FROM {fanta_groups} 
                WHERE c_id = '%d' 
                AND active = 1 
                AND (newsletter_standings > 0 OR newsletter_matches > 0)";
  $result = db_query($sql, $c_id);
  while ($row = db_fetch_object($result)) {
    $groups[$row->g_id] = $row;
  }  

  $out = "";

  if ($groups) {
    
    foreach ($groups as $g_id => $group) {
      
      $out .= "<h2>$group->name</h2>";

      if ($round > 0) $out .= "<h3>" . $round . "&ordf; giornata</h3>";

      if ($group->newsletter_matches > 0) $out .= home_last_matches($g_id, $round);
      if ($group->newsletter_standings > 0) $out .= home_standings($g_id);
      if ($group->newsletter_matches > 0) $out .= home_next_matches($g_id, $round);

      $out .= "<div class=\"clear\"></div>";
      $out .= "<hr>";
      $out .= "<div class=\"clear\"></div>";
    }
  }

  return $out;
}

/************************/
/* END newsletter    */
/************************/


/************************/
/* Sfide        */
/************************/

function fantacalcio_aggiorna_sfide($vote_round) {
  drupal_set_title(filter_xss('Risultati ' . $vote_round . '&ordf; giornata'));

  $sql = "UPDATE {fanta_sfide} SET status = 3 WHERE status = 1";
  $result = db_query($sql);
  
  $sql = "DELETE FROM {fanta_sfide} WHERE (status = 2 OR status = 0)";
  $result = db_query($sql);
  return "Sfide aggiornate";    

}

/************************/
/* END Sfide      */
/************************/


function fantacalcio_risultati_automatici($round = NULL, $step = 0) {
    switch($step) {
      case 1: 
        //caricamento voti
        $providers = array(1 => "Milano", 2 => "Roma", 3 => "Napoli");
        $votes = get_votes_html_data($round, $providers);

        foreach($providers as $provider) {
          $out = insert_votes($votes[$provider], $round, $provider);
        }    
        update_players($round + 1);  
        //risultati provvisori
        db_query("UPDATE {fanta_rounds} SET status = -1 WHERE round = '%d'", $round);
        drupal_goto("calcola/" . $round . "/2");
        break;

      case 2:
        fantacalcio_trova_titolari($round);
        fantacalcio_calcola_modificatori($round);
        drupal_goto("calcola/" . $round . "/3");
        break;

      case 3:
        fantacalcio_calcola_totali($round);
        fantacalcio_passaggio_turno($round);
        drupal_goto("calcola/" . $round . "/4");
        break;

      case 4:
        fantacalcio_crea_news($round);
        fantacalcio_send_newsletter($round);
        break;

  }

return "";

}

