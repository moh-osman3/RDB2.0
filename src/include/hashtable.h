#ifndef HASH_TABLE
#define HASH_TABLE

#define MAX_SIZE 4096
#define PRIME 786433 // for sizes b/w 2^19 and 2^20 https://planetmath.org/goodhashtableprimes

/*
 * keys for this hash table are strings
 * and the values are integers.
 */
typedef char keyType[MAX_SIZE];
typedef int valType;

typedef struct ht_elt {
    keyType key;
    valType val;
    struct ht_elt *next;
} ht_elt;

typedef struct hashtable {
    struct ht_elt **table;
    int capacity;
    int num_elt;
} hashtable;


int allocate(hashtable **map, int size);

int insert(hashtable *map, keyType key, valType val);

int get(hashtable *map, keyType key, valType *val);

int erase(hashtable *map, keyType key);

int deallocate(hashtable **map);

#endif
