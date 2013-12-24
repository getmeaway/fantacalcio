<?php

/**
  * @file fantacalcio.common.inc
*/

/****************************/
/* recupero dati      */
/****************************/

function get_players($round = '' ) {

  $players = array();
  if ($round == "") $round = db_result(db_query("SELECT MAX(round) FROM {fanta_players_teams}"));
  
  $sql = "SELECT DISTINCT p.pl_id, p.name, p.role, rt.long_name AS team, rt.short_name AS team_short, t.active FROM {fanta_players} p, {fanta_players_teams} t, {fanta_real_teams} rt
      WHERE p.pl_id = t.pl_id 
      AND t.rt_id = rt.rt_id
      AND t.round = '%d'" ;
  $result = db_query($sql, $round);
  while ($row = db_fetch_object($result)) {
    $players[$row->pl_id] = $row;
  }
  return $players;
}

function get_all_players() {

  $players = array();
  $sql = "SELECT * FROM {fanta_players}" ;
  $result = db_query($sql, $round);
  while ($row = db_fetch_object($result)) {
    $players[$row->pl_id] = $row;
  }
  return $players;
}

function get_player_by_id($pl_id, $round = "") {
  if ($round == "") 
    $round = db_result(db_query("SELECT MAX(round) FROM {fanta_players_teams}"));
  
  $sql = "SELECT * FROM {fanta_players} p, {fanta_players_teams} t 
      WHERE p.pl_id = t.pl_id       
      AND p.pl_id = '%d'" ;

  if ($round == "") 
    $sql .= " AND t.round = '%d'";	  
	
  $result = db_query($sql, $pl_id, $round);
  while ($row = db_fetch_object($result)) {
    $player = $row;
  }
  return $player;
}

function get_player($pl_id) {
  $sql = "SELECT * FROM {fanta_players} p, {fanta_players_teams} r, {fanta_real_teams} t 
          WHERE p.pl_id = r.pl_id 
          AND r.rt_id = t.rt_id
          AND r.round = '%d' 
          AND p.pl_id = '%d'";
  $result = db_query($sql, get_last_competition_round(variable_get("fantacalcio_main_competition", 1)),  $pl_id);
  while ($row = db_fetch_object($result)) 
   return $row;
}

function get_all_rounds_played() {
  $sql = "SELECT c.competition_round AS round FROM {fanta_rounds} r 
        INNER JOIN {fanta_rounds_competitions} c
        ON c.round = r.round
        WHERE r.status = 1 ORDER BY competition_round";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->round] = $row->round; 
  }
  return $rounds;  
}

function get_player_rounds($pl_id) {
  $sql = "SELECT * FROM {fanta_players_teams} p, {fanta_real_teams} t
      WHERE p.rt_id = t.rt_id
      AND p.pl_id = '%d' 
      ORDER BY p.round" ;
  $result = db_query($sql, $pl_id);
  while ($row = db_fetch_object($result)) {
    $player[$row->round] = $row;
  }
  return $player;
}

function get_players_names() {
  $sql = "SELECT * FROM {fanta_players}" ;
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $players[strtolower($row->name)] = $row->pl_id;
  }
  return $players;
}

function get_competitions($args = array()) {

  $competitions = array();
  $sql = "SELECT * FROM {fanta_competitions} WHERE active = 1";
  if ($args) {
    foreach ($args as $key => $value) $sql .= " AND $key = $value";    
  }
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $c_id = $row->c_id;
    $competitions[$c_id] = $row;
  }

  return $competitions;

}

function get_groups($c_id = '', $arg = "") {

  if (!empty($c_id) && competition_exists($c_id)) $sql = "SELECT * FROM {fanta_groups} WHERE active = 1 AND c_id = '%d'";
  else $sql = "SELECT * FROM {fanta_groups} WHERE active = 1";

  if ($arg) {
    $sql .= " AND " . $arg . " > 0";
    $order = " ORDER BY $arg ASC";
  }

  $sql .= $order;

  $result = db_query($sql, $c_id);
  while ($row = db_fetch_object($result)) {
    $g_id = $row->g_id;
    $groups[$g_id] = $row;
  }
  return $groups;

}

function get_group_name($g_id) {
  return db_result(db_query("SELECT name FROM {fanta_groups} WHERE g_id = '%d'", $g_id));
}

function get_groups_teams($c_id) {

  $result = db_query("SELECT * FROM {fanta_groups} WHERE c_id = '%d' AND active = 1 AND lineups >= 1 ORDER BY lineups", $c_id);
  while ($row = db_fetch_object($result)) {
    $resulta = db_query("SELECT * FROM {fanta_teams_groups} WHERE g_id = '%d' AND active = 1", $row->g_id);
    while ($rowa = db_fetch_object($resulta)) {
      $groups[$row->g_id][$rowa->t_id] = $rowa->t_id;
    }
  }

  return $groups;
}

