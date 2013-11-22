<?php
/**
* @file fantacalcio.block.inc
* 
*/

#Funzioni per la visualizzazione dei blocchi del modulo fantacalcio

function fantacalcio_block_riepilogo() {

  $out = ''; $out_li = '';

  $out_li .= fantacalcio_avviso_formazioni();
  $out_li .= fantacalcio_avviso_sondaggi();
  if (module_exists('fantasfide')) $out_li .= fantacalcio_avviso_sfide();
  if (module_exists('privatemsg')) $out_li .= fantacalcio_avviso_messaggi();
  if (module_exists('facebook_status')) $out_li .= fantacalcio_avviso_bacheca();
  if (module_exists('bets')) $out_li .= fantacalcio_avviso_scommesse();

  if (!empty($out_li))
    $out .= "\n<div>
        <ul>"
      . $out_li
      . "\n</ul>
        </div>";

  return $out;
}

function fantacalcio_avviso_formazioni() {

  $check = check_formazione_inserita();
  if ($check['errors'] != 0) {
    $text = "Inserire la formazione entro " . date_transform($check['date']);
    $out = l($text, 'formazioni/insert');
  }
  else $out = '';

  if (empty($out)) return;
  return "\n<li>" . $out . "</li>";
}

function fantacalcio_avviso_sondaggi() {

  $non_voted_polls = get_non_voted_polls();
  
  if (count($non_voted_polls) > 0) {
    $sondaggi = count($non_voted_polls) == 1 ? 'sondaggio non votato' : 'sondaggi non votati';
    $out = l(count($non_voted_polls) . ' ' . $sondaggi, 'node/' . $non_voted_polls[0]);
  }
  else $out = '';

  if (empty($out)) return;
  return "\n<li>" . $out . "</li>";
}

function fantacalcio_avviso_bacheca() {

  global $user;

  $ts_last_round = get_ts_last_round();

  $sql = "SELECT sid, uid, pid, status_time, status FROM {facebook_status} 
      WHERE pid = '%d' 
      AND status_time > '%d' 
      UNION 
      SELECT cid, sid, uid, comment_time, comment FROM {fbssc} 
      WHERE uid = '%d' 
      AND comment_time > '%d' " ;
  $result = db_query($sql, $user->uid, $ts_last_round, $user->uid, $ts_last_round);
  if (db_affected_rows($result) == 0)
    return "<li>" . l('Non hai lasciato un messaggio in bacheca', 'bacheca') . "</li>";

  return;

}

function fantacalcio_avviso_messaggi() {
  global $user;

  $sql = "SELECT count(*) FROM {pm_index} 
      WHERE uid = '%d' 
      AND is_new = 1
      AND deleted = 0 ";
  $result = db_query($sql, $user->uid);
  $messages = db_result($result);
  if ($messages == 1)
    return "<li>" . l('1 messaggio non letto', 'messages') . "</li>";
  elseif ($messages > 1)
    return "<li>" . l($messages . ' messaggi non letti', 'messages') . "</li>";

  return;

}

function get_non_voted_polls() {

  global $user;

  if ($user->uid > 0) {

    #active polls
    $active_polls = array();
    $sql = "SELECT nid FROM {poll} WHERE active = 1";
    $result = db_query($sql);
    while ($row = db_fetch_array($result)) {
      array_push($active_polls, $row['nid']);
    }

    #active decisions without electoral list
    $sql = "SELECT nid FROM {decisions} WHERE active = 1 AND uselist = 0";
    $result = db_query($sql);
    while ($row = db_fetch_array($result)) {
      array_push($active_polls, $row['nid']);
    }

    #active decisions with electoral list
    $sql = "SELECT nid FROM {decisions_electoral_list} WHERE uid = '%d' AND nid IN (SELECT nid FROM {decisions} WHERE active = 1 AND uselist = 1)";
    $result = db_query($sql, $user->uid);
    while ($row = db_fetch_array($result)) {
      array_push($active_polls, $row['nid']);
    }

    #voted_polls
    $voted_polls = array();
    $sql = "SELECT * FROM {poll_votes} WHERE uid = '%d'";
    $result = db_query($sql, $user->uid);
    while ($row = db_fetch_array($result)) {
      $voted_polls[] = $row['nid'];
    }

    #voted decisions
    $sql = "SELECT DISTINCT content_id FROM {votingapi_vote} WHERE uid = '%d'";
    $result = db_query($sql, $user->uid);
    while ($row = db_fetch_array($result)) {
      $voted_polls[] = $row['content_id'];
    }

    $non_voted_polls = array();
    foreach ($active_polls as $poll_id => $poll) {
      if (!in_array($poll, $voted_polls))
        $non_voted_polls[] = $poll;
    }

    return $non_voted_polls;

  }
  else return array();
}

