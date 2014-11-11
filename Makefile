CC=gcc
CFLAGS=-pedantic -Wall
CFILES=search.c main.c


all: index search

search:
	$(CC) $(CFILES) $(CFLAGS) -o search

index:
	$(CC) $(CFLAGS) indexer.c -o index

searchdebug:
	$(CC) -g $(CFILES) $(CFLAGS) -o search

indexdebug:
	$(CC) -g $(CFLAGS) indexer.c -o index

debug: indexdebug searchdebug

clean:
	rm search index
