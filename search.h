#ifndef SEARCH_H
#define SEARCH_H

#include "indexer.h"

/* Checks file validity of given inverted index */
int fileValid(FILE* fp);

/* Checks alphanumeric validity of given search term */
int termValid(char* word);

/* Loads words from inverted index into hashtable */
void loadFile(hashTable* ht, FILE* fp);

/* Loops through hashtable until 'q' is inputted*/
void infiniteLoop(hashTable* ht);

/* Checks hashtable presence of given search term and returns file list */
FileNode *getFilelist(char* word, hashTable* ht);

/* Search function with 'AND' logical operator */
int sa(char* word, hashTable* ht);

/* Search function with 'OR' logical operator */
int so(char* word, hashTable* ht);

#endif
