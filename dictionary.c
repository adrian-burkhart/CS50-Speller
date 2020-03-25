// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

#define N 1985

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//Hash prototype
unsigned long hash(char *str);

// Hash table
node *table[N] = {NULL};

int count_words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int n = strlen(word);
    char copy[n + 1];

    //Add null terminator to end of the lower case word
    copy[n] = '\0';

    for (int i = 0; i < n; i++)
    {
        copy[i] = tolower(word[i]);
    }

    int index = hash(copy) % N;

    node *head = table[index];

    if (head != NULL)
    {
        //Points the cursor to the same location
        node *cursor = head;

        //Traverse the linked list
        while (cursor != NULL)
        {
            if (strcmp(copy, cursor->word) == 0)
            {
                //Return true if word matches the word in our dictionary
                return true;
            }

            //Else move cursor to the next linked list
            cursor = cursor->next;
        }
    }

    return false;
}

// Hashes word to a number
unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;    /* hash * 33 + c */
    }

    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file

    FILE *file_dict = fopen(dictionary, "r");
    if (file_dict == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // Read strings from file_dict one at a time
    int n = LENGTH + 2;
    char word[LENGTH + 1];

    while (fgets(word, n, file_dict) != NULL)
    {

        // add null terminator to the end of the word
        word[strlen(word) - 1] = '\0';

        //hash the word
        int index = hash(word) % N;

        // create a temporary node
        node *temp = malloc(sizeof(node));

        //Test to see if node is null
        if (temp == NULL)
        {
            fclose(file_dict);
            return false;
        }

        //Move to the next node in the list
        strcpy(temp -> word, word);
        temp -> next = table[index];

        table[index] = temp;
        count_words++;
    }


    // close the file
    fclose(file_dict);

    return true;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return (count_words);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // create a variable to go through index
    int index = 0;

    // iterate through entire hashtable array
    while (index < N)
    {
        // if hashtable is empty at index, go to next index
        if (table[index] == NULL)
        {
            index++;
        }

        // if hashtable is not empty, iterate through nodes and start freeing
        else
        {
            while (table[index] != NULL)
            {
                node *cursor = table[index];
                table[index] = cursor->next;
                free(cursor);
            }

            // once hashtable is empty at index, go to next index
            index++;
        }
    }

    // return true if successful
    return true;
}