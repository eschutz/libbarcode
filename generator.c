#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "generator.h"

extern int errno;

int codelen(Symbology symb) {
 return strlen(symb.code);
}

// for each digit, map it to a predefined char representing the bit map of that barcode digit
int encode(Symbology symb, char[] id, char[] dest) {
  int maxlen = codelen(symb);

  if (strlen(id) != maxlen) {
    fprintf(stderr, "identification number must be 12-digits (chars) long");
    return -1;
  }
  if (strlen(dest) != symb.patterns) {
    fprintf(stderr, "destination array must be of length %d", symb.patterns);
    return -1;
  }

  int status = 0;

  dest[0] = symb.start;
  dest[maxlen - 1] = symb.end;
  int middle = maxlen / 2 - (maxlen % 2 == 0 ? 1 : 0);
  dest[middle] = symb.middle;

  for (int i = 1; i < maxlen - 1; i++) {
    if (i == middle) {
      continue;
    }
    if (id[i] > 9) {
      errno = EDOM;
      fprintf(stderr, "errno: %d\n", errno);
      fprintf(stderr, "%d is not a single-digit number", id[i]);
      perror("numbers of two digits or more are forbidden");
      status = -1;
      break;
    }
    dest[i] = symb.code[i];
  }
  return status;
}
