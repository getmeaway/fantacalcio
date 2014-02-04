<?php
/**
* @file fantacalcio.calendar.inc
* 
*/

function get_matches($g_id = '') {

  if (!empty($g_id)) $sql = "SELECT * FROM {fanta_matches} WHERE g_id = '%d'";
  else $sql = "SELECT * FROM {fanta_matches} ";
  
  $dates = get_matches_date();
  $groups = get_competitions_groups();

  $result = db_query($sql, $g_id);
  $matches = array();
  while ($row = db_fetch_object($result)) {
    $m_id = $row->m_id;
    $matches[$m_id] = $row;
    $matches[$m_id]->date = $dates[$groups[$row->g_id]][$row->round];
  }
  return $matches;
}

function get_matches_date() {
  $sql = "SELECT * FROM {fanta_rounds} r, {fanta_rounds_competitions} c
      WHERE c.round = r.round";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $dates[$row->c_id][$row->round] = $row->date;
  }
  return $dates;
}


function get_matches_table($g_id) {

  drupal_add_js("$('.top').click(function() { $(document).scrollTop(0);});", "inline", "footer");

  //ultima e prossima giornata
  if ($last_round = get_last_competition_round(get_cid_by_gid($g_id))) {
    $matches = get_round_matches($last_round, $g_id);
    $out_last = "\n<div class='round'>" .
        "\n<h4>Ultima giornata</h4>";
    $out_last .= show_matches($matches, $g_id, TRUE);
    $out_last .= "\n</div>";
  }

  if ($next_round = get_next_competition_round(get_cid_by_gid($g_id))) {
    $matches = get_round_matches($next_round, $g_id);
    $out_next = "\n<div class='round'>".
        "\n<h4>" . (get_round_date($next_round, get_cid_by_gid($g_id)) > time() ? "Prossima giornata" : "Giornata in corso") . "</h4>";
    $out_next .= show_matches($matches, $g_id, TRUE);
    $out_next .= "\n</div>";
  }
  
  if ( (time() - get_round_date($last_round, get_cid_by_gid($g_id))) > (get_round_date($next_round, get_cid_by_gid($g_id)) - time()))
    $out .= $out_next . $out_last;
  else $out .= $out_last . $out_next;

  $out .= "\n<div style='clear:both;height:20px;'></div>";

  $num_rounds = get_number_rounds($g_id);

  //elenco giornate
  if ($num_rounds > 1)
    $out .= "<div style='clear:both;'></div>" . (!is_mobile() ? get_round_list($g_id) : "");

  //calendario completo
  $matches = get_matches($g_id);
  $out .= show_matches($matches, $g_id);

  return $out;
}

function get_matches_round($g_id, $args = array()) {

  $out = "\n<div style='clear:both;height:20px;'></div>";

  //calendario completo
  $matches = get_round_matches($args['round'], $g_id);
  $out .= show_matches($matches, $g_id);
  
  return $out;
}

function get_round_list($g_id) {
  $result = db_query("SELECT DISTINCT round FROM {fanta_matches} WHERE g_id = '%d'", $g_id);  
  while ($row = db_fetch_array($result)) {
    $rounds_gid[] = $row['round'];
  }
  $c_id = get_cid_by_gid($g_id);
  $sql = "SELECT * FROM {fanta_rounds} r, {fanta_rounds_competitions} c WHERE c.round = r.round AND c.c_id = '%d' ";
  $result = db_query($sql, $c_id);
  while ($row = db_fetch_array($result)) {
    $round = $row['competition_round'];
    if (in_array($round, $rounds_gid))
      $rounds[$round] = empty($row["round_label"]) ? $row["competition_round"] : $row["round_label"];
  }

  ksort($rounds);

  $out = "";
  $out .= "<div class='round_list' id='round-list'>";
  $out .= "\n<ul>";
  foreach ($rounds as $round => $round_label) {
    $out .= "\n<li class='round_list'>" .
          "<a href='#" . $g_id . "_" . $round . "'>" . $round_label . "</a>" .
        "</li>";
  }
  $out .= "\n</ul>";
  $out .= "\n</div>" .
      "\n<div style='clear:both;'></div>";
  return $out;
}

