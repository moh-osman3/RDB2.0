#include "cs165_api.h"
#include "utils.h"
#include <string.h>

// only one active database at a time
Db *current_db;


/* 
 * Here you will create a table object. The Status object can be used to return
 * to the caller that there was an error in table creation
 */
Status create_table(Db* db,                 // IN
                    const char* name,       // IN
                    size_t num_columns)     // IN
{
    int name_length = strlen(name);
    int chars_to_copy = name_length < MAX_SIZE_NAME ? name_length : MAX_SIZE_NAME;
    Status ret_status;
    ret_status.error_message = QUERY_INVALID_STR; 
    ret_status.code = ERROR;
    Table *tb = malloc(sizeof(Table));
    int length_of_name = strlen(db->name);
    length_of_name += 1; // add 1 for the "." between db.tbl
    length_of_name += strlen(name);
    
    /*
     * The name of the table will be formatted as db_name.tbl_name
     * Check that this name will not overflow.
     */
    if (length_of_name > MAX_SIZE_NAME) {
        log_err("%s:%d: Table name is too long\n", __FUNCTION__, __LINE__);
        return ret_status;
    }
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

    if (sprintf(&tb->name, "%s.%s", db->name, name) <= 0) {
        log_err("%s:%d: Failed to copy table name\n",
                __FUNCTION__, __LINE__);
    };
    
    printf("name of tbl: %s\n", tb->name);

    tb->col_count = num_columns;
    tb->table_length = 0;
    tb->columns = malloc(sizeof(Column) * num_columns);

    db->tables[db->tables_size] = *tb;
    db->tables_size += 1;

    ret_status.code = OK;
    ret_status.error_message = SUCCESS_STR;
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
