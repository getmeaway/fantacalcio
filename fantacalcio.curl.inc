<?php

/**
  * @file fantacalcio.curl.inc
*/

function test_curl_voti($round) {
return fantacalcio_load_voti($round);
}

function parse_votes($data, $providers) {

  $lines = split("<div class=\"graphic-big\">", $data);
  $lines = split("<div class=\"footer\">", $lines[1]);
  $lines = $lines[0];

  $regex_player = '/<td class=".{1,8}">(.{1,7})?<\/td>/';
  $regex_votes = '/<td class="v.{0,3}">(.{1,7})?<span>/';
  $regex_novotes = '/<td class="u">(.{1,7})?<span>/';
  $regex_name = '#<td class=\"n\"><a title=\"(.*?)\"#';
  $regex_yellow_cards = '#<td class=\"vamm\">(.*?)\"#';
  $regex_red_cards = '#<td class=\"vesp\">(.*?)\"#';
  $regex_win_goals = '#<img border=\"0\" alt=\"(.*?)\"#';
  $regex_draw_goals = '#<img border=\"0\" alt=\"(.*?)\"#';

  $players = split("<tr class=\"P\">", $lines);
  unset($players[0]);

  $players_names = get_players_names();

  $vote_keys = array("vote", "goals_for", "penalty_goals", "goals_against", "saved_penalties", "missed_penalties", "own_goals", "assists");

  $player_votes = array();
  $starts = array(1 => 2, 2 => 10, 3 => 18);

  //giocatori
  foreach($players as $player) {
    preg_match($regex_name, $player, $result_names); //nome giocatore

    $pl_id = $players_names[strtolower($result_names[1])]; //id giocatore

    if ($pl_id > 0) {

      preg_match_all('#<td class="(.{1,8})">(.*?)<\/td>#', $player, $all_votes);

      $index = 0;

      $common_slice = array_slice($all_votes[2], 0, 2);

      foreach($providers as $p_id => $provider) {
        $votes = array_slice($all_votes[2], $starts[$p_id], count($vote_keys)); //spezzo l'array dei voti e prendo solo la parte relativa al provider di voti
        $votes = array_merge($common_slice, $votes); //aggiungo la parte comune in testa
     
        //voti e dati
        $player_votes[$p_id][$pl_id] = clean_votes(array_slice($votes, 2), $vote_keys); 

        //ammonizioni
        preg_match($regex_yellow_cards, $player, $results_yellow_cards);
        $player_votes[$p_id][$pl_id]["yellow_cards"] = isset($results_yellow_cards[1]) ? 1 : 0;

        //espulsioni
        preg_match($regex_red_cards, $player, $results_red_cards);
        $player_votes[$p_id][$pl_id]["red_cards"] = isset($results_red_cards[1]) ? 1 : 0;

        //gol vittoria
        preg_match($regex_win_goals, $player, $results_win_goals);
        $player_votes[$p_id][$pl_id]["win_goals"] = (isset($results_win_goals[1]) && strtolower($results_win_goals[1]) == "gol vittoria") ? 1 : 0;

        //gol pareggio
        preg_match($regex_draw_goals, $player, $results_draw_goals);
        $player_votes[$p_id][$pl_id]["draw_goals"] = (isset($results_draw_goals[1]) && strtolower($results_draw_goals[1]) == "gol pareggio") ? 1 : 0;

        //se senza voto e senza punteggi non lo inserisco
        if ($all_votes[1][$starts[$p_id]] == "u" && array_sum(array_splice($votes, 3)) == 0)
          unset($player_votes[$p_id][$pl_id]);
      }
    }
  }

  return $player_votes;
}

/**********
* voti    *
**********/
function get_votes_html_data($round, $providers) {
  
  $votes_url = variable_get("fantacalcio_votes_page", "http://www.fantagazzetta.com/voti-fantagazzetta-serie-A"). "-" . $round . "-giornata";

  $data = get_data_by_curl($votes_url);

  $votes = parse_votes($data, $providers);
  return $votes;
}

