<?php
// $Id$

/**
 * @file
 * Set up for the wine (advanced) example.
 */
function migrate_example_wine_schema() {
  $schema['migrate_example_wine_account'] = migrate_example_wine_schema_account();
  $schema['migrate_example_wine_topic'] = migrate_example_wine_schema_topic();
  $schema['migrate_example_wine_node'] = migrate_example_wine_schema_node();
  $schema['migrate_example_wine_topic_node'] = migrate_example_wine_schema_topic_node();
  $schema['migrate_example_wine_comment'] = migrate_example_wine_schema_comment();

  // These two tables are primarily for testing the table_copy plugin.
  // They do provide some guidance for uri redirection per uri_map_redirect.php
  $schema['migrate_example_wine_legacy_urls'] = migrate_example_wine_schema_legacy_urls();
  $schema['migrate_example_wine_copy_urls'] = migrate_example_wine_schema_legacy_urls();

  return $schema;
}

function migrate_example_wine_install() {
  migrate_example_wine_content_type();
  migrate_example_wine_tags();
  migrate_example_wine_image();

  // Populate our tables.
  migrate_example_wine_data_account();
  migrate_example_wine_data_topic();
  migrate_example_wine_data_topic_node();
  migrate_example_wine_data_node();
  migrate_example_wine_data_comment();
  migrate_example_wine_data_urls();
}

function migrate_example_wine_uninstall() {
  migrate_example_wine_content_type_delete();
  if ($vids = taxonomy_vocabulary_load_multiple(array(), array('machine_name' => 'migrate_example_wine_styles'))) {
    // Grab key of the first returned vocabulary.
    taxonomy_vocabulary_delete(key($vids));
  }
}

function migrate_example_wine_schema_node() {
  return array(
    'description' => 'wines of the world.',
    'fields' => array(
      'bid'  => array(
        'type' => 'serial',
        'not null' => TRUE,
        'description' => 'wine ID.',
      ),
      'name'  => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'body' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Full description of the wine.',
      ),
      'excerpt' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Abstract for this wine.',
      ),
      'countries' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Countries of origin. Multiple values, delimited by pipe',
      ),
      'accountid' => array(
        'type' => 'int',
        'length' => 11,
        'not null' => FALSE,
        'description' => 'Account Id of the author.',
      ),
      'image' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Image path',
      ),
    ),
    'primary key' => array('bid'),
  );
}

function migrate_example_wine_schema_topic() {
  return array(
    'description' => 'Categories',
    'fields' => array(
      'style'  => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'details' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
      ),
      'style_parent' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Parent topic, if any',
      ),
      'region' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Region first associated with this style',
      ),
      'hoppiness' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Relative hoppiness of the wine',
      ),
    ),
    'primary key' => array('style'),
  );
}

function migrate_example_wine_schema_topic_node() {
  return array(
    'description' => 'wines topic pairs.',
    'fields' => array(
      'bid'  => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'wine ID.',
      ),
      'style'  => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Topic name',
      ),
    ),
    'primary key' => array('style', 'bid'),
  );
}

function migrate_example_wine_schema_comment() {
  return array(
    'description' => 'wines comments.',
    'fields' => array(
      'cid'  => array(
        'type' => 'serial',
        'not null' => TRUE,
        'description' => 'Comment ID.',
      ),
      'bid'  => array(
        'type' => 'int',
        'length' => 11,
        'not null' => TRUE,
        'description' => 'wine ID that is being commented upon',
      ),
      'cid_parent' => array(
        'type' => 'int',
        'length' => 11,
        'not null' => FALSE,
        'description' => 'Parent comment ID in case of comment replies.',
      ),
      'subject' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment subject',
      ),
      'body' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment body',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment name (if anon)',
      ),
      'mail' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment email (if anon)',
      ),
      'accountid' => array(
        'type' => 'int',
        'length' => 11,
        'not null' => FALSE,
        'description' => 'Account ID (if any).',
      ),
    ),
    'primary key' => array('cid'),
  );
}

