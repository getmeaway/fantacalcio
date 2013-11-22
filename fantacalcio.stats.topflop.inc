<?php

/**
 * @file fantastats.topflop.inc
 *
 */
 
function statistiche_topflop_all() {
  $round = get_last_votes();
  return drupal_get_form("topflop_choose_round_all_form", $c_id, $round) . topflop_all_round($round);
}

function statistiche_topflop_all_round($round) {
  return drupal_get_form("topflop_choose_round_all_form", $c_id, $round) . topflop_all_round($round);
}

function statistiche_topflop() {
  $round = get_last_votes();
  return make_columns("topflop_round_team", "statistiche/topflop/" . $round, array("round" => $round, "t_id" => $t_id), "", "topflop_choose_round_form");
}

function statistiche_topflop_round($round) {
  return make_columns("topflop_round_team", "statistiche/topflop/" . $round, array("round" => $round, "t_id" => $t_id), "", "topflop_choose_round_form");
}

function statistiche_topflop_round_team($round, $t_id) {
  return make_columns("topflop_round_team", "statistiche/topflop/" . $round, array("round" => $round, "t_id" => $t_id), "", "topflop_choose_round_form");
}

function topflop_round_team($args) {
  $round = $args["round"];
  $t_id = $args["t_id"];
  
  $team = get_team_data($t_id);
  $top = top_round_team_lineup($round, $t_id);
  $flop = flop_round_team_lineup($round, $t_id);
  $tot_match = get_tot_match($round, $t_id);
  
  $header = array("Ruolo", "Nome", "", "Totale", "Voto", "GF", "GS", "Assist", "Rig. Parato", "Rig. Sbagliato", "Autogol");
  
  $out = "<h3>" . $round . "&ordf; giornata</h3><br>";
  $out .= "<h3 class='team_name'>" . $team['name'] . "</h3><br>";
  $out .= "<div class='shirt'><img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/shirts/" . $team['shirt'] . "'></div>";
  
  //TOP
  $out .= "<p><strong>TOP</strong></p>";
  $out .= "<p><strong>" . substr($top['module'], 2) . "</strong></p>";
  $out .= theme_table($header, $top['line_up']);
  
  //TOP - differenza col totale
  $rows_summary = array();
  $rows_summary[] = array("Totale Partita", $tot_match);
  $rows_summary[] = array("Totale Top", $top['total']);
  $rows_summary[] = array("Differenza", ($tot_match - $top['total']));
  $out .= theme_table(array(), $rows_summary);

  //FLOP
  $out .= "<p><strong>FLOP</strong></p>";
  $out .= "<p><strong>" . substr($flop['module'], 2) . "</strong></p>";
  $out .= theme_table($header, $flop['line_up']);
  
  //FLOP - differenza col totale
  $rows_summary = array();
  $rows_summary[] = array("Totale Partita", $tot_match);
  $rows_summary[] = array("Totale Flop", $flop['total']);
  $rows_summary[] = array("Differenza", ($tot_match - $flop['total']));
  $out .= theme_table(array(), $rows_summary);
  
  return $out;
}

function top_round_team_lineup($round, $t_id) {
  $modules = explode("\r\n", variable_get("fantacalcio_moduli_titolari", ""));
  $max = 0; $max_module = "";
  foreach ($modules as $module) {
    $number_roles = explode("-", $module);
    $tot_module = 0;
    
    foreach ($number_roles as $role => $number_role) {
      $sql = "SELECT * FROM {fanta_votes} v, {fanta_players} p
          WHERE v.round = '%d' 
          AND p.pl_id = v.pl_id
          AND p.role = '%d' 
          AND v.provider = '%d'
          AND p.pl_id IN (SELECT pl_id FROM {fanta_squads} WHERE t_id = '%d' AND in_team = 1)          
          ORDER BY v.total DESC";
      $result = db_query_range($sql, array($round, $role, variable_get("fantacalcio_votes_provider", 1), $t_id), 0, $number_role);
      while ($row = db_fetch_array($result)) {
        $position = get_player_position($row['pl_id'], variable_get("fantacalcio_main_competition", 1), $t_id, $round);
        $position_class = ($position == 0) ? "out_0" : (($position > 1) ? "out_2" : "");
        $row_data = array( array("data" => convert_role($row['role']), "class" => "role"), $row['name'], get_amm_esp($row['yellow_cards'], $row['red_cards']),
                  "<strong>" . $row['total'] . "</strong>", "<strong>" . $row['vote'] . "</strong>", 
                  ($row['goals_for'] + $row['penalty_goals']), $row['goals_against'], $row['assists'],  
                  $row['saved_penalties'], $row['missed_penalties'], $row['own_goals']);
        $line_up[$module][] = array("data" => $row_data, "class" => "role_" . $row['role'] . " " . $position_class);
        $tot_module += $row['total'];
      }
    }
    
    if ($tot_module > $max) {
      $max = $tot_module;
      $max_module = $module;
    }
    
  }
  
  return array("module" => $max_module, "line_up" => $line_up[$max_module], "total" => $max);
}