function get_teams_groups($c_id = '') {

  if (empty($c_id) || !competition_exists($c_id)) $c_id = variable_get('fantacalcio_main_competition', 1);

  $sql = "SELECT * FROM {fanta_teams} t, {fanta_teams_groups} g 
      WHERE t.t_id = g.t_id 
      AND g.g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = '%d' )
      AND g.active = 1";
  $result = db_query($sql, $c_id);
  while ($row = db_fetch_object($result)) {
    $teams_groups[$row->g_id][$row->t_id] = $row;
  }
  return $teams_groups;

}

function get_group_teams($g_id) {

  $sql = "SELECT * FROM {fanta_teams} t, {fanta_teams_groups} g 
      WHERE t.t_id = g.t_id 
      AND g.g_id = '%d' 
      AND g.active = 1";
  $result = db_query($sql, $g_id);
  while ($row = db_fetch_object($result)) {
    $teams[$row->t_id] = $row;
  }
  return $teams;

}

function get_competition_group($g_id) {

  $sql = "SELECT c_id FROM {fanta_groups} WHERE g_id = '%d'";
  $result = db_query($sql, $g_id);
  return db_result($result);
}

function get_competitions_groups() {
  $sql = "SELECT * FROM {fanta_groups}";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $groups[$row->g_id] = $row->c_id;
  }
  return $groups;
}

function get_teams() {
  $sql = "SELECT * FROM {fanta_teams} t LEFT JOIN {fanta_teams_groups} g
      ON t.t_id = g.t_id
      WHERE t.active = 1";
  $sql = "SELECT * FROM {fanta_teams} 
      WHERE active = 1 ORDER BY name";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $t_id = $row->t_id;
    if (!isset($teams[$t_id])) $teams[$t_id] = $row;
    if (!isset($teams[$t_id]->groups)) 
      $teams[$t_id]->groups = array();
    array_push($teams[$t_id]->groups, $row->g_id);
  }
  return $teams;
}

function get_team_name($t_id) {
  return db_result(db_query("SELECT name FROM {fanta_teams} WHERE t_id = '%d'", $t_id));
}

function get_real_teams() {
  $real_teams = array();
  $sql = "SELECT * FROM {fanta_real_teams} WHERE active = 1 ORDER BY long_name";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $real_teams[$row->rt_id] = $row;
  }

  return $real_teams;
}

function get_real_teams_options() {
  $real_teams = get_real_teams();
  foreach ($real_teams as $rt_id => $real_team)
    $new_real_teams[$rt_id] = strtoupper($real_team->long_name);
  
  return $new_real_teams;
}

function get_real_teams_long_name() {
  $real_teams = get_real_teams();
  $real_teams_long_name = array();

  foreach ($real_teams as $rt_id => $real_team) {
    $real_teams_long_name[strtolower($real_team->long_name)] = $rt_id;
  }

  return $real_teams_long_name;
}

function get_real_teams_short_name() {
  $real_teams = get_real_teams();
  $real_teams_short_name = array();

  foreach ($real_teams as $rt_id => $real_team) {
    $real_teams_short_name[strtolower($real_team->short_name)] = $rt_id;
  }

  return $real_teams_short_name;
}

function get_vote_rounds() {
  $rounds = array();
  $sql = "SELECT * FROM {fanta_rounds} ORDER BY round ";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->round] = $row;
  }
  return $rounds;
}

function get_rounds($c_id = "") {
  $rounds = array();
  $sql = "SELECT * FROM {fanta_rounds} r 
      LEFT JOIN {fanta_rounds_competitions} c 
      ON r.round = c.round ";
  $sql .= (!empty($c_id)) ? "WHERE c_id = $c_id" : "";
  $sql .= " ORDER BY r.round";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->rc_id] = $row;
  }
  return $rounds;
}

function get_rounds_played($c_id) {
  $sql = "SELECT competition_round FROM {fanta_rounds_competitions} c, {fanta_rounds} r
        WHERE r.round = c.round
        AND c.c_id = '%d' 
        AND r.status = 1
        ORDER BY r.round";
  $result = db_query($sql, $c_id);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->competition_round] = $row->competition_round; 
  }
  return $rounds;  
}

function get_competition_rounds($c_id = "") {
  $rounds = array();
  $sql = "SELECT * FROM {fanta_rounds_competitions} c
      LEFT JOIN {fanta_rounds} r 
      ON r.round = c.round ";
  $sql .= (!empty($c_id)) ? "WHERE c.c_id = $c_id" : "";
  $sql .= " ORDER BY r.round";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->rc_id] = $row;
  }
  return $rounds;
}

function get_rounds_list_with_status() {
  $rounds = array();
  $sql = "SELECT r.round, s.status FROM {fanta_rounds} r, {fanta_round_statuses} s WHERE r.status = s.s_id ORDER BY round ";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->round] = $row->round . " (" . $row->status . ")";
  }
  return $rounds;
}

function get_round_statuses() {
  $statuses = array();
  $sql = "SELECT * FROM {fanta_round_statuses} ORDER BY status ";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $statuses[$row->s_id] = $row->status;
  }
  return $statuses;
}

function get_rounds_by_g_id($g_id) {
  $c_id = get_cid_by_gid($g_id);
  $sql = "SELECT * 
      FROM {fanta_rounds} r, {fanta_rounds_competitions} c 
      WHERE r.round = c.round 
      AND c.c_id = '%d'";
  $result = db_query($sql, $c_id);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->competition_round] = $row;
  }
  return $rounds;
}

