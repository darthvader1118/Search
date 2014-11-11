// Krishna Yellayi and Arnesh Sahay
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include "indexer.h"
#include <unistd.h>
#include <errno.h>



int GetIndex(char *word) {
	char firstLetter = word[0];
	if(!word) return -1;
	if(isalpha(firstLetter)){
		firstLetter = tolower(firstLetter);
		if (firstLetter != '\0') {
			return (firstLetter - 'a');
		} else {
			return -1;
		}
	}
	else if(isdigit(firstLetter)){
		return (firstLetter-'0') + 26;
	}
	else{
		return -1;
	}
}
FileNode *CreateFileNode(char *file){
	FileNode *fn = (FileNode *)malloc(sizeof(FileNode));
	fn->filename = file;
	fn->occ =1;
	fn->next = NULL;
	return fn;
}
//CreateNode Needs to be edited 10/16/14 add filename
Node *CreateNode(char *word) {
	word = lowerCase(word);
	Node *new_Node = (Node *)malloc(sizeof(Node));
	//printf("Creating node with: %s\n", word);
	new_Node->info = NULL;
	new_Node->value = word;
	new_Node->next = NULL;
	new_Node->info = NULL;//need to change this
	new_Node->freq = 1;
	return new_Node;
}

LList *CreateLL() {
	LList *new_LL = (LList *)malloc(sizeof(LList));
	new_LL->size = 0;
	new_LL->root = NULL;
	return new_LL;
}

void InsertLL(LList *list, char *word){
	Node *newNode = CreateNode(word);
	if(!list){
		list = CreateLL();
		newNode = list->root;
		return;
	}
	if(list->root == NULL){
	   list->root = newNode;
	   return;
	}
	Node *listptr = list->root;
	while(listptr!= NULL){
		if(strcmp(word, listptr->value) == 0){
			listptr->freq++;
			return;
		}
		if(listptr->next == NULL){
			listptr->next = newNode;
			return;
		}
		listptr = listptr->next;
	}
	puts("error did not work correctly");
	return;
}
hashTable *CreateTable() {
	hashTable *new_table = (hashTable *)malloc(sizeof(hashTable));
	new_table->buckets = (LList **)malloc(sizeof(LList *) * 36);
	new_table->size = 36;
	return new_table;
}
char *lowerCase(char *word){
	int i;
	i=0;
	for(i = 0; i < strlen(word); i++){
		word[i] = tolower(word[i]);
	}
	return word;
}
void InsertToTable(hashTable *hash, char *word, char *filename){
	int i;
	i = GetIndex(word);
	Node *ptr;
	
	if(!hash){
		return;
	}
	if(i == -1){
		return;
	}
	if(hash->buckets[i] == NULL){
		hash->buckets[i] = CreateLL();
		hash->buckets[i]->root = CreateNode(word);
		hash->buckets[i]->size++;
		hash->buckets[i]->root->info = CreateFileNode(filename);
		//printLL(hash, i);
		//printf("%s\n",hash->buckets[i]->root->value);
		//printTable(hash);

		//need to add more here
		return;
	}else{
		ptr = hash->buckets[i]->root;

		while(ptr != NULL){
			if(strcmp(lowerCase(word),ptr->value)== 0){
				FileNode *fptr = ptr->info;
				while(fptr != NULL){
					if(strcmp(filename,fptr->filename)==0){
						fptr->occ++;
						/*if(isalpha(ptr->value[0])){
							if(ptr->value[0] - 'a' != i){
								puts("error aborting now");
								exit(0);
							}
						}
							else if(isdigit(ptr->value[0])){
								if((ptr->value[0]-'0') + 26 != i){
									puts("error aborting now");
									exit(0);
								}
							}
								else
									return;*/
					return;
					}
					else if(strcmp(filename, fptr->filename) < 0){
						FileNode *newf = CreateFileNode(filename);
						newf->next = fptr;
						fptr  = newf;
						//puts(newf->filename);
						//printLL(hash, i);
						/*if(isalpha(ptr->value[0])){
							if(ptr->value[0] - 'a' != i){
								puts("error aborting now");
								exit(42);
							}
						}
							else if(isdigit(ptr->value[0])){
								if((ptr->value[0]-'0') + 26 != i){
									puts("error aborting now");
									exit(42);
								}
							}
								else
									return;*/
					return;
					
					}
					else{
						if(fptr->next == NULL){
							FileNode *newf = CreateFileNode(filename);
							fptr->next = newf;
							return;
						}
					}
				fptr=fptr->next;
			}
		}
		else if(strcmp(lowerCase(word), ptr->value) < 0){
				Node *newptr = CreateNode(word);
				ptr->next = newptr;
				ptr  = newptr;
				return;
		}
		else{
			if(ptr->next == NULL){
				Node *newOne = CreateNode(word);
				FileNode *fn = CreateFileNode(filename);
				newOne->info = fn;
				ptr->next = newOne;
				hash->buckets[i]->size++;
				//printLL(hash, i);
				//printf("%s\n",ptr->value);
				//printTable(hash);
				/*if(isalpha(ptr->value[0])){
								if(ptr->value[0] - 'a' != i){
									puts("error aborting now");
									exit(42);
								}
							}
								else if(isdigit(ptr->value[0])){
									if((ptr->value[0]-'0') + 26 != i){
										puts("error aborting now");
										exit(42);
									}
								}
								else
									return;*/
		return;
		}
	}
		ptr = ptr->next;
		}
	}
}
char *getseparators(char *string){
	char *delims;
	delims = (char *)malloc(100*sizeof(char));
	int i,j;
	j = 0;
	for(i = 0; string[i] != '\0'; i++){
		if(!isalnum(string[i])){
			if(j == 99){
				delims = realloc(delims, 2*sizeof(delims));
			}
			delims[j] = string[i];
			j++;
		}
	}
	delims = realloc(delims, (j+1)*sizeof(char));
	return delims;
}
char *FormatString(char *input){
	int i;
	for(i = 0; i < strlen(input); i++){
		if(!isalnum(input[i])){
			input[i] = ' ';
		}
	}
	return input;
}


