<?php

/**
 * @file
 * utility functions for islandora image annotation module
 */

/**
 * 
 * @param type $pid
 * @return type
 */
function islandora_image_annotation_get_dsid($pid) {
  $object = islandora_object_load($pid);
  $dsid = NULL;
  $datastreams = variable_get('islandora_image_annotation_datastreams', 'OBJ JPG JPEG IMAGE');
  $datastreams = explode(' ', $datastreams);
  foreach ($datastreams as $datastream) {
    if (isset($object["$datastream"])) {
      $dsid = $datastream;
      break;
    }
  }
  return $dsid;
}

/**
 * checks our admin table for an allowed datastream
 * @param string $pid
 * @return string 
 */
function islandora_image_annotation_get_dsid_from_model($pid) {
  $dsid = 'OBJ'; //default
  $item = islandora_object_load($pid);
  $models = $item->models;
  if (($key = array_search('fedora-system:FedoraObject-3.0', $models)) !== FALSE) {
    unset($models[$key]);
  }
  $models_to_dsids = variable_get('islandora_annotation_mappings');
  foreach ($models as $model) {
    if (isset($models_to_dsids[$model])) {
      $tmp_dsid = $models_to_dsids[$model]['DSID'];
      if (isset($item[$tmp_dsid])) {
        $dsid = $tmp_dsid;
      }
    }
  }
  return $dsid;
}