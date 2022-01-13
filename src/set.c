/*
 * -- set.c
 *
 *  implements a set library for C
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "set.h"


/******************************************************************************
 * -- hash --
 *
 * This function is responsible for hashing key into the
 * appropriate index. 
 * From https://en.wikipedia.org/wiki/Jenkins_hash_function
 *
 * Params:
 * key [in]      key to be hashed 
 * 
 * Returns -1 on failure
 *          otherwise returns index
 *
 ******************************************************************************
 */

unsigned int hash(keyType key) // IN
{
    int length = strlen(key);
    int i = 0;
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
 * for a new set
 *
 * Params:
 * map [in/out]  set to allocate space for
 * size [in]     number of "buckets"
 * 
 * Returns -1 on failure
 *          0 on success
 *
 ******************************************************************************
 */

int allocate(set **map,        // IN/OUT
             int size)         // IN
{
    *map = malloc(sizeof(set));

    if (*map == NULL) {
        fprintf(stdout, "%s:%d: Out of memory\n", __FUNCTION__, __LINE__);
        return -1;
    }
    (*map)->table = malloc(sizeof(set_elt *) * size);
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
 * into the set
 *
 * Params:
 * map [in/out]  set of strings 
 * key [in]      key to be inserted
 *
 * Returns -1 on failure
 *          0 on success
 *
 ******************************************************************************
 */

int insert(set *map,        // IN/OUT
           keyType key)     // IN
{
    int index = hash(key);

    set_elt *tmp = map->table[index];

    /*
     * Check if key is already in the set.
     */
    while (tmp != NULL) {
        if (strcmp(tmp->key, key) == 0) {
            return 0;
        }
        tmp = tmp->next;
    }

    tmp = map->table[index];
    set_elt *new_node = malloc(sizeof(set_elt));
     
    if (new_node == NULL) {
        fprintf(stdout, "%s:%d: Out of memory\n", __FUNCTION__, __LINE__);
        return -1;
    }
    strcpy(new_node->key, key);

    new_node->next = tmp;

    map->table[index] = new_node;
    map->num_elt += 1;

    return 0;
}


/****************************************************************************** 
 * -- is_present --
 *
 * This function is responsible for checking if key is in the set
 *
 * Params:
 * map [in/out]  set of strings 
 * key [in]      key of item to check
 *
 * Returns -1 if element is not present 
 *          0 if element is found
 *
 ******************************************************************************
 */

int is_present(set *map,  // IN
               keyType key)     // IN
{
    int index = hash(key);
    set_elt *tmp = map->table[index];

    while (tmp != NULL) {
        if (strcmp(tmp->key, key) == 0) {
            return 0; 
        }
    }
    return -1;
}


/****************************************************************************** 
 * -- erase --
 *
 * This function is responsible for deleting a key
 * from the set 
 *
 * Params:
 * map [in/out]  set of strings 
 * key [in]      key to delete 
 *
 * Returns -1 on failure
 *          0 on success
 *
 ******************************************************************************
 */

int erase(set *map, // IN/OUT
          keyType key)    // IN
{
    int index = hash(key);
    set_elt *tmp = map->table[index];

    if (tmp == NULL) {
        fprintf(stdout, "Unable to find key to delete\n");
        return -1;
    }

    map->num_elt -= 1;
    set_elt *prev = tmp;
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
 * associated with the set.
 *
 * Params:
 * map [in/out]  set to deallocate
 *
 * Returns -1 on failure
 *          0 on success
 *
 ******************************************************************************
 */

int deallocate(set **map) // IN
{
    for (int i = 0; i < (*map)->capacity; i++) {
        free((*map)->table[i]);
    }

    free((*map)->table);
    free(*map);
    *map = NULL;
    return 0;
}