/******************
* lista giocatori *
******************/

function get_players_html_data() {

  $players_pages_url = array(
                        0 => variable_get("fantacalcio_lista_portieri_page", "http://www.fantagazzetta.com/quotazioni-serie-a/portieri/costodesc"),
                        1 => variable_get("fantacalcio_lista_difensori_page", "http://www.fantagazzetta.com/quotazioni-serie-a/difensori/costodesc"),
                        2 => variable_get("fantacalcio_lista_centrocampisti_page", "http://www.fantagazzetta.com/quotazioni-serie-a/centrocampisti/costodesc"),
                        3 => variable_get("fantacalcio_lista_attaccanti_page", "http://www.fantagazzetta.com/quotazioni-serie-a/attaccanti/costodesc")
                           );

  $data = array();

  foreach($players_pages_url as $role => $page) {
    $data[$role] = get_data_by_curl($page);
  }

  return $data;

}

/**********
* utility *
**********/

function get_data_by_curl($url) {

  /************************************************
  * ASP.NET web site scraping script;
  * Developed by MishaInTheCloud.com
  * Copyright 2009 MishaInTheCloud.com. All rights reserved.
  * The use of this script is governed by the CodeProject Open License
  * See the following link for full details on use and restrictions.
  *   http://www.codeproject.com/info/cpol10.aspx
  *
  * The above copyright notice must be included in any reproductions of this script.
  ************************************************/

  // urls to call - the login page and the secured page
  $url_login = variable_get("fantacalcio_voti_login_page", "http://www.fantagazzetta.com");
  
  // POST names and values to support login
  $textbox_username = 'ctl00%24HeaderMasterPageKnob%24LoginMasterKnob%24TextBoxUserName';       // the name of the username textbox on the login form
  $textbox_password = 'ctl00%24HeaderMasterPageKnob%24LoginMasterKnob%24TextBoxPassword';       // the name of the password textbox on the login form
  $button_login = 'ctl00%24HeaderMasterPageKnob%24LoginMasterKnob%24ButtonSubmit';          // the name of the login button (submit) on the login form
  $username = variable_get("fantacalcio_voti_login_username", "");        // the value to submit for the username
  $password = variable_get("fantacalcio_voti_login_password", "");        // the value to submit for the password
  $login_value = 'Login';             // the text value of the login button itself
//echo $username."...".$password."...";
  $cookie = "./tmp/cookie.txt";

  // regular expressions to parse out the special ASP.NET
  // values for __VIEWSTATE and __EVENTVALIDATION
  $regex_viewstate = '/__VIEWSTATE\" value=\"(.*)\"/i';
  $regex_eventval  = '/__EVENTVALIDATION\" value=\"(.*)\"/i';

  $regs = array();

  /************************************************
  * initialize a curl handle; we'll use this
  *   handle throughout the script
  ************************************************/
  $ch = curl_init();


  /************************************************
  * first, issue a GET call to the ASP.NET login
  *   page.  This is necessary to retrieve the
  *   __VIEWSTATE and __EVENTVALIDATION values
  *   that the server issues
  ************************************************/
  curl_setopt($ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:11.0) Gecko/20100101 Firefox/11.0' );
  curl_setopt($ch, CURLOPT_URL, $url_login);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch, CURLOPT_FOLLOWLOCATION, FALSE);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
  $data = curl_exec($ch);

  // from the returned html, parse out the __VIEWSTATE and
  // __EVENTVALIDATION values
  $viewstate = regex_extract($data, $regex_viewstate, $regs, 1);
  $eventval = regex_extract($data, $regex_eventval, $regs, 1);

  /************************************************
  * now issue a second call to the Login page;
  *   this time, it will be a POST; we'll send back
  *   as post data the __VIEWSTATE and __EVENTVALIDATION
  *   values the server previously sent us, as well as the
  *   username/password.  We'll also set up a cookie
  *   jar to retrieve the authentication cookie that
  *   the server will generate and send us upon login.
  ************************************************/

  $post_data = '__VIEWSTATE='.rawurlencode($viewstate)
          .'&__EVENTVALIDATION='.rawurlencode($eventval)
          .'&'.$textbox_username.'='.$username
          .'&'.$textbox_password.'='.$password
          .'&'.$button_login.'='.$login_value
          ;
		  
	//	 echo $post_data;die();
  
  curl_setopt($ch, CURLOPT_USERAGENT, $_SERVER["HTTP_USER_AGENT"]);
  curl_setOpt($ch, CURLOPT_POST, TRUE);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
  curl_setopt($ch, CURLOPT_URL, $url_login);   
  curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie);     
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);

  //  $data = curl_exec($ch);
  curl_exec($ch);


  /************************************************
  * with the authentication cookie in the jar,
  * we'll now issue a GET to the secured page;
  * we set curl's COOKIEFILE option to the same
  * file we used for the jar before to ensure the
  * authentication cookie is sent back to the
  * server
  ************************************************/
  curl_setopt($ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:11.0) Gecko/20100101 Firefox/11.0' );
  curl_setOpt($ch, CURLOPT_POST, FALSE);
  curl_setopt($ch, CURLOPT_URL, $url);   
  curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie);     
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);

  $data = curl_exec($ch);

  /************************************************
  * that's it! Close the curl handle
  ************************************************/
  curl_close($ch);

  return $data;
}