function get_round_by_competition_round($competition_round, $c_id) {
  $sql = "SELECT round 
      FROM {fanta_rounds_competitions} 
      WHERE competition_round = '%d' 
      AND c_id = '%d'";
  $result = db_query($sql, $competition_round, $c_id);  
  return db_result($result);
}

function get_competition_round_by_round($round, $c_id) {
  $sql = "SELECT competition_round 
      FROM {fanta_rounds_competitions} 
      WHERE round = '%d' 
      AND c_id = '%d'";
  $result = db_query($sql, $round, $c_id);  
  return db_result($result);
}

function get_competition_round($round, $c_id) {
  $sql = "SELECT * 
      FROM {fanta_rounds_competitions} 
      WHERE competition_round = '%d' 
      AND c_id = '%d'";
  $result = db_query($sql, $round, $c_id);  
  while ($row = db_fetch_object($result)) {
    $round = $row;
  }
  return $round;
}

function get_round_label($match) {
  $sql = "SELECT round_label FROM {fanta_rounds_competitions} WHERE competition_round = '%d' AND c_id = '%d' ";
  $result = db_query($sql, $match->round, get_cid_by_gid($match->g_id));
  $label = db_result($result);
  return (empty($label)) ? $match->round . "&ordf; Giornata" : $label;
}

function get_all_rounds() {
  $sql = "SELECT r.round, r.date, r.end_date, r.status, c.c_id, c.competition_round, c.round_label, c.next 
    FROM {fanta_rounds} r 
    LEFT JOIN {fanta_rounds_competitions} c 
    ON r.round = c.round 
    ORDER BY r.round, c.c_id";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    if (!isset($rounds[$row->round])) {
      $rounds[$row->round] = $row;
      $rounds[$row->round]->competitions = array();
    }
    if (isset($rounds[$row->round]->competitions)) {
      $rounds[$row->round]->competitions[$row->c_id] = array("round" => $row->competition_round, "label" => $row->round_label, "next" => $row->next);
    }
  }
  return $rounds;
}

function get_all_teams() {
  $sql = "SELECT t.t_id, t.name, t.uid, t.active, g.g_id FROM {fanta_teams} t LEFT OUTER JOIN {fanta_teams_groups} g
      ON t.t_id = g.t_id
      ORDER BY t.name
      ";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $t_id = $row->t_id;
    if (!isset($teams[$t_id])) $teams[$t_id] = $row;
    if (!isset($teams[$t_id]->groups)) 
      $teams[$t_id]->groups = array();
    array_push($teams[$t_id]->groups, $row->g_id);
  }
  return $teams;
}

function get_round_date($round, $c_id) {
  $sql = "SELECT round FROM {fanta_rounds_competitions}
      WHERE competition_round = '%d'
      AND c_id = '%d'";
  $result = db_query($sql, $round, $c_id);
  $vote_round = db_result($result);

  $sql = "SELECT date FROM {fanta_rounds} WHERE round = '%d'";
  $result = db_query($sql, $vote_round);
  return db_result($result);
}

function get_next_round_formazione($c_id) {
  $sql = "SELECT MAX(competition_round) 
      FROM {fanta_rounds_competitions} c, {fanta_rounds} r
      WHERE c.round = r.round 
      AND c.c_id = '%d' 
      AND r.status = 1";
  return db_result(db_query($sql, $c_id)) + 1;
}

function get_last_round_formazione($c_id) {

  $sql = "SELECT max(round) FROM {fanta_lineups} WHERE c_id = '%d'";
  $result = db_query($sql, $c_id);
  return db_result($result);
}

function get_match($round, $t_id, $c_id) {

  $sql = "SELECT * FROM {fanta_matches} " .
  "WHERE g_id IN (SELECT g_id FROM {fanta_groups} WHERE  c_id = '%d') " .
  "AND round = '%d' " .
  "AND (t1_id = '%d' OR t2_id = '%d')";
  $result = db_query($sql, $c_id, $round, $t_id, $t_id);
  return db_fetch_object($result);
}

function get_match_id($round, $t_id, $g_id) {
  $match = get_match($round, $t_id, $g_id);
  return $match->m_id;
}

function get_team_data($t_id = "") {

  global $user;

  if ($t_id) {
    $sql = "SELECT * FROM {fanta_teams} WHERE t_id = '%d'";
      $result = db_query($sql, $t_id);
  }
  else {
    $sql = "SELECT * FROM {fanta_teams} WHERE uid = '%d'";
    $result = db_query($sql, $user->uid);
  }
  return db_fetch_array($result);
}

function get_group_data($g_id) {
  $sql = "SELECT * FROM {fanta_groups} WHERE g_id = '%d'";
  $result = db_query($sql, $g_id);
  return db_fetch_array($result);
}

function get_competition_data($c_id) {
  $sql = "SELECT * FROM {fanta_competitions} WHERE c_id = '%d'";
  $result = db_query($sql, $c_id);
  return db_fetch_array($result);
}