function get_round_matches($round, $g_id = '', $c_id = '') {
  $dates = get_matches_date();
  $groups = get_competitions_groups();

  if ($g_id) {
    $sql = "SELECT * FROM {fanta_matches} WHERE g_id = '%d' AND round = '%d'";
    $result = db_query($sql, $g_id, $round);
  }
  if ($c_id) {
    $sql = "SELECT * FROM {fanta_matches} WHERE g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id =  '%d') AND round = '%d'";
    $result = db_query($sql, $c_id, $round);
  }
  
  $matches = array();
  while ($row = db_fetch_object($result)) {
    $m_id = $row->m_id;
    $matches[$m_id] = $row;
    $matches[$m_id]->date = $dates[$groups[$row->g_id]][$row->round];
  }
  return $matches;
}

function show_matches($matches, $g_id = '', $is_header = FALSE) {

  global $user;

  $out = "<div class='no-padding'>";

  $teams = get_teams();
  $rounds = get_rounds_by_g_id($g_id);
    
  foreach ($matches as $m_id => $match) {
    $round = $match->round;
    $m_id = $match->m_id;
    $round_matches[$round][$m_id] = $match;
  }

  if ($round_matches) {

  ksort($round_matches);

  $i = 0;

  #table body
  foreach ($round_matches as $r_id => $round) {

    $header = array(); $rows = array();
    $i++;
    $attributes = array("id" => "round_" . $r_id, "data-role" => "table_");

    foreach ($round as $m_id => $match) $date = $match->date;
    $date = get_round_date($match->round, get_cid_by_gid($g_id));
    
    #table header
    $round_name = (empty($rounds[$match->round]->round_label)) ? $r_id . "&deg; Giornata" : $rounds[$match->round]->round_label;  
#    $header[] = array("data" => "<span>" . $round_name . "</span><span style='margin-left:14px; font-size: 80%;'>" . date("d-m-Y H:i", $date) . "</span>" . $round_label, "colspan" => 3);

    $header[] = array("data" => $round_name . " " . date("d-m-Y H:i", $date) . " " . $round_label, "colspan" => 3);

    if (count($round_matches) > 1) {
      if (is_mobile()) {
        $header[] = array("data" => "<a href='#' class='top' title='Torna su' data-role=button data-icon=arrow-u data-iconpos='notext' data-inline='true' data-theme='c'>Top</a>", "colspan" => 2);
      }
      else {
        $header[] = array("data" => "<a href='#' class='top' title='Torna su'>" .
          "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/top.png'>" .
          "</a>", "colspan" => 2);
      }
    }
    else $header[] = "";

    foreach ($round as $m_id => $match) {

      if (!empty($match->match_label)) $rows[$m_id][] = array("data" => "(" . $match->match_label . ")", "class" => "team");

      $t1_name = ($match->t1_id != 0) ? l($teams[$match->t1_id]->name, "squadre/" . $match->t1_id) : $match->t1_label;
      $t2_name = ($match->t2_id != 0) ? l($teams[$match->t2_id]->name, "squadre/" . $match->t2_id) : $match->t2_label;

      $t1_name = ($match->t1_id == $user->t_id) ? "<span class=bold>" . $t1_name . "</span>" : $t1_name;
      $t2_name = ($match->t2_id == $user->t_id) ? "<span class=bold>" . $t2_name . "</span>" : $t2_name;

      $rows[$m_id][] = array("data" => $t1_name . " - " . $t2_name, "class" => "team");
      
      if (!is_mobile())
        $link_attributes = array("class" => "_thickbox", "rel" => "lightframe['" . $match->round . "_" . $match->g_id . "'|width:970px;]");
      else $link_attributes = array();

      if ($match->played == 1) {
        $overtime = ""; $penalties = "";
        if (is_match_draw($match) && is_passaggio_turno(get_cid_by_gid($match->g_id), $match->round)) {
          $overtime = " (" . $match->goals_ot_1 . "-" . $match->goals_ot_2 . ")ot";
          if ($match->goals_ot_1 == $match->goals_ot_2)
            $penalties = " (" . $match->penalties_1 . "-" . $match->penalties_2 . ")rig";
        }        
        $rows[$m_id][] = array("data" => $match->goals_1 . " - " . $match->goals_2 . $overtime . $penalties, "class" => "gol");
        $rows[$m_id][] = array("data" => $match->tot_1 . " - " . $match->tot_2, "class" => "pt");
      }
      else $rows[$m_id][] = array("data" => "&nbsp;", "colspan" => 2);

      //link statistiche      
      if ($match->round == get_next_competition_round(get_cid_by_gid($match->g_id)) || $match->played == 1) {
        if (is_mobile())
          $rows[$m_id][] = l("", "scheda/partita/" . $m_id, array("attributes" => array('data-role' => 'button', 'data-icon' => "info", 'data-iconpos' => "notext", 'data-inline' => 'true', 'data-theme' => 'a')));
        else 
          $rows[$m_id][] = l("<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/details.png'>", "scheda/partita/" . $m_id, array("html" => "true", "attributes" => $link_attributes));
      }
      else $rows[$m_id][] = array("data" => "&nbsp;", "colspan" => 1);
    }

    $out .= "<div class='round' id='" . $g_id . "_" . $r_id . "'>" . theme_table($header, $rows, $attributes) . "</div>";

    if ($i%2 == 0) $out .= "<div style='clear:both;'></div>";
  }

  }

  if ($i != 1)
    $out .= "<div style='clear:both;'></div>";

  $out .= "</div>";

  if (is_mobile() && !$is_header)
    return "<div data-role=collapsible data-theme=c data-collpsed=true><h4>Altre giornate...</h4>" . $out . "</div>";
  else 
    return $out;
}

