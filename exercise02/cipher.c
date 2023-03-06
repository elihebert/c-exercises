
#include <stdio.h>
#include <string.h>

// should be a program that accepts a 26-character string that represents a
// substitution cipher, which apparently is a method of encrypting in which
// units of plaintext are replaced with the ciphertext, in a defined manner,
// with the kelp of a key; the "units" may be single letters, pairs of letters,
// triplets of letters, mixtures of them all, etc. The reciever deciphers the
// text by performing the inverse substitution process to extract the original
// message.

int main() {
  char cipherSub[27];
  printf("Enter substitution pad: ");
  fgets(cipherSub, 27, stdin);
  cipherSub[strcspn(cipherSub, "\n")] = '\0'; // remove newline
  // input validation
  if (strlen(cipherSub) != 26) {
    printf("Invalid substituion pad; needs 26 unique characters\n");
    return 1;
  }

  // without this line the program was exiting prematurely;
  // clears input buffer
  while (getchar() != '\n')
    ;
  // loop that encrypts/decrypts
  char res[1024];
  printf("Enter what you want enciphered or deciphered: ");
  fgets(res, 1024, stdin);
  res[strcspn(res, "\n")] = '\0'; // remove the newline from res

  int resLength = strlen(res);
  for (int i = 0; i < resLength; i++) {
    if (res[i] >= 'a' && res[i] <= 'z') {
      res[i] = cipherSub[res[i] - 'a'];
    } else if (res[i] >= 'A' && res[i] <= 'Z') {
      res[i] = cipherSub[res[i] - 'A'] - ('a' - 'A');
    }
  }
  printf("%s\n", res);
  return 0;
}
