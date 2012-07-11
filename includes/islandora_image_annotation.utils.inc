<?php

function islandora_image_annotation_get_dsid($pid) {
  $object = islandora_object_load($pid);
  $dsid = NULL;
  $datastreams = variable_get('islandora_image_annotation_datastreams', 'OBJ JPG JPEG IMAGE');
  $datastreams = explode(' ', $datastreams);
  foreach($datastreams as $datastream){
    if(isset($object["$datastream"])){
      $dsid = $datastream;
      break;
    }
  }
  return $dsid;
}
