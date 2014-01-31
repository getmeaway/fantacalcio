<?php
/**
  * @file fantacalcio.cron.inc
*/

function fantacalcio_send_formazioni_email() {

  $round = db_result(db_query("SELECT MAX(round) FROM {fanta_rounds} WHERE status = 1"));
  $sql = "SELECT date FROM {fanta_rounds} WHERE round = '%d'";
  $result = db_query($sql, $round + 1 );
  while ($row = db_fetch_object($result)) {
    $date = $row->date;
  }

  if ( ($date - time() ) < 3600 * 24) {
    if (variable_get("fantacalcio_newsletter_round", $round) <= $round) {
      fantacalcio_send_formazioni_newsletter($date, $round + 1);
      fantacalcio_crea_formazioni_news($round + 1);
      variable_set("fantacalcio_newsletter_round", $round + 1);
      watchdog("fantacalcio", "Promemoria inviato");
    }
  }
}

function fantacalcio_results() {
  $votes_round = variable_get("fantacalcio_results_round", 0);
  $sql = "SELECT end_date FROM {fanta_rounds} WHERE round = '%d'";
  $result = db_query($sql, $votes_round);
  while ($row = db_fetch_object($result)) {
    $date = $row->end_date;
  }

  if ( $date < time()) {
    switch (variable_get("fantacalcio_results_step", 0)) {
      case 1:
        fantacalcio_upload_votes_cron($votes_round);
        //controllo caricamento voti
        $sql = "SELECT COUNT(*) FROM {fanta_votes} WHERE round = '%d' "; 
        $result = db_result(db_query($sql, $votes_round));
        if ($result > 0) {
          variable_set("fantacalcio_results_step", 2);
          watchdog("fantacalcio", "Risultati - voti caricati");
        }
        break;
      case 2:
        fantacalcio_trova_titolari($votes_round);
        fantacalcio_calcola_modificatori($votes_round);
        fantacalcio_calcola_totali($votes_round);
        variable_set("fantacalcio_results_step", 3);
        watchdog("fantacalcio", "Risultati - totali calcolati");
        break;
      case 3:
        fantacalcio_passaggio_turno($votes_round);
        fantacalcio_crea_news($votes_round);
        fantacalcio_send_newsletter($votes_round);
        variable_set("fantacalcio_results_step", 1);
        variable_set("fantacalcio_results_round", ($votes_round + 1));
        watchdog("fantacalcio", "Risultati - News create");
        break;
    }
  }
}

function fantacalcio_send_formazioni_newsletter($ts_match, $round) {
  //newsletters
  $tid = 13;
  $uid = 1; //Admin

  $body = "Fantallenatore ti ricordo di " . l("inserire la formazione", "formazioni/insert") . " per la " . $round . "a Giornata entro " . date_transform($ts_match) . "."
    . "\nA presto.";
  $title = "Formazioni " . $round . "a Giornata";

  //create node
  $newsletter = array();
  $newsletter = (object) array('title'=> $title, 'body'=> $body, 'type' => 'simplenews', 'uid' => $uid, 'language' => 'it');

  //taxonomy
  $newsletter->taxonomy[$tid] = taxonomy_get_term($tid);
  $newsletter->simplenews = array('tid' => $tid, 's_format' => 'html', 'send' => SIMPLENEWS_COMMAND_SEND_NOW);

  node_save($newsletter);

  module_invoke('simplenews', 'simplenews_nodeapi', $newsletter, 'insert');
  
}

function fantacalcio_crea_formazioni_news($round) {
  
  $competitions = get_competitions();

  $sql = "SELECT * FROM {fanta_rounds_competitions} WHERE round = '%d'";
  $result = db_query($sql, $round);
  while ($row = db_fetch_object($result)) {
    $competition_roundrounds[$row->c_id] = (!empty($row->round_label)) ? $row->round_label : $row->competition_round . "&ordf; giornata";
  }
  
  $body = "";
  foreach ($competition_roundrounds as $c_id => $competition_round) {
     $body .= $competition_round . " " . $competitions[$c_id]->name . ", ";
  }
  
  $body = substr($body, 0, -2) . ": inserire la formazione entro " . date_transform(get_timestamp_round($round));
  
  $title = "Formazioni " . $round . "&ordf; Giornata";

  $news = (object) array('title'=> $title, 'body'=> $body, 'type' => 'news', 'uid' => 1, 'language' => 'it');
  node_save($news);
}
