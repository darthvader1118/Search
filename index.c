#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

#include "indexer.h"
#include "index.h"

void Fparse(char *filename, hashTable *ftable){
  FILE *stream;
  char *contents; 
  char *token;
  int j, size;
  stream = fopen(filename, "rb+");
  if ( stream != NULL ){

    fseek(stream, 0L, SEEK_END);
    size = (int)ftell(stream);
    rewind(stream);
    contents = (char *)malloc(size +1);
    for(j = 0; j <(size/sizeof(char)); j++){
      contents[j] = fgetc(stream);
    }
    fclose(stream);
    contents = FormatString(contents);
  
  token = strtok(contents, " ");
  while(token != NULL){
    InsertToTable(ftable, token, filename);
    
    token = strtok(NULL, " ");
    }
  }
  
}
int WriteToFile(char * invfile, hashTable *hash){
  FILE *fp;
  int i;
  LList *ll;
  Node *ptr;
  FileNode *fnp;
  fp = fopen(invfile, "wb+");
  if(fp == NULL){
    return 0;
  }
  if(hash == NULL){
    return 0;
  }
 
  
  for(i = 0; i < 36; i++){
    ll = hash->buckets[i];
    
    if(ll == NULL){
      continue;
    }
    ptr = ll->root;
    if(ptr == NULL){
      continue;
    }
    while(ptr != NULL){

      fprintf(fp,"<list> %s\n",ptr->value);
      fnp = ptr->info;
      while(fnp !=NULL){
        fprintf(fp, "%s %d ",fnp->filename,fnp->occ);
        fnp = fnp->next;
      }
      fprintf(fp,"\n</list>\n");
      ptr = ptr->next;
    }
  }
  fclose(fp);
  return 1;
}

void dir_traversal(char *path, hashTable *table) {

  /*Declare directory & file variables*/
  DIR *dir; struct dirent *fil; size_t fil_pathsize;
  char *fil_path, *fil_name;

  /*Tells user what directory is currently being accessed*/
  printf("Current directory: %s\n", path);

  /*Error checking*/
  if(!(dir=opendir(path)))
    puts("Invalid Directory");
  /*While the directory can be read, the function continues to traverse*/
  while((fil = readdir(dir))){
    /*Extract filename from fil variable*/
    fil_name = (fil->d_name);
    
    /*Continue to next instance of while loop if file is not an actual file*/
    if(strcmp(fil_name, ".") == 0 || strcmp(fil_name, "..") == 0)
      continue;
    /*Designate size for the filepath variable*/
    fil_pathsize = (2 * sizeof(char)) + strlen(fil_name) + (strlen(path));
    /*Declaration & memory allocation for filepath variable*/
    fil_path = malloc(fil_pathsize);
    /*Copy the variable contents of path to the filepath variable*/
    strcpy(fil_path, path);
    /*Add forward slash character to the filepath variable*/
    strcat(fil_path, "/");
    /*Add filename variable contents to the filepath variable*/
    strcat(fil_path, fil_name);
    /*Recursive call to dir_traversal method if fil contains a directory*/
    if(fil->d_type == DT_DIR){
      dir_traversal(fil_path,table);
    /*Calls file parsing function if fil contains a regular file*/
    }else if(fil->d_type == DT_REG){
      Fparse(fil_path, table);
      continue;
    }else
      puts("error");
    /*Frees memory for filepath variable*/
    /*free(fil_path);*/
  }
  /*Closes the current directory after all its directories are traversed & files are parsed*/  
  closedir(dir);
}
void freeHashTable(hashTable *hash){
  int i;
  LList *ll;
  Node *ptr;
  for(i = 0; i < 36; i++){
    ll = hash->buckets[i];
      if(ll == NULL){
      free(ll);
      continue;
    }
    ptr = ll->root;
    if(ptr == NULL){
      free(ptr);
      continue;
    }
    while(ptr != NULL){

      
      FileNode *fnp = ptr->info;
      while(fnp !=NULL){
        free(fnp->filename);
        
        fnp = fnp->next;
      }
      free(ptr->value);
      free(ptr->info);
      free(ptr);
      ptr = ptr->next;
    }
    free(ll);
  }
  free(hash);
}
void printTable(hashTable *hash){
  int i;
  LList *ll;
  Node *ptr;
  FileNode *fnp;
  for(i = 0; i < hash->size; i++){
    ll = hash->buckets[i];
    
    if(ll == NULL){
      free(ll);
      continue;
    }
    ptr = ll->root;
    if(ptr == NULL){
      
      continue;
    }
    while(ptr != NULL){

      printf("%s\n", ptr->value);
      fnp = ptr->info;
      while(fnp !=NULL){
        printf("%s\n", fnp->filename);
        
        fnp = fnp->next;
      }
      
      ptr = ptr->next;
    }
    /*free(ll);*/
  }
  /*free(hash);*/
}


/*char *FNToString(FileNode *fn){
  char *fname;
  char *res;
  char *intstring;
  intstring = (char *)malloc(sizeof(char)*8);
  fname = fn->filename;
  sprintf(intstring, "%d", fn->occ);
  res = strcat(res, fname);
  res = strcat(res, " ");
  res = strcat(res, intstring);
  return res;
}
*/
void printLL(hashTable *hash, int i){
  LList *ll = hash->buckets[i];
  Node *ptr = ll->root;
  if(ptr == NULL){
    puts("empty linked list");
    return;
  }
  while(ptr != NULL){
    printf("%s\n", ptr->value);
    ptr = ptr->next;
  }
}
int main(int argc, char **argv){
  char *path = argv[2];
  char *inv = argv[1];
  hashTable *ht = CreateTable();
  struct stat s;

  if( stat(path,&s) == 0 )
  {
      if( s.st_mode & S_IFDIR )
      {
          /*it's a directory*/
          dir_traversal(path,ht);
      }
      else if( s.st_mode & S_IFREG )
      {
          /*it's a file*/
          Fparse(path, ht);
      }
      else
      {
          /*something else*/
          puts("Did not type in a valid directory or file");
      }
  }
  else
  {
      /*error*/
      puts("Try again");
  }
  WriteToFile(inv, ht);
  puts("file successfully written");
  /*printTable(ht);*/
  /*freeHashTable(ht);*/ 

  
return 0;
}
