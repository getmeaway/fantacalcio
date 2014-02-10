<?php
/**
*
* @file fantacalcio.admin.inc
*
*/

/************************/
/* SETTINGS    */
/************************/

function fantacalcio_admin_settings() {

  $form["fantacalcio_generali"] = array(
    "#type" => "fieldset",
    "#collapsed" => TRUE,
    "#collapsible" => TRUE,
    "#title" => t("Opzioni generali"),
  );

  $form["fantacalcio_generali"]['fantacalcio_main_competition'] = array(
    '#type' => 'select',
    '#title' => t('Competizione principale'),
    '#default_value' => variable_get('fantacalcio_main_competition', '1'),
    '#options' => get_competitions_options(),
    '#description' => t('Competizione principale per la Lega (per Sfide e Scommesse).'),
  );
  
  $form["fantacalcio_generali"]['fantacalcio_coppa_competition'] = array(
    '#type' => 'select',
    '#title' => t('Competizione Coppa'),
    '#default_value' => variable_get('fantacalcio_coppa_competition', '1'),
    '#options' => get_competitions_options(),
    '#description' => t('Competizione di Coppa per la Lega.'),
  );
  
  $form["fantacalcio_generali"]['fantacalcio_credits'] = array(
    '#type' => 'textfield',
    '#title' => t('Crediti'),
    '#default_value' => variable_get('fantacalcio_credits', '350'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Numero di crediti per creare la propria rosa.'),
  );

  $form["fantacalcio_generali"]['fantacalcio_scarto'] = array(
    '#type' => 'checkbox',
    '#title' => t('Vittoria con scarto'),
    '#default_value' => variable_get('fantacalcio_scarto', '0'),
    '#description' => t('Selezionare se si assegna la vittoria ad una squadra che supera di tot punti l\'avversaria.'),
  );
  
  $form["fantacalcio_generali"]['fantacalcio_scarto_punti'] = array(
    '#type' => 'textfield',
    '#title' => t('Differenza tra i punteggi per la vittoria'),
    '#default_value' => variable_get('fantacalcio_scarto_punti', '0'),
    '#description' => t('Selezionare il numero di punti minimi di differenza tra le due squadre per assegnare la vittoria.'),
  );
  
  $form["fantacalcio_generali"]['fantacalcio_riserve_fisse'] = array(
    '#type' => 'checkbox',
    '#title' => t('Riserve fisse'),
    '#default_value' => variable_get('fantacalcio_riserve_fisse', '0'),
    '#description' => t('Selezionare se in formazione vanno inserite 2 riserve per ruolo (1 portiere).'),
  );
    
  $form["fantacalcio_rosa"] = array(
    "#type" => "fieldset",
    "#collapsed" => TRUE,
    "#collapsible" => TRUE,
    "#title" => "Rosa",
  );
    
  $form['fantacalcio_rosa']['fantacalcio_number_role_0'] = array(
    '#type' => 'textfield',
    '#title' => t('Numero Portieri'),
    '#default_value' => variable_get('fantacalcio_number_role_0', '3'),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('Numero di portieri da inserire in rosa.'),
  );
  
  $form['fantacalcio_rosa']['fantacalcio_number_role_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Numero Difensori'),
    '#default_value' => variable_get('fantacalcio_number_role_1', '8'),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('Numero di difensori da inserire in rosa.'),
  );
  
  $form['fantacalcio_rosa']['fantacalcio_number_role_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Numero Centrocampisti'),
    '#default_value' => variable_get('fantacalcio_number_role_2', '8'),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('Numero di centrocampisti da inserire in rosa.'),
  );
  
  $form['fantacalcio_rosa']['fantacalcio_number_role_3'] = array(
    '#type' => 'textfield',
    '#title' => t('Numero Attaccanti'),
    '#default_value' => variable_get('fantacalcio_number_role_3', '6'),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('Numero di attaccanti da inserire in rosa.'),
  );
  
  $form["fantacalcio_moduli"] = array(
    "#type" => "fieldset",
    "#collapsed" => TRUE,
    "#collapsible" => TRUE,
    "#title" => "Moduli",
  );
  
  $form["fantacalcio_moduli"]['fantacalcio_moduli_titolari'] = array(
    '#type' => 'textarea',
    '#title' => t('Moduli titolari'),
    '#default_value' => variable_get('fantacalcio_moduli_titolari', ''),
    '#description' => t('Moduli permessi per i titolari (un modulo per riga, es: 1-4-4-2 o 1-3-5-2)'),
  );
  
  $form["fantacalcio_moduli"]['fantacalcio_moduli_riserve'] = array(
    '#type' => 'textarea',
    '#title' => t('Moduli riserve'),
    '#default_value' => variable_get('fantacalcio_moduli_riserve', ''),
    '#description' => t('Moduli permessi per le riserve (un modulo per riga, es: 1-4-4-2 o 1-3-5-2)'),
  );
  
  $form["fantacalcio_modificatori"] = array(
    "#type" => "fieldset",
    "#collapsed" => TRUE,
    "#collapsible" => TRUE,
    "#title" => "Modificatori",
  );
  
  $form["fantacalcio_modificatori"]['fantacalcio_modificatore_portiere'] = array(
    '#type' => 'checkbox',
    '#title' => t('Modificatore Portiere'),
    '#default_value' => variable_get('fantacalcio_modificatore_portiere', '0'),
    '#description' => t('Selezionare se si applica il modificatore del portiere.'),
  );
  
  $form["fantacalcio_modificatori"]['fantacalcio_modificatore_difesa'] = array(
    '#type' => 'checkbox',
    '#title' => t('Modificatore Difesa'),
    '#default_value' => variable_get('fantacalcio_modificatore_difesa', '0'),
    '#description' => t('Selezionare se si applica il modificatore della difesa.'),
  );
  
  $form["fantacalcio_modificatori"]['fantacalcio_modificatore_centrocampo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Modificatore Centrocampo'),
    '#default_value' => variable_get('fantacalcio_modificatore_centrocampo', '0'),
    '#description' => t('Selezionare se si applica il modificatore del centrocampo.'),
  );
  
  $form["fantacalcio_modificatori"]['fantacalcio_modificatore_attacco'] = array(
    '#type' => 'checkbox',
    '#title' => t('Modificatore Attacco'),
    '#default_value' => variable_get('fantacalcio_modificatore_attacco', '0'),
    '#description' => t('Selezionare se si applica il modificatore dell\'attacco.'),
  );
    
  $form["fantacalcio_points"] = array(
    "#type" => "fieldset",
    "#collapsed" => TRUE,
    "#collapsible" => TRUE,
    "#title" => "Bonus e malus",
  );

  $form["fantacalcio_points"]['fantacalcio_points_goals_for'] = array(
    '#type' => 'textfield',
    '#title' => t('Punteggio per goal segnato'),
    '#default_value' => variable_get('fantacalcio_points_goals_for', '3'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Punteggio attribuito per ogni goal segnato'),
  );

  $form["fantacalcio_points"]['fantacalcio_points_goals_against'] = array(
    '#type' => 'textfield',
    '#title' => t('Punteggio per goal subito'),
    '#default_value' => variable_get('fantacalcio_points_goals_against', '-1'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Punteggio attribuito per ogni goal subito'),
  );
  
  $form["fantacalcio_points"]['fantacalcio_points_penalty_goals'] = array(
    '#type' => 'textfield',
    '#title' => t('Punteggio per rigore segnato'),
    '#default_value' => variable_get('fantacalcio_points_penalty_goals', '3'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Punteggio attribuito per ogni rigore segnato'),
  );

  $form["fantacalcio_points"]['fantacalcio_points_missed_penalties'] = array(
    '#type' => 'textfield',
    '#title' => t('Punteggio per rigore sbagliato'),
    '#default_value' => variable_get('fantacalcio_points_missed_penalties', '-3'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Punteggio attribuito per ogni rigore sbagliato'),
  );
  
  $form["fantacalcio_points"]['fantacalcio_points_saved_penalties'] = array(
    '#type' => 'textfield',
    '#title' => t('Punteggio per rigore parato'),
    '#default_value' => variable_get('fantacalcio_points_saved_penalties', '3'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Punteggio attribuito per ogni rigore parato'),
  );
  
  $form["fantacalcio_points"]['fantacalcio_points_yellow_cards'] = array(
    '#type' => 'textfield',
    '#title' => t('Punteggio per ammonizione'),
    '#default_value' => variable_get('fantacalcio_points_yellow_cards', '-0.5'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Punteggio attribuito per ogni ammonizione'),
  );
  
  $form["fantacalcio_points"]['fantacalcio_points_red_cards'] = array(
    '#type' => 'textfield',
    '#title' => t('Punteggio per espulsione'),
    '#default_value' => variable_get('fantacalcio_points_red_cards', '-1'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Punteggio attribuito per ogni espulsione'),
  );
  
  $form["fantacalcio_points"]['fantacalcio_points_own_goals'] = array(
    '#type' => 'textfield',
    '#title' => t('Punteggio per autogoal'),
    '#default_value' => variable_get('fantacalcio_points_own_goals', '-2'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Punteggio attribuito per ogni autogoal'),
  );
  
  $form["fantacalcio_points"]['fantacalcio_points_assists'] = array(
    '#type' => 'textfield',
    '#title' => t('Punteggio per assist'),
    '#default_value' => variable_get('fantacalcio_points_assists', '1'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Punteggio attribuito per ogni assist'),
  );
  
  $form["fantacalcio_points"]['fantacalcio_points_draw_goals'] = array(
    '#type' => 'textfield',
    '#title' => t('Punteggio per gol pareggio'),
    '#default_value' => variable_get('fantacalcio_points_draw_goals', '1'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Punteggio attribuito per ogni gol pareggio'),
  );
  
  $form["fantacalcio_points"]['fantacalcio_points_win_goals'] = array(
    '#type' => 'textfield',
    '#title' => t('Punteggio per gol vittoria'),
    '#default_value' => variable_get('fantacalcio_points_win_goals', '3'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Punteggio attribuito per ogni gol vittoria'),
  );

  $form["fantacalcio_points"]['fantacalcio_default_vote_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Voto di default #1'),
    '#default_value' => variable_get('fantacalcio_default_vote_1', '6'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Voto di default #1 per i giocatori non scesi in campo'),
  );
  
  $form["fantacalcio_points"]['fantacalcio_default_vote_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Voto di default #2'),
    '#default_value' => variable_get('fantacalcio_default_vote_2', '6'),
    '#size' => 64,
    '#maxlength' => 64,
    '#description' => t('Voto di default #2 per i giocatori non scesi in campo'),
  );

  $form["fantacalcio_voti_params"] = array(
    "#type" => "fieldset",
    "#collapsed" => TRUE,
    "#collapsible" => TRUE,
    "#title" => t("Caricamento Voti"),
  );
  
  $form["fantacalcio_voti_params"]['fantacalcio_votes_provider'] = array(
    '#type' => 'select',
    '#title' => t('Fonte dei voti su Fantagazzetta.com'),
    '#default_value' => variable_get('fantacalcio_votes_provider', ''),
	'#options' => get_providers_for_select(),
    '#description' => t('Redazione di Fantagazzetta.com che fornisce i voti per il conteggio'),
  );

  $form["fantacalcio_voti_params"]['fantacalcio_voti_login_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username per login su Fantagazzetta.com'),
    '#default_value' => variable_get('fantacalcio_voti_login_username', ''),
    '#size' => 255,
    '#maxlength' => 255,
    '#description' => t('Username per login su Fantagazzetta.com'),
  );

  $form["fantacalcio_voti_params"]['fantacalcio_voti_login_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password per login su Fantagazzetta.com'),
    '#default_value' => variable_get('fantacalcio_voti_login_password', ''),
    '#size' => 255,
    '#maxlength' => 255,
    '#description' => t('Password per login su Fantagazzetta.com'),
  );

  $form["fantacalcio_voti_params"]['fantacalcio_voti_login_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Url per login su Fantagazzetta.com'),
    '#default_value' => variable_get('fantacalcio_voti_login_page', ''),
    '#size' => 255,
    '#maxlength' => 255,
    '#description' => t('Url per login su Fantagazzetta.com'),
  );

  $form["fantacalcio_voti_params"]['fantacalcio_voti_secured_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Url pagina voti su Fantagazzetta.com'),
    '#default_value' => variable_get('fantacalcio_voti_secured_page', ''),
    '#size' => 255,
    '#maxlength' => 255,
    '#description' => t('Url pagina voti su Fantagazzetta.com'),
  );

  $form["fantacalcio_voti_params"]['fantacalcio_lista_portieri_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Url lista portieri su Fantagazzetta.com'),
    '#default_value' => variable_get('fantacalcio_lista_portieri_page', ''),
    '#size' => 255,
    '#maxlength' => 255,
    '#description' => t('Url lista portieri su Fantagazzetta.com'),
  );

  $form["fantacalcio_voti_params"]['fantacalcio_lista_difensori_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Url lista difensori su Fantagazzetta.com'),
    '#default_value' => variable_get('fantacalcio_lista_difensori_page', ''),
    '#size' => 255,
    '#maxlength' => 255,
    '#description' => t('Url lista difensori su Fantagazzetta.com'),
  );

  $form["fantacalcio_voti_params"]['fantacalcio_lista_centrocampisti_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Url lista centrocampisti su Fantagazzetta.com'),
    '#default_value' => variable_get('fantacalcio_lista_centrocampisti_page', ''),
    '#size' => 255,
    '#maxlength' => 255,
    '#description' => t('Url lista centrocampisti su Fantagazzetta.com'),
  );

  $form["fantacalcio_voti_params"]['fantacalcio_lista_attaccanti_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Url lista attaccanti su Fantagazzetta.com'),
    '#default_value' => variable_get('fantacalcio_lista_attaccanti_page', ''),
    '#size' => 255,
    '#maxlength' => 255,
    '#description' => t('Url lista attaccanti su Fantagazzetta.com'),
  );



  $form["fantacalcio_utilities"] = array(
    "#type" => "fieldset",
    "#collapsed" => TRUE,
    "#collapsible" => TRUE,
    "#title" => t("Altro"),
  );
  
  $form["fantacalcio_utilities"]['fantacalcio_votes_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url della pagina da cui scaricare i voti'),
    '#default_value' => variable_get('fantacalcio_votes_url', ''),
    '#size' => 255,
    '#maxlength' => 255,
    '#description' => t('Url della pagina da cui scaricare i voti'),
  );

  $form["fantacalcio_utilities"]['fantacalcio_feed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url feed delle notizie'),
    '#default_value' => variable_get('fantacalcio_feed_url', ''),
    '#size' => 255,
    '#maxlength' => 255,
    '#description' => t('Url del feed delle notizie'),
  );

  return system_settings_form($form);

}