function migrate_example_wine_schema_account() {
  return array(
    'description' => 'Wine accounts.',
    'fields' => array(
      'accountid'  => array(
        'type' => 'serial',
        'not null' => TRUE,
        'description' => 'Account ID',
      ),
      'status'  => array(
        'type' => 'int',
        'length' => 11,
        'not null' => TRUE,
        'description' => 'Blocked_Allowed',
      ),
      'posted' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Registration date',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Account name (for login)',
      ),
      'nickname' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Account name (for display)',
      ),
      'password' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Account password (raw)',
      ),
      'mail' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Account email',
      ),
    ),
    'primary key' => array('accountid'),
  );
}

function migrate_example_wine_schema_legacy_urls() {
  return array(
    'description' => 'Stores legacy paths and destination ids for redirection.',
    'fields' => array(
      'id'  => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Primary Key: ID.',
      ),
      'migration_name' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => TRUE,
        'default' => '',
      ),
      'source_id' => array(
        'type' => 'int',
        'length' => 11,
        'not null' => FALSE,
      ),
      'source_uri' => array(
        'type' => 'varchar',
        'length' => 500,
        'not null' => FALSE,
      ),
      'modificationdate' => array(
        'type' => 'datetime',
        'not null' => TRUE,
      ),
    ),
    'primary key' => array('ID'),
    'indexes' => array(
      'source_uri' => array(array('source_uri', 255)),
    ),
  );
}

function migrate_example_wine_content_type() {
  // This code based on from standard.profile.
  // Insert default user-defined node types into the database.
  $types = array(
    array(
      'type' => 'migrate_example_wine',
      'name' => st('wine'),
      'base' => 'node_content',
      'description' => st("wine is what we drink."),
      'custom' => 1,
      'modified' => 1,
      'locked' => 1,
    ),
  );

  foreach ($types as $type) {
    $type = node_type_set_defaults($type);
    node_type_save($type);
    node_add_body_field($type);
  }
}

function migrate_example_wine_tags() {
  // Create a vocabulary named "Migrate Example wine Styles", enabled for the 'migrate_example_wine' content type.
  $description = st('Use tags to group wines on similar topics into categories.');
  $help = st('Enter a comma-separated list of words to describe your content.');
  $vocabulary = (object) array(
    'name' => 'Migrate Example wine Styles',
    'description' => $description,
    'machine_name' => 'migrate_example_wine_styles',
    'help' => $help,

  );
  taxonomy_vocabulary_save($vocabulary);

  if (!field_info_field('migrate_example_wine_styles')) {
    $field = array(
      'field_name' => $vocabulary->machine_name,
      'type' => 'taxonomy_term_reference',
      // Set cardinality to unlimited for tagging.
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
      'settings' => array(
        'allowed_values' => array(
          array(
            'vid' => $vocabulary->vid,
            'parent' => 0,
          ),
        ),
      ),
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'migrate_example_wine_styles', 'migrate_example_wine')) {
    $instance = array(
      'field_name' => $vocabulary->machine_name,
      'entity_type' => 'node',
      'label' => $vocabulary->name,
      'bundle' => 'migrate_example_wine',
      'description' => $vocabulary->help,
      'widget' => array(
        'type' => 'taxonomy_autocomplete',
      ),
    );
    field_create_instance($instance);
  }
}

// Create an image field named "Migrate Example Image", enabled for the 'wine' content type.
function migrate_example_wine_image() {
  if (!field_info_field('field_migrate_example_image')) {
    $field = array(
      'field_name' => 'field_migrate_example_image',
      'type' => 'image',
      'cardinality' => 1,
      'translatable' => TRUE,
      'indexes' => array('fid' => array('fid')),
      'settings' => array(
        'uri_scheme' => 'public',
        'default_image' => FALSE,
      ),
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'field_migrate_example_image', 'migrate_example_wine')) {
    $instance = array(
      'field_name' => 'field_migrate_example_image',
      'entity_type' => 'node',
      'label' => 'Image',
      'bundle' => 'migrate_example_wine',
      'description' => 'Upload an image to go with this wine.',
      'settings' => array(
        'file_directory' => 'field/migrate_example/image',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'alt_field' => TRUE,
        'title_field' => '',
      ),

      'widget' => array(
        'type' => 'image_image',
        'settings' => array(
          'progress_indicator' => 'throbber',
          'preview_image_style' => 'thumbnail',
        ),
        'weight' => -1,
      ),

      'display' => array(
        'full' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'type' => 'image_link_content__medium',
          'settings' => array(),
          'weight' => -1,
        ),
        'rss' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'search_index' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'search_results' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
      ),
    );
    field_create_instance($instance);
  }
}

