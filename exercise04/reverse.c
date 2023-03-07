#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function begins by allocating array of pointers to strings, while keeping
// track of the current size and the maximum capacity of the array (at that
// point); enters loop that begins to read lines from standard input using fgets
// while allocating memory for said lines using malloc, and adds it to the array
void readAndPrint() {
  int cap = 1;
  int size = 0;
  char **linesR = malloc(cap * sizeof(char *));
}

int main() { return 0; }