/************************/
/*  gestione squadre   */
/************************/

function fantacalcio_admin_teams_list() {
  $out = "";
  
  $out .= l("Aggiungi squadra", "admin/settings/fantacalcio/teams/add") . "<br/><br/>";
  
  $teams = get_all_teams();
  $users = get_users();
  $groups = get_groups();
  $competitions = get_competitions();
  if ($teams) {
    $header = array("Nome", "Utente", "Gironi", "Attiva");
    foreach ($teams as $t_id => $team) {
      $groups_team = "";
      foreach ($team->groups as $g_id) {
        $groups_team .= $competitions[$groups[$g_id]->c_id]->name . " - " . $groups[$g_id]->name . "<br>";
      }
      $rows[] = array(
            l($team->name, "admin/settings/fantacalcio/teams/" . $t_id),
            $users[$team->uid]->name,
            $groups_team,
            "<img src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($team->active) . "'>"
            );
    }
    $out .= theme_table($header, $rows);
  }
  
  return $out;
}

function fantacalcio_admin_team($form_state, $t_id = "") {
  if (groups_count()) {
    $groups_checkboxes = get_groups_checkboxes();
      
    $action = "create"; $username = ""; $groups_checkboxes_default = array(); $title = "Crea squadra";
    if (!empty($t_id) && team_exists($t_id)) {
      $team_data = get_team_data($t_id);
      $action = "edit";
      $account = user_load(array('uid' => check_plain($team_data['uid'])));
      $username = $account->name;
      $groups_checkboxes_default = get_groups_checkboxes_default($t_id);
      $title = "Modifica squadra";
    }
    else $form['#redirect'] = 'admin/settings/fantacalcio/teams';
    
    $form['#prefix'] = '<h3>' . $title . '</h3>';
    
    $form['action'] = array(
      '#type' => 'hidden',
      '#value' => $action,
    );
    
    $form['t_id'] = array(
      '#type' => 'hidden',
      '#value' => $t_id,
    );
    
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Nome'),
      '#required' => TRUE,
      '#default_value' => $team_data['name'], 
    );
    
    $form['user'] = array(
      '#type' => 'textfield',
      '#title' => t('Utente'),
      '#required' => TRUE,
      '#autocomplete_path' => 'user/autocomplete', 
      '#default_value' => $username,
    );
    
    $form['active'] = array(
      '#type' => 'checkbox',
      '#title' => t('Attiva'),
      '#default_value' => $team_data['active'],
    );
    
    /* Gruppi */
    $form['groups'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Gironi'),
      '#options' => $groups_checkboxes, 
      '#default_value' => (count($groups_checkboxes_default) > 0) ? $groups_checkboxes_default : array(), 
    );
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Invia')
    );

    $form['clear'] = array(
      '#type' => 'submit',
      '#value' => 'Annulla',
      '#validate' => array(
        'fantacalcio_admin_team_clear'
      ),
      '#suffix' => l("Elimina", 'admin/settings/fantacalcio/teams/' . $t_id . '/delete')
    );
    return $form;
  }
  else {
    drupal_set_message("Devi prima creare un girone");
    drupal_goto("admin/settings/fantacalcio/groups/add");
  }
}

function fantacalcio_admin_team_submit($form, &$form_state) {
  
  $account = user_load(array('name' => check_plain($form_state["values"]["user"])));
  
  $out = $form_state["values"]["name"] . $account->uid . $form_state["values"]["t_id"];
  
  if ($form_state["values"]["action"] == "edit") {
    //update team
    $sql = "UPDATE {fanta_teams} SET name = '%s', uid = '%d', active = '%d' WHERE t_id = '%d'";
    $result = db_query($sql, $form_state["values"]["name"], $account->uid, $form_state["values"]["active"], $form_state["values"]["t_id"]);
    
    //update teams_groups
    $sql = "DELETE FROM {fanta_teams_groups} WHERE t_id = '%d'";
    $result = db_query($sql, $form_state["values"]["t_id"]);
    
    foreach ($form_state["values"]["groups"] as $group) {
      if ($group != 0) {
        $sql = "INSERT INTO {fanta_teams_groups} (g_id, t_id, active) VALUES ('%d', '%d', 1)";
        $result = db_query($sql, $group, $form_state["values"]["t_id"]);
      }
    }
    drupal_set_message("Squadra modificata");
  }
  elseif ($form_state["values"]["action"] == "create") {
    //crea team
    $sql = "INSERT INTO {fanta_teams} (name, uid, active) VALUES ('%s', '%d', 1)";
    $result = db_query($sql, $form_state["values"]["name"], $account->uid);
    $new_t_id = db_last_insert_id("fanta_teams", "t_id");
    
    //update teams_groups
    $sql = "DELETE FROM {fanta_teams_groups} WHERE t_id = '%d'";
    $result = db_query($sql, $new_t_id);
    
    foreach ($form_state["values"]["groups"] as $group) {
      if ($group != 0) {
        $sql = "INSERT INTO {fanta_teams_groups} (g_id, t_id, active) VALUES ('%d', '%d', 1)";
        $result = db_query($sql, $group, $new_t_id);
      }
    }
    drupal_set_message("Squadra creata");
  }
  
}

function fantacalcio_admin_team_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function fantacalcio_admin_team_delete($form_state, $t_id = "") {

  $form['#redirect'] = 'admin/settings/fantacalcio/teams';
  
  $teams = get_all_teams();
  $t_name = $teams[$t_id]->name;
  
  $form['#prefix'] = 'Sicuro di volere eliminare la squadra <em>' . $t_name . '</em>?';
  
  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => "delete",
  );
  
  $form['t_id'] = array(
    '#type' => 'hidden',
    '#value' => $t_id,
  );
      
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Elimina'),
    '#suffix' => l("Annulla", 'admin/settings/fantacalcio/teams/' . $t_id)
  );

  return $form;
}

function fantacalcio_admin_team_delete_submit($form, &$form_state) {
  
  if ($form_state["values"]["action"] == "delete") {
    //delete team
    $sql = "DELETE FROM {fanta_teams} WHERE t_id = '%d'";
    $result = db_query($sql, $form_state["values"]["t_id"]);
    
    //delete teams_groups
    $sql = "DELETE FROM {fanta_teams_groups} WHERE t_id = '%d'";
    $result = db_query($sql, $form_state["values"]["t_id"]);
    
    drupal_set_message("Squadra eliminata");
  }
}

/************************/
/* end gestione squadre */
/************************/

/************************/
/*  gestione gironi   */
/************************/

function fantacalcio_admin_groups_list() {
  $out = "";
  
  $out .= l("Aggiungi girone", "admin/settings/fantacalcio/groups/add") . "<br/><br/>";
  
  $groups = get_groups();
  $competitions = get_competitions();
  if ($groups) {
    $header = array("Girone", "Attivo", "Calendario", "Classfica", "Formazioni", "Newsletter Calendario", "Newsletter Classifica");
    foreach ($groups as $g_id => $group) {
      $rows[] = array(l($competitions[$group->c_id]->name . " - " . $group->name, "admin/settings/fantacalcio/groups/" . $g_id),
                "<img src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($group->active) . "'>",
                $group->matches,
                $group->standings,
                $group->lineups,
                $group->newsletter_matches,
                $group->newsletter_standings,
              );
    }
    $out .= theme_table($header, $rows);
  }

  return $out;
}

