<?php

/**
 * @file
 * Admin callbacks for Islandora Image Annotation
 *
 */
function islandora_image_annotation_admin() {
  module_load_include('inc', 'islandora_image_annotation', 'includes/admin.util');
  drupal_add_css(drupal_get_path('module', 'islandora_image_annotation') . '/css/islandora_annotation.css');
  $form = array();
  $coptions = islandora_annotation_get_content_models();
  $selected = variable_get('islandora_annotation_mappings', array());
  $enforced = variable_get('islandora_annotation_enforce_taxonomy', 0);
  $optimized = variable_get('islandora_annotation_optimized', 1);
  $chosen = array_keys($selected);
  $pids = array_keys($coptions);
  $alink_options = array(
    'attributes' => array('target' => '_blank'),
    'html' => TRUE,
  );
  foreach ($selected as $key => $value) {
    $label = isset($coptions[$key]) ? $coptions[$key] : "Unlabeled";
    unset($coptions['$key']);
    $coptions = array($key => $label) + $coptions;
  }

  /* $form['islandora_image_annotation_djatoka_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Provide the djatoka url'),
    '#default_value' => variable_get('islandora_image_annotation_djatoka_path', 'http://localhost:8080/adore-djatoka'),
    '#description' => t('Image annotation uses djatoka to get file sizes for images. If you have the large image
    solution pack or book solution pack enabled you should be able to use the same path to djatoka here'),
    '#weight' => -10
    ); */

  $form['content_model_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure Content Models'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['content_model_wrapper']['annotation_table'] = array(
    '#type' => 'item',
    '#title' => t('Select Content Models'),
    '#description' => t('Allow annotation of objects associated with these content models.'),
    '#tree' => TRUE, // this attribute is important to return the submitted values in a deeper nested arrays in
    '#theme' => 'islandora_annotation_table',
  );



  foreach ($coptions as $pid => $label) {
    $dsids = get_annotation_dsids_from_dscomp($pid);
    $taxes = islandora_annotation_get_taxonomies();
    if (empty($dsids)) {
      continue;
    }
    $options[$pid] = '';
    // label
    $form['content_model_wrapper']['annotation_table']['name'][$pid] = array(
      '#type' => 'item',
      '#markup' => l($label, "islandora/object/$pid", $alink_options),
    );
    // pid
    $form['content_model_wrapper']['annotation_table']['content_model'][$pid] = array(
      '#type' => 'item',
      '#markup' => $pid,
    );

    // dsid


    $form['content_model_wrapper']['annotation_table']['DSID'][$pid] = array(
      '#prefix' => '<div class="dsidSelect">',
      '#suffix' => '</div>',
      '#type' => 'select',
      '#options' => $dsids,
      '#default_value' => isset($selected[$pid]['DSID']) ? $selected[$pid]['DSID'] : '',
    );

    $form['content_model_wrapper']['annotation_table']['TAX'][$pid] = array(
      '#prefix' => '<div class="dsidSelect">',
      '#suffix' => '</div>',
      '#type' => 'select',
      '#options' => $taxes,
      '#default_value' => isset($selected[$pid]['TAX']) ? $selected[$pid]['TAX'] : '',
    );

    $form['content_model_wrapper']['annotation_table']['enabled'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $chosen,
    );
  }

  $form['annotation_categories_fs'] = array(
    '#title' => 'Annotation Categories',
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
  );
  $form['annotation_categories_fs']['annotation_categories'] = array(
    '#type' => 'radios',
    '#title' => t('Annotation Categories'),
    '#options' => array(0 => t('User defined'), 1 => t("Administrator defined")),
    '#description' => t('Allows administrators to select predefined taxonomies to determine annotation categorization terminology'),
    '#default_value' => $enforced,
  );

  $form['annotation_categories_fs']['annotation_type_search_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Annotation Type Search Field'),
    '#description' => t('If Annotation Categories is set to User Defined and you want to do auto complete enter a solr field.
        this field\'s contents will be used to populate the auto complete.'),
    '#default_value' => variable_get('annotation_type_search_field', 'mads_categories_s'),
    '#weight' => 3,
  );
  $form['annotation_colors_fs'] = array(
    '#title' => 'Annotation Colors',
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
  );
  $form['annotation_colors_fs']['annotation_colors'] = array(
    '#type' => 'radios',
    '#title' => t('Annotation Colors'),
    '#options' => array(0 => t('User defined (user selects when creating annotation)'), 1 => t("Optimized (Colors are selected randomly.  Recommended for overlapping annotations")),
    '#default_value' => $optimized,
  );
  /* $form['annotation_colors_fs']['annotation_stroke_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Annotation Stroke Width'),
    '#default_value' => variable_get('annotation_stroke_width',1),
    '#description' => t('The width of the lines to use when drawing the annotations. Must be a number,
    can include a decimal.  For instance .5 is valid'),
    '#required' => TRUE,
    ); */
  $form['annotation_colors_fs']['annotation_stroke_widths'] = array(
    '#type' => 'textarea',
    '#title' => t('Annotation Stroke Widths for dropdown'),
    '#default_value' => variable_get('annotation_stroke_widths', '.3'),
    '#description' => t('The width of the lines to use when drawing the annotations. Must be a number, 
      can include a decimal.  For instance .5 is valid.  You can Enter several values here seperated by a space.'),
    '#required' => TRUE,
  );
  $form['annotation_controlled_titles'] = array(
    '#title' => 'Controlled Vocabulary for Titles',
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
  );
  $form['annotation_controlled_titles']['annotation_title_search_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Annotation Title Search Field'),
    '#description' => t('This must be a single valued solr field.'),
    '#default_value' => variable_get('annotation_title_search_field', 'mads_titles_sv'),
  );
  $form['annotation_controlled_titles']['annotation_use_title_vocabulary'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use a Controlled Vocabulary for Annotation Titles'),
    '#default_value' => variable_get('annotation_use_title_vocabulary', FALSE),
    '#weight' => -5,
    '#description' => t('If checked you must configure a Solr Field to retrieve the titles from.')
  );
  $form['annotation_controlled_titles']['annotation_title_search_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Annotation Title Search Field'),
    '#description' => t('This must be a single valued solr field.'),
    '#default_value' => variable_get('annotation_title_search_field', 'mads_titles_sv'),
    '#weight' => 3,
  );
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#weight' => 0,
    '#submit' => array('islandora_image_annotation_admin_submit'),
  );


  return $form;
}