function get_round_data($round) {
  $sql = "SELECT * 
            FROM {fanta_rounds} r
            LEFT JOIN {fanta_rounds_competitions} c 
            ON r.round = c.round
            WHERE r.round = '%d'";
  $result = db_query($sql, $round);
  while ($row = db_fetch_array($result)) {
    if (!isset($round_data)) {
      $round_data = $row;
      $round_data['competitions'] = array();
    }
    if (isset($round_data['competitions'])) {
      $round_data['competitions'][$row['c_id']] = array("competition_round" => $row['competition_round'], "label" => $row['round_label'], "next" => $row['next']);
    }
  }
  return $round_data;
}

function get_match_data($m_id) {
  $sql = "SELECT * FROM {fanta_matches} WHERE m_id = '%d'";
  $result = db_query($sql, $m_id);
  return db_fetch_array($result);
}

function get_team_id_by_user($u_id) {
  $sql = "SELECT t_id FROM {fanta_teams} WHERE uid = '%d' AND active = 1";
  $result = db_query($sql, $u_id);
  return db_result($result);
}

function get_last_votes() {
  $sql = "SELECT max(round) FROM {fanta_votes}";
  $result = db_query($sql);
  return db_result($result);
}

function get_last_competition_round($c_id) {
  $vote_round = get_last_votes($c_id);
  $sql = "SELECT competition_round FROM {fanta_rounds_competitions} " .
      "WHERE c_id = '%d' " .
      "AND round = '%d'";
  $result = db_query($sql, $c_id, $vote_round);
  return db_result($result);
}

function get_next_competition_round($c_id) {
  $vote_round = get_last_votes($c_id);
  $sql = "SELECT competition_round FROM {fanta_rounds_competitions} " .
      "WHERE c_id = '%d' " .
      "AND round = '%d'";
  $result = db_query($sql, $c_id, $vote_round +1);
  return db_result($result);
}

function get_votes($round) {
  $votes = array();
  $sql = "SELECT * FROM {fanta_votes} " .
  "WHERE round = '%d'
   AND provider = '%d' ";
  $result = db_query($sql, $round, variable_get("fantacalcio_votes_provider", 1));
  while ($row = db_fetch_object($result)) {
    $pl_id = $row->pl_id;
    $votes[$pl_id] = $row;
  }
  return $votes;
}

function get_vote_providers() {
  $providers = array();
  $sql = "SELECT * FROM {fanta_vote_providers}";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $p_id = $row->p_id;
    $providers[$p_id] = $row;
  }
  return $providers;
}

function get_users() {
  $users = array();
  $sql = "SELECT * FROM {users}";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $u_id = $row->uid;
    $users[$u_id] = $row;
  }
  return $users;
}

function get_team_users() {
  $team_users = array();
  $sql = "SELECT t_id, uid FROM {fanta_teams} WHERE active = 1";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $t_id = $row->t_id;
    $team_users[$t_id] = $row;
  }
  return $team_users;
}

function get_timestamp_round($round) {
  $sql = "SELECT date FROM {fanta_rounds} WHERE round = '%d'";
  $result = db_query($sql, $round);
  return db_result($result);
}


function get_timestamp_match($c_id, $round) {
  $sql = "SELECT r.date FROM {fanta_rounds} r, {fanta_rounds_competitions} c " .
      "WHERE c.round = r.round " .
      "AND c.c_id = '%d' " .
      "AND c.competition_round = '%d'";
  $result = db_query($sql, $c_id, $round);
  return db_result($result);
}

function get_competition_id($c_name) {
  $sql = "SELECT c_id FROM {fanta_competitions} " .
      "WHERE LOWER(name) = '%s' ";
  $result = db_query($sql, strtolower($c_name));
  return db_result($result);
}

function get_competition_name($c_id) {
  $sql = "SELECT name FROM {fanta_competitions} " .
      "WHERE c_id = '%d' ";
  $result = db_query($sql, $c_id);
  return db_result($result);
}

function get_competitions_options() {
  $sql = "SELECT * FROM {fanta_competitions} WHERE active = 1";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $competitions[$row->c_id] = $row->name;
  }
  return $competitions;
}

function get_group_options($c_id = '') {
  $sql = "SELECT * FROM {fanta_groups} WHERE active = 1";
  if (competition_exists($c_id))
    $sql .= " AND c_id = '%d'";
  $result = db_query($sql, $c_id);
  while ($row = db_fetch_object($result)) {
    $groups[$row->g_id] = $row->name;
  }
  return $groups;
}

function get_teams_options() {
  $sql = "SELECT * FROM {fanta_teams} t 
      WHERE t.active = 1 
      ORDER BY t.name";
  $result = db_query($sql, $g_id);
  while ($row = db_fetch_object($result)) {
    $t_id = $row->t_id;
    $teams[$t_id] = $row->name;
  }
  return $teams;
}

