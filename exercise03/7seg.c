#include <stdio.h>
#include <string.h>

// program that repeatedly reads integers from standard input and displays them
// as digits in a 7 segment display

const char ssdNums[10][7][4] = {
    {" _ ", "| |", "|_|"}, // 0
    {"   ", "  |", "  |"}, // 1
    {" _ ", " _|", "|_ "}, // 2
    {" _ ", " _|", " _|"}, // 3
    {"   ", "|_|", "  |"}, // 4
    {" _ ", "|_ ", " _|"}, // 5
    {" _ ", "|_ ", "|_|"}, // 6
    {" _ ", "  |", "  |"}, // 7
    {" _ ", "|_|", "|_|"}, // 8
    {" _ ", "|_|", " _|"}  // 9
};

int main() {

  char input[100];

  int numDigits[10];
  int digitNow;

  while (1) {
    digitNow = 0;

    printf("Enter a number: ");
    fgets(input, sizeof(input), stdin);

    // convert string input to array of digits
    for (int i = 0; input[i] != '\0' && input[i] != '\n'; i++) {
      if (digitNow >= 10) {
        printf("Too many digits\n");
        return 1;
      }
      if (input[i] < '0' || input[i] > '9') {
        printf("Invalid input\n");
        return 1;
      }
      numDigits[digitNow++] = input[i] - '0';
    }

    // display as 7-segment
    for (int i = 0; i < 7; i++) {
      for (int x = 0; x < digitNow; x++) {
        int digitOn = numDigits[x];
        if (digitOn < 0 || digitOn > 9) {
          printf("invalid");
          return 1;
        }
        printf("%s ", ssdNums[digitOn][i]);
      }
      printf("\n");
    }
  }
  return 0;
}
