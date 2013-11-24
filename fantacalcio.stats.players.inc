<?php

/**
 * @file fantastats.players.inc
 *
 */

/**
ricerca giocatore
**/

function statistiche_autocomplete_players($s) {
  $players = array();
  $round = db_result(db_query("SELECT MAX(round) FROM {fanta_players_teams}"));
  $sql = "SELECT p.pl_id, p.name, r.long_name as team 
      FROM {fanta_players} p, {fanta_players_teams} t, {fanta_real_teams} r
      WHERE t.pl_id = p.pl_id
      AND r.rt_id = t.rt_id
      AND t.round = '%d'
      AND LOWER(p.name) LIKE LOWER('%s%%')
      ORDER BY p.name";
  $result = db_query_range($sql, $round, $s, 0, 10);
  while ($row = db_fetch_array($result)) {
    $id = $row['pl_id'] . ", " . $row['name'] . " (" . ucwords($row['team']) . ")";
    $players[$id] = $id;
  }
  print drupal_to_js($players);
  exit();
}

function dettagli_giocatore($pl_id = '') {

  $out = drupal_get_form("choose_pl_id_form");

  if (!empty($pl_id)) {
    if (player_exists($pl_id))
      $out .= scheda_giocatore($pl_id);
    else $out .= "Nessun giocatore con questo id";
  }

  return $out;
}


function scheda_giocatore($pl_id) {

  $teams_groups = get_teams_groups();ksort($teams_groups);
  $groups = get_groups();
  $player = get_player_by_id($pl_id);
  $roles = array("Portiere", "Difensore", "Centrocampista", "Attaccante");

  $out = "<br><h2>" . $player->name . "</h2><br>";
  $out .= "<h3>" . $roles[$player->role] . "</h3><br>";
  $out .= "<h4>" . ucwords($player->long_name) . "</h4><br>";

  $out .= "<div class=clear>" . get_player_fantateams($player->pl_id, $groups, $teams_groups) . "</div><br/>";

  $out .= player_stats($pl_id);

  return $out;
}

function choose_pl_id_form() {

  $form['players'] = array("#type" => "fieldset");

  $form['players']['player'] = array(
            "#title" => "Giocatore",
            "#type" => "textfield",
            "#autocomplete_path" => "statistiche/autocomplete/players"
            );

  $form['players']['submit'] = array(
            "#type" => "submit",
            "#value" => t("Ok"),
            );

  return $form;

}

function choose_pl_id_form_submit($form, $form_state) {

  $player = $form_state['values']['player'];
  $player_exploded = explode(',', $player);
  $pl_id = $player_exploded[0];

  drupal_goto("statistiche/giocatore/" . $pl_id);
}

function player_stats($pl_id) {

  $votes = array(); $rounds = 0; $summary = array();
  $sql = "SELECT v.round AS round, UPPER(r.long_name) AS team, v.*
          FROM {fanta_votes} v
          INNER JOIN {fanta_players_teams} t
          ON v.pl_id = t.pl_id
          AND v.round = t.round
          INNER JOIN {fanta_real_teams} r
          ON t.rt_id = r.rt_id
          WHERE v.pl_id = '%d'
          AND v.provider = '%d'
          ORDER BY v.round";
  $result = db_query($sql, $pl_id, variable_get("fantacalcio_votes_provider", 1));
  while ($row = db_fetch_array($result)) {
    $votes[$row['round']] = $row;
    $votes[$row['round']]['goals_for'] = $row['goals_for'] + $row['penalty_goals'];
    unset($votes[$row['round']]['v_id']);
    unset($votes[$row['round']]['provider']);
    unset($votes[$row['round']]['pl_id']);
    unset($votes[$row['round']]['penalty_goals']);
	unset($votes[$row['round']]['win_goals']);
	unset($votes[$row['round']]['draw_goals']);
    $summary['round'] = "Riepilogo";
    $summary['team'] = "";
    $summary['total'] += $row['total'];
    $summary['vote'] += $row['vote'];
    $summary['goals_for'] += $row['goals_for'] + $row['penalty_goals'];
    $summary['goals_against'] += $row['goals_against'];
    $summary['assists'] += $row['assists'];
    $summary['yellow_cards'] += $row['yellow_cards'];
    $summary['red_cards'] += $row['red_cards'];
    $summary['saved_penalties'] += $row['saved_penalties'];
    $summary['missed_penalties'] += $row['missed_penalties'];
    $summary['own_goals'] += $row['own_goals'];

    $rounds++;

  }

  if ($rounds != 0) {

    $summary['vote'] = round($summary['vote'] / $rounds , 2 );
    $summary['total'] = round($summary['total'] / $rounds , 2 );

    sort($votes);

    $votes[$rounds] = array("data" => $summary, "class" => "bold");

    $header = array("Giornata", "Squadra", "Totale", "Voto", "Gol Fatti", "Gol Subiti", "Assist", "Amm", "Esp", "Rig Parati", "Rig Sbagliati", "Autogol");

    return theme_table($header, $votes);
  }
  else 
    return "Nessuna presenza";
}

