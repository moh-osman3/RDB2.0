#include "client_context.h"
#include "cs165_api.h"
#include "hashtable.h"
#include "utils.h"
#include <string.h>

// only one active database at a time
Db *current_db;

/*****************************************************************************
 * -- create_column -- 
 *
 * This API call creates a new table in the database 
 * 
 * params:
 *    db [in/out]       The database to add a table to
 *    name [in]         The name of the table
 *    num_columns [in]  The number of columns in the table
 *
 * Returns:
 *    Status OK on success
 *           ERROR on failure 
 *
 *****************************************************************************
 */

Status create_column(Table *table,  // IN/OUT
                     char *name)    // IN
{
    Status ret_status;
    int name_length;

    if (table == NULL ||
        name == NULL) {
        log_err("%s:%d: Unable to create column due to invalid inputs\n",
                __FUNCTION__, __LINE__);
    }
    
    name_length = strlen(table->name) + 1 + strlen(name);
    ret_status.code = ERROR;
    ret_status.error_message = QUERY_INVALID_STR;

    if (name_length > MAX_SIZE_NAME) {
        log_err("%s:%d: Column name is too long\n",
                __FUNCTION__, __LINE__);
        return ret_status;
    }
    if (sprintf(table->columns[table->col_count].name, "%s.%s", table->name, name) <= 0) {
        log_err("%s:%d: Failed to copy column name\n",
                __FUNCTION__, __LINE__);
        return ret_status;
    }

    table->columns[table->col_count].data = NULL;
    table->col_count += 1; 

    ret_status.code = OK;
    ret_status.error_message = SUCCESS_STR;
    return ret_status;

}

/*****************************************************************************
 * -- create_table -- 
 *
 * This API call creates a new table in the database 
 * 
 * params:
 *    db [in/out]       The database to add a table to
 *    name [in]         The name of the table
 *    num_columns [in]  The number of columns in the table
 *
 * Returns:
 *    Status OK on success
 *           ERROR on failure 
 *
 *****************************************************************************
 */
 
Status create_table(Db* db,                 // IN
                    const char* name,       // IN
                    size_t num_columns)     // IN
{
    // int name_length = strlen(name);
    // int chars_to_copy = name_length < MAX_SIZE_NAME ? name_length : MAX_SIZE_NAME;
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
        allocate(&table_ht, HT_SIZE);
        current_db->tables = malloc(sizeof(Table) * MAX_TABLES);
    }

    if (current_db->tables_size == current_db->tables_capacity) {
        current_db->tables_capacity *= 2;
        current_db->tables = realloc(current_db->tables, current_db->tables_capacity);
    }

    if (sprintf(tb->name, "%s.%s", db->name, name) <= 0) {
        log_err("%s:%d: Failed to copy table name\n",
                __FUNCTION__, __LINE__);
    };

    tb->col_count = 0;
    tb->table_length = 0;
    tb->columns = malloc(sizeof(Column) * num_columns);

    db->tables[db->tables_size] = *tb;

    insert(table_ht, tb->name, db->tables_size); 
    db->tables_size += 1;
    ret_status.code = OK;
    ret_status.error_message = SUCCESS_STR;
    return ret_status;
}


/*****************************************************************************
 * -- create_db -- 
 *
 * This API call creates a new database.  To start only one database will
 * be supported.
 * TODO: Support multiple databases 
 * 
 * params:
 *    db_name [in]         The name of the database
 *
 * Returns:
 *    Status OK on success
 *           ERROR on failure 
 *
 *****************************************************************************
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
