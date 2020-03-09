#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int hashFunctionSpellChecker(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}
/*
function LevenshteinDistance(char s[0..m - 1], char t[0..n - 1])
{
    // create two work vectors of integer distances
    declare int v0[n + 1]
        declare int v1[n + 1]

        // initialize v0 (the previous row of distances)
        // this row is A[0][i]: edit distance for an empty s
        // the distance is just the number of characters to delete from t
        for i from 0 to n :
    v0[i] = i

        for i from 0 to m - 1 :
            // calculate v1 (current row distances) from the previous row v0

            // first element of v1 is A[i+1][0]
            //   edit distance is delete (i+1) chars from s to match empty t
            v1[0] = i + 1

            // use formula to fill in the rest of the row
            for j from 0 to n - 1:
    // calculating costs for A[i+1][j+1]
deletionCost: = v0[j + 1] + 1
insertionCost : = v1[j] + 1
if s[i] = t[j] :
    substitutionCost : = v0[j]
else :
    substitutionCost : = v0[j] + 1;

v1[j + 1] : = minimum(deletionCost, insertionCost, substitutionCost)

// copy v1 (current row) to v0 (previous row) for next iteration
// since data in v1 is always invalidated, a swap without copy could be more efficient
swap v0 with v1
// after the last swap, the results of v1 are now in v0
return v0[n]
}
*/

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
    char* word = malloc(sizeof(char) * maxLength);
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

    // while nextWord() not equal to NULL

    // hash size table of 26 (a-z, 0-25)

    // hash on first character, then next

    // repeat until out of characters

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
//int main(int argc, const char** argv)
//{
//    // FIXME: implement
//    HashMap* map = hashMapNew(1000);
//
//    FILE* file = fopen("dictionary.txt", "r");
//    clock_t timer = clock();
//    loadDictionary(file, map);
//    timer = clock() - timer;
//    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
//    fclose(file);
//
//    char inputBuffer[256];
//    int quit = 0;
//    while (!quit)
//    {
//        printf("Enter a word or \"quit\" to quit: ");
//        scanf("%s", inputBuffer);
//
//        // Implement the spell checker code here..
//
//        if (strcmp(inputBuffer, "quit") == 0)
//        {
//            quit = 1;
//        }
//    }
//
//    hashMapDelete(map);
//    return 0;
//}
