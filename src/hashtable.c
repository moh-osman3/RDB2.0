/*
 * -- hashtable.c
 *
 *  implements a hashtable library for C
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hashtable.h"


/******************************************************************************
 * -- hash --
 *
 * This function is responsible for hashing key into the
 * appropriate index. 
 * From https://en.wikipedia.org/wiki/Jenkins_hash_function
 *
 * Params:
 * map [in/out]  hashtable to allocate space for
 * size [in]     number of "buckets"
 * 
 * Returns -1 on failure
 *          otherwise returns index
 *
 ******************************************************************************
 */

unsigned int hash(keyType key) // IN
{
    int length = strlen(key);
    size_t i = 0;
    unsigned int hash = 0;
    while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash % PRIME;
}


/****************************************************************************** 
 * -- allocate --
 *
 * This function is responsible for allocating space
 * for a new hashtable
 *
 * Params:
 * map [in/out]  hashtable to allocate space for
 * size [in]     number of "buckets"
 * 
 * Returns -1 on failure
 *          0 on success
 *
 ******************************************************************************
 */

int allocate(hashtable **map,  // IN/OUT
             int size)         // IN
{
    *map = malloc(sizeof(hashtable));

    if (*map == NULL) {
        fprintf(stdout, "%s:%d: Out of memory\n", __FUNCTION__, __LINE__);
        return -1;
    }
    (*map)->table = malloc(sizeof(ht_elt *) * size);
    (*map)->capacity = size;
    (*map)->num_elt = 0;

    for (int i = 0; i < size; i++) {
        (*map)->table[i] = NULL;
    }

    return 0;
}


/****************************************************************************** 
 * -- insert --
 *
 * This function is responsible for inserting a key
 * into the hashtable
 *
 * Params:
 * map [in/out]  hashtable to allocate space for
 * key [in]      key to be inserted
 * val [in]      value associated with the key
 *
 * Returns -1 on failure
 *          0 on success
 *
 ******************************************************************************
 */

int insert(hashtable *map,  // IN/OUT
           keyType key,     // IN
           valType val)     // IN
{
    int index = hash(key);

    ht_elt *tmp = map->table[index];

    /*
     * Check if key is already in the hashtable.
     * If it is, replace the value.
     */
    while (tmp != NULL) {
        if (strcmp(tmp->key, key) == 0) {
            tmp->val = val;
            return 0;
        }
        tmp = tmp->next;
    }

    tmp = map->table[index];
    ht_elt *new_node = malloc(sizeof(ht_elt));
     
    if (new_node == NULL) {
        fprintf(stdout, "%s:%d: Out of memory\n", __FUNCTION__, __LINE__);
        return -1;
    }
    strcpy(new_node->key, key);
    new_node->val = val;

    new_node->next = tmp;

    map->table[index] = new_node;
    map->num_elt += 1;

    return 0;
}


/****************************************************************************** 
 * -- get --
 *
 * This function is responsible for getting the value that
 * is associated with key.
 *
 * Params:
 * map [in/out]  hashtable to allocate space for
 * key [in]      key of item to get
 * val [in/out]  place to store value
 *
 * Returns -1 on failure
 *          0 on success
 *
 ******************************************************************************
 */

int get(hashtable *map,  // IN
        keyType key,     // IN
        valType *val)    // IN/OUT
{
    int index = hash(key);
    ht_elt *tmp = map->table[index];

    while (tmp != NULL) {
        if (strcmp(tmp->key, key) == 0) {
            *val = tmp->val;
            return 0; 
        }
    }
    *val = -1;
    return -1;
}


/****************************************************************************** 
 * -- erase --
 *
 * This function is responsible for deleting a key
 * from the hashtable
 *
 * Params:
 * map [in/out]  hashtable to allocate space for
 * key [in]      key to delete 
 *
 * Returns -1 on failure
 *          0 on success
 *
 ******************************************************************************
 */

int erase(hashtable *map, // IN/OUT
          keyType key)    // IN
{
    int index = hash(key);
    ht_elt *tmp = map->table[index];

    if (tmp == NULL) {
        fprintf(stdout, "Unable to find key to delete\n");
        return -1;
    }

    map->num_elt -= 1;
    ht_elt *prev = tmp;
    tmp = tmp->next;
    while (tmp != NULL) {
        
        if (strcmp(tmp->key, key) == 0) {
            prev->next = tmp->next;
            free(tmp);
            tmp = NULL;
            return 0;
        }

        tmp = tmp->next;
        prev = prev->next;
    }

    if (strcmp(map->table[index]->key, key) != 0) {
        fprintf(stdout, "Unable to find key to delete\n");
        return -1;
    }
    free(map->table[index]);
    map->table[index] = NULL;
    return 0;
}


/****************************************************************************** 
 * -- deallocate --
 *
 * This function is responsible for deallocating all memory
 * associated with the hashtable.
 *
 * Params:
 * map [in/out]  hashtable to deallocate
 *
 * Returns -1 on failure
 *          0 on success
 *
 ******************************************************************************
 */

int deallocate(hashtable **map) // IN
{
    for (int i = 0; i < (*map)->capacity; i++) {
        free((*map)->table[i]);
    }

    free((*map)->table);
    free(*map);
    *map = NULL;
    return 0;
}