function fantacalcio_admin_group($form_state, $g_id = "") {
  if (competitions_count()) {
    $groups_checkboxes = get_groups_checkboxes();
      
    $action = "create"; $title = "Crea girone";
    if (!empty($g_id) && group_exists($g_id)) {
      $group_data = get_group_data($g_id);
      $action = "edit";
      $title = "Modifica girone";
    }
    else $form['#redirect'] = 'admin/settings/fantacalcio/groups';
    
    $form['#prefix'] = '<h3>' . $title . '</h3><br/>';
    
    $form['action'] = array(
      '#type' => 'hidden',
      '#value' => $action,
    );
    
    $form['g_id'] = array(
      '#type' => 'hidden',
      '#value' => $g_id,
    );
    
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Nome'),
      '#required' => TRUE,
      '#default_value' => $group_data['name'], 
    );
    
    $form['c_id'] = array(
      '#type' => 'select',
      '#title' => t('Competizione'),
      '#required' => TRUE,
      '#options' => get_competitions_for_select(),
      '#default_value' => $group_data['c_id'], 
    );
    
    $form['active'] = array(
      '#type' => 'checkbox',
      '#title' => t('Attiva'),
      '#default_value' => $group_data['active'],
    );
		
	$form['teams'] = array(
	  '#type' => 'fieldset',
	  '#title' => t("Teams"),
	  '#collapsed' => true,
	  '#collapsible' => true
	);
	
	$form['teams']['teams'] = array(
	  '#type' => 'checkboxes',
	  '#options' => get_teams_options(),
	  '#default_value' => get_teams_group_options_checked($g_id),
	);
    
    $form['matches'] = array(
      '#type' => 'weight',
      '#delta' => 5,
      '#title' => t('Calendario'),
      '#default_value' => $group_data['matches'],
      '#description' => "0 per non mostrare il calendario, un numero positivo per mostrarlo <br>
                (il numero inserito stabilisce l'ordine del girone rispetto agli altri nella visualizzazione del calendario)"
    );
    $form['standings'] = array(
      '#type' => 'weight',
      '#title' => t('Classifica'),
      '#default_value' => $group_data['standings'],
      '#description' => "0 per non mostrare la classifica, un numero positivo per mostrarla <br>
                (il numero inserito stabilisce l'ordine del girone rispetto agli altri nella visualizzazione della classifica)"
    );
    $form['lineups'] = array(
      '#type' => 'weight',
      '#title' => t('Formazioni'),
      '#default_value' => $group_data['lineups'],
      '#description' => "0 per non mostrare le formazioni, un numero positivo per mostrarle <br>
                (il numero inserito stabilisce l'ordine del girone rispetto agli altri nella visualizzazione delle formazioni)"
    );
    $form['newsletter_matches'] = array(
      '#type' => 'weight',
      '#title' => t('Newsletter Calendario'),
      '#default_value' => $group_data['newsletter_matches'],
      '#description' => "0 per non mostrare il calendario nella newsletter, un numero positivo per mostrarlo <br>
                (il numero inserito stabilisce l'ordine del girone rispetto agli altri nella visualizzazione del calendario)"
    );
    
    $form['newsletter_standings'] = array(
      '#type' => 'weight',
      '#title' => t('Newsletter Classifiche'),
      '#default_value' => $group_data['newsletter_standings'],
      '#description' => "0 per non mostrare il calendario nella newsletter, un numero positivo per mostrarlo <br>
                (il numero inserito stabilisce l'ordine del girone rispetto agli altri nella visualizzazione del calendario)"
    );
	        
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Invia')
    );

    $form['clear'] = array(
      '#type' => 'submit',
      '#value' => 'Annulla',
      '#validate' => array(
        'fantacalcio_admin_group_clear'
      ),
      '#suffix' => l("Elimina", 'admin/settings/fantacalcio/groups/' . $g_id . '/delete')
    );
    return $form;
  }
  else {
    drupal_set_message("Devi prima creare una competizione");
    drupal_goto("admin/settings/fantacalcio/competitions/add");
  }
}

function fantacalcio_admin_group_submit($form, &$form_state) {
  
  if ($form_state["values"]["action"] == "edit") {
    //update team
    $sql = "UPDATE {fanta_groups} SET name = '%s', c_id = '%d', active = '%d', matches = '%d', 
        standings = '%d', lineups = '%d', newsletter_matches = '%d', newsletter_standings = '%d'
        WHERE g_id = '%d'";
    $result = db_query($sql, $form_state["values"]["name"], $form_state["values"]["c_id"], 
              $form_state["values"]["active"], $form_state["values"]["matches"], 
              $form_state["values"]["standings"], $form_state["values"]["lineups"], 
              $form_state["values"]["newsletter_matches"], $form_state["values"]["newsletter_standings"], 
              $form_state["values"]["g_id"]);
			  
	//squadre del girone
	//cancello elementi già inseriti
	$sql = "DELETE FROM {fanta_teams_groups} WHERE g_id = '%d'";
	db_query($sql, $form_state["values"]["g_id"]);
	
	//inserisco nuovi elementi
	foreach($form_state["values"]["teams"] as $t_id => $checked) {
      if ($checked) {
        $sql = "INSERT INTO {fanta_teams_groups} (g_id, t_id, active) VALUES ('%d', '%d', 1)";
	    db_query($sql, $form_state["values"]["g_id"], $t_id);
      }
	}
    
    drupal_set_message("Girone modificato");
  }
  elseif ($form_state["values"]["action"] == "create") {
    //crea team
    $sql = "INSERT INTO {fanta_groups} (name, c_id, active, matches, standings, lineups, newsletter_matches, newsletter_standings) 
        VALUES ('%s', '%d', '%d', '%d', '%d', '%d', '%d', '%d')";
    $result = db_query($sql, $form_state["values"]["name"], $form_state["values"]["c_id"], 
              $form_state["values"]["active"], $form_state["values"]["matches"], 
              $form_state["values"]["standings"], $form_state["values"]["lineups"], 
              $form_state["values"]["newsletter_matches"], $form_state["values"]["newsletter_standings"], 
              $form_state["values"]["g_id"]);
    
	//squadre del girone
	//cancello elementi già inseriti
	$sql = "DELETE FROM {fanta_teams_groups} WHERE g_id = '%d'";
	db_query($sql, $form_state["values"]["g_id"]);
	
	//inserisco nuovi elementi
	foreach($t_id as $form_state["values"]["teams"]) {
	  $sql = "INSERT INTO {fanta_teams_groups} (g_id, t_id, active) VALUES ('%d', '%d', 1)";
	  db_query($sql, $form_state["values"]["g_id"], $t_id);
	}
	
    drupal_set_message(check_plain("Girone creato" . $form_state["values"]["matches"]));
  }
  
}

function fantacalcio_admin_group_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function fantacalcio_admin_group_delete($form_state, $g_id = "") {

  $form['#redirect'] = 'admin/settings/fantacalcio/groups';
  
  $groups = get_groups();
  $g_name = $groups[$g_id]->name;
  
  $form['#prefix'] = 'Sicuro di volere eliminare il girone <em>' . $g_name . '</em>?';
  
  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => "delete",
  );
  
  $form['g_id'] = array(
    '#type' => 'hidden',
    '#value' => $g_id,
  );
      
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Elimina'),
    '#suffix' => l("Annulla", 'admin/settings/fantacalcio/groups/' . $g_id)
  );

  return $form;
}

function fantacalcio_admin_group_delete_submit($form, &$form_state) {
  if ($form_state["values"]["action"] == "delete") {
    //delete group
    $sql = "DELETE FROM {fanta_groups} WHERE g_id = '%d'";
    $result = db_query($sql, $form_state["values"]["g_id"]);
    
    //delete teams_groups
    $sql = "DELETE FROM {fanta_teams_groups} WHERE g_id = '%d'";
    $result = db_query($sql, $form_state["values"]["g_id"]);
    
    drupal_set_message("Girone eliminato");
  }
}
/************************/
/* end gestione gironi */
/************************/

/******************************/
/* gestione calendario gironi */
/******************************/
function fantacalcio_admin_matches_calendar($form_state, $g_id = "") {

  //$form['#redirect'] = 'admin/settings/fantacalcio/groups/' . $g_id;
  
  $groups = get_groups();
  $g_name = $groups[$g_id]->name;
  
  drupal_set_title(filter_xss($g_name . " - Crea calendario"));

  $teams = get_teams_group($g_id);

  $i = 0;
  foreach ($teams as $t_id => $team) {
    //create a partial table row containing the data from the table
    $data = array(
//      $t_id,
      $team->name,
    );
   
    //add our static "row" data into a form value
    $form['rows'][$t_id]['data']=array(
                   '#type' => 'value',
                   '#value' => $data
                   );
   
    //now create the weight form element.  
    //NOTE how we add the id into the element key
    $form['rows'][$t_id]['weight-' . $t_id]=array(
      '#type' => 'weight',
      '#default_value' => $i,
	  '#delta' => count($teams),
      //add a specific class in here - we need this later
      '#attributes' => array('class' => 'weight'),
    );
    $i++;
  }
  
  $form['turns'] = array(
	'#title' => t('Numero di turni'),
    '#type' => 'select',
    '#options' => array(1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7, 8 => 8, 9 => 9, 10 => 10),
  );
  
  $form['home_away'] = array(
	'#title' => t('Inverti squadre (andata e ritorno)'),
    '#type' => 'checkbox'
  );  
  
  $form['bonus_t1'] = array(
	'#title' => t('Bonus Squadra 1'),
    '#type' => 'textfield',
	'#size' => 3,
  );  
  
  $form['bonus_t2'] = array(
	'#title' => t('Bonus Squadra 2'),
    '#type' => 'textfield',
	'#size' => 3,
  );
  
  $form['delete_old'] = array(
	'#title' => t('Cancella vecchio calendario'),
    '#type' => 'checkbox'
  );  
  
  $form['g_id'] = array(
    '#type' => 'hidden',
    '#value' => $g_id,
  );
      
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Crea Calendario')
  );

  return $form;
}