function _fantacalcio_calendario($c_name = '') {

  if (empty($c_name) || !competition_has_arg($c_name, "matches")) 
    $out = choose_competition(array("matches" => 1));

  else {
    drupal_set_title(check_plain("Calendario - " . ucwords($c_name)));
    $out = make_tabs('get_matches_table', $c_name, "matches");
  }
  return $out;

}

function fantacalcio_calendario_round($c_name = '', $round) {

  if (empty($c_name) || !competition_has_arg($c_name, "matches")) 
    $out = choose_competition(array("matches" => 1));

  else {
    if (round_exists($round)) {
      $round_data = get_competition_round($round, get_competition_id($c_name));
      drupal_set_title(filter_xss("Calendario - " . ucwords($c_name) . " - " . ($round_data->round_label != "" ? $round_data->round_label : $round . "&ordf; giornata") ));
      $out = make_tabs('get_matches_round', $c_name, "matches", array('round' => $round));
    }
  }
  return $out;

}



function show_match_details($m_id) {

  $teams = get_teams();
  $matches = get_matches();
  $match = $matches[$m_id];
  $match->c_id = get_cid_by_gid($match->g_id);
  $round = get_competition_round($match->round, $match->c_id);
  
  $empty_rows = get_details_empty_rows($match);

  $out = "<div style='margin:auto; margin-bottom: 10px; text-align: center;'>";

  $out .= "<h3>" . (!empty($round->round_label)  ? $round->round_label : ($match->round . "&ordf; Giornata")) . "</h3><br>";
  $out .= "<h4>" . $teams[$match->t1_id]->name . " - " . $teams[$match->t2_id]->name . "</h4><hr>";
  $out .= "</div>";

  $out .= "<div class='team-details first-team'>"
      . show_team_details($match->t1_id, $match->round, $match->c_id, $match->m_id, $match->played, $empty_rows)
      . "</div>";

  $out .= "<div class='team-details'>"
      . show_team_details($match->t2_id, $match->round, $match->c_id, $match->m_id, $match->played, $empty_rows)
      . "</div>";
      
  if (is_passaggio_turno(get_cid_by_gid($match->g_id), $match->round) && is_match_draw($match)) {
    $out .= "<div class='clear'></div>";
    $out .= "<center>";
    $out .= "<h4>Tempi Supplementari</h4>";
    
    //overtime team 1
    $out .= "<div class='team-details first-team'>"
        . show_match_overtime($match, $match->t1_id)
        . "</div>";
    
    //overtime team 2
    $out .= "<div class='team-details'>"
        . show_match_overtime($match, $match->t2_id)
        . "</div>";
    
    if ($match->goals_ot_1 == $match->goals_ot_2) {
      $last_penalty_index = get_penalties_last_index(get_penalties($match));
      $out .= "<h4>Rigori</h4>";
      
      //penalties team 1
      $out .= "<div class='team-details first-team'>"
          . show_match_penalties($match, $match->t1_id, $last_penalty_index)
          . "</div>";
      
      //penalties team 2
      $out .= "<div class='team-details'>"
          . show_match_penalties($match, $match->t2_id, $last_penalty_index)
          . "</div>";
    }
    $out .= "</center>";
  }

  return $out;

}