function get_ts_last_round() {

  $sql = "SELECT MAX(date) FROM {fanta_rounds} WHERE date < '" . time() . "'";
  $result = db_query($sql);
  return db_result($result);

}

function get_ts_next_round() {

  $sql = "SELECT MIN(date) FROM {fanta_rounds} WHERE date > '" . time() . "'";
  $result = db_query($sql);
  return db_result($result);

}

function check_formazione_inserita() {

  global $user;

  $this_round = get_last_votes() + 1;
  $t_id = get_team_id_by_user($user->uid);
  
  $date = db_result(db_query("SELECT date FROM {fanta_rounds} WHERE round = '%d'", $this_round));

  $rounds = array();
  $sql = "SELECT * FROM {fanta_rounds_competitions} WHERE round = '%d'";
  $result = db_query($sql, $this_round);
  while ($row = db_fetch_array($result)) {
    $rounds[$row['c_id']] = $row['competition_round'];
  }

  $errors = 0;
  foreach ($rounds as $c_id => $round) {
    $sql = "SELECT * FROM {fanta_matches} " .
        "WHERE g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = '%d' ) " .
        "AND round = '%d' " .
        "AND (t1_id = '%d' OR t2_id = '%d')";
    $result = db_query($sql, $c_id, $round, $t_id, $t_id);
    while ($row = db_fetch_array($result)) {
      if ( ($date - time() ) < 86400 && ($date - time() ) > 0 ) {
        $sqla = "SELECT * FROM {fanta_lineups_inserts} 
              WHERE t_id = '%d' 
              AND c_id = '%d' 
              AND round = '%d'";
        $resulta = db_query($sqla, $t_id, $c_id, $round);

        if (db_affected_rows($resulta) == 0) $errors++;
      }
    }
  }

  return array("errors" => $errors, "date" => $date);

}

function fantacalcio_avviso_sfide() {

  global $user;

  $t_id = get_team_id_by_user($user->uid);

  $sql = "SELECT * FROM {fanta_sfide}
    WHERE t2_id = '%d' 
    AND status = 0";
  $result = db_query($sql, $t_id);
  if (db_affected_rows($result) > 0) {
    $sfide = (db_affected_rows($result) == 1) ? "1 sfida" : db_affected_rows($result) . " sfide";
    return "<li>" . l('Hai ricevuto ' . $sfide, 'sfide/personali') . "</li>";
  }
  return;
}

function fantacalcio_avviso_scommesse() {
  global $user;

  $round = get_last_votes() + 1;
  $ts_match = get_ts_next_round();

  $sql = "SELECT DISTINCT u_id FROM {bets}
    WHERE u_id = '%d' 
    AND round = '%d'";
  $result = db_query($sql, $user->uid, $round);
  if (db_result($result) != $user->uid) {
    if ( ($ts_match - time() ) < 86400 && ($ts_match - time() ) > 0 ) {
      return "<li>" . l('Non hai ancora scommesso', 'scommesse/gioca') . "</li>";
    }
  }
  return;
  
}

function fantacalcio_block_countdown() {

  $sql = "SELECT date 
          FROM {fanta_rounds} 
          WHERE round = (SELECT MIN(round) FROM {fanta_rounds} WHERE status = 0)";
  $date = db_result(db_query($sql));

  $diff = $date - time();

  if ($diff > 0) {

    drupal_add_js( "sites/all/plugins/jquerycountdown/jquery.countdown.js", 'footer');
    drupal_add_js( "sites/all/plugins/jquerycountdown/jquery.countdown-it.js", 'footer');
    $js = "$(function(){
             var nextRound = new Date();
             var y = " . date("Y", $date) . ";
			 var m = " . date("m", $date) . ";
             var d = " . date("d", $date) . ";
             var h = " . date("H", $date) . ";
             var i = " . date("i", $date) . ";
             var s = " . date("s", $date) . ";
	     nextRound = new Date(y, m-1, d, h, i, s);
	     $('#counter').countdown({until: nextRound});
           });";
    drupal_add_js($js, "inline");
    drupal_add_css("sites/all/plugins/jquerycountdown/jquery.countdown.css");

    $out = "<div id='counter'></div>";
    
    return $out;
  }
}

function fantacalcio_block_risultati_provvisori() {
  $round = db_result(db_query("SELECT MAX(round) FROM {fanta_votes}"));

  $played = db_result(db_query("SELECT status FROM {fanta_rounds} WHERE round = '%d'", $round));

  if ($played == -1)
    return "<h3>" . strtoupper(t("Risultati provvisori")) . "</h3>";
}