function fantacalcio_admin_matches_calendar_submit($form, &$form_state) {

  $teams = array();
  $turns = $form_state['values']['turns'];
  $g_id = $form_state['values']['g_id'];
  
  //cancello vecchio calendario
  if($form_state['values']['delete_old'] == true) {
    $sql = "DELETE FROM {fanta_matches} WHERE g_id = '%d'";
	$result = db_query($sql, $g_id);
  }

  //ordino le squadre
  $i = 0;
  foreach ($form_state['values'] as $key => $data) {
    //we are only interested in weight elements
    if (substr($key, 0, 6) == 'weight') {
      //cunningly we have the DB id of the row in the element name
      $teams[$data] = substr($key, 7);
	  $i++;
    }
  }
  print_r($teams);
  ksort($teams);
  
  $i = 0;
  $new_teams = array();
  foreach($teams as $key => $t_id){
    $new_teams[$i] = $t_id;
	$i++;
  }
  $teams = $new_teams;
  
  print_r($teams);
  
  $teams_number = count($teams);
  
  if ($teams_number % 2 == 1) {
    $teams[] = 0;   // numero giocatori dispari? aggiungere un riposo (BYE)!
    $teams_number++;
  }
	
  $rounds = $teams_number - 1;
	
  $total_rounds = 0;

  for($k = 0; $k < $turns; $k++) {
    /* crea gli array per le due liste in casa e fuori */
    for ($i = 0; $i < $teams_number /2; $i++) 
    {
      $home[$i] = $teams[$i]; 
      $away[$i] = $teams[$teams_number - 1 - $i]; 
    }
 
    for ($i = 0; $i < $rounds; $i++) 
    {
      /* stampa le partite di questa giornata */
	  $round = $k * $rounds + $i + 1;
      echo '<BR>' . $round . 'a Giornata<BR>';
 
	  ////////////////////////////////////
      /* alterna le partite in casa e fuori */
	  if($form_state['values']['home_away'] != "") {

        if (($i % 2) == 0) 
        {
          for ($j = 0; $j < $teams_number /2 ; $j++)
          {
            echo ' '.$away[$j].' - '.$home[$j].'<BR>';
            //query insert
            $sql = "INSERT INTO {fanta_matches} (g_id, round, t1_id, t2_id, bonus_t1, bonus_t2) VALUES ('%d', '%d', '%d', '%d', '%f', '%f')";
            $result = db_query($sql, $g_id, $round, $away[$j], $home[$j], $form_state['values']['bonus_t1'], $form_state['values']['bonus_t2']);
          }
        }
        else 
        {
          for ($j = 0; $j < $teams_number /2 ; $j++) 
          {
            echo ' '.$home[$j].' - '.$away[$j].'<BR>';
            //query insert
            $sql = "INSERT INTO {fanta_matches} (g_id, round, t1_id, t2_id, bonus_t1, bonus_t2) VALUES ('%d', '%d', '%d', '%d', '%f', '%f')";
            $result = db_query($sql, $g_id, $round, $home[$j], $away[$j], $form_state['values']['bonus_t1'], $form_state['values']['bonus_t2']);
          } 
        }
	  }
      /* NON alterna le partite in casa e fuori */
	  else {

        for ($j = 0; $j < $teams_number /2 ; $j++)
        {
          echo ' '.$home[$j].' - '.$away[$j].'<BR>';
          //query insert
          $sql = "INSERT INTO {fanta_matches} (g_id, round, t1_id, t2_id, bonus_t1, bonus_t2) VALUES ('%d', '%d', '%d', '%d', '%f', '%f')";
          $result = db_query($sql, $g_id, $round, $home[$j], $away[$j], $form_state['values']['bonus_t1'], $form_state['values']['bonus_t2']);
        }
      }
	
      ///////////
 
      // Ruota in gli elementi delle liste, tenendo fisso il primo elemento
      // Salva l'elemento fisso
      $pivot = $home[0];
 
      /* sposta in avanti gli elementi di "trasferta" inserendo 
      all'inizio l'elemento casa[1] e salva l'elemento uscente in "riporto" */
      array_unshift($away, $home[1]);
      $riporto = array_pop($away);
 
      /* sposta a sinistra gli elementi di "casa" inserendo all'ultimo 
      posto l'elemento "riporto" */
      array_shift($home);
      array_push($home, $riporto);
 
      // ripristina l'elemento fisso
      $home[0] = $pivot ; 

      $total_rounds++;
    }
  }

  drupal_set_message("Calendario creato");
  drupal_set_message("Giornate inserite: " . $total_rounds);
}

function theme_fantacalcio_admin_matches_calendar($form) {
  //loop through each "row" in the table array
  foreach ($form['rows'] as $id => $row) {
    
    //we are only interested in numeric keys
    if (intval($id)) { 
      $this_row = $row['data']['#value'];

      //Add the weight field to the row
      $this_row[] = drupal_render($form['rows'][$id]['weight-' . $id]);

      //Add the row to the array of rows
      $table_rows[] = array('data' => $this_row, 'class' => 'draggable');
    }
  }

  //Make sure the header count matches the column count
  $header = array(
    "", "Squadra"
  );

  $output = theme('table', $header, $table_rows, array('id' => 'calendar-teams-table'));
  $output .= drupal_render($form);

  // Call add_tabledrag to add and setup the JS for us
  // The key thing here is the first param - the table ID
  // and the 4th param, the class of the form item which holds the weight
  drupal_add_tabledrag('calendar-teams-table', 'order', 'sibling', 'weight');      

  return $output;
}

/**********************************/
/* end gestione calendario gironi */
/**********************************/

/************************/
/*  gestione competizioni   */
/************************/

function fantacalcio_admin_competitions_list() {
  $out = "";
  
  $out .= l("Aggiungi competizione", "admin/settings/fantacalcio/competitions/add") . "<br/><br/>";
  
  $competitions = get_competitions();
  if ($competitions) {
    $header = array("Competizione", "Attiva", "Calendario", "Classfica", "Formazioni", "Newsletter");
    foreach ($competitions as $c_id => $competition) {
      $rows[] = array(l($competition->name, "admin/settings/fantacalcio/competitions/" . $c_id),
                "<img src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($competition->active) . "'>",
                "<img src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($competition->matches) . "'>",
                "<img src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($competition->standings) . "'>",
                "<img src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($competition->lineups) . "'>",
                "<img src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($competition->newsletter) . "'>",
              );
    }
    $out .= theme_table($header, $rows);
  }
  return $out;
}

function fantacalcio_admin_competition($form_state, $c_id = "") {
  
  $action = "create"; $title = "Crea competizione";
  if (!empty($c_id) && competition_exists($c_id)) {
    $competition_data = get_competition_data($c_id);
    $action = "edit";
    $title = "Modifica competizione";
  }
  else $form['#redirect'] = 'admin/settings/fantacalcio/competitions';
  
  $form['#prefix'] = '<h3>' . $title . '</h3>';
  
  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => $action,
  );
  
  $form['c_id'] = array(
    '#type' => 'hidden',
    '#value' => $c_id,
  );
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Nome'),
    '#required' => TRUE,
    '#default_value' => $competition_data['name'], 
  );
  
  $form['type'] = array(
    '#type' => 'textfield',
    '#title' => t('Tipo'),
    '#default_value' => $competition_data['type'],
  );
  
  $form['active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Attiva'),
    '#default_value' => $competition_data['active'],
  );
  
  $form['calendario'] = array(
    '#type' => 'checkbox',
    '#title' => t('Calendario'),
    '#default_value' => $competition_data['calendario'],
  );
  
  $form['classifica'] = array(
    '#type' => 'checkbox',
    '#title' => t('Classifica'),
    '#default_value' => $competition_data['classifica'],
  );
  
  $form['formazioni'] = array(
    '#type' => 'checkbox',
    '#title' => t('Formazioni'),
    '#default_value' => $competition_data['formazioni'],
  );
  
  $form['newsletter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Newsletter'),
    '#default_value' => $competition_data['newsletter'],
  );
      
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Invia')
  );

  $form['clear'] = array(
    '#type' => 'submit',
    '#value' => 'Annulla',
    '#validate' => array(
      'fantacalcio_admin_competition_clear'
    ),
    '#suffix' => l("Elimina", 'admin/settings/fantacalcio/competitions/' . $c_id . '/delete')
  );
  
  return $form;
}

function fantacalcio_admin_competition_submit($form, &$form_state) {
  
  if ($form_state["values"]["action"] == "edit") {
    //update competition
    $sql = "UPDATE {fanta_competitions} SET name = '%s', type = '%s', 
        active = '%d', calendario = '%d', 
        classifica = '%d', formazioni = '%d', newsletter = '%d' 
        WHERE c_id = '%d'";
    $result = db_query($sql, $form_state["values"]["name"], $form_state["values"]["type"],
              $form_state["values"]["active"], $form_state["values"]["calendario"], 
              $form_state["values"]["classifica"], $form_state["values"]["formazioni"], 
              $form_state["values"]["newsletter"], $form_state["values"]["c_id"]);
    
    drupal_set_message("Competizione modificata");
  }
  elseif ($form_state["values"]["action"] == "create") {
    //crea competition
    $sql = "INSERT INTO {fanta_competitions} (name, type, active, calendario, classifica, formazioni, newsletter) 
        VALUES ('%s', '%s', '%d', '%d', '%d', '%d', '%d')";
    $result = db_query($sql, $form_state["values"]["name"], $form_state["values"]["type"],
              $form_state["values"]["active"], $form_state["values"]["calendario"], 
              $form_state["values"]["classifica"], $form_state["values"]["formazioni"], $form_state["values"]["newsletter"]);
    
    drupal_set_message("Competizione creata");
  }
  
}

function fantacalcio_admin_competition_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function fantacalcio_admin_competition_delete($form_state, $c_id = "") {

  $form['#redirect'] = 'admin/settings/fantacalcio/competitions';
  
  $competitions = get_competitions();
  $c_name = $competitions[$c_id]->name;
  
  $form['#prefix'] = 'Sicuro di volere eliminare la competizione <em>' . $c_name . '</em>?
            <br>Così facendo eliminerai anche i gironi che comprende.<br>';
  
  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => "delete",
  );
  
  $form['c_id'] = array(
    '#type' => 'hidden',
    '#value' => $c_id,
  );
      
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Elimina'),
    '#suffix' => l("Annulla", 'admin/settings/fantacalcio/competitions/' . $c_id)
  );

  return $form;
}

function fantacalcio_admin_competition_delete_submit($form, &$form_state) {
  
  if ($form_state["values"]["action"] == "delete") {
    
    $sql = "SELECT g_id FROM {fanta_groups} WHERE c_id = '%d'";
    $result = db_query($sql, $form_state["values"]["c_id"]);
    while ($row = db_fetch_array($result)) {
      //delete teams_groups
      $sql = "DELETE FROM {fanta_teams_groups} WHERE g_id = '%d'";
      $result = db_query($sql, $row["g_id"]);
    }
    
    //delete group
    $sql = "DELETE FROM {fanta_competitions} WHERE c_id = '%d'";
    $result = db_query($sql, $form_state["values"]["c_id"]);
        
    //delete groups
    $sql = "DELETE FROM {fanta_groups} WHERE c_id = '%d'";
    $result = db_query($sql, $form_state["values"]["c_id"]);
    
    drupal_set_message("Competizione eliminata");
  }
}

