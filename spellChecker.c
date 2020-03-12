#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isCharAlpha(char c);
int levenshteinDistance(char str1[], char str2[], int len1, int len2);

int hashFunctionSpellChecker(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

int levenshteinDistance(char str1[], char str2[], int len1, int len2)
{
    int distance;
    int deleteCost, insertCost, subCost = 0;
    int* temp = NULL;
    //int len1 = (sizeof(str1) / sizeof(str1[0]));
    //int len2 = (sizeof(str2) / sizeof(str2[0]));

    // TESTPRINT
    //printf("%d ", len1);
    //printf("%d ", len2);

    
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

    // initialize previous row with distance to delete
    //for (i = 0; i < len2; i++)
    //{
    //    previous[i] = i;
    //}

    //TESTPRINT
    //for (i = 0; i < len1; i++)
    //{
    //    printf("%d ", previous[i]);
    //}

    for (int i = 0; i < len2; i++)
    {
        // fill first element of current
        current[0] = i + 1;
    
        // fill remaining rows of current
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

            current[j + 1] = min(deleteCost, min(insertCost, subCost));

        }
        //TESTPRINT
        printf("previous row:  ");
        for (int z = 0; z < len1 + 1; z++)
        {
            printf("%d ", previous[z]);
        }
        printf("\n");
        //TESTPRINT
        printf("current row:  ");
        for (int y = 0; y < len1 +1; y++)
        {
            printf("%d ", current[y]);
        }
        printf("\n");
        // swap current and previous
        temp = previous;
        previous = NULL;
        previous = current;
        current = NULL;
        current = temp;
        temp = NULL;

        //TESTPRINT
        printf("previous row swapped:  ");
        for (int w = 0; w < len1 + 1; w++)
        {
            printf("%d ", previous[w]);
        }
        printf("\n");
        //TESTPRINT
        printf("current row swapped:  ");
        for (int u = 0; u < len1 + 1; u++)
        {
            printf("%d ", current[u]);
        }
        printf("\n");

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

/*
int levenshteinDistance(char s[0..m - 1], char t[0..n - 1])
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

    char* currentWord = nextWord(file);

    // while nextWord() not equal to NULL
    while (nextWord(file) != NULL)
    {
        hashMapPut(map, currentWord, 0);

        currentWord = nextWord(file);
    }

    // test
    //hashMapPrint(map);



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

    //FILE* file = fopen("dictionary.txt", "r");
    FILE* file = fopen("C:\dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);

    char inputbuffer[256];
    char inputbuffer2[256]; //TESTME
    int quit = 0;
    while (!quit)
    {
        printf("enter a word or \"quit\" to quit: ");
        scanf("%[^\n]%*1[\n]", inputbuffer);

        // input validation
        int invalid = 0;
        int length = 0;

        for (int i = 0; inputbuffer[i] != '\0'; ++i)
        {
            printf("%c", inputbuffer[i]);
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
                printf("%c", inputbuffer[i]);
            }

            // calculate levenshtein distance
            //char test1[6] = { 'k', 'i', 't', 't', 'e', 'n'};
            //char test2[7] = { 's', 'i', 't', 't', 'i', 'n', 'g'};

            //int distance = levenshteinDistance(inputbuffer, inputbuffer, length, length);

            //printf("%d", distance);

            // if 0
                // print spelled correctly
            // else
                // print 5 suggestions

            printf("enter a word or \"quit\" to quit: ");
            scanf("%[^\n]%*1[\n]", inputbuffer2);

            // input validation
            int invalid2 = 0;
            int length2 = 0;

            for (int i = 0; inputbuffer2[i] != '\0'; ++i)
            {
                printf("%c", inputbuffer2[i]);
                if (inputbuffer2[i] == 32 || !isalpha(inputbuffer2[i]))
                {
                    invalid2 = 1;
                }
            }
            printf("\n");
            if (invalid2 == 1)
            {
                printf("invalid input; try again\n");
            }
            else
                // implement the spell checker code here..
            {
                // convert to lower case
                for (int j = 0; inputbuffer2[j] != '\0'; ++j)
                {
                    inputbuffer2[j] = tolower(inputbuffer2[j]);
                    length2++;
                    printf("%c", inputbuffer2[j]);
                }
                printf("\n");
                int distance = levenshteinDistance(inputbuffer, inputbuffer2, length, length2);

                printf("THE LEVENSHTEIN DISTANCE IS: %d", distance);
            }
        }
        printf("\n");
       
        
        if (strcmp(inputbuffer, "quit") == 0)
        {
            quit = 1;
        }
    }

    hashMapDelete(map);
    return 0;
}
//
//int isCharAlpha(char c)
//{
//    if ((c > 64 && c < 91) || (c > 96 && c < 123))
//    {
//        return 1;
//    }
//    else
//    {
//        return 0;
//    }
//}
