<?php
/**
* @file fantacalcio.standings.inc
* 
*/

function _fantacalcio_classifica($c_name = '') {

  if (empty($c_name) || !competition_has_arg($c_name, "standings")) 
    $out = choose_competition(array("standings" => 1));

  else {
    drupal_set_title(check_plain("Classifica - " . ucwords($c_name)));
    $out = make_tabs('get_standing_table', $c_name, "standings");
  }

  return $out;

}

function get_standing_table($g_id) {

  #jquery tablesorter
  if (!is_mobile()) {
    drupal_add_css('sites/all/plugins/tablesorter/css/theme.blue.css', 'themes', 'all', TRUE);
    drupal_add_js('sites/all/plugins/tablesorter/js/jquery.tablesorter.js', 'footer');
  }

  global $user;

  $teams = get_teams_group($g_id);

  $l_id = get_competition_group($g_id);

  if ($teams) {

    foreach ($teams as $t_id => $team) {

    #$teams[$t_id]['n_played'] = 0 + get_num_played($t_id, $g_id);
    $teams[$t_id]->n_win = 0 + get_num_win($t_id, $g_id);
    $teams[$t_id]->n_lost = 0 + get_num_lost($t_id, $g_id);
    $teams[$t_id]->n_draw = 0 + get_num_draw($t_id, $g_id);
    $teams[$t_id]->goals_in = 0 + get_goals_in($t_id, $g_id);
    $teams[$t_id]->goals_out = 0 + get_goals_out($t_id, $g_id);
    $teams[$t_id]->points_in = 0 + get_points_in($t_id, $g_id);
    $teams[$t_id]->points_out = 0 + get_points_out($t_id, $g_id);
    $teams[$t_id]->pts = $teams[$t_id]->n_win * 3 + $teams[$t_id]->n_draw * 1;
    $pts[$t_id] = $teams[$t_id]->n_win * 3 + $teams[$t_id]->n_draw * 1;
    $gol_f[$t_id] = $teams[$t_id]->goals_in;
    $pts_f[$t_id] = $teams[$t_id]->points_in;
  }

  array_multisort($pts, SORT_DESC, $gol_f, SORT_DESC, $pts_f, SORT_DESC, $teams);

  if (!is_mobile()) {
    $js = "$(function() {
    $('#classifica_" . $g_id . "').tablesorter({sortList: [[2,1],[6,1],[9,1]], widgets: ['zebra'], headers: { 0:{sorter: false}, 1:{sorter: false}}});
  });  ";
    drupal_add_js($js, 'inline', 'footer');
  }

  $out .= "\n<table id='classifica_" . $g_id . "' name='classifica_" . $g_id . "' class='tablesorter tablesorter-blue classifica' data-role=table data-mode=columntoggle data-column-btn-text='Espandi...' data-column-btn-theme=d>" .
      "\n<thead>".
      "\n<tr>" .
        "\n<th></th>" .
        "\n<th>Squadra</th>" .
        "\n<th>Pts&nbsp;&nbsp;&nbsp;</th>" .
        "\n<th data-priority=2 class='padding'>Vinte&nbsp;&nbsp;&nbsp; </th>" .
        "\n<th data-priority=2 class='padding'>Perse&nbsp;&nbsp;&nbsp; </th>" .
        "\n<th data-priority=2 class='padding'>Pari&nbsp;&nbsp;&nbsp; </th>" .
        "\n<th data-priority=1>Gol f.&nbsp;&nbsp;&nbsp;</th>" .
        "\n<th data-priority=1>Gol s.&nbsp;&nbsp;&nbsp;</th>" .
        "\n<th data-priority=3>Diff Gol&nbsp;&nbsp;&nbsp;</th>" .
        "\n<th data-priority=1>Pti f.&nbsp;&nbsp;&nbsp;</th>" .
        "\n<th data-priority=1>Pti s.&nbsp;&nbsp;&nbsp;</th>" .
        "\n<th data-priority=3>Diff Pti&nbsp;&nbsp;&nbsp;</th>" .
      "\n</tr>".
      "\n</thead>";

  $out .= "\n<tbody>";

  $i = 0;

  foreach ($teams as $t_id => $team) {

    $i++;

    $team_class = ($team->t_id == $user->t_id) ? " class=bold" : "";

    $out .= "\n<tr>";
    $out .= "\n<td>" . $i . "</td>";
    $out .= "\n<td" . $team_class . ">" . l($team->name, "squadre/" . $team->t_id) . "</td>";
    $out .= "\n<td>" . $team->pts . "</td>";
    $out .= "\n<td>" . $team->n_win . "</td>";
    $out .= "\n<td>" . $team->n_lost . "</td>";
    $out .= "\n<td>" . $team->n_draw . "</td>";
    $out .= "\n<td>" . $team->goals_in . "</td>";
    $out .= "\n<td>" . $team->goals_out . "</td>";
    $out .= "\n<td>" . ($team->goals_in - $team->goals_out) . "</td>";
    $out .= "\n<td>" . $team->points_in . "</td>";
    $out .= "\n<td>" . $team->points_out . "</td>";
    $out .= "\n<td>" . ($team->points_in - $team->points_out) . "</td>";
    $out .= "\n</tr>";

  }

  $out .= "\n</tbody>";

  $out .= "</table>";

  }
  return $out;
}

