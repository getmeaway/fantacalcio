<?php
/**
  * @file fantacalcio.team.inc
*/

function fantacalcio_team_form($form_state) {

  $team = get_team_data();
  $action = ($team) ? 'update' : 'insert';
  $title = ($team) ? 'Modifica squadra - ' . $team['name'] : 'Crea squadra';
  drupal_set_title(filter_xss($title));

  $form['title'] = array(
    '#title' => check_plain($title)
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Nome squadra',
    '#default_value' => check_plain($team['name']),
    '#required' => TRUE,

  );
  
  $form['coach'] = array(
    '#type' => 'textfield',
    '#title' => 'Allenatore',
    '#default_value' => check_plain($team['coach']),

  );

  $form['stadium'] = array(
    '#type' => 'textfield',
    '#title' => 'Stadio',
    '#default_value' => check_plain($team['stadium']),

  );

  $form['shirt'] = array(
    '#type' => 'fieldset',
    '#title' => 'Divisa',
    '#ahah' => array(
      'wrapper' => 'prova',
        // default so not really needed, but included for clarity
      'method' => 'replace',
      'event' => 'change',
      'path' => 'js/divisa.js',

    ),

  );

  $form['shirt']['type'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => 'solo bordi',
      1 => 'righe verticali',
      2 => 'righe orizzontali',
      3 => 'banda diagonale'
    ),
    '#title' => 'Tipo',
    '#default_value' => check_plain(substr($team['shirt'], -9, 1)), '#ahah' => array(
    'wrapper' => 'shirt_preview',
      // default so not really needed, but included for clarity
    'method' => 'replace',
    'event' => 'change',
    'path' => 'team/js',

  ));

  $form['shirt']['color_1'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => 'Bianco',
      1 => 'Nero',
      2 => 'Rosso',
      3 => 'Blu',
      4 => 'Verde',
      5 => 'Giallo'
    ),
    '#title' => 'Colore 1',
    '#default_value' => check_plain(substr($team['shirt'], -7, 1)), '#ahah' => array(
    'wrapper' => 'shirt_preview',
      // default so not really needed, but included for clarity
    'method' => 'replace',
    'event' => 'change',
    'path' => 'team/js',

  ));

  $form['shirt']['color_2'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => 'Bianco',
      1 => 'Nero',
      2 => 'Rosso',
      3 => 'Blu',
      4 => 'Verde',
      5 => 'Giallo'
    ),
    '#title' => 'Colore 2',
    '#default_value' => check_plain(substr($team['shirt'], -5, 1)), '#ahah' => array(
    'wrapper' => 'shirt_preview',
      // default so not really needed, but included for clarity
    'method' => 'replace',
    'event' => 'change',
    'path' => 'team/js',

  ));

  $form['shirt']['preview'] = array(
    '#prefix' => "<div id='shirt_preview' style='float:left;'>",
    '#suffix' => "</div>",
    '#type' => 'markup',
    '#value' => "<img src='" . base_path() . drupal_get_path("module", "fantacalcio") . "/images/shirts/" . $team['shirt'] . "'>"
  );

  $form['t_id'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($team['t_id']),

  );

  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($action),

  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Ok'
  );

  $form['clear'] = array(
    '#type' => 'submit',
    '#value' => 'Annulla',
    '#validate' => array(
      'fantacalcio_squadra_form_clear'
    ),

  );

  return $form;
}

function fantacalcio_team_form_ahah() {
  $type = $_POST['type'];
  $color_1 = $_POST['color_1'];
  $color_2 = $_POST['color_2'];

  $output = "<img src='" . base_path() . drupal_get_path('module', 'fantacalcio') . "/images/shirts/shirt_" . $type . "_" . $color_1 . "_" . $color_2 . ".png' >";
  print drupal_to_js(array(
    'data' => $output,
    'status' => true
  ));
  exit();
}

function fantacalcio_team_manager() {
  return drupal_get_form('fantacalcio_team_form');
}

function fantacalcio_team_form_clear($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function fantacalcio_team_form_validate($form, &$form_state) {
  $name = $form_state['values']['name'];
  if (!$name)
    form_set_error('name', 'Inserisci un nome per la squadra');
}

function fantacalcio_team_form_submit($form, &$form_state) {

  global $user;

  $team_name = $form_state['values']['name'];
  $team_coach = $form_state['values']['coach'];
  $team_stadium = $form_state['values']['stadium'];
  $team_shirt = "shirt_" . $form_state['values']['type'] . "_" . $form_state['values']['color_1'] . "_" . $form_state['values']['color_2'] . ".png";
  $t_id = $form_state['values']['t_id'];
  $action = $form_state['values']['action'];

  if ($action == 'update') {
    $sql = "UPDATE {fanta_teams} " .
    "SET name = '%s', " .
    "coach = '%s', " .
    "stadium = '%s', " .
    "shirt = '%s' " .
    "WHERE t_id = '%d'";
    $result = db_query($sql, $team_name, $team_coach, $team_stadium, $team_shirt, $t_id);
    return drupal_set_message('Squadra modificata con successo');
  }
  elseif ($action == 'insert') {
    $sql = "INSERT INTO {fanta_teams} (name, coach, stadium, shirt, active, uid) " .
    "VALUES ('%s', '%s', '%s', '%s', 1, '%d')";
    $result = db_query($sql, $team_name, $team_coach, $team_stadium, $team_shirt, $user->uid);
    return drupal_set_message('Squadra creata con successo');
  }
}

function fantacalcio_get_title_team() {

  $team = get_team_data();

  if (count($team) == 0)
    return 'Crea Squadra';
  else
    return 'Modifica Squadra';
}
