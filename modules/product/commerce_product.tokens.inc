<?php
// $Id$

/**
 * @file
 * Builds placeholder replacement tokens for product-related data.
 */


/**
 * Implements hook_token_info().
 */
function commerce_product_token_info() {
  $type = array(
    'name' => t('Products'),
    'description' => t('Tokens related to individual products.'),
    'needs-data' => 'product',
  );

  // Tokens for products.
  $product = array();
  
  $product['product-id'] = array(
    'name' => t('Product ID'),
    'description' => t('The internal numeric ID of the product.'),
  );
  $product['sku'] = array(
    'name' => t('SKU'),
    'description' => t('The human readable product SKU.'),
  );
  $product['type'] = array(
    'name' => t('Type'),
    'description' => t('The human readable name of the product type.'),
  );
  $product['title'] = array(
    'name' => t('Title'),
    'description' => t('The title of the product.'),
  );
  $product['edit-url'] = array(
    'name' => t('Edit URL'),
    'description' => t("The URL of the product's edit page."),
  );

  // Chained tokens for products.
  $product['created'] = array(
    'name' => t('Date created'),
    'description' => t('The date the product was created.'),
    'type' => 'date',
  );

  return array(
    'types' => array('product' => $type),
    'tokens' => array('product' => $product),
  );
}

/**
 * Implements hook_tokens().
 */
function commerce_product_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }

  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'product' && !empty($data['product'])) {
    $product = $data['product'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the product.
        case 'product-id':
          $replacements[$original] = $product->product_id;
          break;

        case 'sku':
          $replacements[$original] = $sanitize ? check_plain($product->sku) : $product->sku;
          break;

        case 'type':
          $replacements[$original] = $sanitize ? check_plain(commerce_product_type_get_name($product->type)) : commerce_product_type_get_name($product->type);
          break;

        case 'title':
          $replacements[$original] = $sanitize ? check_plain($product->title) : $product->title;
          break;

        case 'edit-url':
          $replacements[$original] = url('admin/commerce/products/' . $product->product_id . '/edit', $url_options);
          break;

        // Default values for the chained tokens handled below.
        case 'created':
          $replacements[$original] = format_date($product->created, 'medium', '', NULL, $language_code);
          break;
      }
    }

    if ($created_tokens = token_find_with_prefix($tokens, 'created')) {
      $replacements += token_generate('date', $created_tokens, array('date' => $product->created), $options);
    }
  }

  return $replacements;
}