function get_num_played($t_id, $g_id) {
  $sql = "SELECT count(*) as n FROM {fanta_matches} " .
      "WHERE (t1_id = '%d' OR t2_id = '%d') " .
      "AND g_id = '%d' " .
      "AND played = 1";
  $result = db_query($sql, $t_id, $t_id, $g_id);
  return db_result($result);
}

function get_num_win($t_id, $g_id) {
  $sql = "SELECT count(*) as n FROM {fanta_matches} " .
      "WHERE winner_id = '%d' " .
      "AND g_id = '%d' " .
      "AND played = 1";
  $result = db_query($sql, $t_id, $g_id);
  return db_result($result);
}

function get_num_lost($t_id, $g_id) {
  $sql = "SELECT count(*) as n FROM {fanta_matches} " .
      "WHERE (t1_id = '%d' OR t2_id = '%d') " .
      "AND winner_id != '%d' " .
      "AND winner_id != '-1' " .
      "AND g_id = '%d' " .
      "AND played = 1";
  $result = db_query($sql, $t_id, $t_id, $t_id, $g_id);
  return db_result($result);
}

function get_num_draw($t_id, $g_id) {
  $sql = "SELECT count(*) as n FROM {fanta_matches} " .
      "WHERE (t1_id = '%d' OR t2_id = '%d')" .
      "AND winner_id = '-1' " .
      "AND g_id = '%d' " .
      "AND played = 1";
  $result = db_query($sql, $t_id, $t_id, $g_id);
  return db_result($result);
}

function get_goals_in($t_id, $g_id) {
  $sql = "SELECT sum(goals_1) as n FROM {fanta_matches} " .
      "WHERE t1_id = '$t_id' " .
      "AND played = 1 " .
      "AND g_id = '$g_id' ";
  $result = db_query($sql);
  $goals_1 = db_result($result);

  $sql = "SELECT sum(goals_2) as n FROM {fanta_matches} " .
      "WHERE t2_id = '$t_id' " .
      "AND played = 1 " .
      "AND g_id = '$g_id'" ;
  $result = db_query($sql);
  $goals_2 = db_result($result);
  return $goals_1 + $goals_2;
}

function get_goals_out($t_id, $g_id) {
  $sql = "SELECT sum(goals_1) as n FROM {fanta_matches} " .
      "WHERE t2_id = '$t_id' " .
      "AND played = 1 " .
      "AND g_id = '$g_id' ";
  $result = db_query($sql);
  $goals_1 = db_result($result);

  $sql = "SELECT sum(goals_2) as n FROM {fanta_matches} " .
      "WHERE t1_id = '$t_id' " .
      "AND played = 1 " .
      "AND g_id = '$g_id'" ;
  $result = db_query($sql);
  $goals_2 = db_result($result);
  return $goals_1 + $goals_2;
}

function get_points_in($t_id, $g_id) {
  $sql = "SELECT sum(tot_1) as n FROM {fanta_matches} " .
      "WHERE t1_id = '$t_id' " .
      "AND played = 1 " .
      "AND g_id = '$g_id' ";
  $result = db_query($sql);
  $pt_1 = db_result($result);

  $sql = "SELECT sum(tot_2) as n FROM {fanta_matches} " .
      "WHERE t2_id = '$t_id' " .
      "AND played = 1 " .
      "AND g_id = '$g_id'" ;
  $result = db_query($sql);
  $pt_2 = db_result($result);
  return $pt_1 + $pt_2;

}

function get_points_out($t_id, $g_id) {

  $sql = "SELECT sum(tot_1) as n FROM {fanta_matches} " .
      "WHERE t2_id = '$t_id' " .
      "AND played = 1 " .
      "AND g_id = '$g_id' ";
  $result = db_query($sql);
  $pt_1 = db_result($result);

  $sql = "SELECT sum(tot_2) as n FROM {fanta_matches} " .
      "WHERE t1_id = '$t_id' " .
      "AND played = 1 " .
      "AND g_id = '$g_id'" ;
  $result = db_query($sql);
  $pt_2 = db_result($result);
  return $pt_1 + $pt_2;

}
