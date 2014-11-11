Assignment 3 -- Indexer
========
Ian Lozinski (iml22), Edward Zaneski(epz5)
CS 214

The goal of the assignment was to implement a search utility which would take a
filename of an inverted-index file as its argument. It would load the data from
the file into memory and allow the following queries to be done on it:

---
  so \<term(s)\>
    returns the filenames of files that contain ANY of the terms
  sa \<term\>
    returns the filenames of files that contain ALL of the terms
---

Before settling on one, we analyzed the runtimes of
a few different data structures. The naive approach would be to keep a sorted
linked list, and keeping a second linked list for each file associated with every
word. Obviously, the worst case runtime of this would be O(n^2) and thus, very
inefficient. We quickly discarded that idea and considered using a hashtable.
It seemed like a good idea at first because of it's O(n) insertion time, but
in order to get everything in order, we'd need to throw every tuple,
(word, filename(s), count), into an array and then sort it using either mergesort
or quicksort, yielding O(nlogn). We still weren't satisfied with this. 
Finally, we decided to implement a prefix-tree. Every prefix-tree node has
a 36-index array of pointers to other nodes. Index 0 through 9 represent digits,
and index 10-35 hold letters. Each word of length w added yields an insertion
time of O(w), which is practically constant.

###This is what happens when the word "cat" is read in:
  * Starting at the root of the prefix-tree, we see 'c'.

  * Since 'c' is the third character in the alphabet, we check index 12.
    - If index 12 has a pointer stored, we follow it to the next node
    - Otherwise, we create a new node, store the pointer to it, then follow it.

  * The next character 'a' is read in and we do the same thing, but starting
    at the node we just traversed to.

  * Same thing for 't'.

  * Now that we are out of character, we know that we have complete word stored
    in the tree.

  * The filename is also stored in a prefix tree in the exact same way,
    except we additionally have an associated count at leaf nodes.


###Complexity analysis:
  Inserting n words to the tree takes O(n) time.
  Searching for a word in memory (1).