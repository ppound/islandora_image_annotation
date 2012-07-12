<?php

/**
 * Ajax callback for Shared Canvas
 * Supplies the basic data, including maifest uri
 * @global url $base_url
 * @param string $pid
 */
function islandora_canvas_setup($pid) {
  global $base_url;
  global $user;
  
  module_load_include('inc', 'islandora_image_annotation', 'includes/islandora_rdf_builder');
  //$books = get_collection_from_pid($pid);
  $book = $pid;//$books[0];
  $item = islandora_object_load($pid);
  $title = $item->label;
  $pages = array();
  $pages[1] =  $pid;
  $position = array_search($pid, $pages);
  $results = array();
  $results['manifest_url'] = "$base_url/emic/serve/$pid/Manifest/manifest.xml";
  $results['page_title'] = $item->label;
  $results['object_base'] = "$base_url/islandora/object/$pid";
  $results['islandora_post_url'] = "$base_url/emic/shared/add_annotation/$pid";
  $results['islandora_get_annotation'] = "$base_url/emic/get_annotation/$pid/";
  $results['islandora_delete_annotation'] = "$base_url/emic/delete_annotation/$pid/";
  $results['get_annotation_list_url'] = "$base_url/emic/get_urns/$pid";
  $results['position'] = $position;
  $results['pages'] = $pages;
  $results['no_edit'] = FALSE;
  $json = json_encode($results);
  echo $json;
}

/**
 * Adds annotation created in editor to saved datastream
 * @global url $base_url
 * @param string $pid
 */
function islandora_add_annotation($pid) {
  global $base_url;
  $doc_url = "$base_url/fedora/repository/$pid";
  $doc_path_url = "$base_url/emic/serve/$pid";
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  module_load_include('inc', 'islandora_image_annotation', 'includes/islandora_rdf_builder');
  module_load_include('inc', 'islandora_image_annotation', 'includes/islandora_image_annotation_build_streams');
// create domdoc and use it to populate builder
  $item = islandora_object_load($pid);
  $domDoc = new DOMDocument();
  if (!isset($item['AnnotationList'])) {
    $list = get_annotationList($doc_url, $doc_path_url);
  }
  else {
    $list = $item['AnnotationList']->content;
  }
  $domDoc->loadXML($list);
  $rdf = new IslandoraRdfBuilder($domDoc);
  $out = $rdf->printRDF();
// create Domdoc object with transformed data from SharedCanvas
  $annotationDomDoc = new DOMDocument();
  $annotation_xml = emic_shared_transform_annotationList(urldecode($_POST['data']), 'html');
  $simplexml = simplexml_load_string($annotation_xml);
  $annotations = $simplexml->xpath('//rdf:type[@rdf:resource="http://www.openannotation.org/ns/Annotation"]');
  $parents = $annotations[0]->xpath("parent::*");
  $parent = $parents[0];
  foreach ($parent->attributes('rdf', TRUE) as $key => $value) {
    echo (string) $value;
  }

  $annotationDomDoc->loadXML($annotation_xml);

// Get Description elements as nodeList and send them to
  $descriptions = $annotationDomDoc->getElementsByTagName('Description');
  $rdf->addDescriptionsFromNodelist($descriptions);
  $out = $rdf->printRDF();

  islandora_save_updated_annotations($pid, $rdf);
}

/**
 * transforms input from SharedCanvas to AnnotationList
 * @param string $xml_in
 * @return string
 */
function emic_shared_transform_annotationList($xml_in, $input_type) {

  if ($xml_in == NULL || strlen($xml_in) < 5) {
    return " ";
  }

  $path = drupal_get_path('module', 'islandora_image_annotation') . '/xslt/';
  $xslts = array(
    'html' => $path . 'html_to_AnnotationList.xsl',
    'rdf' => $path . 'AnnotationList_to_html.xsl'
  );
  try {
    $proc = new XsltProcessor();
  } catch (Exception $e) {
    drupal_set_message(t('@e', array('@e' => check_plain($e->getMessage()))), 'error');
    return " ";
  }

  $xsl = new DomDocument();
  $xsl->load($xslts[$input_type]);
  $input = new DomDocument();
  $input->loadXML(trim($xml_in));
  $xsl = $proc->importStylesheet($xsl);
  $newdom = $proc->transformToDoc($input);
  $return_xml = $newdom->saveXML();

  return $return_xml;
}

/**
 * Gets individual annotatio  urns from datastream
 * @global url $base_url
 * @param <type> $pid
 */
function islandora_shared_get_annotation_urns($pid) {
  $rdf = build_annotationList($pid);

  $descriptions = $rdf->domDoc->getElementsByTagName('Description');

  foreach ($descriptions as $description) {
    $about = $description->getAttribute('rdf:about');
    $types = $description->getElementsByTagName('type');
    foreach ($types as $type) {
      if ($type->getAttribute('rdf:resource') == 'http://www.openannotation.org/ns/Annotation') {
        $urns[] = $about;
      }
    }
  }
  $results = array();
  $results['info'] = $urns;
  $json = json_encode($urns);
  echo $json;
}

/**
 * Retrieves details of individual annotation from datastream
 * @global url $base_url
 * @param string $pid
 * @param urn $urn
 * @return IslandoraRdfBuilder
 */
