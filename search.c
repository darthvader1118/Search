#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "search.h"
#include "indexer.h"
#include "index.h"

/* Checks file validity of given inverted index */
int fileValid(FILE* fp) {
	char readin[256];
	/*Finds first occurrence of the needle '<list>' in the haystack readin*/
	if(!strstr(fgets(readin,256,fp), "<list>")) {
		fprintf(stderr, "Error: You have given an invalid input file.\n");
		return 0;
	}
	/*Take pointer back to beginning of file for future file operations*/
	rewind(fp);
	return 1;
}

/* Checks alphanumeric validity of given search term */
int termValid(char* word){
	for(; *word; ++word) {
		if (!isalnum(*word))
		return 0;
	}
	return 1;
}

/* Loads words from inverted index into hashtable */
void loadFile(hashTable* ht, FILE* fp){
	char *readin;
	char *tk, *word, *file;
		
	/*Name these steps based on their function*/
	enum steps {step1, step2, step3};
	
	int currstep = 0;
	int isFile;
	readin = (char *)malloc(256*sizeof(char));
	/*This while loop means this*/
	while(fgets(readin, 256, fp)) {
		switch(currstep) {
			/*Meaningful description for step1*/
			case step1:
				tk = strtok(readin, " \n");
				if(tk != NULL && strcmp(tk, "<list>") == 0) {
					word = strdup(strtok(NULL, " \n"));
					currstep = step2;
				}
			/*Meaningful description for step2*/
			case step2:
				while(fgets(readin, 256, fp)) {
					if(strcmp(readin, "") == 0)
						continue;
					else {
						for(isFile = 1, file = strtok(readin, " \n"); file != NULL; file = strtok(NULL, " \n"), isFile ^= 1) {
							if(isFile)
								InsertToTable(ht, word, file);
						}
					currstep = step3;
					break;
					}
				}
				break;
			/*Meaningful description for step3*/
			case step3:
				do{
					if(strcmp(readin, "</list>\n") == 0) {
						currstep = step1;
						/*printf("%d\n", currstep);*/
						/*free(word);*/
						word = NULL;
						word = realloc(word, 256);
						file = NULL;
						file = realloc(file, 256);
						break;
					}
				}while(fgets(readin, 256, fp));
				break;
			/*In case something bad happens*/
			default:
				exit(1);
		}
	}
}

/* Loops through hashtable until 'q' is inputted*/
void infiniteLoop(hashTable* ht) {
	char readin[256];
	char *term, *command;
	
	while(gets(readin)) {
		term = strdup(readin);
		command = strtok(readin, " ");
		
		if(command) {
			if(strcmp(command, "sa") == 0)
				sa(term, ht);
			else if(strcmp(command, "so") == 0)
				so(term, ht);
			else if(strcmp(command, "q") == 0) {
				free(term);
				break;
			}
			else{
				puts("did not put a valid input!");
			}
		}
		puts("Please enter next query or 'q' to exit: ");
	}

	/*free(term);*/
	
}

/* Checks hashtable presence of given search term and returns file list */
FileNode *getFilelist(char* word, hashTable* ht){
	int i;
	Node *nodeptr;
	i = GetIndex(word);
  		nodeptr = ht->buckets[i]->root;
  		if(nodeptr == NULL){
  			puts("word is not in inverted index");
  			return NULL;
  		}
  		while(nodeptr != NULL){
  			if (strcmp(word,nodeptr->value) == 0)
  			{
  				return nodeptr->info;
  				
  			}
  			nodeptr = nodeptr->next;
  		}
  return NULL;		
}

/* Search function with 'AND' logical operator */
int sa(char* str, hashTable* ht){
	char *word;
  	int count = 0;
  	Node* nodeptr;
	FileNode *fptr;
	LList *files = CreateLL();
	char *final = (char *)malloc(sizeof(1000));

	if (strcmp(strtok(str, " "), "sa") != 0) {
    	return 0;
    }
    while ((word = strtok(NULL, " "))) {
    /* goes through each filename for associated word */
    for (fptr = getFilelist(word, ht); fptr != NULL; fptr = fptr->next) {
    	if(fptr == NULL){
    		puts(" one of the words is not found in index");
    		return 0;
    	}
      	InsertLL(files, fptr->filename);
    }
    count++;
  }
for(nodeptr = files->root; nodeptr != NULL; nodeptr= nodeptr->next){
	if(nodeptr->freq == count){
		strcat(final, " ");
		strcat(final, nodeptr->value);
	}
}
if(final == NULL){
	puts("Output:");
	puts("no files found");
	return 0;
}
if (strcmp(final, "") == 0){
	puts("Output:");
	puts("no files found");
	return 0;
}
puts("Output:");
printf("%s\n", final);
free(final);
return 1;
}

/* Search function with 'OR' logical operator */
int so(char* input, hashTable* ht) {
	char* word;
	Node* nodeptr;
	FileNode *fptr;
	LList *files = CreateLL(); /*list that holds all the filnames*/
	
	
	if (strcmp(strtok(input, " "), "so") != 0) { 
    printf("I did not get 'so' inside sa function!!\n");
    return 0;
  	}
  	while ((word = strtok(NULL, " "))) { /*goes through each word in the input*/
  		fptr = getFilelist(word, ht); /*files the list of files associated with the word*/
  		/*if(fptr == NULL){
    		puts("word not found in index");
    		return 0;
    	}*/
  		if(fptr == NULL){ /*checks if that list is null, this should never happen but is here just in case*/
  			continue;
  		}
  		while(fptr != NULL){ /*parses through the filelist and puts it in the llist of filenames*/
  					InsertLL(files, fptr->filename);
  					fptr = fptr->next;
  		}
  	}
  	if(files->root == NULL){ /*cheacks if there is anything in the list*/
  		puts("no files found");
  		return 0;
  	}
  	puts("Output:");	
  	for(nodeptr = files->root; nodeptr != NULL; nodeptr= nodeptr->next){
  		printf("%s ", nodeptr->value); /*prints out all the associated filenames*/
  	}
  	printf("\n");
return 1; /*returns one on success*/

}