function top_round_all_lineup($round) {
  $modules = explode("\r\n", variable_get("fantacalcio_moduli_titolari", ""));
  $max = 0; $max_module = "";
  foreach ($modules as $module) {
    $number_roles = explode("-", $module);
    $tot_module = 0;
    
    foreach ($number_roles as $role => $number_role) {
      $sql = "SELECT * FROM {fanta_votes} v, {fanta_players} p
          WHERE v.round = '%d' 
          AND p.pl_id = v.pl_id
          AND p.role = '%d' 
          AND v.provider = '%d'
          ORDER BY v.total DESC";
      $result = db_query_range($sql, array($round, $role, variable_get("fantacalcio_votes_provider", 1)), 0, $number_role);
      while ($row = db_fetch_array($result)) {
        $row_data = array( array("data" => convert_role($row['role']), "class" => "role"), $row['name'], get_amm_esp($row['yellow_cards'], $row['red_cards']),
                  "<strong>" . $row['total'] . "</strong>", "<strong>" . $row['vote'] . "</strong>", 
                  ($row['goals_for'] + $row['penalty_goals']), $row['goals_against'], $row['assists'],  
                  $row['saved_penalties'], $row['missed_penalties'], $row['own_goals']);
        $line_up[$module][] = array("data" => $row_data, "class" => "role_" . $row['role']);
        $tot_module += $row['total'];
      }
    }
    
    if ($tot_module > $max) {
      $max = $tot_module;
      $max_module = $module;
    }
    
  }
  
  return array("module" => $max_module, "line_up" => $line_up[$max_module], "total" => $max);
}

function flop_round_team_lineup($round, $t_id) {
  $modules = explode("\r\n", variable_get("fantacalcio_moduli_titolari", ""));
  $min = 1000; $min_module = "";
  foreach ($modules as $module) {
    $number_roles = explode("-", $module);
    $tot_module = 0; $module_ok = TRUE;

    foreach ($number_roles as $role => $number_role) {
      $sql = "SELECT * FROM {fanta_votes} v, {fanta_players} p
          WHERE v.round = '%d' 
          AND p.pl_id = v.pl_id
          AND p.role = '%d'
		  AND v.provider = '%d'
          AND p.pl_id IN (SELECT pl_id FROM {fanta_squads} WHERE t_id = '%d' AND in_team = 1)          
          ORDER BY v.total ASC";
      $result = db_query_range($sql, array($round, $role, variable_get("fantacalcio_votes_provider", 1), $t_id), 0, $number_role);
      if (db_affected_rows($result) == $number_role) {
        while ($row = db_fetch_array($result)) {
          $position = get_player_position($row['pl_id'], variable_get("fantacalcio_main_competition", 1), $t_id, $round);
          $position_class = ($position == 0) ? "out_0" : (($position > 1) ? "out_2" : "");
          $row_data = array( array("data" => convert_role($row['role']), "class" => "role"), $row['name'], get_amm_esp($row['yellow_cards'], $row['red_cards']),
                    "<strong>" . $row['total'] . "</strong>", "<strong>" . $row['vote'] . "</strong>", 
                    ($row['goals_for'] + $row['penalty_goals']), $row['goals_against'], $row['assists'],  
                    $row['missed_penalties'], $row['saved_penalties'], $row['own_goals']);
          $line_up[$module][$row['pl_id']] = array("data" => $row_data, "class" => "role_" . $row['role'] . " " . $position_class);
          $tot_module += $row['total'];
        }
      }
      else {
        $module_ok = FALSE;
      }
    }
    
    if ($module_ok) {
      if ($tot_module < $min) {
        $min = $tot_module;
        $min_module = $module;
      }
    }
    else unset($line_up[$module]);
    
  }

  return array("module" => $min_module, "line_up" => $line_up[$min_module], "total" => $min);
}

