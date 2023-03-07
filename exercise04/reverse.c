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

//
//
//
//
//
//
int main() {
  readAndPrint();

  return 0;
}

//
//
//
//
//
