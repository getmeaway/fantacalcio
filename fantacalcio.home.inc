<?php

/**
 * @file fantacalcio.home.inc
 * Funzioni per la pagina Home
 *
 */


function home_last_blog_post($n = 5) {

  drupal_add_js("sites/all/plugins/tabSwitch/jquery.tabswitch.yui.js");
  drupal_add_js("sites/all/plugins/tabSwitch/tabSwitch.title.js");
  drupal_add_css("sites/all/modules/fantacalcio/css/home.css");
  drupal_add_css("sites/all/plugins/tabSwitch/tabSwitch.css");
  drupal_add_css("sites/all/plugins/jquery.tabs/jquery.tabs.css");

  //dati
  $sql = "SELECT * FROM {node} n INNER JOIN {node_revisions} r " .
      "ON n.nid = r.nid " .
      "WHERE (n.type = 'article' OR n.type = 'audio' OR n.type = 'video') " .
      "AND n.status = 1 " .
      "AND n.promote = 1 " .
      "ORDER BY n.created DESC ";
  $result = db_query_range(db_rewrite_sql($sql), 0, $n);
  while ($row = db_fetch_object($result)) {
    $posts[$row->nid] = $row;
    $posts[$row->nid]->image_large = get_node_image($row->nid, "homepage"); //immagine grande
    $posts[$row->nid]->image_small = get_node_image($row->nid, "homepage-small"); //immagine piccola
  }

  //visualizzazione
  $out = "<div id='HeadTitle'>";
  
  if ($posts) {
  
  $out .= "<div id='MainTWI'>";
  foreach ($posts as $post) {
    $out .= "<div class='TWITab'>
          <div class='home-last-stories'>
          <img src='" . $post->image_large . "' title=\"" . $post->title . "\">
          <div class='dark-panel bottom-panel'>
            <h1>" . l(truncate(strtolower($post->title), 60), "node/" . $post->nid) . "</h1>
            <p>" . truncate(strip_tags($post->teaser), 180) . "</p>
          </div>
        </div>
        </div>";
  }
  $out .= "</div>";
  
  $out .= "<div id='ColorSelector'>";
  $class = " first";
  foreach ($posts as $post) {
      $out .= "<div class='TWICS" . $class . "' title=\"" . $post->title . "\">
            <img src='" . $post->image_small . "'>
          </div>";
      $class = "";
  }
  $out .= "</div>";
  } 
  
  $out .= "</div>";

  return $out;
}

function get_node_image($nid, $size) {
  
  //iid
  $sql = "SELECT iid FROM {image_attach} WHERE nid = '%d' ORDER BY weight ASC";
  $result = db_query_range($sql, $nid, 0, 1);
  $iid = db_result($result);
  
  if ($iid) {

    //fid
    $sql = "SELECT fid FROM {image} WHERE nid = '%d' AND image_size = '%s'";
    $result = db_query($sql, $iid, $size);
    $fid = db_result($result);

    //file
    $sql = "SELECT * FROM {files} WHERE fid = '%d'";
    $result = db_query($sql, $fid);
    $file = db_fetch_object($result);
    
    if ($file) return $file->filepath;
    else return base_path() . drupal_get_path("module", "fantacalcio") . "/images/default." . $size . ".jpg";
  }
  else return base_path() . drupal_get_path("module", "fantacalcio") . "/images/default." . $size . ".jpg";
}

function home_last_rss_news($n = 5) {

  include('sites/all/plugins/magpierss/rss_fetch.inc');

  $feed_url = variable_get("fantacalcio_feed_url", "");
  $rss = fetch_rss($feed_url);
    
  if ($rss) {
    $i = 0;
    $out = "\n<ul>";
    foreach ($rss->items as $item) {
      if ($i < $n) {
          if (!empty($item['description'])) {
              $out .= "\n<li><a href='" . $item["link"] . "'>"
                  . htmlentities(strip_tags($item["title"]))
                  . "</a></li>";
        }
      }
      $i++;
    }
    $out .= "</ul>";
  }

  return $out;

}