function get_teams_group($g_id) {
  $sql = "SELECT * FROM {fanta_teams_groups} g, {fanta_teams} t " .
      "WHERE t.active = 1 " .
      "AND g.active = 1 ".
      "AND g.t_id = t.t_id " .
      "AND g.g_id = '%d' 
      ORDER BY t.name";
  $result = db_query($sql, $g_id);
  while ($row = db_fetch_object($result)) {
    $t_id = $row->t_id;
    $teams[$t_id] = $row;
  }
  return $teams;
}

function get_teams_group_options($g_id) {
  $sql = "SELECT * FROM {fanta_teams_groups} g, {fanta_teams} t " .
      "WHERE t.active = 1 " .
      "AND g.active = 1 ".
      "AND g.t_id = t.t_id " .
      "AND g.g_id = '%d' 
      ORDER BY t.name";
  $result = db_query($sql, $g_id);
  while ($row = db_fetch_object($result)) {
    $t_id = $row->t_id;
    $teams[$t_id] = $row->name;
  }
  return $teams;
}

function get_teams_group_options_checked($g_id) {
  $sql = "SELECT * FROM {fanta_teams} t 
      INNER JOIN {fanta_teams_groups} g
	  ON g.t_id = t.t_id 
      WHERE t.active = 1
	  AND g.g_id = '%d'
      ORDER BY t.name";
  $result = db_query($sql, $g_id);
  while ($row = db_fetch_object($result)) {
    $t_id = $row->t_id;
    $teams[$t_id] = $row->t_id;
  }
  return $teams;
}

function get_rounds_options($all = FALSE) {
  $sql = "SELECT round FROM {fanta_rounds} ";
  if (!$all) $sql .= "WHERE status = 1";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->round] = $row->round;
  }
  return $rounds;
}

function get_round_by_match($c_id, $round) {
  $sql = "SELECT round FROM {fanta_rounds_competitions} 
      WHERE c_id = '%d' 
      AND competition_round = %d";
  return db_result(db_query($sql, $c_id, $round));
}

function get_group_team($c_id, $t_id) {
  $sql = "SELECT g_id 
      FROM {fanta_teams_groups} 
      WHERE t_id = '%d' 
      AND g_id IN (
        SELECT g_id 
        FROM {fanta_groups} 
        WHERE c_id = '%d')";
  return db_result(db_query($sql, $t_id, $c_id));
}

function get_cid_by_gid($g_id) {
  return db_result(db_query("SELECT c_id FROM {fanta_groups} WHERE g_id = '%d'", $g_id));
}

function get_teams_names() {
  $teams = get_teams();
  foreach ($teams as $team) {
    $t[$team->t_id] = $team->name;
  }
  return $t;
}

function get_number_rounds($g_id) {
  $sql = "SELECT DISTINCT round FROM {fanta_matches} WHERE g_id = '%d'";
  $result = db_query($sql, $g_id);
  return db_affected_rows($result);
}

function get_player_position($pl_id, $c_id, $t_id, $round) {
  $sql = "SELECT has_played, position FROM {fanta_lineups} 
          WHERE pl_id = '%d' 
          AND c_id = '%d' 
          AND t_id = '%d' 
          AND round = '%d'";
  $result = db_query($sql, $pl_id, $c_id, $t_id, get_competition_round_by_round($round, $c_id));
  while ($row = db_fetch_object($result)) {
    if ($row->has_played == 1) return 1;
    $position = $row->position;
  }
  if ($position) return $position; 
  else return 0;
}

/****************************/
/* end recupero dati    */
/****************************/

function filter_team_group($teams, $g_id) {

  $group_teams = array();
  $sql = "SELECT t_id FROM {fanta_teams_groups} WHERE g_id = '%d'";
  $result = db_query($sql, $g_id);
  while ($row = db_fetch_array($result)) {
    $group_teams[] = $row['t_id'];
  }

  $filtered_teams = array();
  foreach ($teams as $team) {
    if (in_array($team->t_id, $group_teams)) $filtered_teams[] = $team;
  }

  return $filtered_teams;
}

function team_exists($t_id) {
  $result = db_query("SELECT * FROM {fanta_teams} WHERE t_id = '%d'", $t_id);
  if (db_affected_rows($result) == 1) return TRUE;
  return FALSE;
}

function round_exists($round) {
  $result = db_query("SELECT * FROM {fanta_rounds} WHERE round = '%d'", $round);
  if (db_affected_rows($result) == 1) return TRUE;
  return FALSE;
}

function group_exists($g_id) {
  $result = db_query("SELECT * FROM {fanta_groups} WHERE g_id = '%d'", $g_id);
  if (db_affected_rows($result) == 1) return TRUE;
  return FALSE;
}

function competition_exists($c_id) {
  $result = db_query("SELECT * FROM {fanta_competitions} WHERE c_id = '%d'", $c_id);
  if (db_affected_rows($result) == 1) return TRUE;
  return FALSE;
}

function player_exists($pl_id) {
  $sql = "SELECT pl_id FROM {fanta_players} WHERE pl_id = '%d'";
  $result = db_query($sql, $pl_id);
  if (db_affected_rows($result) > 0) return TRUE;
  return FALSE;
}

