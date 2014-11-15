CC=gcc
CFLAGS=-g -Wall -pedantic
CFILES= search.c main.c indexer.c


all: index search

search:
	$(CC) $(CFILES) $(CFLAGS) -o search

index:
	$(CC) $(CFLAGS) index.c indexer.c -o index
	
clean:
	rm search index