/**
ricerca avanzata
**/

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

/**
giocatori liberi
**/

function fantacalcio_giocatori_liberi($g_id = "") {

  $js = "$(function(){
      $('#roles_" . $g_id . "').tabs({fxFade:true});
      });
      ";
  drupal_add_js($js, 'inline', 'footer');

  $sql = "SELECT * FROM {fanta_groups} WHERE c_id = '%d'";
  $result = db_query($sql, variable_get('fantacalcio_main_competition', 1));
  while ($row = db_fetch_array($result)) {
    $groups[$row['g_id']] = $row['name'];
  }

  $out = choose_group($groups, $g_id);

  if (!empty($g_id) && group_exists($g_id)) {

    $out .= "<h3>" . $groups[$g_id] . "</h3>";

    $players = get_players();

    $nonfree_players = array(); $roles = array();
    $sql = "SELECT pl_id FROM {fanta_squads} WHERE t_id IN " .
        "(SELECT t_id FROM {fanta_teams_groups} WHERE g_id = '%d') " .
        "AND in_team = 1";
    $result = db_query($sql, $g_id);
    while ($row = db_fetch_array($result)) {
      $nonfree_players[] = $row['pl_id'];
    }
    
    #tab per ogni ruolo
    $roles = array(0 => 0, 1 => 1, 2 => 2, 3 => 3);

    $out .= "\n<div id='roles_" . $g_id ."' class='tabs' data-role=navbar>" .
        "\n<ul>";
    foreach ($roles as $role_id => $role) {
      $role_format = (is_mobile()) ? "short" : "long";
      $width = (is_mobile()) ? "width: 0px;" : "";
      $out .= "\n<li style='background:none; $width'><a href='#r_" . $role_id . "'><span>" . convert_role($role, $role_format) . "</span></a></li>";
    }
    $out .= "\n</ul>";

    foreach ($roles as $role_id => $role) {
      $out .= "\n<div id='r_" . $role_id . "'>"
          . get_free_players($role_id, $g_id, $players, $nonfree_players)
          . "\n</div>";
    }

    $out .= "\n</div>";

  }

  return $out;
}


function choose_group($groups, $g_id = '') {
  return drupal_get_form("choose_group_form", $groups, $g_id);
}

function choose_group_form(&$form_state, $groups, $g_id) {
  $form['groups'] = array("#type" => "fieldset");

  $form['groups']['groups'] = array(
                  "#type" => "select",
                  "#title" => "Scegli un girone",
                  "#options" => $groups,
                  "#default_value" => $g_id,
                    );

  $form['groups']['submit'] = array(
                  "#value" => "Ok",
                  "#type" => "submit",
                  );
  return $form;
}

function choose_group_form_submit($form, $form_state) {
  $g_id = $form_state['values']['groups'];
  drupal_goto("giocatori/liberi/" . $g_id);
}

function get_free_players($role, $g_id, $players, $nonfree_players) {

  $header = array("Ruolo", "Nome", "Squadra", "");

  $rows = array();
  
  foreach ($players as $pl_id => $player) {
    $names[$pl_id] = $player->name;
    $teams[$pl_id] = $player->team;
  }
  
  array_multisort($names, SORT_ASC, $teams, SORT_ASC, $players);

  foreach ($players as $pl_id => $player) {
    if (!in_array($player->pl_id, $nonfree_players) && $player->role == $role) {
      $link_pl_id = "<a rel='lightframe[" . $role . "]' href='" . base_path() . "scheda/giocatore/" . $player->pl_id . "' title='Dettagli giocatore'>" .
            "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/stats.png'>";
      $class = ($player->active) ? "active" : "not-active" ;
      $rows[$pl_id] = array("data" => array(convert_role($player->role), $player->name, ucwords(convert_teams($player->team)), $link_pl_id), 
  "class" => $class);
    }
  }

  $out .= theme_table($header, $rows);

  return $out;
}

