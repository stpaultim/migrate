<?php
// $Id$

/**
 * @file
 * A basic migration example.
 */

/**
 * To define a migration process from a set of source data to a particular
 * kind of Drupal object (for example, a specific node type), you define
 * a class derived from Migration. You must define a constructor to initialize
 * your migration object. The name of the class must end in "Migration", and the
 * portion of the name before "Migration" will be the machine name of your migration -
 * this is the name you will use in drush commands.
 *
 * There are three essential components to set up in your constructor:
 *  $this->source - An instance of a class derived from MigrateSource, this
 *    will feed data to the migration.
 *  $this->destination - An instance of a class derived from MigrateDestination,
 *    this will receive data that originated from the source and has been mapped
 *    by the Migration class, and create Drupal objects.
 *  Mappings - Use $this->addFieldMapping and $this->addFieldMappingCallback
 *    to tell the Migration class what source fields correspond to what destination
 *    fields, and additional information associated with the destination objects.
 */
class BeerNodeMigration extends Migration {
  public function __construct() {
    // Always call the parent constructor first - it performs some essential
    // initialization.
    parent::__construct();

    // Human-friendly description of your migration process
    $this->description = 'Beers of the world';

    // You may optionally declare dependencies for your migration - other migrations
    // which should run first. In this case, terms assigned to our nodes and
    // the authors of the nodes should be migrated before the nodes themselves.
    $this->dependencies = array('BeerTermMigration', 'BeerUserMigration');

    // We are getting data from tables in the Drupal default database - first,
    // set up a query for this data.
    $query = db_select('migrate_example_beer', 'b')
             ->fields('b', array('bid', 'name', 'body', 'excerpt', 'aid', 'countries', 'image'));
    $query->leftJoin('migrate_example_topic_beer', 'tb', 'b.bid = tb.bid');
    // Gives a single comma-separated list of terms
    $query->groupBy('tb.bid');
    $query->addExpression('GROUP_CONCAT(tb.name)', 'terms');

    // Use the query as the basis of our MigrateSource. Note that we need to
    // provide a schema definition for the primary key of this source, so the
    // Migration class can track mappings from source data to destination data.
    $this->source = new MigrateSourceSQL($query,
                                  array(
                                    'bid' => array(
                                      'type' => 'int',
                                      'not null' => TRUE,
                                      'description' => 'Beer ID.',
                                      'alias' => 'b',
                                    )
                                  ));

    // Set up our destination - nodes of type migrate_example_beer
    $this->destination = new MigrateDestinationNode('migrate_example_beer');

    // Assign mappings to destination fields from source fields. To discover
    // the names used in these calls, use the drush commands
    // drush migrate-fields-destination beernode
    // drush migrate-fields-source beernode
    $this->addFieldMapping('title', 'name');

    // We want to preserve old beer id as nid in Drupal.
    $this->addFieldMapping('nid', 'bid');
    $this->addFieldMapping('is_new', NULL, TRUE);

    // Lookup the uid for a given account id (aid).
    $arguments = array('source_field_name' => 'aid', 'migration' => 'BeerUser');
    // Note: uid is a required property so we use the 4th argument below to set a default.
    $this->addFieldMappingCallback('uid', 'prepareLookup', $arguments, 1);

    // The taxonomy terms are a comma-separated list in the query
    $this->addFieldMapping('taxonomy_migrate_example_tags', 'terms', NULL, ',');

    // Example of a single value image.
    $arguments = array('source_field_name' => 'image', 'source_path' => drupal_get_path('module', 'migrate_example'));
    $this->addFieldMappingCallback('field_migrate_example_image', 'prepareFile', $arguments);

    // Example of delimited field handling. Also example of unformatted multi-value text field.
    $this->addFieldMapping('field_migrate_example_country', 'countries', NULL, '|');

    // Example of a single value textfield with summary. In the Drupal 7 Field API,
    // what used to be a separate "teaser" field is now a "summary" attribute on
    // the body field.

    // This tells the Migration class that the body's summary attribute will come
    // from the excerpt field in the source.
    $arguments = array(
      'summary' => array('source_field' => 'excerpt'),
    );
    $this->addFieldMapping('body', 'body', NULL, NULL, $arguments);
  }
}

class BeerUserMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->description = 'Beers Drinkers of the world';
    $query = db_select('migrate_example_account', 'mea')
             ->fields('mea', array('aid', 'status', 'created', 'name', 'nickname', 'password', 'mail', 'sex', 'beers'));
    $this->source = new MigrateSourceSQL($query,
                                  array(
                                    'aid' => array(
                                      'type' => 'int',
                                      'not null' => TRUE,
                                      'description' => 'Account ID.',
                                    )
                                  ));
    $this->destination = new MigrateDestinationUser();
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('mail', 'mail');
    $this->addFieldMapping('pass', 'password');
    $this->addFieldMapping('roles', NULL, drupal_map_assoc(array(3)));

    // Example of a simple 'value' Field. Used for reference fields, list fields, etc.
    $this->addFieldMapping('field_migrate_example_gender', 'sex');

    // Need to have node_reference module enabled for this
    $this->addFieldMapping('field_migrate_example_favbeers', 'beers', NULL, '|');
  }
}


class BeerCommentMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->description = 'Comments about beers';
    $this->dependencies = array('BeerUserMigration', 'BeerNodeMigration');
    $query = db_select('migrate_example_comment', 'mec')
             ->fields('mec', array('cid', 'cid_parent', 'name', 'mail', 'aid', 'body', 'bid', 'subject'))
             ->orderBy('cid_parent', 'ASC');
    $this->source = new MigrateSourceSQL($query,
                                  array(
                                    'cid' => array(
                                      'type' => 'int',
                                      'not null' => TRUE,
                                      'description' => 'Account ID.',
                                    )
                                  ));
    $this->destination = new MigrateDestinationComment('comment_node_migrate_example_beer');

    //$this->addFieldMapping('name', 'name');
    $this->addFieldMapping('subject', 'subject');
    $this->addFieldMapping('mail', 'mail');
    $this->addFieldMapping('status', NULL, COMMENT_PUBLISHED);
    // We preserved bid => nid ids during BeerNodeMigration so simple mapping suffices.
    $this->addFieldMapping('nid', 'bid');

    $arguments = array('source_field_name' => 'aid', 'migration' => 'BeerUser');
    $this->addFieldMappingCallback('uid', 'prepareLookup', $arguments, 1);
    $arguments = array('source_field_name' => 'cid_parent', 'cache_size' => 0, 'migration' => 'BeerComment');
    $this->addFieldMappingCallback('pid', 'prepareLookup', $arguments);

    $this->addFieldMapping('comment_body', 'body');
    $dest = $this->lookupDestinationID(array(1));
  }
}

class BeerTermMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->description = 'Beer topics';
    // This sort assures that parents are saved before children.
    $query = db_select('migrate_example_topic', 'met')
             ->fields('met', array('name', 'description', 'parent_name'))
             ->orderBy('parent_name', 'ASC');
    $this->source = new MigrateSourceSQL($query,
                                  array(
                                    'name' => array(
                                      'type' => 'varchar',
                                      'length' => 255,
                                      'not null' => TRUE,
                                      'description' => 'Topic ID',
                                    )
                                  ));

    $this->destination = new MigrateDestinationTerm('migrate_example_tags');
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('description', 'description');
    $this->addFieldMapping('parent', 'parent_name');
  }
}