function show_match_overtime($match, $t_id) {
  $sql = "SELECT round FROM {fanta_rounds_competitions} " .
      "WHERE competition_round = '%d' " .
      "AND c_id = '%d'";
  $result = db_query($sql, $match->round, get_cid_by_gid($match->g_id));
  $round = db_result($result);

  $votes = get_votes($round);
  $players = get_players($round);

  //riserve non entrate
  $sql = "SELECT * FROM {fanta_lineups} f, {fanta_players} p
      WHERE f.pl_id = p.pl_id
      AND f.t_id = '%d'
      AND f.round = '%d'
      AND f.c_id = '%d'
      AND f.has_played = 2 
      ORDER BY p.role";
  $result = db_query($sql, $t_id, $match->round, get_cid_by_gid($match->g_id));
  while ($row = db_fetch_array($result)) {
    $riserve[$row['pl_id']] = $row;
  }
  
  $teams = get_teams();
  
  $out = "<div style='margin:auto; margin-top: 10px; text-align: center;'>";
  
  $points = 0;

  $header_data = array("", "", "Nome", "Squadra", "", "Tot", "Voto", "Gol Fatti", "Gol Subiti", "Assist", "Rigori Sbagliati", "Rigori Parati", "Autogol");
  //$header = array("", "", "Nome", "Squadra", "", "Tot", "Voto", "GF", "GS", "Assist", "Rig Sba", "Rig Par", "Auto gol");
  $header[] = array("data" => "", "class" => "team-details-header");
  $header[] = array("data" => "Nome", "class" => "team-details-header");
  $header[] = array("data" => "", "class" => "team-details-header");
  $header[] = array("data" => "", "class" => "team-details-header");
  $header[] = array("data" => "Tot", "class" => "team-details-header");
  $header[] = array("data" => "Voto", "class" => "team-details-header");
  $header[] = array("data" => "Goal Fatti", "class" => "team-details-header");
  $header[] = array("data" => "Goal Subiti", "class" => "team-details-header");
  $header[] = array("data" => "Assist", "class" => "team-details-header");
  $header[] = array("data" => "Rigori Sbagliati", "class" => "team-details-header");
  $header[] = array("data" => "Rigori Parati", "class" => "team-details-header");
  $header[] = array("data" => "Auto goal", "class" => "team-details-header");

  $rows = array();

  foreach ($riserve as $riserva) {
    $pl_id = $riserva['pl_id'];
    
    $data = array();
    
    $data[] = array("data" => "<span class='team-details-'>" . convert_role($players[$riserva['pl_id']]->role) . "</span>", "class" => "role");
    //$data[] = "<span class='team-details-role role'>" . convert_role($players[$riserva['pl_id']]->role) . "</span>";
    $data[] = "<span class='team-details-name'>" . compress_string($players[$riserva['pl_id']]->name) . "</span>";
    $data[] = "<span class='team-details-role'>" . substr(ucwords(convert_teams($players[$riserva['pl_id']]->team)), 0, 3) . "</span>";
    $data[] = get_amm_esp($votes[$pl_id]->amm, $votes[$pl_id]->esp);
    $data[] = array("data" => $votes[$pl_id]->tot, "class" => get_details_class($votes[$pl_id]->tot) . " team-details-tot");
    $data[] = array("data" => $votes[$pl_id]->vote, "class" => get_details_class($votes[$pl_id]->vote));
    $data[] = array("data" => ($votes[$pl_id]->gol_fatto + $votes[$pl_id]->rig_fatto)
            . (($votes[$pl_id]->rig_fatto > 0) ? "(" . $votes[$pl_id]->rig_fatto . ")" : ""), 
            "class" => get_details_class(($votes[$pl_id]->gol_fatto + $votes[$pl_id]->rig_fatto),
            "bonus"));
    $data[] = array("data" => $votes[$pl_id]->gol_subito, "class" => get_details_class($votes[$pl_id]->gol_subito, "malus"));
    $data[] = array("data" => $votes[$pl_id]->assist, "class" => get_details_class($votes[$pl_id]->assist, "bonus"));
    $data[] = array("data" => $votes[$pl_id]->rig_sbagliato, "class" => get_details_class($votes[$pl_id]->rig_sbagliato, "malus"));
    $data[] = array("data" => $votes[$pl_id]->rig_parato, "class" => get_details_class($votes[$pl_id]->rig_parato, "bonus"));
    $data[] = array("data" => $votes[$pl_id]->autogol, "class" => get_details_class($votes[$pl_id]->autogol, "malus"));

    $points += $votes[$pl_id]->tot;
    
    $rows[$pl_id] = array("data" => $data, "class" => "role_" . $players[$pl_id]->role);

  }

  for($i = count($riserve); $i < 3; $i++) {
    $rows[][] = array("data" => "&nbsp;", "colspan" => 13, "class" => "role_" . $players[$pl_id]->role);
  }

  $rows['space'][] = array("data" => "", "colspan" => 13);
    
  $out .= "<div class='team-details-lineup'>" . theme_table($header, $rows, array( "style" => "")) . "</div>";

  $goals_ot = floor(($points - 16)/ 4);
  $goals_ot = ($goals_ot >= 0) ? $goals_ot : 0 ;

  $rows = array();
  $rows[] = array("Punti", $points);
  
  $rows[] = array("data" => array("Totale", $points), "style" => "font-weight:bold") ;
  $rows[] = array("data" => array("Gol", $goals_ot), "style" => "font-weight:bold") ;

  $out .= "<div style='width: 40%; margin:auto; margin-top: 10px;'>"
      . theme_table(array(), $rows, array("width" => "100%", "style" => "", "align" => "char", "char" => "."))
      . "</div>";
      
  $out .= "</div>";
      
  return $out;
}

