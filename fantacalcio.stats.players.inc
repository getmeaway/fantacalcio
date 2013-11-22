<?php

/**
 * @file fantastats.topflop.inc
 *
 */

function choose_players_stats($form_state) {

  $rounds = get_all_rounds_played();
  
  $form['#theme'] = "fantastats_choose_stats_form";
            
  $form['type'] = array(
            "#type" => "select", 
            "#title" => "Tipo statistica",
            '#options' => get_stats_types(),
            "#default_value" => arg(2),
            );

  if ($rounds)
    $form['round_start'] = array(
            "#type" => "select", 
            "#title" => "Giornata inizio",
            '#options' => $rounds,
            "#default_value" => arg(3),
            );
            
  if ($rounds)
    krsort($rounds);
    
  if ($rounds)	
    $form['round_stop'] = array(
            "#type" => "select", 
            "#title" => "Giornata fine",
            "#default_value" => arg(4),
            '#options' => $rounds
            );
            
  $form['roles'] = array(
            "#type" => "checkboxes",
            "#options" => array("0" => "P", "1" => "D", "2" => "C", "3" => "A"),
            "#default_value" => explode("-", arg(6)),
            "#title" => "Ruoli"
            );

  $form['presences'] = array(
            "#type" => "select", 
            "#title" => "Presenze<br/>minime",
            "#default_value" => arg(5),
            '#options' => ($rounds)
            );
            
  $form['rows'] = array(
            "#type" => "select", 
            "#title" => "Giocatori da<br/>visualizzare",
            '#options' => array("10" => 10, "20" => 20, "30" => 30, "40" => 40, "50" => 50),
            "#default_value" => arg(7),
            );
            
  $form['sort'] = array(
            "#type" => "select", 
            "#title" => "Ordinamento",
            '#options' => array("desc" => "Decrescente", "asc" => "Cresecente"),
            "#default_value" => arg(8),
            );

  $form['group'] = array(
            "#type" => "select", 
            "#title" => t("Girone"),
            '#options' => get_group_options(variable_get("fantacalcio_main_competition", 1)),
            "#default_value" => arg(9),
            );

  $form['in_team'] = array(
            "#type" => "select",
            "#default_value" => arg(10),
            "#title" => "Tesserato",
            '#options' => array("1" => "Tesserato", "-1" => "Non tesserato", "0" => "Indifferente"),
            );
  
  $form['submit'] = array(
            "#type" => "submit",
            "#value" => "Ok",
            );
  
  $form['clear'] = array(
            '#type' => 'submit',
            '#value' => 'Annulla',
            '#validate' => array('choose_players_stats_clear'),
            );
            
  return $form;
}


function choose_players_stats_submit($form, &$form_state) {
    
  $type = $form_state["values"]["type"];
  $round_start = $form_state["values"]["round_start"];
  $round_stop = $form_state["values"]["round_stop"];
  $presences = $form_state["values"]["presences"];
  $rows = $form_state["values"]["rows"];
  $sort = $form_state["values"]["sort"];
  $g_id = $form_state["values"]["group"];
  $in_team = $form_state["values"]["in_team"];
  
  $roles_list = "";
  if ($form_state['clicked_button']['#post']['roles']) {
    foreach ($form_state['clicked_button']['#post']['roles'] as $key => $value) {    
      if ($value != '') $roles_list .= $key . "-";
    }
  }
  if (strlen($roles_list) > 0)  $roles_list = substr($roles_list, 0, -1);
  else $roles_list = "0-1-2-3";
  drupal_goto("statistiche/players/" . $type . "/" . $round_start . "/" . $round_stop . "/" . $presences . "/" . $roles_list . "/" . $rows . "/" . $sort . "/" . $g_id . "/" . $in_team);
}

function choose_players_stats_clear($form, &$form_state) {
  drupal_goto("statistiche/players");
}