function flop_round_all_lineup($round) {
  $modules = explode("\r\n", variable_get("fantacalcio_moduli_titolari", ""));
  $min = 1000; $min_module = "";
  foreach ($modules as $module) {
    $number_roles = explode("-", $module);
    $tot_module = 0; $module_ok = TRUE;
    
    foreach ($number_roles as $role => $number_role) {
      $sql = "SELECT * FROM {fanta_votes} v, {fanta_players} p
          WHERE v.round = '%d' 
          AND p.pl_id = v.pl_id
          AND p.role = '%d'
		  AND v.provider = '%d'
          ORDER BY v.total ASC";
      $result = db_query_range($sql, array($round, $role, variable_get("fantacalcio_votes_provider", 1)), 0, $number_role);
      if (db_affected_rows($result) == $number_role) {
        while ($row = db_fetch_array($result)) {
          $row_data = array( array("data" => convert_role($row['role']), "class" => "role"), $row['name'], get_amm_esp($row['yellow_cards'], $row['red_cards']),
                    "<strong>" . $row['total'] . "</strong>", "<strong>" . $row['vote'] . "</strong>", 
                    ($row['goals_for'] + $row['penalty_goals']), $row['goals_against'], $row['assists'],  
                    $row['missed_penalties'], $row['saved_penalties'], $row['own_goals']);
          $line_up[$module][$row['pl_id']] = array("data" => $row_data, "class" => "role_" . $row['role'] );
          $tot_module += $row['total'];
        }
      }
      else {
        $module_ok = FALSE;
      }
    }
    
    if ($module_ok) {
      if ($tot_module < $min) {
        $min = $tot_module;
        $min_module = $module;
      }
    }
    else unset($line_up[$module]);
    
  }
  
  return array("module" => $min_module, "line_up" => $line_up[$min_module], "total" => $min);
}

function topflop_all_round($round) {
  $top = top_round_all_lineup($round);
  $flop = flop_round_all_lineup($round);
  
  $header = array("Ruolo", "Nome", "", "Totale", "Voto", "GF", "GS", "Assist", "Rig. Parato", "Rig. Sbagliato", "Autogol");
  
  $out = "<h3>" . $round . "&ordf; giornata</h3><br>";
  
  //TOP
  $out .= "<p><strong>TOP</strong></p>";
  $out .= "<p><strong>" . substr($top['module'], 2) . "</strong></p>";
  $out .= theme_table($header, $top['line_up']);
  
  //TOP - differenza col totale
  $rows_summary = array();
  $rows_summary[] = array("Totale Top", array("data" => $top['total'], "class" => "bold"));
  $out .= theme_table(array(), $rows_summary);

  //FLOP
  $out .= "<p><strong>FLOP</strong></p>";
  $out .= "<p><strong>" . substr($flop['module'], 2) . "</strong></p>";
  $out .= theme_table($header, $flop['line_up']);
  
  //FLOP - differenza col totale
  $rows_summary = array();
  $rows_summary[] = array("Totale Flop", array("data" => $flop['total'], "class" => "bold"));
  $out .= theme_table(array(), $rows_summary);
  
  return $out;
}

function get_tot_match($round, $t_id) {
  $sql = "SELECT * FROM {fanta_matches} 
      WHERE g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = '%d')
      AND round = '%d' 
      AND (t1_id = '%d' OR t2_id = '%d')";
  $result = db_query($sql, variable_get("fantacalcio_main_competition", 1), get_competition_round_by_round($round, variable_get("fantacalcio_main_competition", 1)), $t_id, $t_id);
  while ($row = db_fetch_object($result)) {
    if ($t_id == $row->t1_id) return ($row->tot_1 - $row->mod_por_1 - $row->mod_dif_2 - $row->mod_centr_1 - $row->mod_att_1);
    if ($t_id == $row->t2_id) return ($row->tot_2 - $row->mod_por_2 - $row->mod_dif_1 - $row->mod_centr_2 - $row->mod_att_2);
  }
}

function topflop_choose_round_form($form_state, $c_id, $round) {

  $max_round = get_last_votes();
  
  for ($i = 1; $i <= $max_round; $i++)
    $rounds[$i] = $i . filter_xss("a Giornata");
  
  $form['round'] = array(
    '#type' => 'select',
    '#options' => $rounds,
    '#default_value' => $round,
    '#attributes' => array("data-mini" => "false", "data-inline" => "true", "onchange" => "this.form.submit()")
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#attributes' => array("class" => "hide")
  );
  
  return $form;
}

function topflop_choose_round_form_submit($form, &$form_state) {
  $round = $form_state['values']['round'];

  drupal_goto("statistiche/topflop/" . $round);
}

function topflop_choose_round_all_form($form_state, $c_id, $round) {

  $max_round = get_last_votes();
  
  for ($i = 1; $i <= $max_round; $i++)
    $rounds[$i] = $i . filter_xss("a Giornata");
  
  $form['round'] = array(
    '#type' => 'select',
    '#options' => $rounds,
    '#default_value' => $round,
    '#attributes' => array("data-mini" => "false", "data-inline" => "true", "onchange" => "this.form.submit()")
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#attributes' => array("class" => "hide")
  );
  
  return $form;
}

function topflop_choose_round_all_form_submit($form, &$form_state) {
  $round = $form_state['values']['round'];

  drupal_goto("statistiche/topflop-all/" . $round);
}