function fantacalcio_home_situation() {
  return make_tabs('home_situation', 'campionato');
}

function home_situation($g_id) {

  $round = get_last_competition_round(get_cid_by_gid($g_id));
  if ($round > 0)
    $out = "<h3>" . $round . "&ordf; giornata</h3><br>";

  $out_last = home_last_matches($g_id, $round) . "<br>";
  $out_standing = home_standings($g_id) . "<br>";
  $out_next .= home_next_matches($g_id, $round);
  
  if ( (time() - get_round_date($round, get_cid_by_gid($g_id))) > (get_round_date( ($round + 1), get_cid_by_gid($g_id)) - time()))
    $out .= $out_next . $out_last . $out_standing;
  else $out .= $out_last . $out_standing . $out_next;

  return $out;
}

function home_last_matches($g_id, $round) {

  if ($round == 0) return '';

  $teams = get_teams();
  $sql = "SELECT m_id, g_id, t1_id, t2_id, goals_1, goals_2
      FROM {fanta_matches}
      WHERE round = '%d'
      AND g_id = '%d'";
  $result = db_query($sql, $round, $g_id);
  while ($row = db_fetch_object($result)) {

    if (is_mobile())
      $link = l("", "scheda/partita/" . $row->m_id, array("attributes" => array('data-role' => 'button', 'data-icon' => "info", 'data-iconpos' => "notext", 'data-inline' => 'true', 'data-theme' => 'a')));
    else 
      $link = l("<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/details.png'>", "scheda/partita/" . $row->m_id, array("html" => "true", "attributes" => array("class" => "thickbox", "rel" => "lightframe[" . $row->round . "_" . $row->g_id . "|width:970px;]")));
        
    $rows[] = array("data" => array($teams[$row->t1_id]->name, $teams[$row->t2_id]->name, $row->goals_1, $row->goals_2, $link), "class" => $class);
  }
  
  $attr = array("style" => "width: 350px; font-size: 90%;");
  $attr = array();
  $header[] = array("data" => "Risultati", "colspan" => 5);

  return theme_table($header, $rows, $attr);

}

