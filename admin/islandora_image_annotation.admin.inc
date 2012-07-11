<?php

function islandora_image_annotation_admin() {
  $form = array();
  $form['islandora_image_annotation_djatoka_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Provide the djatoka url'),
    '#default_value' => variable_get('islandora_image_annotation_djatoka_path', 'http://localhost:8080/adore-djatoka'),
    '#description' => t('Image annotation uses djatoka to get file sizes for images.  If you have the large image
       solution pack or book solution pack enabled you should be able to use the same path to djatoka here'),
    '#weight' => -10
  );

  return system_settings_form($form);
}
