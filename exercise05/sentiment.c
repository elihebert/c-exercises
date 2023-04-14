#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

#define MAP_SIZE (1 << 14)
#define BUFFER_SIZE 2048

// Represents the sentiment value for a single word/term.
typedef struct sentiment_t {
  double pos; // mean positive sentiment for the term
  double neg; // mean negative sentiment for the term
  int count;  // number of times the term appears
} sentiment_t;

// Map from string words/terms to sentiment_t values.
map_t *sentiments = NULL;

// Convert a string to lower case and strip whitespace and punctuation.
//
// Resizes string by nulling the unused suffix.
void lower_and_strip(char *str) {
  char *t = str; // insert point
  char *c = str; // current char
  for (; *c; c++) {
    if (isalnum(*c)) {
      // convert to lowercase; append at insert point.
      *t = tolower(*c); // t <= c
      t++;
    }
  }
  // c is null terminator; cut end of string after alnum chars.
  while (t < c) {
    *(t++) = '\0';
  }
}

// Print a single entry in a map string -> sentiment_t.
void *print_map_entry(const char *key, void *value) {
  sentiment_t *v = value;
  printf("%s : +%f -%f (%d)\n", key, v->pos, v->neg, v->count);
  return value;
}

// Free a single value from a map.
void *free_map_value(const char *key, void *value) {
  free(value);
  return NULL;
}

// Free all values in a map.
void free_map_values() { map_apply(sentiments, free_map_value); }

// Print all sentiments in a map string -> sentiment_t.
void dump_sentiments() { map_apply(sentiments, print_map_entry); }

// Build a map string -> sentiment_t by parsing input file.
//
// Modifies sentiments map.
// need to open input file, parse file line by line; for each
// non-comment file, tokenize line on tab characters to get fields,
// toeknize words field on spaces, extract each word from word#sense format
// call lower_and_strip to normalize word; check if word is in map already -
// if it is, update values with new averages and count, if not, create new
// struct on heap and put it in map with first values it read
// close input file
void build_sentiment_map(FILE *f) {
  // error checks for invalid file pointer(s)
  if (!f) {
    fprintf(stderr, "Error, invalid file pointer\n");
    exit(1);
  }

  // frees the existing sentiment map and creates a new one
  if (sentiments) {
    free_map_values();
    map_free(&sentiments);
  }
  sentiments = map_create(MAP_SIZE);

  char buff[BUFFER_SIZE];
  int line_no = -1;

  // reads input file line by line
  while (fgets(buff, BUFFER_SIZE, f) != NULL) {
    line_no++;
    // break if end of file is reached
    if (feof(f)) {
      printf("reached end of file at line%d\n", line_no);
      break;
    }

    // ignore comment lines starting with #
    if (buff[0] == '#') {
      continue;
    }

    // tokenize line to extract relevant fields like position, workId,
    // positiveScore, negativeScore, wordsStr
    char *pos = strtok(buff, "\t");
    char *wordId = strtok(NULL, "\t");
    char *positiveScoreStr = strtok(NULL, "\t");
    char *negativeScoreStr = strtok(NULL, "\t");
    char *wordsStr = strtok(NULL, "\t");

    if (!pos || !wordId || !positiveScoreStr || !negativeScoreStr ||
        !wordsStr) {
      fprintf(stderr, "Error: invalid format on line %d.\n", line_no);
      continue;
    }
    double positiveScore = atof(positiveScoreStr);
    double negativeScore = atof(negativeScoreStr);

    // iterate through each word_sense in wordsStr
    char *word_sense = wordsStr;
    // check if word is already in sentiment map; if so, update values, or
    // create new sentiment_t struct and add it to map ; move to next word_sense
    // in wordsStr
    while (word_sense != NULL && *word_sense != '\0') {
      char word[BUFFER_SIZE];
      char *hashPosition = strchr(word_sense, '#');
      if (hashPosition != NULL) {
        snprintf(word, hashPosition - word_sense + 1, "%s", word_sense);
      } else {
        break; // break loop if no '#' in word_sense
      }
      lower_and_strip(word);
      sentiment_t *sentiM;
      int inMap = map_get(sentiments, word, (void **)&sentiM);
      if (inMap) {
        sentiM->count++;
        sentiM->pos += (positiveScore - sentiM->pos) / sentiM->count;
        sentiM->neg += (negativeScore - sentiM->neg) / sentiM->count;

      } else {
        sentiM = (sentiment_t *)malloc(sizeof(sentiment_t));
        sentiM->pos = positiveScore;
        sentiM->neg = negativeScore;
        sentiM->count = 1;
        map_put(sentiments, word, sentiM);
      }
      wordsStr = NULL;
      word_sense += strcspn(word_sense, " \t");
      word_sense += strspn(word_sense, " \t");
    }
  }
}

// read input text from stdin, tokenize, and calculate sentiment score; print
// the results
void sentiment_stdin() {
  char buffer[BUFFER_SIZE];
  char input[BUFFER_SIZE];
  // read input lines from stdin
  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
    // copy input line and remove newline character
    // strncpy(input, buffer, BUFFER_SIZE);
    size_t buffer_len = strlen(buffer);
    if (buffer[buffer_len - 1] == '\n') {
      buffer[buffer_len - 1] = '\0';
    }
    strncpy(input, buffer, BUFFER_SIZE);
    // initialize sentiment score
    double sentiment = 0;
    // tokenize input line into words and calculate sentiment score
    char *word = strtok(input, " ");
    while (word != NULL) {
      lower_and_strip(word);
      sentiment_t *sentiM;

      // look up word in sentiment map; update score based on sentiment values,
      // and then move to next word
      if (map_get(sentiments, word, (void **)&sentiM)) {
        sentiment += sentiM->pos - sentiM->neg;
      }
      word = strtok(NULL, " ");
    }
    // prints input line and sentiment score
    printf("%s : %f\n", buffer, sentiment);
  }
}

void usage(char *name) {
  printf("Usage: %s <sentiment_file.txt>\n", name);
  exit(1);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Must supply filename.\n");
    usage(argv[0]);
  }

  FILE *f = fopen(argv[1], "r");
  if (!f) {
    perror("fopen");
    exit(1);
  }
  fprintf(stderr, "loading sentiment map...");
  // Build the map of sentiments...
  build_sentiment_map(f);
  fprintf(stderr, "loaded.\n");
  // if you want to get sentiments out of your map in order to debug:
  // dump_sentiments();
  // Read sentiments from standard in...
  sentiment_stdin();

  // free memory allocated to sentiment map
  free_map_values();
  map_free(&sentiments);
}
