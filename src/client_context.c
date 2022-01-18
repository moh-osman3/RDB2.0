#include <string.h>
#include <assert.h>
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
Table *lookup_table(char *name) {
    int val;
	get(table_ht, name, &val);
	return &current_db->tables[val];
}

Column *lookup_column(char *tbl_name, char *col_name) {
	assert(col_name != NULL && tbl_name != NULL);
	const Table *tbl = lookup_table(tbl_name);
	int tbl_name_length = strlen(tbl_name);
	int col_length = strlen(col_name);
    char full_col_name[tbl_name_length + col_length + 2];
	strncpy(full_col_name, tbl_name, tbl_name_length);
	full_col_name[tbl_name_length] = '.';
	strncpy(full_col_name + tbl_name_length + 1, col_name, col_length);
	full_col_name[col_length + tbl_name_length + 1] = '\0';

	log_info("%s:%d: Full column name: %s\n",
	         __FUNCTION__, __LINE__, full_col_name);
    /*
	 * Doing linear scan of columns to find desired column.
	 * TODO: maybe store a hashtable in each table for quicker
	 *       column lookup?
	 */
	for (size_t i = 0; i < tbl->col_count; i++) {
		if (strcmp(full_col_name, tbl->columns[i].name) == 0) {
			log_info("%s:%d: Found column with name: %s\n",
			         __FUNCTION__, __LINE__, col_name);
			return &(tbl->columns[i]);
		}
	}

	log_info("%s:%d: Column not found\n", __FUNCTION__, __LINE__); 
	return NULL;
}

/**
*  Getting started hint:
* 		What other entities are context related (and contextual with respect to what scope in your design)?
* 		What else will you define in this file?
**/