function show_match_penalties($match, $t_id, $last_penalty_index) {
  $sql = "SELECT round FROM {fanta_rounds_competitions} " .
      "WHERE competition_round = '%d' " .
      "AND c_id = '%d'";
  $result = db_query($sql, $match->round, get_cid_by_gid($match->g_id));
  $round = db_result($result);

  $votes = get_votes($round);
  $players = get_players($round);

  //rigoristi ordinati (solo quelli necessari)
  $sql = "SELECT * FROM {fanta_lineups} f
      WHERE f.t_id = '%d'
      AND f.round = '%d'
      AND f.c_id = '%d'
      AND f.has_played = 1 
      ORDER BY penalty_position";
  $result = db_query($sql, $t_id, $match->round, get_cid_by_gid($match->g_id));
  while ($row = db_fetch_array($result)) {
    $penalties[$row['pl_id']] = $row;
  }
  
  $teams = get_teams();
  
  $out = "<div style='margin:auto; margin-top: 10px; text-align: center;'>";
  
  $points = 0;

  $header[] = array("data" => "", "class" => "team-details-header");
  $header[] = array("data" => "", "class" => "team-details-header");
  $header[] = array("data" => "Voto", "class" => "team-details-header");
  $header[] = array("data" => "", "class" => "team-details-header");
  $header[] = array("data" => "", "class" => "team-details-header");

  $rows = array();

  $i = 0;
  foreach ($penalties as $penalty) {
    if ($i < $last_penalty_index) {
      $pl_id = $penalty['pl_id'];
      
      $data = array();
      $data[] = array("data" => "<span class='team-details-'>" . convert_role($players[$penalty['pl_id']]->role) . "</span>", "class" => "role");
      //$data[] = "<span class='team-details-role role'>" . convert_role($players[$penalty['pl_id']]->role) . "</span>";
      $data[] = "<span class='team-details-name'>" . compress_string($players[$penalty['pl_id']]->name) . "</span>";
      $data[] = "<span class='team-details-role'>" . ucwords(convert_teams($players[$penalty['pl_id']]->team)) . "</span>";
      $data[] = array("data" => $votes[$pl_id]->vote, "class" => get_details_class($votes[$pl_id]->vote));
      $data[] = array("data" => get_penalty_result($votes[$pl_id]->vote), 
              "class" => get_details_class(($votes[$pl_id]->gol_fatto + $votes[$pl_id]->rig_fatto), "bonus"));

      $points += ($votes[$pl_id]->vote >= 6) ? 1 : 0;
      
      $rows[$pl_id] = array("data" => $data, "class" => "role_" . $players[$pl_id]->role);
      
      $i++;
    }
  }

  $rows['space'][] = array("data" => "", "colspan" => 5);
  $rows[] = array("data" => array(array("data" => "Totale", "colspan" => 4), $points), "style" => "font-weight:bold") ;
    
  $out .= "<div class='team-details-lineup'>" . theme_table($header, $rows, array( "style" => "")) . "</div>";
  
  $out .= "</div>";
      
  return $out;
}

function show_team_details($t_id, $competition_round, $c_id, $m_id, $played, $empty_rows) {
  if ($played == 0) return show_team_details_not_played($t_id, $competition_round, $c_id, $m_id, $empty_rows);
  elseif ($played == 1) return show_team_details_played($t_id, $competition_round, $c_id, $m_id, $empty_rows);
}

