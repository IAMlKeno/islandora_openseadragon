<?php

/**
 * @file
 * Defines menu callback for downloading a clip of the given image.
 */

/**
 * Menu callback downloads the given clip.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 */
function islandora_openseadragon_download_clip(AbstractObject $object) {
  if (isset($_GET['clip'])) {
    module_load_include('inc', 'islandora_openseadragon', 'includes/utilities');
    $clip_parts = islandora_openseadragon_construct_clip_url($_GET['clip'], TRUE);
    if ($clip_parts) {
      $regex = '/\'|\"|@|,|%|\+|\{|\}|\[|\]|#|&|=|\.|\$/';
      $space_regex = '/\s/';
      $filename = strtolower($object->label);
      if (preg_match($regex, $filename)) {
        $escaped = preg_replace($regex, '', $filename);
        $filename = preg_replace($space_regex, '_', $escaped);
      }
      header("Content-Disposition: attachment; filename=\"{$filename}.jpg\"");
      header("Content-type: image/jpeg");
      header("Content-Transfer-Encoding: binary");
      $ch = curl_init();
      curl_setopt($ch, CURLOPT_BINARYTRANSFER, 1);
      curl_setopt($ch, CURLOPT_HEADER, 0);
      curl_setopt($ch, CURLOPT_URL, $clip_parts['image_url']);
      curl_exec($ch);
      curl_close($ch);
    }
    else {
      drupal_access_denied();
      watchdog('islandora_openseadragon', 'Invalid parameters specified for downloading of a clip for @pid. Parameters attempted: @params.', array(
        '@pid' => $object->id,
        '@params' => $_GET['clip'],
      ));
    }
  }
  drupal_exit();
}
