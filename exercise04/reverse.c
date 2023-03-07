#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function begins by allocating array of pointers to strings, while keeping
// track of the current size and the maximum capacity of the array (at that
// point); enters loop that begins to read lines from standard input using fgets
// while allocating memory for said lines using malloc, and adds it to the
// array; if the array is full, the realloc function is used to double its
// capacity; after all input has been read and stored, the function allocates an
// array to hold the intergers, and then uses strtok and atoi to split each line
// into integers (tokens) and stores them in the array. The final part of the
// function prints out the integers in the order which they were read from the
// array/stored, which is reversed; assumes the input contains only valid
// integers that are separated by spaces; no line of input can be over 99
// characters

void readAndPrint() {
  int cap = 1;
  int size = 0;
  // allocates block of memory dynamically, with size of cap times the size of a
  // pointer to a character determined by sizeof; result is founter to first
  // byte of block of memory; by alocating cap times char* pointers, it uses
  // lines to store an array of strings, with each pointer pointing to the first
  // character of the string
  char **linesR = malloc(cap * sizeof(char *));

  while (1) {
    char *lineR = malloc(100 * sizeof(char));
    if (fgets(lineR, 100, stdin) == NULL) {
      break;
    }
    if (size == cap) {
      cap *= 2;
      linesR = realloc(linesR, cap * sizeof(char *));
    }
    linesR[size] = lineR;
    size++;
  }

  int *numbers = malloc(size * sizeof(int));
  int numbers_amount = 0;

  for (int i = size - 1; i >= 0; i--) {
    char *lineR = linesR[i];
    char *token = strtok(lineR, " ");

    while (token != NULL) {
      int number = atoi(token);
      numbers[numbers_amount] = number;
      numbers_amount++;
      token = strtok(NULL, " ");
    }

    free(lineR);
  }

  for (int i = 0; i < numbers_amount; i++) {
    printf("%d ", numbers[i]);
  }

  printf("\n");

  free(numbers);
  free(linesR);
}

int main() {
  readAndPrint();

  return 0;
}