function is_competition_name($c_name) {

  $sql = "SELECT c_id FROM {fanta_competitions} WHERE name = '%s'";
  $result = db_query($sql, $c_name);
  return db_result($result);
}

function competition_has_arg($c_name, $arg) {
  $sql = "SELECT c_id FROM {fanta_competitions} 
      WHERE name = '%s'
      AND $arg = 1";
  $result = db_query($sql, $c_name);
  if (db_affected_rows(db_result($result)) > 0) return TRUE;
  return FALSE;
}

/************************/
/* template        */
/************************/

function make_tabs($function, $c_name, $arg = "", $params = "") {

  global $user;
  
  $c_id = get_competition_id($c_name);

  $groups = get_groups($c_id, $arg);

  $selected_g_id = isset($user->groups[$c_id][$arg]) ? $user->groups[$c_id][$arg] : 0;

  if (count($groups) > 1 ) {

    $out .= "\n<div id='tabs_g' class='tabs' data-role=navbar data-mini=true data-theme=d>" .
        "\n<ul>";

    $i = 0;
    foreach ($groups as $g_id => $group) {
      $class = ($selected_g_id > 0)  ? ($g_id == $selected_g_id ? "ui-btn-active" : "") : ($i == 0 ? "ui-btn-active" : "");
      if ($selected_g_id == $g_id)
        $active_tab = $i;

      $out .= "\n<li><a data-theme=c href='#g_" . $g_id . "' class='tab-button " . $class . "' id='tab-" . $g_id . "'><span>" . $group->name . "</span></a></li>";
      $i++;
    }
    $out .= "\n</ul>";

    if (is_mobile())
      $out .= "\n</div>";

    $i = 0;
    foreach ($groups as $g_id => $group) {
      $display = ($selected_g_id > 0)  ? ($g_id == $selected_g_id ? "block" : "none") : ($i == 0 ? "block" : "none");
      $out .= "\n<div class='tab' " . (is_mobile() ? "style='display: " . $display . ";'" : "") .  " id='g_" . $g_id . "'>"
          . $function($g_id, $params)
          ."\n</div>";
      $i++;
    }

    if (!is_mobile())
      $out .= "\n</div>";

    $js_web = "$(document).ready(function(){
        $('#tabs_g').tabs({active: " . ($active_tab != "" ? $active_tab : 0) . ", hide: { effect: 'fade', duration: 750 }});
        });";

    $js_mobile = "$(function() {
        $('.tab-button').click(function() { 
          $('.tab').fadeOut('fast');
          var div_id = $(this).attr('href');
          $('.tab-button').removeClass('ui-btn-active');
          var a_id = $(this);
          $(div_id).fadeIn('fast', 'swing', function(){ $(a_id).addClass('ui-btn-active');});
        });
        });";
  
    if (is_mobile())
      drupal_add_js( $js_mobile, 'inline', 'footer');
    else
      drupal_add_js( $js_web, 'inline');


  }
  elseif (count($groups) == 1) {
    foreach ($groups as $g_id => $group) {
      $out .= $function($g_id, $params);
    }
  }
  else $out = ""; 

  return $out;
}

function make_competition_tabs($function, $args = array(), $filter = array() ) {

  $out = "";

  $competitions = get_competitions($filter);

  if (count($competitions) > 1) {
    
    $tab_selected = (is_competition_name($args["c_name"])) ? get_competition_id($args["c_name"]) : variable_get("fantacalcio_main_competition", 1) ;
    $args["selected_competition"] = $args["c_name"];
    
    $out .= "\n<div id='tabs_c' class='tabs' data-role=navbar data-mini=true data-theme=d	>" .
        "\n<ul>";
    foreach ($competitions as $c_id => $competition) {
      $class = ($tab_selected > 0)  ? ($competition->c_id == $tab_selected ? "ui-btn-active" : "") : ($i == 0 ? "ui-btn-active" : "");
      $out .= "\n<li data-theme=c><a data-theme=c href='#c_" . $c_id . "' class='tab-button " . $class . "' id='tab-c-" . $c_id . "'><span>" . $competition->name . "</span></a></li>";
    }
    $out .= "\n</ul>";

    if (is_mobile())
      $out .= "\n</div>";

    $i = 0;
    foreach ($competitions as $c_id => $competition) {

      $competition_args = array(
              "c_name" => $competition->name,
              "round" => (is_numeric($args["round"]) && $args["c_name"] == $competition->name) ? $args["round"] : get_last_round_formazione($competition->c_id),
              "t_id" => $args["t_id"],
              "selected_competition" => $args["selected_competition"]);

      $display = ($tab_selected > 0)  ? ($competition->c_id == $tab_selected ? "block" : "none") : ($i == 0 ? "block" : "none");
      if ($competition->c_id == $tab_selected)
        $selected_index = $i;

      $out .= "\n<div class='tab' " . (is_mobile() ? "style='display: " . $display . ";'" : "") .  " id='c_" . $competition->c_id . "'>"
          . $function($competition_args)
          . "<div style='clear:both;'></div>"
          . "\n</div>";
      $i++;
    }
   
    if (!is_mobile())
      $out .= "\n</div>";
  }
  elseif (count($competitions) == 1) {
    $args["selected_competition"] = $args["c_name"];
    foreach ($competitions as $c_id => $competition) {

      $competition_args = array(
              "c_name" => $competition->name,
              "round" => (is_numeric($args["round"]) && $args["c_name"] == $competition->name) ? $args["round"] : get_last_round_formazione($competition->c_id),
              "t_id" => $args["t_id"],
              "selected_competition" => $args["selected_competition"]);

      $out .= $function($competition_args);
    }
  }

  $js_web = "$(document).ready(function(){
        $('#tabs_c').tabs({active: " . $selected_index . ", hide: { effect: 'fade', duration: 750 }});
        });";

  $js_mobile = "$(function() {
        $('.tab-button').click(function() { 
          $('.tab').fadeOut('fast');
          var div_id = $(this).attr('href');
          $('.tab-button').removeClass('ui-btn-active');
          var a_id = $(this);
          $(div_id).fadeIn('fast', 'swing', function(){ $(a_id).addClass('ui-btn-active');});
        });
        });";

  if (is_mobile())
    drupal_add_js( $js_mobile, 'inline', 'footer');
  else
    drupal_add_js( $js_web, 'inline');

  return $out;

}