/**
 * NEED VALIDATION 
 * @param type $form
 * @param type $form_state 
 */
function islandora_image_annotation_admin_submit($form, &$form_state) {
  $enabled = array_filter($form_state['values']['annotation_table']['enabled']);
  $content_model_mappings = array();
  foreach ($enabled as $label => $pid) {
    $content_model_mappings[$pid]['DSID'] = $form_state['values']['annotation_table']['DSID'][$pid];
    $content_model_mappings[$pid]['TAX'] = $form_state['values']['annotation_table']['TAX'][$pid];
  }
  // variable_set('islandora_image_annotation_djatoka_path',$form_state['values']['islandora_image_annotation_djatoka_path']);
  variable_set('islandora_annotation_mappings', $content_model_mappings);
  variable_set('islandora_annotation_enforce_taxonomy', $form_state['values']['annotation_categories']);
  variable_set('islandora_annotation_optimized', $form_state['values']['annotation_colors']);
  variable_set('annotation_title_search_field', $form_state['values']['annotation_title_search_field']);
  variable_set('annotation_use_title_vocabulary', $form_state['values']['annotation_use_title_vocabulary']);
  variable_set('annotation_type_search_field', $form_state['values']['annotation_type_search_field']);
  variable_set('annotation_stroke_widths', $form_state['values']['annotation_stroke_widths']);
  //$str = variable_get('annotation_stroke_widths','.3 .7 .9');
  //$arr = explode(' ',$str);
  //$json_str = json_encode($arr);
  //variable_set('annotation_stroke_widths', $json_str);
}

