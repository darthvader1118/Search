#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "index.h"
#include "search.h"


int main(int argc, char **argv) {
  FILE *rf; /* read file */
  tnode trie;

  /* Check if file exists */
  if (access(argv[1], F_OK) == -1) {
    printf("Input file does not exist\n");
    return 1;
  }

  rf = fopen(argv[1], "r");
  if (!fileIsValid(rf)) {
    return 1;
  }

  trie = createTree();
  loadFileToTree(trie, rf);


  /* Start menu loop */
  menu();

  doSearchLoop(trie);


  fclose(rf);
  destroyTree(trie);
  destroy_list(FILES);

  return 0;
}