function make_columns($function, $link, $array_args = array(), $filter = '', $choose_round = '') {

  global $user;

  $is_mobile = is_mobile();

  $teams = get_teams();
  $competitions = get_competitions();
  $groups = get_groups();

  $out = "";
  
  if (isset($array_args['c_id'])) 
    $c_id_groups = $array_args['c_id'];
  else 
    $c_id_groups = variable_get('fantacalcio_main_competition', 1);

  $groups_teams = get_groups_teams($c_id_groups);

  #squadre
  $out_web = "\n<div id='teams'>";  
  
  if ($groups_teams) {

    if ($filter != '' && function_exists($filter)) 
      $teams = $filter ($teams, $array_args);

    $out_mobile = "<div data-role=fieldcontain>";
    $out_mobile .= "<fieldset data-theme=a>";

    if (isset ($array_args['c_id'])) {
      $competition_name = $competitions[$array_args['c_id']]->name;
    }
    if ($choose_round != '' && function_exists($choose_round)) {    
      $out_web .= drupal_get_form($choose_round, $array_args['c_id'], $array_args['round']);
      $out_mobile .= "<div>" . drupal_get_form($choose_round, $array_args['c_id'], $array_args['round']) . "</div>";
    }
	
	$out_web .= "<div style='margin-top: 20px;' id='accordion_" . $array_args['c_id'] . "'>";

    $out_mobile .= "<div>" 
                  . "<a href='#popup_teams_" . $c_id_groups . "' data-rel='popup' data-theme=c data-role='button' data-icon='plus' data-inline='true' data-mini='true'>" 
                  . t('Squadre') 
                  . "</a>"
                  . "</div>";

    $out_mobile .= "</fieldset>";
    $out_mobile .= "</div>";

    $out_teams_mobile = "<div data-role='collapsible-set' style='margin:0;'>";
    $out_teams_web = "";

    $i = 0; 

    $default_div = 0; 	
    foreach ($groups_teams as $g_id => $group_teams) {

      $teams_names = array();
      foreach ($group_teams as $t_id => $team) {
        $teams_names[$t_id] = strtolower($teams[$t_id]->name);
      }

      array_multisort($teams_names, $group_teams);

      $out_teams_mobile .= "\n<div data-role='collapsible' data-theme='a' data-content-theme='c' data-collapsed-icon='arrow-r' data-expanded-icon='arrow-d' data-collapsed='" . ((count($groups_teams) == 1) ? "false" : "true") . "'>";


      $out_teams_web .= "<h3>" . $groups[$g_id]->name . "</h3>";

      $out_teams_mobile .= "\n<h3>" . $groups[$g_id]->name . "</h3>";

      $out_teams_list = ""; $display = "none"; 

      foreach ($group_teams as $group_team) {
        if (array_key_exists($group_team, $teams)) {
          $class = ($group_team == $array_args['t_id'] && $array_args["c_name"] == $array_args["selected_competition"]) ? "selected" : "";
          $out_teams_list .= "\n<li data-theme=d class='" . $class . "'>" . l($teams[$group_team]->name, $link . "/" . $group_team) . "</li>";

          if (isset($array_args['t_id']) && !empty($array_args['t_id'])) {
		    if ($group_team == $array_args['t_id'] && strtolower($array_args["c_name"]) == strtolower($array_args["selected_competition"])) {
              $default_div = $i;
			}
		  }
		  elseif (isset($user->t_id) && $user->t_id == $group_team) {
		    $default_div = $i;
		  }
        }
      }

      #$out_teams_web .= "\n<div id='group_" . $g_id . "' class='_content'>";
      $out_teams_web .= "\n<div>";
      $out_teams_web .= "\n<ul>" . $out_teams_list . "\n</ul>";
      $out_teams_web .= "\n</div>";

      $out_teams_mobile .= "\n<ul data-role='listview'>" . $out_teams_list . "\n</ul>";
      $out_teams_mobile .= "\n</div>";

      $i++;
    }

    $out_teams_mobile .= "</div>";

    $out_web .= $out_teams_web;

    $out_mobile .= "\n<div data-role='popup' id='popup_teams_" . $c_id_groups . "'>" . $out_teams_mobile . "</div>";

	$out_web .= "</div>";

    $js_web = '$(document).ready(function() {
          $("#accordion_' . $array_args['c_id'] . '").accordion({active:' . $default_div . '});
        });
      ';

    if (!is_mobile())
      drupal_add_js($js_web, 'inline');

  }
  
  $out_web .= "</div>";

  $out .= ($is_mobile) ? $out_mobile : $out_web;

  #dati
  if (!empty ($array_args['t_id']) && is_numeric($array_args['t_id']) && $array_args["c_name"] == $array_args["selected_competition"]) {
    $out .= ($is_mobile) ? "" : "\n<div id='data'>";
    $out .= $function($array_args);
    $out .= ($is_mobile) ? "" : "\n</div>";
  }
  return $out;
}