function show_team_details_played($t_id, $competition_round, $c_id, $m_id, $empty_rows) {

  $teams = get_teams();
  
  $out = "<div style='margin:auto; margin-top: 10px; text-align: center;'>";
  $out .= "<h3>" . $teams[$t_id]->name . "</h3>";
  $out .= "<p><img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/shirts/" . $teams[$t_id]->shirt . "'></p>";

  $sql = "SELECT round FROM {fanta_rounds_competitions} " .
      "WHERE competition_round = '%d' " .
      "AND c_id = '%d'";
  $result = db_query($sql, $competition_round, $c_id);
  $round = db_result($result);

  $votes = get_votes($round);
  $players = get_players($round);

  $titolari = get_details_titolari($t_id, $c_id, $competition_round, $players);
  $riserve = get_details_riserve($t_id, $c_id, $competition_round, $players);

  $modulo = array();
  foreach ($titolari as $titolare) {
    $modulo[$players[$titolare['pl_id']]->role]++;
  }
  unset($modulo[0]);

  $out .= "<p>" . implode("-", $modulo) . "</p>";
  $out .= "</div>";

  $points = 0;

  $header_data = array("", "", "Nome", "Squadra", "", "Tot", "Voto", "Gol Fatti", "Gol Subiti", "Assist", "Rigori Sbagliati", "Rigori Parati", "Autogol");
  $header[] = array("data" => "", "class" => "team-details-header");
  $header[] = array("data" => "", "class" => "team-details-header");
  $header[] = array("data" => "Nome", "class" => "team-details-header");
  $header[] = array("data" => "", "class" => "team-details-header");
  $header[] = array("data" => "", "class" => "team-details-header");
  $header[] = array("data" => "Tot", "class" => "team-details-header");
  $header[] = array("data" => "Voto", "class" => "team-details-header");
  $header[] = array("data" => "Goal Fatti", "class" => "team-details-header");
  $header[] = array("data" => "Goal Subiti", "class" => "team-details-header");
  $header[] = array("data" => "Assist", "class" => "team-details-header");
  $header[] = array("data" => "Rigori Sbagliati", "class" => "team-details-header");
  $header[] = array("data" => "Rigori Parati", "class" => "team-details-header");
  $header[] = array("data" => "Auto goal", "class" => "team-details-header");

  $rows = array();

  foreach ($titolari as $titolare) {
    $pl_id = $titolare['pl_id'];
    
    $data = array();
    
    $data[] = ($titolare['has_played'] == 1) ? "" : "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/out.png'>";

    $data[] = array("data" => "<span class='team-details-'>" . convert_role($players[$titolare['pl_id']]->role) . "</span>", "class" => "role");
    $data[] = "<span class='team-details-name'>" . compress_string($players[$titolare['pl_id']]->name) . "</span>";
    $data[] = "<span class='team-details-role'>" . substr(ucwords(convert_teams($players[$titolare['pl_id']]->team)), 0, 3) . "</span>";
    if ($titolare['has_played'] == 1) {
      $data[] = get_amm_esp($votes[$pl_id]->yellow_cards, $votes[$pl_id]->red_cards);
      $data[] = array("data" => $votes[$pl_id]->total, "class" => get_details_class($votes[$pl_id]->tot) . " team-details-tot");
      $data[] = array("data" => $votes[$pl_id]->vote);
      $data[] = array("data" => ($votes[$pl_id]->goals_for + $votes[$pl_id]->penalty_goals > 0 ?($votes[$pl_id]->goals_for + $votes[$pl_id]->penalty_goals) . (($votes[$pl_id]->penalty_goals > 0) ? "(" . $votes[$pl_id]->penalty_goals . ")" : "") : "-"), 
                  "class" => get_details_class(($votes[$pl_id]->goals_for + $votes[$pl_id]->penalty_goals), "bonus"));

      $data[] = array("data" => ($votes[$pl_id]->goals_against > 0 ? $votes[$pl_id]->goals_against : "-") , "class" => get_details_class($votes[$pl_id]->goals_against, "malus"));
      $data[] = array("data" => ($votes[$pl_id]->assists > 0 ? $votes[$pl_id]->assists : "-"), "class" => get_details_class($votes[$pl_id]->assists, "bonus"));
      $data[] = array("data" => ($votes[$pl_id]->missed_penalties > 0 ? $votes[$pl_id]->missed_penalties : "-"), "class" => get_details_class($votes[$pl_id]->missed_penalties, "malus"));
      $data[] = array("data" => ($votes[$pl_id]->saved_penalties > 0 ? $votes[$pl_id]->saved_penalties : "-"), "class" => get_details_class($votes[$pl_id]->saved_penalties, "bonus"));
      $data[] = array("data" => ($votes[$pl_id]->own_goals > 0 ? $votes[$pl_id]->own_goals : "-"), "class" => get_details_class($votes[$pl_id]->own_goals, "malus"));

      $points += $votes[$pl_id]->total;
    }
    else $data[] = array("data" => "", "colspan" => 10);
    
    $rows[$pl_id] = array("data" => $data, "class" => "role_" . $players[$pl_id]->role);

  }

  $rows['space'][] = array("data" => "", "colspan" => 13);
  
  $played_reserves = 0;

  foreach ($riserve as $riserva) {
    if ($riserva['has_played'] == 1) {
      $pl_id = $riserva['pl_id'];
      
      $data = array();
      
      $data[] = "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/in.png'>";
      //$data[] = "<span class='team-details-role role'>" . convert_role($players[$riserva['pl_id']]->role) . "</span>";
      $data[] = array("data" => "<span class='team-details-'>" . convert_role($players[$riserva['pl_id']]->role) . "</span>", "class" => "role");
      $data[] = "<span class='team-details-name'>" . compress_string($players[$riserva['pl_id']]->name) . "</span>";
      $data[] = "<span class='team-details-role'>" . substr(ucwords(convert_teams($players[$riserva['pl_id']]->team)), 0, 3) . "</span>";
      
      if ($riserva['has_played'] == 1) {
        $data[] = get_amm_esp($votes[$pl_id]->yellow_cards, $votes[$pl_id]->red_cards);
        $data[] = array("data" => $votes[$pl_id]->total, "class" => get_details_class($votes[$pl_id]->tot) . " team-details-tot");
        $data[] = array("data" => $votes[$pl_id]->vote);
        $data[] = array("data" => ($votes[$pl_id]->goals_for + $votes[$pl_id]->penalty_goals > 0 ?($votes[$pl_id]->goals_for + $votes[$pl_id]->penalty_goals) . (($votes[$pl_id]->penalty_goals > 0) ? "(" . $votes[$pl_id]->penalty_goals . ")" : "") : "-"), 
                  "class" => get_details_class(($votes[$pl_id]->goals_for + $votes[$pl_id]->penalty_goals), "bonus"));

        $data[] = array("data" => ($votes[$pl_id]->goals_against > 0 ? $votes[$pl_id]->goals_against : "-") , "class" => get_details_class($votes[$pl_id]->goals_against, "malus"));
        $data[] = array("data" => ($votes[$pl_id]->assists > 0 ? $votes[$pl_id]->assists : "-"), "class" => get_details_class($votes[$pl_id]->assists, "bonus"));
        $data[] = array("data" => ($votes[$pl_id]->missed_penalties > 0 ? $votes[$pl_id]->missed_penalties : "-"), "class" => get_details_class($votes[$pl_id]->missed_penalties, "malus"));
        $data[] = array("data" => ($votes[$pl_id]->saved_penalties > 0 ? $votes[$pl_id]->saved_penalties : "-"), "class" => get_details_class($votes[$pl_id]->saved_penalties, "bonus"));
        $data[] = array("data" => ($votes[$pl_id]->own_goals > 0 ? $votes[$pl_id]->own_goals : "-"), "class" => get_details_class($votes[$pl_id]->own_goals, "malus"));
        $points += $votes[$pl_id]->total;
        
        $played_reserves++;
      }
      else $data[] = array("data" => "", "colspan" => 10);
    }
    
    $rows[$pl_id] = array("data" => $data, "class" => "role_" . $players[$pl_id]->role);
  }
  
  for ($i = $played_reserves; $i < $empty_rows; $i++) 
    $rows[] = array("data" => array("", "&nbsp;"), "class" => "team-details-empty-row");
    
  $out .= "<div class='team-details-lineup'>" . theme_table($header, $rows, array( "style" => "")) . "</div>";

  $mod = get_details_modificatori($t_id, $m_id);

  $bonus = get_bonus($t_id, $c_id, $competition_round);

  $total = $points + $mod['por'] + $mod['dif'] + $mod['centr'] + $mod['att'] + $bonus;
  $goals = floor(($total - 60)/ 6);
  $goals = ($gol >= 0) ? $goals : 0 ;

  $rows = array();
  $rows[] = array("Punti", $points);
  if (variable_get("fantacalcio_modificatore_portiere", 0) ) $rows[] = array("Mod Portiere", $mod['por']);
  if (variable_get("fantacalcio_modificatore_difesa", 0) ) $rows[] = array("Mod Difesa", $mod['dif']);
  if (variable_get("fantacalcio_modificatore_centrocampo", 0) ) $rows[] = array("Mod Centrocampo", $mod['centr']);
  if (variable_get("fantacalcio_modificatore_attacco", 0) ) $rows[] = array("Mod Attacco", $mod['att']);

  if ($bonus) $rows[] = array("Bonus", $bonus);

  $rows[] = array("data" => array("Totale", $total), "style" => "font-weight:bold") ;
  $rows[] = array("data" => array("Gol", $goals), "style" => "font-weight:bold") ;

  $out .= "<div style='width: 40%; margin:auto; margin-top: 10px;'>"
      . theme_table(array(), $rows, array("width" => "100%", "style" => "", "align" => "char", "char" => "."))
      . "</div>";
      
  return $out;
}

