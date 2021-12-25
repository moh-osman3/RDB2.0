#include "cs165_api.h"
#include <string.h>

// only one active database at a time
Db *current_db;


/* 
 * Here you will create a table object. The Status object can be used to return
 * to the caller that there was an error in table creation
 */
Status create_table(Db* db,					// IN
					const char* name,		// IN
					size_t num_columns)     // IN
{
	int name_length = strlen(name);
	int chars_to_copy = name_length < MAX_SIZE_NAME ? name_length : MAX_SIZE_NAME;
    Status ret_status;
	/*
	 * Make sure there is enough space in the current db for another table
	 */
	if (current_db->tables == NULL) {
		current_db->tables = malloc(sizeof(Table) * MAX_TABLES);
	}

	if (current_db->tables_size == current_db->tables_capacity) {
		current_db->tables_capacity *= 2;
		current_db->tables = realloc(current_db->tables, current_db->tables_capacity);
	}


//	strncpy(tb->name,  ) 
	ret_status.code=OK;
	return ret_status;
}

/* 
 * Similarly, this method is meant to create a database.
 */
Status create_db(const char* db_name) {
	// void pattern for 'using' a variable to prevent compiler unused variable warning
	struct Status ret_status;
	int db_name_length = strlen(db_name);
	int chars_to_copy = db_name_length < MAX_SIZE_NAME ? db_name_length : MAX_SIZE_NAME;

	current_db = malloc(sizeof(Db));

	if (current_db == NULL) {
		ret_status.code = ERROR;
		ret_status.error_message = OUT_OF_MEMORY_STR;
		return ret_status;
	}
	strncpy(current_db->name, db_name, chars_to_copy);
	current_db->tables = NULL;
	current_db->tables_size = 0;
	current_db->tables_capacity = MAX_TABLES;

	ret_status.code = OK;
	ret_status.error_message = SUCCESS_STR;
	return ret_status;
}