function home_next_matches($g_id, $round) {

  $max_round = db_result(db_query("SELECT MAX(round) FROM {fanta_matches} WHERE g_id IN (SELECT g_id FROM {fanta_groups} WHERE c_id = '%d')", variable_get("fantacalcio_main_competition", 1)));
  if ($round == $max_round) return "";
  
  $teams = get_teams();

  $round = $round + 1;
  
  $date = db_result(db_query("SELECT date FROM {fanta_rounds} r, {fanta_rounds_competitions} c 
                WHERE c.round = r.round AND c.competition_round = '%d' AND c_id = '%d'", $round, get_cid_by_gid($g_id)));

  $out_matches = "";
  $sql = "SELECT m_id, g_id, t1_id, t2_id
      FROM {fanta_matches}
      WHERE round = '%d'
      AND g_id = '%d'";
  $result = db_query($sql, $round, $g_id);
  while ($row = db_fetch_object($result)) {
    if (is_mobile())
      $link = l("", "scheda/partita/" . $row->m_id, array("attributes" => array('data-role' => 'button', 'data-icon' => "info", 'data-iconpos' => "notext", 'data-inline' => 'true', 'data-theme' => 'a')));
    else 
      $link = l("<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/details.png'>", "scheda/partita/" . $row->m_id, array("html" => "true", "attributes" => array("class" => "thickbox", "rel" => "lightframe[" . $row->round . "_" . $row->g_id . "|width:970px;]")));
        
    $rows[] = array("data" => array($teams[$row->t1_id]->name, $teams[$row->t2_id]->name, $link), "class" => $class);
  }
  
  $attr = array("style" => "width: 350px; font-size: 90%;");

  $header[] = array(  "data" => (get_round_date($round, get_cid_by_gid($g_id)) > time() ? "Prossimo turno" : "Giornata in corso")
                . " (" . date("d-m-Y", $date) . "&nbsp;&nbsp;&nbsp;&nbsp;" . date("H:i", $date) . ")", 
            "colspan" => 3);

  return theme_table($header, $rows);

}

function home_standings($g_id) {

  $teams = get_teams(); $points = array();

  $teams = filter_team_group($teams, $g_id);

  foreach ($teams as $team) {
    $t_id = $team->t_id;
    $points[$t_id] = 0; $goals[$t_id] = 0; $pts[$t_id] = 0;
    $sql = "SELECT *
        FROM {fanta_matches}
        WHERE (t1_id = '%d' OR t2_id = '%d')
        AND g_id = '%d'
        AND played = 1 ";
    $result = db_query($sql, $t_id, $t_id, $g_id);
    if (db_affected_rows($result) > 0) {
      while ($row = db_fetch_array($result)) {
        if ($row['winner_id'] == $t_id)
          $points[$t_id] += 3;
        elseif ($row['winner_id'] == -1)
          $points[$t_id] += 1;
        $goals[$t_id] += (($t_id == $row['t1_id']) ? $row['gol_1'] : $row['gol_2']);
        $pts[$t_id] += (($t_id == $row['t1_id']) ? $row['pt_1'] : $row['pt_2']);
      }
    }
    else return "";
  }

  array_multisort($points, SORT_DESC, $goals, SORT_DESC, $pts, SORT_DESC, $teams);

  $i = 0; $standings = array();
  foreach ($points as $t_id => $point) {
    $standings[$i]['t_id'] = $t_id;
    $standings[$i]['point'] = $point;
    $i++;
  }

  $attr = array("style" => "width: 350px; font-size: 90%;");
  $attr = array();
  $header = array("data" => "Classifica", "attributes" => array("colspan" => 4));

  $half = ceil(count($standings) / 2);
  foreach ($standings as $i => $standing) {
    if ($i < $half) {
      $j = $i%2 +1;
      $t2_id = $standings[$i + $half]['t_id'];
      $class = "role_" . $j;
      
      $rows[] = array("data" => array($teams[$standing['t_id']]->name, $standing['point'], 
              $teams[$t2_id]->name, $standings[$t2_id]['point']), "class" => $class);
    }
  }
  return theme_table($header, $rows, $attr);
}

function home_last_wall_messages($n = 5) {
  
  $out = "<ul class='wall-block'>";

  $sql = "SELECT * FROM {facebook_status}
      ORDER BY status_time DESC";
  $result = db_query_range($sql, 0, $n);
  while ($row = db_fetch_object($result)) {
    $out .= home_wall_message_theme($row);
  }
  $out .= "\n</ul>";
  $out .= "<p style='width:100%;text-align:right;'>" . l("Leggi tutto &raquo;", "bacheca", array("html" => "true")) . "</p>";

  return $out;
}


function home_wall_message_theme($status) {
  $users = get_users();
  $out = "\n<li class='home-wall-message'>"
    . "<div class='home-wall-picture'>"
    . (($users[$status->pid]->picture) ? "<img src='" . base_path() . $users[$status->pid]->picture . "' width='50'>" : "&nbsp;")
    . "</div>"
    . "<div class='home-wall-text'><strong>"
    . $users[$status->pid]->name
    . (($status->pid != $status->uid) ? " &raquo; " . $users[$status->uid]->name : "")
    . "</strong> " . truncate(strip_tags($status->status), 100) . get_message_comments($status->sid) . "</div>"
    . "<div class='clear'></div>"
    . "</li>";  
  return $out;
}

function get_message_comments($sid) {
  $sql = "SELECT count(*) FROM {fbssc} WHERE sid = '%d'";
  $number = db_result(db_query($sql, $sid));
  
  if ($number == 0) return "";
  elseif ($number == 1) return " <em>(1 commento)</em>";
  else return " <em>(" . $number . " commenti)</em>";
}
