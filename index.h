#ifndef INDEX_H
#define INDEX_H

typedef struct _hashTable {
	struct _LList **buckets;
	int size;
} hashTable;

typedef struct _LList {
	struct _Node *root;
	int size;
} LList;

typedef struct _Node {
	char *value;
	struct _Node *next;
	struct _FileNode *info;
} Node;

typedef struct _FileNode{
	char *filename;
	int occ;
	struct _FileNode *next;
}FileNode;

void Fparse(char *filename, hashTable *ftable);
int WriteToFile(char * invfile, hashTable *hash);
static void dir_traversal(char *path, hashTable *table);
void freeHashTable(hashTable *hash);
void printTable(hashTable *hash);
void printLL(hashTable *hash, int i);
char *FormatString(char *input);

#endif