function migrate_example_wine_content_type_delete() {
  $bundle = 'migrate_example_wine';
  $field_names = array('migrate_example_wine_styles', 'field_migrate_example_image');
  foreach ($field_names as $field_name) {
    $instance = field_info_instance('node', $field_name, $bundle);
    field_delete_instance($instance);
    field_delete_field($field_name);
  }
  node_type_delete($bundle);
}

function migrate_example_wine_data_node() {
  $fields = array('bid', 'name', 'body', 'excerpt', 'countries', 'accountid', 'image');
  $query = db_insert('migrate_example_wine_node')
    ->fields($fields);
  // Use high bid numbers to avoid overwriting an existing node id.
  $data = array(
    array(99999999, 'Heineken', 'Blab Blah Blah Green', 'Green', 'Netherlands|Belgium', 0, 'heineken.jpg'), // comes with migrate_example project.
    array(99999998, 'Miller Lite', 'We love Miller Brewing', 'Tasteless', 'USA|Canada', 1, NULL),
    array(99999997, 'Boddington', 'English occassionally get something right', 'A treat', 'United Kingdom', 1, NULL),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}

function migrate_example_wine_data_account() {
  $fields = array('status', 'posted', 'name', 'nickname', 'password', 'mail');
  $query = db_insert('migrate_example_wine_account')
    ->fields($fields);
  $data = array(
    array(1, '2010-03-30 10:31:05', 'alice', 'alice hot pants', 'alicepass', 'alice@example.com'),
    array(0, '2007-03-15 10:31:05', 'bob', 'rebob', 'bobpass', 'bob@example.com'),
    array(1, '2004-02-29 10:31:05', 'charlie', 'charlie chocolate', 'mykids', 'charlie@example.com'),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}

function migrate_example_wine_data_comment() {
  $fields = array('bid', 'cid_parent', 'subject', 'body', 'name', 'mail', 'accountid');
  $query = db_insert('migrate_example_wine_comment')
    ->fields($fields);
  $data = array(
    array(99999998, NULL, 'im first', 'hot body', 'alice', 'alice@example.com', 0),
    array(99999998, NULL, 'im second', 'hot body', 'alice', 'alice@example.com', 0),
    array(99999999, NULL, 'im parent', 'hot body', 'alice', 'alice@example.com', 0),
    array(99999999, 1, 'im child', 'cold body', 'bob', NULL, 1),
    array(99999999, 2, 'im grandchild', 'bitter body', 'charlie@example.com', NULL, 1),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}

function migrate_example_wine_data_topic() {
  $fields = array('style', 'details', 'style_parent', 'region', 'hoppiness');
  $query = db_insert('migrate_example_wine_topic')
    ->fields($fields);
  $data = array(
    array('ale', 'traditional', NULL, 'Medieval British Isles', 'Medium'),
    array('red ale', 'colorful', 'ale', NULL, NULL),
    array('pilsner', 'refreshing', NULL, 'Pilsen, Bohemia (now Czech Republic)', 'Low'),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}

function migrate_example_wine_data_topic_node() {
  $fields = array('bid', 'style');
  $query = db_insert('migrate_example_wine_topic_node')
    ->fields($fields);
  $data = array(
    array(99999999, 'pilsner'),
    array(99999999, 'red ale'),
    array(99999998, 'red ale'),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}

function migrate_example_wine_data_urls() {
  $fields = array('id', 'migration_name', 'source_id', 'source_uri', 'modificationdate');
  $query = db_insert('migrate_example_wine_legacy_urls')
    ->fields($fields);
  $data = array(
    array(1, 'wineNode', 99999997, 'the_boddington/main', '2010-04-12 08:32:06'),
    array(2, 'wineNode', 99999998, 'Miller Lite taste', '2010-04-12 08:32:05'),
    array(3, 'wineNode', 99999999, 'green wonder', '2010-04-12 08:32:03'),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}