function show_team_details_not_played($t_id, $competition_round, $c_id, $m_id, $empty_rows) {
  return show_formazione(array("t_id" => $t_id, "round" => $competition_round, "c_id" => $c_id ));
}

function get_details_titolari($t_id, $c_id, $round, $players) {

  $formazione = array(); $names = array(); $positions = array(); $roles = array();
  $sql = "SELECT * FROM {fanta_lineups} " .
      "WHERE t_id = '%d' " .
      "AND c_id = '%d' " .
      "AND round = '%d' " .
      "AND position = 1";
  $result = db_query($sql, $t_id, $c_id, $round);
  while ($row = db_fetch_array($result)) {
    $formazione[] = $row;
    $names[] = $players[$row['pl_id']]->name;
    $roles[] = $players[$row['pl_id']]->role;
    $positions[] = $row['position'];
  }
  array_multisort($positions, SORT_ASC, $roles, SORT_ASC, $names, SORT_ASC, $formazione);

  return $formazione;
}

function get_details_riserve($t_id, $c_id, $round, $players) {

  $formazione = array(); $names = array(); $positions = array(); $roles = array();
  $sql = "SELECT * FROM {fanta_lineups} " .
      "WHERE t_id = '%d' " .
      "AND c_id = '%d' " .
      "AND round = '%d' " .
      "AND position > 1";
  $result = db_query($sql, $t_id, $c_id, $round);
  while ($row = db_fetch_array($result)) {
    $formazione[] = $row;
    $names[] = $players[$row['pl_id']]->name;
    $roles[] = $players[$row['pl_id']]->role;
    $positions[] = $row['position'];
  }

  array_multisort($roles, SORT_ASC, $positions, SORT_ASC, $names, SORT_ASC, $formazione);

  return $formazione;

}

