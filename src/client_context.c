#include <string.h>
#include "client_context.h"
#include "hashtable.h"
#include "utils.h"

hashtable *table_ht;

/* This is an example of a function you will need to
 * implement in your catalogue. It takes in a string (char *)
 * and outputs a pointer to a table object. Similar methods
 * will be needed for columns and databases. How you choose
 * to implement the method is up to you.
 * 
 */
Table* lookup_table(char *name) {
    int val;
	get(table_ht, name, &val);
	return &current_db->tables[val];
}

/**
*  Getting started hint:
* 		What other entities are context related (and contextual with respect to what scope in your design)?
* 		What else will you define in this file?
**/
