#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "indexer.h"
#include "search.h"
#include "index.h"

int main(int argc, char **argv) {
  FILE *fp;
  hashTable* ht;

  /* Checks existentiality of given file */
  if (access(argv[1], F_OK) == -1) {
    printf("Input file does not exist\n");
    return 1;
  }

  fp = fopen(argv[1], "r");
  if (!fileValid(fp)) {
    return 1;
  }



  ht = CreateTable();

  loadFile(ht, fp);

  printf("everything in menu\n");

  infiniteLoop(ht);


  fclose(fp);

  /*freeHashTable(ht);*/
  return 0;
}