function get_details_empty_rows($match) {
  $sql = "SELECT count(*) FROM {fanta_lineups} WHERE c_id = '%d' AND t_id = '%d' AND round = '%d' AND position > 1 AND has_played = 1";
  $t1_reserves = db_result(db_query($sql, $match->c_id, $match->t1_id, $match->round));
  $t2_reserves = db_result(db_query($sql, $match->c_id, $match->t2_id, $match->round));
  return max($t1_reserves, $t2_reserves);
}

function get_amm_esp($amm, $esp) {

  if ($amm == 1) return "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/yellow_card.png'>";
  elseif ($esp == 1) return "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/red_card.png'>";
  else return "";
}
  
function get_penalty_result($vote) {
  if ($vote >= 6) return "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/penalty_goal.png'>";
  else return "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/penalty_missed.png'>";
}

function get_details_modificatori($t_id, $m_id) {

  $sql = "SELECT * FROM {fanta_matches} " .
      "WHERE m_id = '%d' ";
  $result = db_query($sql, $m_id);
  while ($row = db_fetch_array($result)) {
    if ($t_id == $row['t1_id']) {
      $mod['por'] = $row['mod_por_1'];
      $mod['dif'] = $row['mod_dif_2'];
      $mod['centr'] = $row['mod_centr_1'];
      $mod['att'] = $row['mod_att_1'];
    }
    if ($t_id == $row['t2_id']) {
      $mod['por'] = $row['mod_por_2'];
      $mod['dif'] = $row['mod_dif_1'];
      $mod['centr'] = $row['mod_centr_2'];
      $mod['att'] = $row['mod_att_2'];
    }
  }

  return $mod;
}

function get_bonus($t_id, $c_id, $round) {

  $sql = "SELECT * FROM {fanta_matches} 
      WHERE g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = '%d') 
      AND round = '%d' 
      AND (t1_id = '%d' OR t2_id = '%d')";
  $result = db_query($sql, $c_id, $round, $t_id, $t_id);
  while ($row = db_fetch_array($result)) {
    if ($row['t1_id'] == $t_id) $bonus = $row['bonus_t1'];
    if ($row['t2_id'] == $t_id) $bonus = $row['bonus_t2'];
  }

  return ($bonus > 0) ? $bonus : "";

}

function get_details_class($value, $type = "") {
  $class = "";
  if ($value > 0) $class = "bold " . $type;
  
  return $class;
}
