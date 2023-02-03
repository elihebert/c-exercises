#include <stdio.h>

// program that computes the total number of iterations for (user-inputted)
// numbers to complete their Collatz sequences

int collatzSum(int n) {
  int steps = 0;
  while (n > 1) {
    steps++;
    if (n % 2 == 0) {
      n /= 2;
    } else {
      n = 3 * n + 1;
    }
  }
  return steps + 1;
}

int main() {
  int n, steps0;
  printf("Enter a number: ");
  scanf("%d", &n);
  steps0 = collatzSum(n);
  printf("%d -> %d", n, steps0);
  return 0;
}
