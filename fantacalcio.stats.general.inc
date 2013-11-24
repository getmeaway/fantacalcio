<?php

/**
 * @file fantastats.general.inc
 *
 */

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
