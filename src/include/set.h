#ifndef SET_H
#define SET_H

#define MAX_SIZE 4096
#define PRIME 786433 // for sizes b/w 2^19 and 2^20 https://planetmath.org/goodhashtableprimes

/*
 * This set contains elements that are strings 
 */
typedef char keyType[MAX_SIZE];

typedef struct set_elt {
    keyType key;
    struct set_elt *next;
} set_elt;

typedef struct set {
    struct set_elt **table;
    int capacity;
    int num_elt;
} set;


int allocate(set **map, int size);

int insert(set *map, keyType key);

int is_present(set *map, keyType key);

int erase(set *map, keyType key);

int deallocate(set **map);

#endif