function choose_competition($args = array()) {

  $out = "";

  $competitions = get_competitions($args);

  if ($competitions) {
    $out .= "\n<ul>";
    foreach ($competitions as $l_id => $competition) {
      $out .= "\n<li><a href='" . base_path() . arg(0) . "/" . $competition->name . $url_tail . "'>" . $competition->name . $show_round . "</a></li>";
    }
    $out .= "\n</ul>";
  }

  return $out;
}

/************************/
/* end template      */
/************************/


/************************/
/* utilities      */
/************************/
function convert_teams($team) {
  return strtolower($team);
}

function convert_role($r, $type = '') {
  $ruolo = array('P', 'D', 'C', 'A');
  if ($type == 'long') $ruolo = array('Portieri', 'Difensori', 'Centrocampisti', 'Attaccanti');
  return $ruolo[$r];
}

function truncate($s, $n) {
  if (strlen($s) < $n) return $s;
  $stringa_tagliata = substr($s, 0, $n);
  $last_space = strrpos($stringa_tagliata, " ");
  return substr($stringa_tagliata, 0, $last_space) . "...";
}

function date_transform($ts) {
  setlocale(LC_TIME, "it_IT");
  if (date('Y-m-d', $ts) == date('Y-m-d')) return " le " . date("H:i", $ts);
  else {
    $d = array("lun" => "lunedi", "mar" => "martedi", "mer" => "mercoledi", "gio" => "giovedi", "ven" => "venerdi", "sab" => "sabato" , "dom" => "domenica" );
    $day = $d[strftime('%a', $ts)];
    return $day . strftime(' %e %B', $ts) . " ore " . date("H:i", $ts);
  }
}

function is_mobile() {

  $mobile_user_agents = array("iPhone", "iPod", "iPad", "BlackBerry", "Android", "IEMobile", "HTC");

  foreach ($mobile_user_agents as $agent)    
    if (strpos($_SERVER['HTTP_USER_AGENT'], $agent) > -1) return TRUE;

  return FALSE;
}

function get_image_check($bool) {
  if ($bool) return "ok.png";
  return "no.png";
}

function get_image_check_int($int) {
  if ($int == 1) 
    return "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/ok.png'>";
  else if($int == 0)
    return "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/no.png'>";
  else if($int == -1)
    return "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/wait.png'>";
}

function compress_string($string) {
  $exploded = explode(" ", $string);
  return implode("&nbsp;", $exploded);
}

function is_passaggio_turno($c_id, $competition_round ) {
  
  $sql = "SELECT * FROM {fanta_rounds_competitions} 
      WHERE competition_round = '%d' 
      AND c_id = '%d'";

  $result = db_query($sql, $competition_round, $c_id);
  while ($row = db_fetch_object($result)) {
    if ($row->next == 1) return TRUE;
  }

  return FALSE;
}

function is_match_draw($match) {
  if (empty($match->match_label))
    return FALSE;

  $num_matches = 0;
  $sql = "SELECT * FROM {fanta_matches} WHERE match_label = '%s'";
  $result = db_query($sql, $match->match_label);
  while ($row = db_fetch_object($result)) {
    $goals[$row->t1_id] += $row->goals_1;
    $goals[$row->t2_id] += $row->goals_2 * 2; //away goals
    $sum_goals[$row->t1_id] += $row->goals_1;
    $sum_goals[$row->t2_id] += $row->goals_2;
    $num_matches++;
  }
  
  if ($sum_goals[$match->t1_id] == $sum_goals[$match->t2_id] && ($goals[$match->t1_id] == $goals[$match->t2_id] || $num_matches == 1) )
    return TRUE;
  else
    return FALSE;
}

function is_in_rosa($pl_id, $t_id) {
  $sql = "SELECT * FROM {fanta_squads} WHERE pl_id = '%d' AND t_id = '%d' AND in_team = 1";
  $result = db_query($sql, $pl_id, $t_id);
  while ($row = db_fetch_object($result)) {
    return TRUE;
  }

  return FALSE;
}
/************************/
/* end utilities    */
/************************/