function curl_exec_follow($ch, $maxredirect = null) {
  
  // we emulate a browser here since some websites detect
  // us as a bot and don't let us do our job
  $mr = $maxredirect === null ? 5 : intval($maxredirect);
  
  if ($mr > 0)
  {
      $original_url = curl_getinfo($ch, CURLINFO_EFFECTIVE_URL);
      $newurl = $original_url;
      
      $rch = curl_copy_handle($ch);
      
      curl_setopt($rch, CURLOPT_HEADER, true);
      curl_setopt($rch, CURLOPT_NOBODY, true);
      curl_setopt($rch, CURLOPT_FORBID_REUSE, false);
      do
      {
        curl_setopt($rch, CURLOPT_URL, $newurl);
        $header = curl_exec($rch);
        if (curl_errno($rch)) {
          $code = 0;
        } else {
          $code = curl_getinfo($rch, CURLINFO_HTTP_CODE);
          if ($code == 301 || $code == 302) {
			preg_match('/Location:(.*?)\n/', $header, $matches);
            $newurl = trim(array_pop($matches));
			
			print_r($new_url); print "<br>"; print $original_url; die();
            
            // if no scheme is present then the new url is a
            // relative path and thus needs some extra care
            if(!preg_match("/^https?:/i", $newurl)){
              $newurl = $original_url . $newurl;
            }   
          } else {
            $code = 0;
          }
        }
      } while ($code && --$mr);
      
      curl_close($rch);
      
      if (!$mr)
      {
        if ($maxredirect === null)
        trigger_error('Too many redirects.', E_USER_WARNING);
        else
        $maxredirect = 0;
        
        return false;
      }
      curl_setopt($ch, CURLOPT_URL, $newurl);
    }
  
  return curl_exec($ch);
}

function regex_extract($text, $regex, $regs, $index)
{
  if (preg_match($regex, $text, $regs)) {
    $result = $regs[$index];
  }
  else {
    $result = "";
  }
  return $result;
}

function clean_votes($votes, $keys) {
  $results = array();//print_r($votes);
  $index = 0;
  foreach($keys as $key) {
    $span = split("<span>", $votes[$index]);
    $results[$key] = ($votes[$index] == "-") ? 0 : str_replace(",", ".", $span[0]) ;
    $index++;
  }
  return $results;
}
