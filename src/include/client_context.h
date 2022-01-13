#ifndef CLIENT_CONTEXT_H
#define CLIENT_CONTEXT_H

#include "cs165_api.h"
#include "hashtable.h"

#define HT_SIZE 1000000

Table* lookup_table(char *name);

extern hashtable *table_ht; 
#endif
