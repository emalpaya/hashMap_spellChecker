#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// declare added structs and functions
struct HashLinkSwapped
{
    char* value;
    int key;
    struct HashLinkSwapped* next;
};

struct HashMapSwapped
{
    struct HashLinkSwapped** table;
    // Number of links in the table.
    int size;
    // Number of buckets in the table.
    int capacity;
};

int hashFunction3(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

// added functions
int levenshteinDistance(char str1[], char str2[], int len1, int len2);
void fillMapWithDistance(HashMap* map, char input[], int lenInput);
void loadDictByValues(HashMap* map, struct HashMapSwapped* mapByValues);
void printFiveSmallest(struct HashMapSwapped* mapByValues);
int getMin(int a, int b);

// hashMapSwapped
struct HashMapSwapped* hashMapSwappedNew(int capacity);
void hashMapSwappedInit(struct HashMapSwapped* map, int capacity);
struct HashLinkSwapped* hashLinkSwappedNew(const char* value, int key, struct HashLinkSwapped* next);
float hashMapSwappedTableLoad(struct HashMapSwapped* map);
void hashMapSwappedPut(struct HashMapSwapped* map, int key, const char* value);
static void hashLinkSwappedDelete(struct HashLinkSwapped* link);
void hashMapSwappedCleanUp(struct HashMapSwapped* map);
void hashMapSwappedDelete(struct HashMapSwapped* map);
void resizeTableSwapped(struct HashMapSwapped* map, int capacity);



/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * Differs from struct HashMap in that key and value's types are switched.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
struct HashMapSwapped* hashMapSwappedNew(int capacity)
{
    struct HashMapSwapped* map = (struct HashMapSwapped*)malloc(sizeof(struct HashMapSwapped));
    hashMapSwappedInit(map, capacity);
    return map;
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * Differs from struct HashMap in that key and value's types are switched.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapSwappedInit(struct HashMapSwapped* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = (struct HashLinkSwapped**)malloc(sizeof(struct HashLinkSwapped*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Creates a new hash table link with a copy of the key string.
 * Differs from struct HashMap in that key and value's types are switched.
 * @param value to copy in the link.
 * @param key Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
struct HashLinkSwapped* hashLinkSwappedNew(const char* value, int key, struct HashLinkSwapped* next)
{
    struct HashLinkSwapped* link = (struct HashLinkSwapped*)malloc(sizeof(struct HashLinkSwapped));
    link->value = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(link->value, value);
    link->key = key;
    link->next = next;
    return link;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * Differs from struct HashMap in that key and value's types are switched.
 * @param map
 * @return Table load.
 */
float hashMapSwappedTableLoad(struct HashMapSwapped* map)
{
    assert(map != NULL);

    return (float)(map->size / map->capacity);
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * Differs from struct HashMap in that key and value's types are switched.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket.
 *
 * @param map
 * @param key
 * @param value
 */
void hashMapSwappedPut(struct HashMapSwapped* map, int key, const char* value)
{
    assert(map != NULL);
    assert(value != NULL);

    char str[50];
    sprintf(str, "%d", key);

    // Compute the hash value to find the correct bucket
    int hashIndex = hashFunction3(str) % map->capacity;

    if (hashIndex < 0)
    {
        hashIndex += map->capacity;
    }

    // Set a current link to the beginning of the list that has been hashed
    struct HashLinkSwapped* current = map->table[hashIndex];

    while (current != NULL)
    {
        // Determine if key exists in bucket
        if (current->key == key)
        {
            // If so, update the value
            /*strcpy(value, *current->value);*/
            const char* temp = value;
            value = NULL;
            value = current->value;
            current->value = NULL;
            current->value = temp;
            temp = NULL;

            return;
        }
        // Move to next link
        current = current->next;
    }

    // Key not found; add new link
    // Make sure have space and under load factor threshold
    if (hashMapSwappedTableLoad(map) > 0.75)
    {
        resizeTableSwapped(map, map->capacity * 2);

        char str[50];
        sprintf(str, "%d", key);

        // Compute the hash value to find the correct bucket
        int hashIndex = hashFunction3(str) % map->capacity;

        if (hashIndex < 0)
        {
            hashIndex += map->capacity;
        }
    }
    struct HashLinkSwapped* newLink = (struct HashLinkSwapped*)malloc(sizeof(struct HashLinkSwapped));
    newLink->value = (char*)malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(newLink->value, value);
    newLink->key = key;
    newLink->next = map->table[hashIndex];
    map->table[hashIndex] = newLink;
    map->size++;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkSwappedNew.
 * Differs from struct HashMap in that key and value's types are switched.
 * @param link
 */
static void hashLinkSwappedDelete(struct HashLinkSwapped* link)
{
    free(link->value);
    free(link);
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkSwappedDelete to free the links.
 * Differs from struct HashMap in that key and value's types are switched.
 * @param map
 */
void hashMapSwappedCleanUp(struct HashMapSwapped* map)
{
    assert(map != NULL);

    // loop through the map, clearing buckets if not empty
    for (int i = 0; i < map->capacity; i++)
    {
        if (map->table[i] != NULL) // bucket not empty
        {
            while (map->table[i]->next != NULL) // pop links until one left
            {
                // store link to pop
                struct HashLinkSwapped* temp;
                temp = map->table[i];

                // link head to rest of bucket
                map->table[i] = map->table[i]->next;

                // pop the stored link
                hashLinkSwappedDelete(temp);
                temp = 0;
            }

            // delete last link
            hashLinkSwappedDelete(map->table[i]);
        }
    }
    map->size = 0;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * Differs from struct HashMap in that key and value's types are switched.
 * @param map
 */
void hashMapSwappedDelete(struct HashMapSwapped* map)
{
    hashMapSwappedCleanUp(map);
    free(map);
}

/**
 * Resizes the hash table to have a number of buckets equal to the given
 * capacity (double of the old capacity). After allocating the new table,
 * all of the links need to rehashed into it because the capacity has changed.
 * Differs from struct HashMap in that key and value's types are switched.
 *
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 *
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTableSwapped(struct HashMapSwapped* map, int capacity)
{
    assert(map != NULL);

    // Create new map with twice the size
    struct HashMapSwapped* newMap = hashMapSwappedNew(capacity);
    struct HashLinkSwapped* current;

    // Loop through old map and traverse links. Add value to new map.
    for (int i = 0; i < capacity / 2; i++)
    {
        current = map->table[i];

        while (current != NULL)
        {
            hashMapSwappedPut(newMap, current->key, current->value);
            current = current->next;
        }
    }

    // Delete old map. Set old map value equal to new map.
    hashMapSwappedCleanUp(map);
    map->capacity = newMap->capacity;
    map->size = newMap->size;
    map->table = newMap->table;

    newMap->table = NULL;
    free(newMap);
}


/**
 * Loads the contents of the HashMap file into a new one where the
 * types of key and value are switched.
 * @param map
 * @param mapByValue
 */
void loadDictByValues(HashMap* map, struct HashMapSwapped* mapByValue)
{

    assert(map != NULL);
    assert(mapByValue != NULL);

    struct HashLink* current = NULL;

    // for the capacity of the map
    for (int i = 0; i < map->capacity; i++)
    {
        if (map->table[i] != NULL) // if bucket not empty
        {
            current = map->table[i];

            while (current != NULL)
            {
                hashMapSwappedPut(mapByValue, current->value, current->key);

                current = current->next;
            }
        }
    }

}

/**
 * Prints the five smallest levenshtein distances from the
 * HashMapSwapped where the types of key and value are switched.
 * @param map
 */
void printFiveSmallest(struct HashMapSwapped* map)
{
    assert(map != NULL);
    int count = 0;
    int fiveOnly = 0;

    struct HashLinkSwapped* current = NULL;

    while (fiveOnly == 0)
    {
        // for the capacity of the map
        for (int i = 0; i < map->capacity; i++)
        {
            if (map->table[i] != NULL) // if bucket not empty
            {
                current = map->table[i];

                while (current != NULL)
                {
                    printf("%s\n", current->value);
                    count++;

                    if (count > 4)
                    {
                        fiveOnly = 1;
                        i = map->capacity;
                    }

                    current = current->next;
                }
            }
        }
    }

}

/**
 * Returns the smaller of two integers.
 * @param a
 * @param b
 */
int getMin(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}



/**
 * Accepts two strings and their lengths, calculates the levenshtein
 * distance between them, and returns it.
 * @param str1
 * @param str2
 * @param len1
 * @param len2
 * @return levenshtein distance
 *
 * Sources consulted for this function (Retrieved March 2020):
 * https://en.wikipedia.org/wiki/Levenshtein_distance
 * https://medium.com/@ethannam/understanding-the-levenshtein-distance-equation-for-beginners-c4285a5604f0
**/
int levenshteinDistance(char str1[], char str2[], int len1, int len2)
{
    int distance;
    int deleteCost, insertCost, subCost = 0;
    int* temp = NULL;

    int* previous = (int*)malloc(sizeof(int) * (len1 + 1));
    int* current = (int*)malloc(sizeof(int) * (len1 + 1));

    // initialize arrays
    for (int k = 0; k < (len1 + 1); k++)
    {
        previous[k] = k;
    }

    for (int m = 0; m < (len1 + 1); m++)
    {
        current[m] = m + 1;
    }

    for (int i = 0; i < len2; i++)
    {
        // fill first element of current
        current[0] = i + 1;

        // fill remaining columns of current
        for (int j = 0; j < len1; j++)
        {
            deleteCost = previous[j + 1] + 1;
            insertCost = current[j] + 1;

            if (str2[i] == str1[j])
            {
                subCost = previous[j];
            }
            else
            {
                subCost = previous[j] + 1;
            }

            current[j + 1] = getMin(deleteCost, getMin(insertCost, subCost));

        }

        // swap current and previous
        temp = previous;
        previous = NULL;
        previous = current;
        current = NULL;
        current = temp;
        temp = NULL;

    }

    // store return value
    distance = previous[len1];

    // free memory
    free(previous);
    free(current);
    previous = NULL;
    current = NULL;

    // return levenshtein distance
    return distance;
}

/**
 * Fills the original HashMap with the levenshtein distance
 * @param map
 * @param input[]
 * @param lenInput
**/
void fillMapWithDistance(HashMap* map, char input[], int lenInput)
{
    assert(map != NULL);

    int currentValueLength = 0;

    struct HashLink* current = NULL;

    // for the capacity of the map
    for (int i = 0; i < map->capacity; i++)
    {
        if (map->table[i] != NULL) // if bucket not empty
        {
            current = map->table[i];

            while (current != NULL)
            {
                // get length of current->value
                for (int i = 0; current->key[i] != '\0'; ++i)
                {
                    currentValueLength++;
                }


                current->value = levenshteinDistance(current->key, input, currentValueLength, lenInput);

                current = current->next;
                currentValueLength = 0; // reset for next key
            }
        }
    }
}

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = (char*)malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    // FIXME: implement

    char* currentWord = nextWord(file);

    // while nextWord() not equal to NULL
    while (nextWord(file) != NULL)
    {
        hashMapPut(map, currentWord, 0);

        currentWord = nextWord(file);
    }

}


/**
 * Checks the spelling of the word provded by the user. If the word is spelled incorrectly,
 * print the 5 closest words as determined by a metric like the Levenshtein distance.
 * Otherwise, indicate that the provded word is spelled correctly. Use dictionary.txt to
 * create the dictionary.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // fixme: implement
    HashMap* map = hashMapNew(1000);
    struct HashMapSwapped* mapByValue = hashMapSwappedNew(1000);

    FILE* file = fopen("dictionary.txt", "r");
    //FILE* file = fopen("C:\dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);

    char inputbuffer[256];
    int quit = 0;
    while (!quit)
    {
        printf("enter a word or \"quit\" to quit: ");
        scanf("%[^\n]%*1[\n]", inputbuffer);

        // input validation
        // Sources consulted for input validator (Retrieved March 2020):
        // https://stackoverflow.com/questions/27273303/alternate-method-for-clearing-input-buffer-in-c
        //
        int invalid = 0;
        int length = 0;

        for (int i = 0; inputbuffer[i] != '\0'; ++i)
        {
            if (inputbuffer[i] == 32 || !isalpha(inputbuffer[i]))
            {
                invalid = 1;
            }
        }
        printf("\n");
        if (invalid == 1)
        {
            printf("invalid input; try again\n");
        }
        else
            // implement the spell checker code here..
        {
            // convert to lower case
            for (int i = 0; inputbuffer[i] != '\0'; ++i)
            {
                inputbuffer[i] = tolower(inputbuffer[i]);
                length++;
            }

            // calculate levenshtein distance
            fillMapWithDistance(map, inputbuffer, length);

            // spelled correctly
            if (hashMapContainsKey(map, inputbuffer))
            {
                printf("The inputted word %s is spelled correctly\n", inputbuffer);
            }
            else if (strcmp(inputbuffer, "quit") == 0)
            {
                quit = 1;
            }
            // suggest 5 correct spellings
            else
            {
                printf("The inputted word %s is spelled incorrectly\n", inputbuffer);
                printf("Did you mean...?\n");

                // copy dictionary into new one sorted by value instead of key
                loadDictByValues(map, mapByValue);

                // print five smallest from dict sorted by values
                printFiveSmallest(mapByValue);
            }


        }

        if (strcmp(inputbuffer, "quit") == 0)
        {
            quit = 1;
        }
    }

    hashMapDelete(map);
    hashMapSwappedDelete(mapByValue);
    return 0;
}

