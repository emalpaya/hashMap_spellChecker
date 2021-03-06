/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Eva Malpaya
 * Date: 3/11/2020
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
    // FIXME: implement

    assert(map != NULL);

    // loop through the map, clearing buckets if not empty
    for (int i = 0; i < map->capacity; i++)
    {
        if (map->table[i] != NULL) // bucket not empty
        {
            while (map->table[i]->next != NULL) // pop links until one left
            {
                // store link to pop
                struct HashLink* temp; 
                temp = map->table[i];

                // link head to rest of bucket
                map->table[i] = map->table[i]->next;

                // pop the stored link
                hashLinkDelete(temp);
                temp = 0;
            }

            // delete last link
            hashLinkDelete(map->table[i]);
        }
    }
    map->size = 0;
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key  and skip traversing as well. Returns NULL
 * if no link with that key is in the table.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
    // FIXME: implement
    assert(map != NULL);
    assert(key != NULL);

    // Compute the hash value to find the correct bucket
    int hashIndex = HASH_FUNCTION(key) % map->capacity;

    // Set a current link to the beginning of the list that has been hashed
    struct HashLink* current = map->table[hashIndex];


    while (current != NULL)
    {
        // Determine if current link is one being searched for
        if (strcmp(current->key, key) == 0)
        {
            // If so, return the value
            int* found = &(current->value);
            return found;
        }
        // Move to next link
        current = current->next;
    }

    // Key not found
    return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given 
 * capacity (double of the old capacity). After allocating the new table, 
 * all of the links need to rehashed into it because the capacity has changed.
 * 
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 * 
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
    // FIXME: implement
    assert(map != NULL);

    // Create new map with twice the size
    struct HashMap* newMap = hashMapNew(capacity);
    struct HashLink *current;

    // Loop through old map and traverse links. Add value to new map.
    for (int i = 0; i < capacity / 2; i++)
    {
        current = map->table[i];

        while (current != NULL)
        {
            hashMapPut(newMap, current->key, current->value);
            current = current->next;
        }
    }

    // Delete old map. Set old map value equal to new map.
    hashMapCleanUp(map);
    map->capacity = newMap->capacity;
    map->size = newMap->size;
    map->table = newMap->table;

    newMap->table = NULL;
    free(newMap);
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket.
 * 
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
    // FIXME: implement
    assert(map != NULL);
    assert(key != NULL);

    // Compute the hash value to find the correct bucket
    int hashIndex = HASH_FUNCTION(key) % map->capacity;

    if (hashIndex < 0)
    {
        hashIndex += map->capacity;
    }

    // Set a current link to the beginning of the list that has been hashed
    struct HashLink* current = map->table[hashIndex];

    while (current != NULL)
    {
        // Determine if key exists in bucket
        if (strcmp(current->key, key) == 0)
        {
            // If so, update the value
            current->value = value;
            return;
        }
        // Move to next link
        current = current->next;
    }

    // Key not found; add new link
    // Make sure have space and under load factor threshold
    if (hashMapTableLoad(map) > 0.75)
    {
        resizeTable(map, map->capacity * 2);

        // Re-compute the hash value to find the correct bucket after resize
        hashIndex = HASH_FUNCTION(key) % map->capacity;

        if (hashIndex < 0)
        {
            hashIndex += map->capacity;
        }
    }

    struct HashLink* newLink = hashLinkNew(key, value, map->table[hashIndex]);
    map->table[hashIndex] = newLink;
    map->size++;

}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
    // FIXME: implement
    assert(map != NULL);
    assert(key != NULL);

    // Compute the hash value to find the correct bucket
    int hashIndex = HASH_FUNCTION(key) % map->capacity;

    // Set a current link to the beginning of the list that has been hashed
    struct HashLink* current = map->table[hashIndex];
    struct HashLink* prev = 0;

    while (current != NULL)
    {
        // Determine if current link is one being searched for
        if (strcmp(current->key, key) == 0)
        {
            // If value found at first entry, set beginning to next entry
            if (prev == 0)
            {
                map->table[hashIndex] = current->next;
            }
            // If value is in the middle, link previous and next
            else
            {
                prev->next = current->next;
            }

            // Delete the link and decrement count; end function
            hashLinkDelete(current);
            map->size--;
            return;
        }
        // Move to next link
        prev = current;
        current = current->next;

    }
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
    // FIXME: implement
    assert(map != NULL);
    assert(key != NULL);

    // Compute the hash value to find the correct bucket
    int hashIndex = HASH_FUNCTION(key) % map->capacity;

    // Set a current link to the beginning of the list that has been hashed
    struct HashLink* current = map->table[hashIndex];

    while (current != NULL)
    {
        // Determine if current link is one being searched for
        if (strcmp(current->key, key) == 0)
        {
            // If so, return true if found
            return 1;
        }
        // Move to next link
        current = current->next;
    }
    // Return false if reached of list without finding
    return 0;
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    // FIXME: implement
    assert(map != NULL);

    return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    // FIXME: implement
    assert(map != NULL);

    return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    // FIXME: implement
    assert(map != NULL);

    int countEmpty = 0;

    for (int i = 0; i < map->capacity; i++)
    {
        if (map->table[i] == NULL)
        {
            countEmpty++;
        }
    }

    return countEmpty;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
    // FIXME: implement
    assert(map != NULL);

    return (float)(map->size / map->capacity);
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
  // FIXME: implement
    assert(map != NULL);

    struct HashLink* current = NULL;

    // for the capacity of the map
    for (int i = 0; i < map->capacity; i++)
    {
        printf("\nBucket %d: ", i);

        if (map->table[i] != NULL) // if bucket not empty
        {
            current = map->table[i]; 

            while (current != NULL)
            {
                printf("(%s, %d) ", current->key, current->value);

                current = current->next;
            }
        }
    }
   
}
