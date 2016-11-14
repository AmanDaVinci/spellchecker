/****************************************************************************
 * 
 * Utility Functions for Spell Checker
 *
 ***************************************************************************/

#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

typedef struct node
{
    bool isword;
    struct node* children[27];
}node;
node* dict = NULL;

/**
* Helper Functions
* by Aman
**/
int index(char c)
{
    if (isalpha(c)) { return (toupper(c) - 'A'); }
    else if (c=='\'') { return 26; }
    else { return -1; }
}
node* newnode(void)
{
    node* newnode = (node*)malloc(sizeof(node));
    
    newnode->isword = false;
    for (int i = 0; i<27; i++)
        newnode->children[i] = NULL;
    
    return newnode;    
}
void insert(char* word)
{
    int i;
    int length = strlen(word);
    // debug
    //printf("Word:%s Len: %i\n", word, length);
    // debug
    node* crawler = dict;
    for (i = 0; i < length; i++)
    {       
        //printf("%c ", word[i]);
        if (!crawler->children[ index(word[i]) ])
        {
            node* node = newnode();
            crawler->children[ index(word[i]) ] = node;       
        }
        crawler = crawler->children[ index(word[i]) ];
    }
    crawler->isword = true;
}
bool search(char* word)
{
    int i;
    int length = strlen(word);
    
    node* crawler = dict;
    for (i = 0; i < length; i++)
    {
        if (crawler->children[ index(word[i]) ]) { crawler = crawler->children[ index(word[i]) ]; }
        else { return false; }
    }
    
    if (crawler->isword) { return true; }
    else { return false; }
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int i;
    int length = strlen(word);
    
    node* crawler = dict;
    for (i = 0; i < length; i++)
    {
        if (crawler->children[ index(word[i]) ]) { crawler = crawler->children[ index(word[i]) ]; }
        else { return false; }
    }
    
    if (crawler->isword) { return true; }
    else { return false; }
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    dict = newnode();
    FILE* dictptr = fopen(dictionary, "r");
    char word[46];
    while(!feof(dictptr))
    {
        fscanf(dictptr, "%s", word);
        insert(word); 
    }
    fclose(dictptr);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
void traverse(node* crawler, unsigned int *size)
{
    if (crawler->isword) { *size += 1; /*printf("\n");*/ }
    
    for (int i = 0; i < 27 ; i++)
    {
        if (crawler->children[i])
        {
            //printf("%c ", i+'A');
            traverse(crawler->children[i], size);       
        }  
    }
}
unsigned int size(void)
{
    unsigned int size = 0;
    node* crawler = dict;
    
    if (dict == NULL) { return 0; }
    
    traverse(crawler, &size);
    return size;
}


/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
void traversefree(node* crawler)
{
    for (int i = 0; i < 27; i++)
    {
        if(crawler->children[i])
        {
            traversefree(crawler->children[i]);   
        }
    }
    free(crawler);
}
bool unload(void)
{
    node* crawler = dict;
    if (dict == NULL) { return false; }
    traversefree(crawler);
    return true;
}
/*
int main(int argc, char* argv[])
{
    load("/home/jharvard/cs50/psets/pset5/pset5/dict.txt");
    //load("/home/cs50/pset5/dictionaries/large");
    if (check("battle"))
        printf("Found battle\n");
    if (check("apple"))
        printf("Found apple\n");
    if (check("zooming"))
        printf("Found zooming\n");
       
    unsigned int n = size();
    printf("size: %i\n", n);
    
    if(unload())
        printf("Unloaded\n");

    return 0;
}*/