function get_annotation_data($pid, $urn) {

  global $base_url;
  
  module_load_include('inc', 'islandora_image_annotation', 'includes/islandora_rdf_builder');
  $item = islandora_object_load($pid);
  $domDoc = new DOMDocument();
  $list = $item['AnnotationList']->content;
  $domDoc->loadXML($list);
  $rdf = new IslandoraRdfBuilder($domDoc);

  $descriptions = $rdf->domDoc->getElementsByTagName('Description');
  $count = $descriptions->length;
  $body_urns = array();
  $target_urns = array();
  $constraint_urns = array();
  foreach ($descriptions as $description) {
    if ($urn == $description->getAttribute('rdf:about')) {    // gets description element matching URN
      $rdf->addDescriptionElement($description);              // add this element to new RDF object
      $bodies = $description->getElementsByTagName('hasBody'); // get body element from Annotation object
      foreach ($bodies as $body) {                            // get urns from hasBody elements
        $body_urns[] = $body->getAttribute("rdf:resource");
      }
      $targets = $description->getElementsByTagName('hasTarget');
      foreach ($targets as $target) {                           // gets urns from target elements
        $target_urns[] = $target->getAttribute("rdf:resource");


        foreach ($descriptions as $description) {
          $about = $description->getAttribute('rdf:about');
          if (in_array($description->getAttribute('rdf:about'), $target_urns)) {
            $contraints = $description->getElementsByTagName('constrainedBy');
            foreach ($contraints as $contraint) {                            // get urns from hasBody elements
              $constraint_urns[] = $contraint->getAttribute("rdf:resource");
            }
          }
        }
      }
      foreach ($descriptions as $description) {
        $about = $description->getAttribute('rdf:about');
        if (in_array($description->getAttribute('rdf:about'), array_merge($body_urns, $target_urns, $constraint_urns))) {
          $rdf->addDescriptionElement($description);
        }
      }
    }
  }
  return $rdf;
}

/**
 * Retrieves transforms annotation data for the editor
 * Doctype declarations are removed
 * @param string $pid
 * @param urn $urn
 */
function islandora_shared_get_annotation($pid, $urn) {

  $rdf = get_annotation_data($pid, $urn);
  $results = emic_shared_transform_annotationList($rdf->printRDF(), 'rdf');
  $results = preg_replace('/<\?xml.*?\?>/', '', $results);
  $results = preg_replace('/<!DOCTYPE.*?>/', '', $results);

  echo $results;
}

/**
 *

 * @param string $pid
 * @param urn $urn
 */
function islandora_delete_annotation($pid, $urn) {

  module_load_include('inc', 'islandora_image_annotation', 'includes/islandora_rdf_builder');
  
  $item = islandora_object_load($pid);
  $domDoc = new DOMDocument();
  $list = $item['AnnotationList']->content;
  $domDoc->loadXML($list);
  $rdf = new IslandoraRdfBuilder($domDoc);
  $body_urn = $rdf->getAnnotationBodyUrn("urn:uuid:$urn");
  $target_urn = $rdf->getAnnotationTargetUrn("urn:uuid:$urn");
  $constrained_target_urn = $rdf->getConstrainedTarget($target_urn);   
  
  $rdf->deleteDescriptionElement("urn:uuid:$urn");//delete the orginal annotation
  $rdf->deleteDescriptionElement($body_urn);
  $rdf->deleteDescriptionElement($target_urn);
  $rdf->deleteDescriptionElement($constrained_target_urn);
  islandora_save_updated_annotations($pid, $rdf);
}

/**
 * Persists new annotations
 * @global url $base_url
 * @param <type> $pid
 * @param <type> $rdf
 */
function islandora_save_updated_annotations($pid, $rdf) {
  global $base_url;
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  $item = islandora_object_load($pid);
  $updated_annotationList = $rdf->printRDF();
  
  try {
    $item->repository->api->m->addDatastream($pid, 'AnnotationList', 'string', $updated_annotationList, array('controlGroup' => 'M', 'dsLabel' => 'AnnotationList'));
  } catch (Exception $e) {
    drupal_set_message(t('error updating AnnotationList datastream  %message', array('%message' => $e->getMessage())), 'error');
    return '';
  }
}

function build_annotationList($pid) {
  global $base_url;
  $doc_url = "$base_url/islandora/object/$pid";
  $doc_path_url = "$base_url/emic/serve/$pid";
  
  module_load_include('inc', 'islandora_image_annotation', 'includes/islandora_image_annotation_build_streams');
  module_load_include('inc', 'islandora_image_annotation', 'includes/islandora_rdf_builder');
 
  $item = islandora_object_load($pid);
  $annotationList = get_annotationList($doc_url, $doc_path_url);
  $domDoc = new DOMDocument();
  $domDoc->formatOutput = true;
  $domDoc->loadXML($annotationList);
  $rdf = new IslandoraRdfBuilder($domDoc);

  if (isset($item['AnnotationList'])) {
    $list = $item['AnnotationList']->content;
    if (strlen($list) < 50) {
      return;
    }
    $annoDoc = new DOMDocument();
    $annoDoc->loadXML($list);
    $descriptions = $annoDoc->getElementsByTagName('Description');
    foreach ($descriptions as $description) {
      $constrains = $description->getElementsByTagName('constrains');
      foreach ($constrains as $candidate) {
        $candidate->setAttribute("rdf:resource", "$base_url/fedora/repository/$pid/Canvas");
      }
      $newNode = $domDoc->importNode($description, TRUE);
      $rdf->root->appendChild($newNode);
    }
  }
  return $rdf;
}