/************************/
/* end gestione competizioni */
/************************/

/************************/
/* gestione giornate competizioni */
/************************/

function fantacalcio_admin_competition_rounds($c_id = "") {

  drupal_add_css('sites/all/plugins/interface/interface.css', 'themes', 'all', TRUE);
  drupal_add_js('sites/all/plugins/interface/interface.js');
  drupal_add_js('sites/all/plugins/interface/compressed/idrag.js');
  drupal_add_js('sites/all/plugins/interface/compressed/idrop.js');
  drupal_add_js('sites/all/plugins/interface/compressed/isortables.js');
  drupal_add_js('sites/all/modules/fantacalcio/js/move_competition_rounds.js');
  
  $competitions = get_competitions();
  $c_name = $competitions[$c_id]->name;
  $round_statuses = get_round_statuses();
  
  drupal_set_title(filter_xss($c_name . " - Gestione giornate"));

  $competition_rounds = get_competition_rounds($c_id);

  $out = "<div id=message style='height:30px'></div><div class=clear></div>";

  $out .= "<div id='teams'>
           <div id='0' class='groupWrapper' style='width: 100%; min-height: 200px'>";

  foreach ($competition_rounds as $rc_id => $round) {
    $items[$round->round] = "\n<div class='groupItem' id='" . $c_id . "_" . $round->competition_round . "' name='" . $round->competition_round . "'>"    
            . "\n<div class='itemHeader' id='" . $round->competition_round . "-item'>"
            . (!empty($round->round_label) ? $round->round_label : $round->competition_round . "&ordf; giornata" )
            . "</div>"
            . "\n</div>";
    if ($round->round == 0) 
      $out .= $items[$round->round];
      
  }
  $out .= "\n</div></div>";

  $header = array("N°", "Data", "Stato", "Giornata competizione");
  $rounds = get_vote_rounds();
  foreach($rounds as $number => $round) {
    $rows[$number][] = array("data" => $number, "width" => 40);
    $rows[$number][] = array("data" => date("d-m-Y H:i", $round->date), "width" => 120);
    $rows[$number][] = array("data" => $round_statuses[$round->status], "width" => 90);
    $rows[$number][] = array("data" => "<div id='" . $number . "' class='groupWrapper' style='width: 100%; height:30px; min-height:30px;'>" . $items[$number] . "</div>");
  }

  $out .= "<div id=data>" . theme_table($header, $rows) . "</div>";

  return $out;
}

function fantacalcio_update_competition_round($c_id, $competition_round, $vote_round) {
  $sql = "DELETE FROM {fanta_rounds_competitions} WHERE c_id = '%d' AND competition_round = '%d'";
  $result = db_query($sql, $c_id, $competition_round);

  $sql = "INSERT INTO {fanta_rounds_competitions} (c_id, competition_round, round) VALUES ('%d', '%d', '%d')";
  $result = db_query($sql, $c_id, $competition_round, $vote_round);

  print "<img src='" . base_path() . drupal_get_path('module', 'fantacalcio') . "/images/ok.png'>" . t("Modifica effettuata");
}

/************************/
/* end gestione giornate competizioni */
/************************/

/************************/
/* gestione partite    */
/************************/

function _fantacalcio_admin_matches($g_id = '', $round = '') {
  return fantacalcio_admin_matches($g_id, $round);
}

function fantacalcio_admin_matches($g_id = "", $round = "", $add = FALSE) {
  $groups = get_groups();
  if (group_exists($g_id)) {
    if (fantacalcio_round_exists($round, $g_id)) {
      drupal_set_title(filter_xss("Partite: " . $groups[$g_id]->name . " - " . $round . "&ordf; giornata"));
      
      if ($add) return drupal_get_form("fantacalcio_add_matches_form", $g_id, $round) . fantacalcio_get_back("admin/settings/fantacalcio/matches", $g_id, $round);
      else return drupal_get_form("fantacalcio_update_matches_form", $g_id, $round) . fantacalcio_get_back("admin/settings/fantacalcio/matches", $g_id, $round);
    }
    else {
      if (rounds_count($g_id))
        return drupal_get_form('fantacalcio_choose_round', $g_id) . fantacalcio_get_back("league/partite", $g_id);
      else {
        drupal_set_message("Devi prima creare una giornata per il girone");
        drupal_goto("admin/settings/fantacalcio/rounds/add");
      }
    }
  }
  else {
    if (groups_count())
      return drupal_get_form('fantacalcio_choose_group');
    else {
      drupal_set_message("Devi prima creare un girone");
      drupal_goto("admin/settings/fantacalcio/groups/add");
    }
  }
}

function fantacalcio_choose_group($form_state) {

  $form['group'] = array(
    '#type' => 'select',
    '#title' => t('Girone'),
    '#options' => fantacalcio_get_group_options(),
    '#description' => t('Il girone o serie in cui aggiungere/modificare partite'),
    );
    
  $form['submit'] = array(
              '#type' => 'submit',
              '#value' => t('Ok'),
            );

  return $form;
}

function fantacalcio_choose_group_submit($form, &$form_state) {
  if (group_exists($form_state['values']['group'])) 
    drupal_goto("admin/settings/fantacalcio/matches/" . $form_state['values']['group']);
}

function fantacalcio_choose_round($form_state, $g_id) {
  $groups = get_groups();

  $form['#prefix'] = "<h3>" . $groups[$g_id]->name . " - Scegli giornata</h3>" . l("Crea Calendario", "admin/settings/fantacalcio/matches/" . $g_id . "/calendar");

  $form['g_id'] = array(
    '#type' => 'hidden',
    '#title' => t('Rounds'),
    '#value' => $g_id,
    );
    
  $form['round'] = array(
    '#type' => 'select',
    '#title' => t('Giornata'),
    '#default_value' => get_next_competition_round(get_cid_by_gid($g_id)),
    '#options' => fantacalcio_get_round_options($g_id),
    '#description' => $description,
    );

  $form['submit'] = array(
              '#type' => 'submit',
              '#value' => t('Ok'),
            );

  return $form;
}

function fantacalcio_choose_round_submit($form, &$form_state) {
  if (fantacalcio_round_exists($form_state['values']['round'], $form_state['values']['g_id'])) 
    drupal_goto("admin/settings/fantacalcio/matches/" . $form_state['values']['g_id'] . "/" . $form_state['values']['round']);
}

function fantacalcio_get_round_options($g_id) {
  $sql = "SELECT * FROM {fanta_rounds_competitions} 
      WHERE c_id = (SELECT c_id FROM {fanta_groups} WHERE g_id = '%d')";
  $result = db_query($sql, $g_id);
  while ($row = db_fetch_object($result)) {
    $rounds[$row->competition_round] = $row->competition_round;
  }
  
  return $rounds;
}

function fantacalcio_get_group_options() {
  $competitions = get_competitions();
  $groups = get_groups();
  
  foreach ($groups as $g_id => $group) {
    $groups_list[$g_id] = $competitions[$group->c_id]->name
                . " - " . $group->name;
  }
  return $groups_list;
}

function fantacalcio_round_exists($round, $g_id) {
  $sql = "SELECT round FROM {fanta_rounds_competitions} 
      WHERE competition_round = '%d' 
      AND c_id = (SELECT c_id FROM {fanta_groups} WHERE g_id = '%d')";
  if (db_affected_rows(db_query($sql, $round, $g_id)) > 0) return TRUE;
  return FALSE;
}