function players_stats($type, $round_start, $round_stop, $presences, $roles_list, $number, $sort, $g_id, $in_team) {
  $roles = explode("-", $roles_list);
  $players = get_players();
  $teams_groups = get_teams_groups(); ksort($teams_groups);
  $groups = get_groups(); 
  
  $avg_types = array("vote", "total");
  
  $sql = "SELECT * FROM {fanta_votes} v, {fanta_players} p
      WHERE v.pl_id = p.pl_id 
	  AND v.provider = '%d' ";

  if (count($roles) > 0 && count($roles) < 4) $sql .= " AND p.role IN (" . implode(",", $roles) . ")";

  //tesserati
  if ($in_team == 1 && $g_id > 0)
    $sql .= " AND p.pl_id IN (SELECT pl_id FROM {fanta_squads} WHERE in_team = 1 AND t_id IN (SELECT t_id FROM {fanta_teams_groups} WHERE g_id = '%d'))";

  //non tesserati
  if ($in_team == -1 && $g_id > 0)
    $sql .= " AND p.pl_id NOT IN (SELECT pl_id FROM {fanta_squads} WHERE in_team = 1 AND t_id IN (SELECT t_id FROM {fanta_teams_groups} WHERE g_id = '%d'))";
  
  $result = db_query($sql, variable_get('fantacalcio_votes_provider', 1), $g_id);
  while ($row = db_fetch_object($result)) {
    if ($row->round >= $round_start && $row->round <= $round_stop) {
      $stats[$row->pl_id]["pl_id"] = $row->pl_id;
      $stats[$row->pl_id]["value"] += $row->$type;
      $stats[$row->pl_id]["votes"] ++;
    }
    $stats[$row->pl_id]["presences"] ++;
  }
  
  if (count($stats) > 0) {
  $i = 0;
  foreach ($stats as $pl_id => $stat) {
    if ($stat["presences"] >= $presences && $stat['votes'] > 0) {
      if (in_array($type, $avg_types))
        $stats[$pl_id]['value'] = round(($stat['value'] / $stat['votes']), 2);
      $values[$pl_id] = $stats[$pl_id]['value'];
    }
    else
      unset($stats[$pl_id]);
  }
  
  $flag = ($sort == "asc") ? SORT_ASC : SORT_DESC;
  
  array_multisort($values, $flag, $stats);
  
  foreach ($stats as $pl_id => $stat) {
    if ($i < $number) {
      $rows[] = array(convert_role($players[$stat['pl_id']]->role), 
              $players[$stat['pl_id']]->name, 
              ucwords(strtolower($players[$stat['pl_id']]->team)), 
              $stat['votes'], 
              $stat['value'], 
              get_player_fantateams($stat['pl_id'], $groups, $teams_groups),
              l("<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/stats.png'>",
                "scheda/giocatore/" . $stat['pl_id'], 
                array("html" => TRUE, 
                  "attributes" => array("title" => "Dettagli giocatore", "rel" => "lightframe[width:870px;]", "class" => "thickbox")))
              );              
    }
    $i++;
  }
  
  $types = get_stats_types();
    
  $header = array("Ruolo", "Giocatore", "Squadra", "Presenze", ucwords($types[$type]), "Tesserato", "" );
  
  return drupal_get_form('choose_players_stats') . theme_table($header, $rows, $attributes);
  }

  else {
  return drupal_get_form('choose_players_stats') . t("Non è stato trovato nessun giocatore corrispondente ai criteri di ricerca");
  }
}


function theme_fantastats_choose_stats_form($form) {
  
  $rows[0]= array(  
            drupal_render($form["type"]),
            drupal_render($form["round_start"]) . drupal_render($form["round_stop"]),
            drupal_render($form["presences"]),
            drupal_render($form["roles"]),
            drupal_render($form["rows"]),
            drupal_render($form["sort"]),
            drupal_render($form["group"]),
            drupal_render($form["in_team"]),
            );

  $rows[1][] = array("data" => drupal_render($form["submit"]) . drupal_render($form["clear"]) . drupal_render($form), "colspan" => 8);
        
  
  
    
  return theme_table(array(), $rows);
  
}

function get_stats_types() {
  return  array("total" => "Media FantaVoto", "vote" => "Media Voto", "goals_for" => "Goal fatti", "goals_against" => "Goal subiti", 
          "assists" => "Assist", "yellow_card" => "Ammonizioni", "red_card" => "Espulsioni", 
          "penalty_goals" => "Gol rigore", "saved_penalties" => "Rigore parato", "missed_penalties" => "Rigore sbagliato", "own_goals" => "Autogol" );
}

function get_player_fantateams($pl_id, $groups, $teams_groups) {
  $fanta_teams = array();
  $sql = "SELECT * FROM {fanta_squads} 
      WHERE pl_id = '%d' 
      AND in_team = 1";
  $result = db_query($sql, $pl_id);
  while ($row = db_fetch_object($result)) {
    $fanta_teams[] = $row->t_id;
  }
  
  foreach ($teams_groups as $g_id => $teams_group) {
    $team_name = "-";
    
    foreach ($teams_group as $t_id => $team) {
      if (in_array($t_id, $fanta_teams)) {
        $team_name = $team->name;
        continue;
      }
    }
    $items[$g_id] = $groups[$g_id]->name . ": " . $team_name;
  }
    
  return theme_item_list($items);
}
