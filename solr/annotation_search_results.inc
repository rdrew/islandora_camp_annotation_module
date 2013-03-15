<?php

/**
 * @file
 * Class override for grid display in solr search results
 */

/**
 * Extention of IslandoraSolrResults to create an alternative display type.  This
 * display is designed for the VDP project at UPEI and assumes certain fields.
 */
class IslandoraSolrResultsAnno extends IslandoraSolrResults {

  /**
   * Renders the primary or secondary display profile based on the raw Solr
   * results. This is the method most Islandora Solr display plugins would
   * override.
   *
   * @param type $results
   *   The raw Solr results from IslandoraSolrQueryProcessor::solrResult
   * @return
   *   Rendered Solr results
   *
   * @see IslandoraSolrResults::displayResults()
   */
  function printResults($solr_results) {
     drupal_add_css(drupal_get_path('module', 'islandora_solr_config') . '/css/islandora_solr_config.theme.css');
    $object_results = $solr_results['response']['objects'];
    $object_results = islandora_solr_prepare_solr_doc($object_results);    
    $elements = array();
    // total
    $elements['solr_total'] = $solr_results['response']['numFound'];
    // start
    $elements['solr_start'] = $solr_results['response']['start'];
    // Return themed search results.
    
    return theme('islandora_image_annotation_solr', array('results' => $object_results, 'elements' => $elements));
  }

  

}

