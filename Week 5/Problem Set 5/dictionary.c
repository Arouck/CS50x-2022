// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = (LENGTH + 1) * 'z';

// Hash table
node *table[N];
int total_words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);

    node *actual_node = table[index];

    while (actual_node != NULL)
    {
        if (strcasecmp(actual_node->word, word) == 0)
        {
            return true;
        }

        actual_node = actual_node->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // The hash chosen is the sum of all ascii codes from the letters on the word.
    int hash = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        hash += tolower(word[i]);
    }

    return (hash % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    // Reads the file word by word until EOF (END OF FILE).
    while (fscanf(file, "%s", word) != EOF)
    {
        node *actual_node = malloc(sizeof(node));

        if (actual_node == NULL)
        {
            return false;
        }

        // Copy word to actual_node.
        strcpy(actual_node->word, word);
        actual_node->next = NULL;

        // Gets the word hash.
        int index = hash(actual_node->word);

        // If there is no other word with this hash, it's setted as the first element.
        if (table[index] == NULL)
        {
            table[index] = actual_node;
        }
        // If there is other words, it is inserted on the list.
        else
        {
            actual_node->next = table[index];
            table[index] = actual_node;
        }

        // Increment the total of words on dictionary on each loop.
        total_words++;
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return total_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *actual_node;
    node *previous_node;
    for (int i = 0; i < N; i++)
    {
        actual_node = table[i];

        while (actual_node != NULL)
        {
            previous_node = actual_node;
            actual_node = actual_node->next;
            free(previous_node);
        }
    }

    return true;
}