function fantacalcio_update_matches_form($form_state, $g_id, $round) {
    
  $matches = get_round_matches($round, $g_id);
  $teams = get_teams();
  $teams_options = get_teams_for_select($g_id);
  
  $form['#theme'] = "fantacalcio_update_matches";
  
  $form['g_id'] = array(
    '#type' => 'hidden',
    '#value' => $g_id,
    );
    
  $form['round'] = array(
    '#type' => 'hidden',
    '#value' => $round,
  );

  foreach ($matches as $m_id => $match) {
    $form["match_label_" . $m_id] = array(
                  '#type' => 'textfield',
                  '#size' => 4,
                  '#default_value' => $match->match_label,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
  
    $form["t1_id_" . $m_id] = array(
                  '#type' => 'select',
                  '#default_value' => $match->t1_id,
                  '#options' => $teams_options,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
    
    $form["t2_id_" . $m_id] = array(
                  '#type' => 'select',
                  '#default_value' => $match->t2_id,
                  '#options' => $teams_options,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
    
    $form["t1_label_" . $m_id] = array(
                  '#type' => 'textfield',
                  '#size' => 2,
                  '#default_value' => $match->t1_label,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
                  
    $form["t2_label_" . $m_id] = array(
                  '#type' => 'textfield',
                  '#size' => 2,
                  '#default_value' => $match->t2_label,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
                  
    $form["bonus_t1_" . $m_id] = array(
                  '#type' => 'textfield',
                  '#size' => 2,
                  '#default_value' => $match->bonus_t1,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
                  
    $form["bonus_t2_" . $m_id] = array(
                  '#type' => 'textfield',
                  '#size' => 2,
                  '#default_value' => $match->bonus_t2,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
                      
    $m_ids[] = $m_id;
  }
  
  if ($m_ids)
    $form['m_ids'] = array(
      '#type' => 'hidden',
      '#value' => implode(",", $m_ids),
    );
  
  
  $form['submit'] = array(
              '#type' => 'submit',
              '#value' => t('Ok'),
            );
            
  $form['clear'] = array(
              '#type' => 'submit',
              '#value' => t('Annulla'),
              '#validate' => array('fantacalcio_update_matches_form_clear'),
            );
            
  return $form;
}

function fantacalcio_update_matches_form_submit($form, &$form_state) {

  $m_ids = explode(",", $form_state['values']['m_ids']);
  
  foreach ($m_ids as $m_id) {
    
    $sql = "UPDATE {fanta_matches} 
        SET t1_id = '" . $form_state['values']["t1_id_" . $m_id] . "', 
        t2_id = '" . $form_state['values']["t2_id_" . $m_id] . "', 
        t1_label = '" . $form_state['values']["t1_label_" . $m_id] . "', 
        t2_label = '" . $form_state['values']["t2_label_" . $m_id] . "', 
        bonus_t1 = '" . $form_state['values']["bonus_t1_" . $m_id] . "', 
        bonus_t2 = '" . $form_state['values']["bonus_t2_" . $m_id] . "', 
        match_label = '" . $form_state['values']["match_label_" . $m_id] . "'
        WHERE m_id = " . $m_id;
      
    db_query($sql);
    
  }

  drupal_set_message("Partite aggiornate");
}

function fantacalcio_update_matches_form_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function fantacalcio_add_matches_form($form_state, $g_id, $round) {
    
  #$matches = get_round_matches($round, $g_id);
  $teams = get_teams();
  $teams_options = get_teams_for_select($g_id);
  
  $form['#theme'] = "fantacalcio_add_matches";
  $form['#redirect'] = "admin/settings/fantacalcio/matches/" . $g_id . "/" . $round;
  
  $form['g_id'] = array(
    '#type' => 'hidden',
    '#value' => $g_id,
    );
    
  $form['round'] = array(
    '#type' => 'hidden',
    '#value' => $round,
  );

  for ($i = 0; $i < 5; $i++) {
    $form["match_label_" . $i] = array(
                  '#type' => 'textfield',
                  '#size' => 6,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
  
    $form["t1_id_" . $i] = array(
                  '#type' => 'select',
                  '#options' => $teams_options,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
    
    $form["t2_id_" . $i] = array(
                  '#type' => 'select',
                  '#options' => $teams_options,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
    
    $form["t1_label_" . $i] = array(
                  '#type' => 'textfield',
                  '#size' => 4,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
                  
    $form["t2_label_" . $i] = array(
                  '#type' => 'textfield',
                  '#size' => 4,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
                  
    
    $form["bonus_t1_" . $i] = array(
                  '#type' => 'textfield',
                  '#size' => 2,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
                  
    $form["bonus_t2_" . $i] = array(
                  '#type' => 'textfield',
                  '#size' => 2,
                  '#prefix' => '<span style="float:left; width:40px;">',
                  '#suffix' => '</span>',
                  );
  }
  
  $form['submit'] = array(
              '#type' => 'submit',
              '#value' => t('Ok'),
            );
            
  $form['clear'] = array(
              '#type' => 'submit',
              '#value' => t('Annulla'),
              '#validate' => array('fantacalcio_add_matches_form_clear'),
            );
            
  return $form;
}

function fantacalcio_add_matches_form_submit($form, &$form_state) {

  $count = 0;
  
  for ($i = 0; $i < 5; $i++) {
  
    if ( ($form_state['values']["t1_id_" . $i] > 0 || !empty($form_state['values']["t1_label_" . $i])) 
      && ($form_state['values']["t2_id_" . $i] > 0 || !empty($form_state['values']["t2_label_" . $i])) 
      && ($form_state['values']["t1_id_" . $i] != $form_state['values']["t2_id_" . $i] 
      || ($form_state['values']["t1_id_" . $i] == 0 && $form_state['values']["t2_id_" . $i] == 0)) ) {
      
    
      $sql = "INSERT INTO {fanta_matches} (t1_id, t2_id, t1_label, t2_label, bonus_t1, bonus_t2, match_label, g_id, round) VALUES (
          " . $form_state['values']["t1_id_" . $i] . ", 
          " . $form_state['values']["t2_id_" . $i] . ", 
          '" . $form_state['values']["t1_label_" . $i] . "', 
          '" . $form_state['values']["t2_label_" . $i] . "', 
          '" . $form_state['values']["bonus_t1_" . $i] . "', 
          '" . $form_state['values']["bonus_t2_" . $i] . "', 
          '" . $form_state['values']["match_label_" . $i] . "', 
          " . $form_state['values']["g_id"] . ", 
          " . $form_state['values']["round"] . ")";
        
      db_query($sql);
      
      $count++;
    }
  }

  if ($count > 0) {
    $message = ($count > 1) ? $count . " partite inserite" : $count . " partita inserita";
    drupal_set_message(check_plain($message));
  }
  else drupal_set_message("Inserire 2 squadre o 2 label per aggiungere una partita");
}

function fantacalcio_add_matches_form_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function fantacalcio_admin_match_delete($form_state, $m_id = "") {

  $match = get_match_data($m_id);
  $teams = get_teams();
  $groups = get_groups();
  
  if (!$match) {
    drupal_set_message("Partita inesistente");
    drupal_goto('admin/settings/fantacalcio/matches');
  }

  $form['#redirect'] = 'admin/settings/fantacalcio/matches/' . $match['g_id'] . "/" . $match['round'];
    
  $form['#prefix'] = 'Sicuro di volere eliminare la partita <em>'
            . (($match['t1_id'] == 0) ? $match['t1_label'] : $teams[$match['t1_id']]->name)
    . ' - '. (($match['t2_id'] == 0) ? $match['t2_label'] : $teams[$match['t2_id']]->name)
            . " (" . $groups[$match['g_id']]->name . " - " . $match['round'] . '&ordf; giornata)</em> ?';
  
  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => "delete",
  );
  
  $form['m_id'] = array(
    '#type' => 'hidden',
    '#value' => $m_id,
  );
      
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Elimina'),
    '#suffix' => l("Annulla", 'admin/settings/fantacalcio/matches/' . $match['g_id'] . "/" . $match['round'])
  );
  
  return $form;
}

function fantacalcio_admin_match_delete_submit($form, &$form_state) {
  
  if ($form_state["values"]["action"] == "delete") {
    //delete match
    $sql = "DELETE FROM {fanta_matches} WHERE m_id = '%d'";
    $result = db_query($sql, $form_state["values"]["m_id"]);
    
    drupal_set_message("Partita eliminata");
  }
}


function theme_fantacalcio_update_matches($form) {
  $header = array(
          array("data" => "Match Label", "width" => "35"), 
          array("data" => "Squadra 1", "width" => "150"), 
          array("data" => "Squadra 2", "width" => "150"), 
          array("data" => "Label 1", "width" => "30"), 
          array("data" => "Label 2", "width" => "30"), 
          array("data" => "Bonus 1", "width" => "25"), 
          array("data" => "Bonus 2", "width" => "25"), 
          array("data" => "", "width" => "15"), 
          );
  
  $g_id = arg(4);
  $round = arg(5);
  
  $out = l("Aggiungi partite", "admin/settings/fantacalcio/matches/" . $g_id . "/" . $round . "/add") . "<br/><br/>";
  
  
  $matches = get_round_matches($round, $g_id);
  $teams = get_teams();
  
  if ($matches) {  
    foreach ($matches as $m_id => $match) {
      $rows[$m_id]= array(  
              drupal_render($form["match_label_" . $m_id]),
              drupal_render($form["t1_id_" . $m_id]),
              drupal_render($form["t2_id_" . $m_id]),
              drupal_render($form["t1_label_" . $m_id]),
              drupal_render($form["t2_label_" . $m_id]),
              drupal_render($form["bonus_t1_" . $m_id]),
              drupal_render($form["bonus_t2_" . $m_id]),
              l("Elimina", "admin/settings/fantacalcio/matches/" . $m_id . "/delete")
          );
    }
    
    $rows[][] = array("data" => drupal_render($form),
            "colspan" => 5
            );    
    
    return $out . theme_table($header, $rows, array("id" => "form-update-results"));
  }
  else return $out;
}

function theme_fantacalcio_add_matches($form) {
  $header = array(
          array("data" => "Match Label", "width" => "60"), 
          array("data" => "Squadra 1", "width" => "150"), 
          array("data" => "Squadra 2", "width" => "150"), 
          array("data" => "Label 1", "width" => "40"), 
          array("data" => "Label 2", "width" => "40"), 
          array("data" => "Bonus 1", "width" => "25"), 
          array("data" => "Bonus 2", "width" => "25"), 
          );
  
  $g_id = arg(4);
  $round = arg(5);
  
  $matches = get_round_matches($round, $g_id);
  $teams = get_teams();
    
  for ($i = 0; $i < 5; $i++) {
    $rows[$i]= array(  
            drupal_render($form["match_label_" . $i]),
            drupal_render($form["t1_id_" . $i]),
            drupal_render($form["t2_id_" . $i]),
            drupal_render($form["t1_label_" . $i]),
            drupal_render($form["t2_label_" . $i]),
            drupal_render($form["bonus_t1_" . $i]),
            drupal_render($form["bonus_t2_" . $i]),
        );
  }
  
  $rows[][] = array("data" => drupal_render($form),
          "colspan" => 5
          );    
  
  return theme_table($header, $rows, array("id" => "form-update-results"));
}

/************************/
/* end gestione partite */
/************************/

/************************/
/* gestione giornate  */
/************************/

function fantacalcio_admin_rounds_list() {
  $out = "";
  
  $out .= l("Aggiungi giornata", "admin/settings/fantacalcio/rounds/add") . "<br/><br/>";
  
  $rounds = get_all_rounds();
  $round_statuses = get_round_statuses();
#print_r($rounds[1]->competitions);
  $competitions = get_competitions();
  if ($rounds) {
    $header = array("Giornata", "Data", "Data chiusura", "Stato", "Competizione", "Giornata competizione", "Label", "Passaggio turno");
    foreach ($rounds as $round) {
      $competition_cell = array();
      $competitions_count = count($round->competitions);
      $is_first_row = TRUE;

      foreach ($round->competitions as $c_id => $competition_round) {
        $competition_cell[] = array($competitions[$c_id]->name,
                  $competition_round['round'],
                  $competition_round['label'],
                  "<img src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($competition_round['next']) . "'>",
                  );
            
          if ($is_first_row) {
            $rows[$round->round . "_" . $c_id][] = array("data" => l($round->round, "admin/settings/fantacalcio/rounds/" . $round->round), "rowspan" => $competitions_count);
            $rows[$round->round . "_" . $c_id][] = array("data" => date("d-m-Y H:i", $round->date), "rowspan" => $competitions_count);
            $rows[$round->round . "_" . $c_id][] = array("data" => date("d-m-Y H:i", $round->end_date), "rowspan" => $competitions_count);
            $rows[$round->round . "_" . $c_id][] = array("data" => $round_statuses[$round->status], "rowspan" => $competitions_count);
            $is_first_row = FALSE;
          }
                
          $rows[$round->round . "_" . $c_id][] = $competitions[$c_id]->name;
                $rows[$round->round . "_" . $c_id][] = $competition_round['round'];
                $rows[$round->round . "_" . $c_id][] = $competition_round['label'];
                $rows[$round->round . "_" . $c_id][] = "<img src='" .base_path() . drupal_get_path("module", "fantacalcio") . "/images/" . get_image_check($competition_round['next']) . "'>";
              
      }      
    }
    $out .= theme_table($header, $rows);
  }
  
  return $out;
}

function fantacalcio_admin_round($form_state, $round = "") {
  
  if (competitions_count()) {
    $competitions = get_competitions();

    $form['#theme'] = 'fantacalcio_admin_round';

    $action = "create"; $title = t("Crea giornata");
    if (!empty($round) && round_exists($round)) {
      $round_data = get_round_data($round);
      $action = "edit";
      $title = t("Modifica giornata");
    }
    else $form['#redirect'] = 'admin/settings/fantacalcio/rounds';
    
    $form['#prefix'] = '<h3>' . $title . '</h3>';
    
    $form['action'] = array(
      '#type' => 'hidden',
      '#value' => $action,
    );
    
    $form['round'] = array(
      '#type' => 'textfield',
      '#size' => 2,
      '#maxlength' => 2,
      '#default_value' => ($action == 'edit' ? $round : ""),
      #'#disabled' => ($action == 'edit' ? TRUE : FALSE)
    );
        
    $form['date'] = array(
      '#type' => 'date',
      '#title' => t('Data'),
      '#default_value' => array('year' => date('Y', $round_data['date']), 'month' => date('n', $round_data['date']), 'day' => date('j', $round_data['date'])),
    );
    
    $form['hour'] = array(
      '#type' => 'select',
      '#title' => t('Ore'),
      '#options' => array(0 => "00", 1 => "01", 2 => "02", 3 => "03", 4 => "04", 5 => "05", 6 => "06", 7 => "07", 
                8 => "08", 9 => "09", 10 => "10", 11 => "11", 12 => "12", 13 => "13", 14 => "14", 15 => "15", 
                16 => "16", 17 => "17", 18 => "18", 19 => "19", 20 => "20", 21 => "21", 22 => "22", 23 => "23"), 
      '#default_value' => date('H', $round_data['date']),
    );
    
    $form['minute'] = array(
      '#type' => 'select',
      '#title' => t('Minuti'),
      '#options' => array(0 => "00", 5 => "05", 10 => "10", 15 => "15", 20 => "20", 25 => "25", 30 => "30", 35 => "35", 
                40 => "40", 45 => "45", 50 => "50", 55 => "55"), 
      '#default_value' => date('i', $round_data['date']),
    );

    $form['end_date'] = array(
      '#type' => 'date',
      '#title' => t('Data chiusura'),
      '#default_value' => array('year' => date('Y', $round_data['end_date']), 'month' => date('n', $round_data['end_date']), 'day' => date('j', $round_data['end_date'])),
    );
    
    $form['end_hour'] = array(
      '#type' => 'select',
      '#title' => t('Ore'),
      '#options' => array(0 => "00", 1 => "01", 2 => "02", 3 => "03", 4 => "04", 5 => "05", 6 => "06", 7 => "07", 
                8 => "08", 9 => "09", 10 => "10", 11 => "11", 12 => "12", 13 => "13", 14 => "14", 15 => "15", 
                16 => "16", 17 => "17", 18 => "18", 19 => "19", 20 => "20", 21 => "21", 22 => "22", 23 => "23"), 
      '#default_value' => date('H', $round_data['end_date']),
    );
    
    $form['end_minute'] = array(
      '#type' => 'select',
      '#title' => t('Minuti'),
      '#options' => array(0 => "00", 5 => "05", 10 => "10", 15 => "15", 20 => "20", 25 => "25", 30 => "30", 35 => "35", 
                40 => "40", 45 => "45", 50 => "50", 55 => "55"), 
      '#default_value' => date('i', $round_data['end_date']),
    );

    $form['status'] = array(
      '#type' => 'select',
      '#default_value' => $round_data['status'],
      '#options' => get_round_statuses()
    );

    //per ogni competizione
    foreach ($competitions as $c_id => $competition) {
      $form['active_' . $c_id] = array(
        '#type' => 'checkbox',
        '#default_value' => isset($round_data['competitions'][$c_id]), 
      );
      
      $form['competition_round_' . $c_id] = array(
        '#type' => 'textfield',
        '#size' => 4,
        '#default_value' => $round_data['competitions'][$c_id]['competition_round'],
      );
      
      $form['label_' . $c_id] = array(
        '#type' => 'textfield',
        '#size' => 20,
        '#default_value' => $round_data['competitions'][$c_id]['label'],
      );
        
      $form['next_' . $c_id] = array(
        '#type' => 'checkbox',
        '#default_value' => $round_data['competitions'][$c_id]['next'],
      );
    
    }  
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Invia')
    );

    $form['clear'] = array(
      '#type' => 'submit',
      '#value' => 'Annulla',
      '#validate' => array(
        'fantacalcio_admin_round_clear'
      ),
      '#suffix' => l("Elimina", 'admin/settings/fantacalcio/rounds/' . $round . '/delete')
    );
    
    return $form;
  }
  else {
    drupal_set_message("Devi prima creare una competizione");
    drupal_goto("admin/settings/fantacalcio/competitions/add");
  }
}

function theme_fantacalcio_admin_round($form) {
  $header = array("", "Competizione", "Giornata", "Label", "Passaggio turno");
  
  $round = arg(4);
    
  $competitions = get_competitions();

  $out .= drupal_render($form["action"]);

  $rows[] = array("Giornata:", drupal_render($form["round"]), "", "");
  $rows[] = array("Data inizio:", drupal_render($form["date"]), drupal_render($form["hour"]), drupal_render($form["minute"]));
  $rows[] = array("Data chiusura:", drupal_render($form["end_date"]), drupal_render($form["end_hour"]), drupal_render($form["end_minute"]));
  $rows[] = array("Stato:", drupal_render($form["status"]), "", "");

  $out .= theme_table(array(), $rows);
  
  if ($competitions) {  
    foreach ($competitions as $c_id => $competition) {
      $rows[$c_id]= array(  
                drupal_render($form["active_" . $c_id]),
                $competition->name, 
                drupal_render($form["competition_round_" . $c_id]),
                drupal_render($form["label_" . $c_id]),              
                drupal_render($form["next_" . $c_id]),              
          );
    }
        
    return $out . theme_table($header, $rows) . drupal_render($form);
  }
  else 
      return $out;
}

function fantacalcio_admin_round_submit($form, &$form_state) {

  $competitions = get_competitions();

  $ts_start = mktime($form_state["values"]["hour"], $form_state["values"]["minute"], 0, 
          $form_state["values"]["date"]['month'], $form_state["values"]["date"]['day'], $form_state["values"]["date"]['year']);

  $ts_end = mktime($form_state["values"]["end_hour"], $form_state["values"]["end_minute"], 0, 
          $form_state["values"]["end_date"]['month'], $form_state["values"]["end_date"]['day'], $form_state["values"]["end_date"]['year']);

  if ($form_state["values"]["action"] == "edit") {
    //update round
    $sql = "UPDATE {fanta_rounds} SET date = '%d', end_date = '%d', status = '%d' 
        WHERE round = '%d'";
    $result = db_query($sql, $ts_start, $ts_end, $form_state["values"]["status"], $form_state["values"]["round"]);

    //resetto i dati su fanta_rounds_competitions
    $sql = "DELETE FROM {fanta_rounds_competitions} WHERE round = '%d'";
    $result = db_query($sql, $form_state["values"]["round"]);

    foreach ($competitions as $c_id => $competition) {
          //crea round_competition
          if ($form_state["values"]["active_" . $c_id]) {
            $sql = "INSERT INTO {fanta_rounds_competitions} (c_id, round, competition_round, round_label, next) 
                VALUES ('%d', '%d', '%d', '%s', '%d')";
            $result = db_query($sql, $c_id, $form_state["values"]["round"],
                    $form_state["values"]["competition_round_" . $c_id], 
                    $form_state["values"]["label_" . $c_id], $form_state["values"]["next_" . $c_id]);
          }
    }
    
    drupal_set_message("Giornata modificata");

  }
  elseif ($form_state["values"]["action"] == "create") {
    //replace round
    $sql = "DELETE FROM {fanta_rounds} WHERE round = '%d'";
    $result = db_query($sql, $form_state["values"]["round"]);
    
    $sql = "INSERT INTO {fanta_rounds} (round, date, end_date, status) 
        VALUES ('%d', '%d', '%d', '%d')";
    $result = db_query($sql, $form_state["values"]["round"], $ts_start, $ts_end, $form_state["values"]["status"]);  

    foreach ($competitions as $c_id => $competition) {
          //crea round_competition
          if ($form_state["values"]["active_" . $c_id]) {
            $sql = "INSERT INTO {fanta_rounds_competitions} (c_id, round, competition_round, round_label, next) 
                VALUES ('%d', '%d', '%d', '%s', '%d')";
            $result = db_query($sql, $c_id, $form_state["values"]["round"],
                    $form_state["values"]["competition_round_" . $c_id], 
                    $form_state["values"]["label_" . $c_id], $form_state["values"]["next_" . $c_id]);
          }
    }

    drupal_set_message("Giornata creata");
  }
}

function fantacalcio_admin_round_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function fantacalcio_admin_round_delete($form_state, $round = "") {

  $form['#redirect'] = 'admin/settings/fantacalcio/rounds';
  
  $rounds = get_rounds();
  $competitions = get_competitions();
  $c_name = $competitions[$rounds[$rc_id]->c_id]->name;
  
  $form['#prefix'] = 'Sicuro di volere eliminare la giornata n°:<em>' . $round . '</em> ?';
  
  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => "delete",
  );
  
  $form['round'] = array(
    '#type' => 'hidden',
    '#value' => $round,
  );
      
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Elimina'),
    '#suffix' => l("Annulla", 'admin/settings/fantacalcio/rounds/' . $round)
  );

  return $form;
}

function fantacalcio_admin_round_delete_submit($form, &$form_state) {
  if ($form_state["values"]["action"] == "delete") {
    //delete round
    $sql = "DELETE FROM {fanta_rounds} WHERE round = '%d'";
    $result = db_query($sql, $form_state["values"]["round"]);

    //delete round_competition
    $sql = "DELETE FROM {fanta_rounds_competitions} WHERE round = '%d'";
    $result = db_query($sql, $form_state["values"]["round"]);
    
    drupal_set_message("Giornata eliminata");
  }
}
/************************/
/* end gestione giornate*/
/************************/

/****************************/
/* gestione giocatori    */
/****************************/

function fantacalcio_admin_players_list() {
  $out = l("Aggiorna lista", "admin/settings/fantacalcio/players/list") . "<br/>";
  
  $players = get_all_players();

  if ($players) {
  
    $header = array("Id", "Nome", "Ruolo"); 
  
    foreach ($players as $pl_id => $player) {
      $names[] = $player->name;
      $roles[] = $player->role;
    }
  
    array_multisort($names, SORT_ASC, $roles, SORT_ASC, $players);
  
    foreach ($players as $pl_id => $player) {
      $rows[] = array("data" => array(  $player->pl_id, l($player->name, "admin/settings/fantacalcio/players/" . $player->pl_id), 
                    convert_role($player->role) ));
    }
  
    $out .= theme_table($header, $rows);
  }
  return $out;
}

function fantacalcio_admin_players_list_player($pl_id = "") {
  if ($pl_id) {
    $player = get_player_by_id($pl_id);
    if ($player) {
      return drupal_get_form("fantacalcio_admin_players_list_player_form", $player);
    }
    else drupal_goto("admin/settings/fantacalcio/players");
  }
  else drupal_goto("admin/settings/fantacalcio/players");
}

function fantacalcio_admin_players_list_player_form($form_state, $player) {

  $player_rounds = get_player_rounds($player->pl_id);
  $real_teams_list = get_real_teams_options();

  $form['#redirect'] = 'admin/settings/fantacalcio/players';
  $form['#theme'] = "fantacalcio_admin_players_list_player";
  
  $form['pl_id'] = array(
    '#type' => 'hidden',
    '#length' => 30,
    '#default_value' => $player->pl_id,
  );
  
  $form['round'] = array(
    '#type' => 'hidden',
    '#length' => 30,
    '#default_value' => $player->round,
  );
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#length' => 30,
    '#default_value' => $player->name,
    '#title' => t('Nome'),
  );
    
  $form['role'] = array(
    '#type' => 'select',
    '#title' => t('Ruolo'),
    '#default_value' => $player->role,
    '#options' => array("P", "D", "C", "A"),
  );

  //lista per ogni giornata

  foreach ($player_rounds as $round => $player) {
    $form['team_' . $round] = array(
      '#type' => 'select',
      '#length' => 30,
      '#default_value' => $player->rt_id,
      '#options' => $real_teams_list
    );

    $form['active_' . $round] = array(
      '#type' => 'checkbox',
      '#default_value' => $player->active,
    );
  }

  //
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Carica')
  );

  $form['clear'] = array(
    '#type' => 'submit',
    '#value' => 'Annulla',
    '#validate' => array('upload_form_clear'),
  );
  
  return $form;
}

function theme_fantacalcio_admin_players_list_player($form) {
  $header = array(
          array("data" => "Giornata"), 
          array("data" => "Squadra"), 
          array("data" => "Attivo")
          );
  
  $pl_id = arg(4);
    
  $player_rounds = get_player_rounds($pl_id);

  $out = drupal_render($form["pl_id"]);
  $out .= drupal_render($form["round"]);
  $out .= drupal_render($form["name"]);
  $out .= drupal_render($form["role"]);
  
  if ($player_rounds) {  
    foreach ($player_rounds as $round => $player) {
      $rows[$round]= array(  
              $round, 
              drupal_render($form["team_" . $round]),
              drupal_render($form["active_" . $round]),              
          );
    }
    
    $rows[][] = array("data" => drupal_render($form),
            "colspan" => 5
            );    
    
    return $out . theme_table($header, $rows);
  }
  else 
      return $out;
}


function fantacalcio_admin_players_list_player_form_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function fantacalcio_admin_players_list_player_form_submit($form, &$form_state) {

  $name = $form_state["values"]["name"];

  //aggiorna dati giocatore
  $sql = "UPDATE {fanta_players} SET name = '" . $form_state["values"]["name"] . "', 
      role = " . $form_state["values"]["role"] . "
      WHERE pl_id = " . $form_state["values"]["pl_id"];
  $result = db_query($sql);
  
  $player_rounds = get_player_rounds($form_state["values"]["pl_id"]);
  foreach ($player_rounds as $round => $player) {
    $sql = "UPDATE {fanta_players_teams} 
        SET team = '%s',
            active = '%d'
        WHERE pl_id = '%d' 
        AND round = '%d'";
    $result = db_query($sql, $form_state["values"]["team_" . $round], $form_state["values"]["active_" . $round], $form_state["values"]["pl_id"], $round);
  }

  drupal_set_message('Giocatore aggiornato');
  
}

function fantacalcio_admin_players_list_update($form_state) {
  $form['#redirect'] = 'admin/settings/fantacalcio/players';
  
  $form['text'] = array(
    '#type' => 'textarea',
    '#rows' => 20,
    '#cols' => 80,
    '#title' => t('Testo copiato'),
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Carica'
  ));

  $form['clear'] = array(
    '#type' => 'submit',
    '#value' => 'Annulla',
    '#validate' => array(
      'upload_form_clear'
    ),

  );
  return $form;
}



function fantacalcio_admin_players_list_update_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function fantacalcio_admin_players_list_update_submit($form, &$form_state) {

  $text = $form_state["values"]["text"];

  $sql = "select max(round) from {fanta_votes};";
  $result = db_query($sql);
  $round = db_result($result) ;
  if (!$round) $round = 0;

  insert_players($text, $round + 1);
  
}

function insert_players($text, $round) {

  if ($text != "" && $round >= 0) {
    
    //roles
    $roles = array("P" => 0, "D" => 1, "C" => 2, "A" => 3);

    //real teams
    $real_teams = get_real_teams_long_name();

    $query_line = "";
    $new_players = 0;
    $not_active_players = 0;
  
    //formatto il testo in righe
    $lines = explode("\n", $text);
    
    $pl_ids = array();
    foreach ($lines as $line) {
      $line = trim($line);
      if (is_numeric(substr($line, 0, 1))) {
        //separo i campi della riga con il carattere \t "tab"
        $elems = explode(chr(9), $line); //0:pl_id  1:role  2:name  3:team
        
        $player = array("pl_id" => $elems[0], "name" => $elems[2], "team" => $real_teams[strtolower($elems[3])], "role" => $roles[$elems[1]]);
        
        $players[$elems[0]] = $player;

        $query_line .= '(' . $player['pl_id'] . ', ' . $round . ', ' . $player['team'] . ', 1), ';

        array_push($pl_ids, $player['pl_id']);
      }
    }

    $sql = "DELETE FROM {fanta_players_teams} 
        WHERE round = '%d' AND pl_id IN (" . implode(",", $pl_ids) .")";
    $result = db_query($sql, $round);

    //inserisco i nuovi giocatori in fanta_players_teams
    if ($query_line) {
      $query_line = substr($query_line, 0, -2);
      $sql = "INSERT INTO {fanta_players_teams} (pl_id, round, rt_id, active) VALUES " . $query_line;
      $result = db_query($sql);
    }
    
    //giocatori nuovi
    $query_line = "";
    $sql = "SELECT pl_id 
        FROM {fanta_players_teams} 
        WHERE pl_id NOT IN (SELECT pl_id FROM {fanta_players})";
    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
      $pl_id = $row->pl_id;
      if ($players[$pl_id])
        $query_line .= '(' . $pl_id . ', "' . $players[$pl_id]['name'] . '", ' . $players[$pl_id]['role'] . '), ';
      $new_players++;
    }
    if ($query_line) {
      $query_line = substr($query_line, 0, -2);
      $sql = "INSERT INTO {fanta_players} (pl_id, name, role) VALUES " . $query_line;
      $result = db_query($sql);
    }
        
    drupal_set_message(check_plain("Giocatori aggiornati: " . count($players)));
    drupal_set_message(check_plain("Giocatori nuovi: " . $new_players));    
    
  }
  
}

function fantacalcio_admin_players_add() {
  return drupal_get_form("fantacalcio_admin_players_add_form");
}

function fantacalcio_admin_players_add_form($form_state) {
  $real_teams_list = get_real_teams_options();
  
  $form['#redirect'] = 'admin/settings/fantacalcio/players';
    
  $form['pl_id'] = array(
    '#type' => 'textfield',
    '#length' => 30,
    '#title' => t('Codice'),
  );	
	
  $form['name'] = array(
    '#type' => 'textfield',
    '#length' => 30,
    '#title' => t('Nome'),
  );
    
  $form['role'] = array(
    '#type' => 'select',
    '#title' => t('Ruolo'),
    '#default_value' => $player->role,
    '#options' => array("P", "D", "C", "A"),
  );
  
  $form['rt_id'] = array(
    '#type' => 'select',
    '#length' => 30,
	'#title' => t('Squadra'),
    '#options' => $real_teams_list
  );
  
  $form['round'] = array(
    '#type' => 'select',
    '#length' => 30,
    '#title' => t('Giornata'),
	'#options' => get_all_rounds_played(),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Carica')
  );

  $form['clear'] = array(
    '#type' => 'submit',
    '#value' => 'Annulla',
    '#validate' => array('fantacalcio_admin_players_add_form_clear'),
  );
  
  return $form;
}

function fantacalcio_admin_players_add_form_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function fantacalcio_admin_players_add_form_submit($form, &$form_state) {

  $name = $form_state["values"]["name"];

  //inserisce dati giocatore
  $sql = "INSERT INTO {fanta_players} (pl_id, name, role) VALUES ('%d', '%s', '%d')";
  $result = db_query($sql, $form_state["values"]["pl_id"], $form_state["values"]["name"], $form_state["values"]["role"]);
  
  $sql = "INSERT INTO {fanta_players_teams} (pl_id, round, rt_id, active) VALUES ('%d', '%d', '%d', 1)";
  $result = db_query($sql, $form_state["values"]["pl_id"], $form_state["values"]["round"], $form_state["values"]["rt_id"]);
  
  drupal_set_message('Giocatore inserito');
  
}

/****************************/
/* end gestione giocatori  */
/****************************/


function fantacalcio_get_back($url, $arg_1, $arg_2 = "") {
  
  if (!empty($arg_2)) $url .= "/" . $arg_1;
  
  return "<br/><p>" . l("&laquo; Indietro", $url, array("html" => TRUE)) . "</p>";
}

function get_groups_checkboxes() {
  $groups = get_groups();
  $competitions = get_competitions();
  
  foreach ($groups as $g_id => $group) {
    $group_checkboxes[$g_id] = $competitions[$group->c_id]->name . " - " . $group->name;
  }
  return $group_checkboxes;
}

function get_groups_checkboxes_default($t_id) {
  $sql = "SELECT g_id FROM {fanta_teams_groups} WHERE t_id = '%d' AND active = 1";
  $result = db_query($sql, $t_id);
  while ($row = db_fetch_array($result)) {
    $g_ids[] = $row['g_id'];
  }
  return $g_ids;
}

function get_competitions_for_select() {
  $sql = "SELECT c_id, name FROM {fanta_competitions} WHERE active = 1";
  $result = db_query($sql);
  while ($row = db_fetch_array($result)) {
    $options[$row['c_id']] = $row['name'];
  }
  return $options;
}

function get_teams_for_select($g_id) {
  $teams_options[0] = " - ";
  $sql = "SELECT t.t_id, t.name 
      FROM {fanta_teams} t, {fanta_teams_groups} g 
      WHERE t.t_id = g.t_id
      AND t.active = 1
      AND g.g_id = '%d' 
      ORDER BY t.name";
  $result = db_query($sql, $g_id);
  while ($row = db_fetch_object($result)) {
      $teams_options[$row->t_id] = $row->name;
  }
  
  return $teams_options;
}

function get_providers_for_select() {
  $sql = "SELECT p_id, name 
      FROM {fanta_vote_providers}
      WHERE active = 1
      ORDER BY name";
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
      $providers_options[$row->p_id] = $row->name;
  }
  
  return $providers_options;
}

function competitions_count() {
  $competitions = get_competitions();
  if (count($competitions) > 0 ) return TRUE;
  return FALSE;
}

function groups_count() {
  $groups = get_groups();
  if (count($groups) > 0 ) return TRUE;
  return FALSE;
}

function rounds_count($g_id) {
  $c_id = get_cid_by_gid($g_id);
  $rounds = get_rounds($c_id);
  if (count($rounds) > 0 ) return TRUE;
  return FALSE;
}



