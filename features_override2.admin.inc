<?php

/**
 * Form callback for features overrides form.
 */
function features_override2_form($form, $form_state, $feature = NULL) {
  module_load_include('inc', 'features', 'features.export');
  drupal_add_css(drupal_get_path('module', 'features') . '/features.css');
  drupal_add_css(drupal_get_path('module', 'features_override2') . '/features_override2.css');

  $form = array();
  $override_list = array();
  if (isset($feature)) {
    $form['#feature'] = $feature;
    $form['name'] = array(
      '#type' => 'markup', 
      '#markup' => $feature->info['name'],
      '#prefix' => '<div class="features-components"><h3>',
      '#suffix' => '</h3></div>',
    );
    $form['description'] = array(
      '#type' => 'markup', 
      '#markup' => t('Features Override line details.  Select lines to export.'),
      '#prefix' => '<h4>',
      '#suffix' => '</h4>',
    );
    $override_list['New Overrides'] = features_override2_convert( features_override2_get_overrides($feature->name));
    $override_list['Existing Overrides'] = features_override2_get_existing_overrides($feature->name);
    $default_value['New Overrides'] = FALSE;
    $default_value['Existing Overrides'] = TRUE;
  }
  else {
    $form['name'] = array(
      '#type' => 'markup', 
      '#markup' => 'Overrides',
      '#prefix' => '<div class="features-components"><h3>',
      '#suffix' => '</h3></div>',
    );
    $override_list['New Overrides'] = features_override2_convert( features_override2_get_overrides());
    $default_value['New Overrides'] = FALSE;
  }

  foreach ($override_list as $type_name => $overrides) {
    $form['sources'][$type_name]['header'] = array(
      '#type' => 'markup', 
      '#markup' => $type_name,
      '#prefix' => '<h4>',
      '#suffix' => '</h4>',
    );
    
    foreach ($overrides as $component => $element) {
      $form['sources'][$type_name][$component] = array(
        '#type' => 'fieldset',
        '#title' => $component,
      );
      foreach ($element as $key => $value) {
        $code = array();
        $options = array();
        $default_values = array();
        features_override2_export_override( $code, '', $value, NULL);
        $i = 0;
        foreach ($code as $line) {
          $options[$key.'.'.$i] = $line;
          if ($default_value[$type_name]) {
            $default_values[] = $key.'.'.$i;
          }
          $form['#code'][$component][$key.'.'.$i] = "['{$component}']['{$key}']".$line;
          $i++;
        }
        $form['sources'][$type_name][$component][$key][$component] = array(
          '#type' => 'checkboxes',
          '#options' => features_dom_encode_options($options),
          '#title' => $key,
          '#default_value' => features_dom_encode_options($default_values, FALSE),
        );
      }
    }
  }
  
  if (count($overrides) == 0) {
    $form['message'] = array(
      '#type' => 'markup', 
      '#markup' => 'There is nothing overridden to display.',
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
    
  }
  if (isset($feature)) {
    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Download feature'),
      '#weight' => 10,
      '#submit' => array('features_override2_export_submit'),
    );
  }
  return $form;
}

/**
 * Submit handler for features_export_form_build().
 */
function features_override2_export_submit($form, &$form_state) {
  module_load_include('inc', 'features', 'features.export');
  $feature = $form['#feature'];
  $code = $form['#code'];
  $module_name = $feature->name;
  $export = $form_state['values'];
  $version = (isset($feature->info['version'])) ? $feature->info['version'] : '';

  $data = array();
  foreach ($export as $component => $element) {
    if (isset($code[$component])) {
      $element = features_dom_decode_options($element);
      foreach ($element as $key => $value) {
        if ($value) {
          $data[] = '*.' . $code[$component][$key];
        }
      }
    }
  }
  // Generate download
  $export['features']['features_override2'] = $data;
  if ($files = features_export_render($export, $module_name, TRUE)) {
    $filename = (!empty($version) ? "{$module_name}-{$version}" : $module_name) . '.tar';

    // Clear out output buffer to remove any garbage from tar output.
    if (ob_get_level()) {
      ob_end_clean();
    }

    drupal_add_http_header('Content-type', 'application/x-tar');
    drupal_add_http_header('Content-Disposition', 'attachment; filename="'. $filename .'"');
    drupal_send_headers();

    $tar = array();
    $filenames = array();
    foreach ($files as $extension => $file_contents) {
      if (!in_array($extension, array('module', 'info'))) {
        $extension .= '.inc';
      }
      $filenames[] = "{$module_name}.$extension";
      print features_tar_create("{$module_name}/{$module_name}.$extension", $file_contents);
    }
    if (features_get_modules($module_name, TRUE)) {
      $module_path = drupal_get_path('module', $module_name);
      // file_scan_directory() can throw warnings when using PHP 5.3, messing
      // up the output of our file stream. Suppress errors in this one case in
      // order to produce valid output.
      foreach (@file_scan_directory($module_path, '/.*/') as $file) {
        $filename = substr($file->uri, strlen($module_path) + 1);
        if (!in_array($filename, $filenames)) {
          // Add this file.
          $contents = file_get_contents($file->uri);
          print features_tar_create("{$module_name}/{$filename}", $contents);
          unset($contents);
        }
      }
    }
    print pack("a1024","");
    exit;
  }
}

