<?php

/**
 * @file fantastats.inc
 *
 */

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

function stats_home() {

  $out = "<ul>";
  $out .= "<li>" . l("Cerca Giocatore", "statistiche/giocatore") . "</li>";
  $out .= "<li>" . l("Giocatori", "statistiche/players") . "</li>";
  $out .= "<li>" . l("Squadre", "statistiche/squadra") . "</li>";
  $out .= "<li>" . l("Generali", "statistiche/generali") . "</li>";
  $out .= "<li>" . l("Grafici", "statistiche/grafici") . "</li>";
  $out .= "<li>" . l("Top / Flop", "statistiche/topflop") . "</li>";
  $out .= "<li>" . l("Giocatori Liberi", "giocatori/liberi") . "</li>";
  $out .= "</ul>";

  return $out;
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

function dettagli_squadra($t_id = '') {

  $teams = array();
  $sql = "SELECT * FROM {fanta_teams} WHERE active = 1 ORDER BY name";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $teams[$row->t_id] = $row->name;
  }

  if (!empty($t_id)) {
    $team = get_team($t_id);

    if ($team) {
      $out .= scheda_squadra($team);
    }
    else $out .=  "Nessuna squadra con questo id";
  }

  return $out;
}

function statistiche_generali() {

  $matches = get_matches();
  $teams = get_teams();
  $groups = get_groups(variable_get("fantacalcio_main_competition", 1));
  if ($groups) {
    foreach ($groups as $g_id => $group) $g_ids[$g_id] = $g_id;
    $pt_max = 0; $pt_min = 1000;
    $tot_gol = 0; $tot_pts = 0; $n_matches = 0;

    foreach ($matches as $m_id => $match) {
      if ($match->played == 1 && in_array($match->g_id, $g_ids)) {
        $tot_pts += $match->tot_1 + $match->tot_2;
        $tot_gol += $match->goals_1 + $match->goals_2;
        $n_matches++;

        if ($match->tot_1 >= $pt_max) {
          if ($match->tot_1 > $pt_max)$max_args = array();
          $max_args[$m_id]['t_id'] = $match->t1_id;
          $max_args[$m_id]['round'] = $match->round;
          $max_args[$m_id]['pts'] = $match->tot_1;
          $pt_max = $match->tot_1;
        }
        if ($match->tot_2 >= $pt_max) {
          if ($match->tot_2 > $pt_max)$max_args = array();
          $max_args[$m_id]['t_id'] = $match->t2_id;
          $max_args[$m_id]['round'] = $match->round;
          $max_args[$m_id]['pts'] = $match->tot_2;
          $pt_max = $match->tot_2;
        }

        if ($match->tot_1 <= $pt_min) {
          if ($match->tot_1 < $pt_min)$min_args = array();
          $min_args[$m_id]['t_id'] = $match->t1_id;
          $min_args[$m_id]['round'] = $match->round;
          $min_args[$m_id]['pts'] = $match->tot_1;
          $pt_min = $match->tot_1;
        }
        if ($match->tot_2 <= $pt_min) {
          if ($match->tot_2 < $pt_min)$min_args = array();
          $min_args[$m_id]['t_id'] = $match->t2_id;
          $min_args[$m_id]['round'] = $match->round;
          $min_args[$m_id]['pts'] = $match->tot_2;
          $pt_min = $match->tot_2;
        }
      }
    }

    $header = array();

    $max_data = "";
	
	if ($max_args) {
      foreach ($max_args as $max) {
        $max_data .= $teams[$max["t_id"]]->name . " - " . $max['round'] . "&ordf; Giornata, ";
      }
      $max_data = substr($max_data, 0, -2);
    }
	
    $min_data = "";
	if ($max_args) {
      foreach ($min_args as $min) {
        $min_data .= $teams[$min["t_id"]]->name . " - " . $min['round'] . "&ordf; Giornata, ";
      }
      $min_data = substr($min_data, 0, -2);
	}

    $rows[] = array("Punteggio Massimo", $pt_max . " ($max_data)");
    $rows[] = array("Punteggio Minimo", $pt_min . " ($min_data)");
    $rows[] = array("Totale Gol", $tot_gol);
    $rows[] = array("Totale Punti", $tot_pts);
	if ($n_matches > 0)
      $rows[] = array("Media Gol/partita", round($tot_gol / $n_matches, 2));
	if ($n_matches > 0)
      $rows[] = array("Media Punti/partita", round($tot_pts/$n_matches, 2));

    return theme_table($header, $rows);
  }
  else return "Nessuna squadra";
}

function get_team($t_id) {
  $sql = "SELECT * FROM {fanta_teams} WHERE t_id= '%d'";
  $result = db_query($sql, $t_id);
  if (count($result) > 0) return db_fetch_object($result);
  else return FALSE;
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

function choose_pl_id_form_submit($form, $form_state) {

  $player = $form_state['values']['player'];
  $player_exploded = explode(',', $player);
  $pl_id = $player_exploded[0];

  drupal_goto("statistiche/giocatore/" . $pl_id);
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
