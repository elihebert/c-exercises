#include <stdio.h>
#include <string.h>
// program that repeatedly reads strings from standard input and then uses a
// function to reverse a string; termination condition is "stop"

// reverses through the string supplied by user
void reverseString(char *wordD) {
  int length = strlen(wordD);
  for (int i = length - 1; i >= 0; i--) {
    printf("%c", wordD[i]);
  }
  printf("\n");
}

int main() {
  char wordD[50];
  while (1) {
    printf("Enter a word (ctrl-C or STOP to quit): ");
    scanf("%s", wordD);
    if (strcmp(wordD, "STOP") == 0) {
      break;
    }
    printf("%s -> ", wordD);
    reverseString(wordD);
  }
  return 0;
}
