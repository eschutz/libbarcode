/**
*     @file generator.c
*     @brief generator.c contains definitions of functions defined in generator.h.
*
*     @author Elijah Schutz
*
*     @date 10/12/18
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "generator.h"

extern int errno;

const struct Guard null_guard = { '\0', 0, -1 };

const struct Symbology1D upc_a = {
  .patterns = 15,
  .digits = 12,
  .digit_width = 7,
  .code = { 0x72, 0x66, 0x6c, 0x42, 0x5c, 0x4e, 0x50, 0x44, 0x48, 0x74 },
  .invert = 6,                     // Invert patterns after the middle guard
  .guards = { { 0x05, 3, 0  },   // Start
              { 0x05, 3, 14 },   // End
              { 0x0a, 5, 7  } }  // Middle
};

int guard_eql(Guard a, Guard b) {
  return (a.code == b.code) && (a.width == b.width) && (a.index == b.index);
}

int symb_width(Symbology symb) {
  int guard_width_t = 0;
  for (int i = 0; i < sizeof(symb.guards) / sizeof(*symb.guards); i++) {
    guard_width_t += symb.guards[i].width;
  }
  return symb.digits * symb.digit_width + guard_width_t;
}

int indexof(int *array, int len, int value) {
  for (int i = 0; i < len; i++) {
    if (array[i] == value) {
      return i;
    }
  }
  return -1;
}


int encode(Symbology symb, uint8_t *id, size_t id_size, uint8_t *dest, size_t dest_size) {
  if (id_size != symb.digits) {
    fprintf(stderr, "identification number is of length %d; must be %d-digits (chars) long", (int) id_size, symb.digits);
    return -1;
  }
  if (dest_size != symb.patterns) {
    fprintf(stderr, "destination array is of length %d; must be of length %d", (int) dest_size, symb.patterns);
    return -1;
  }

  int status = 0;
  int guard_no = 0;
  int *guard_idxs = malloc(sizeof *guard_idxs * MAX_GUARDS);

  for (int i = 0; i < MAX_GUARDS; i++) {
    Guard guard = symb.guards[i];
    if (!guard_eql(guard, null_guard)) {
      guard_no++;
      guard_idxs[i] = guard.index;
      dest[guard.index] = guard.code;
    }
  }

  if (guard_no != MAX_GUARDS) {
    guard_idxs = realloc(guard_idxs, sizeof *guard_idxs * guard_no);
  }

  int digit_index = 0;
  for (int i = 0; i < symb.patterns; i++) {
    // Skip previously assigned positions (guards)
    if (indexof(guard_idxs, guard_no, i) != -1) {
      continue;
    }

    // Input id should be an array of chars from 0-9 inclusive
    if (id[digit_index] > 9) {
      errno = EDOM;
      fprintf(stderr, "errno: %d\n", errno);
      fprintf(stderr, "%d is not a single-digit number\n", id[digit_index]);
      perror("numbers of two digits or more are forbidden");
      status = -1;
      break;
    }
    dest[i] = symb.code[(int) id[digit_index]];
    digit_index++;
  }

  free(guard_idxs);
  return status;
}

// Unicode mapping a single binary barcode pattern to a black or white square
// 0 => ■
// 1 => ' '
int bar(uint8_t *pat) {
  // 0xe296a0 -> ■
  // 0x20     -> space
  int res = (*pat & 1) == 0 ? 0xe296a0: 0x20;
  *pat >>= 1;
  return res;
}

int barcode_unicode(Symbology symb, uint8_t *barcode, size_t bc_size, int *dest, int dest_size) {
  int w = symb_width(symb);
  if (dest_size != w) {
    fprintf(stderr, "destination array is of length %d; must be %d patterns (chars) long", dest_size, w);
    return -1;
  }

  if (bc_size != symb.patterns) {
    fprintf(stderr, "barcode is of length %d; must be %d patterns (chars) long", (int) bc_size, symb.patterns);
    return -1;
  }

  int guard_no = 0;
  int *guard_idxs = malloc(sizeof *guard_idxs * MAX_GUARDS);

  for (int i = 0; i < MAX_GUARDS; i++) {
    Guard guard = symb.guards[i];
    if (!guard_eql(guard, null_guard)) {
      guard_no++;
      guard_idxs[i] = guard.index;
    }
  }

  if (guard_no != MAX_GUARDS) {
    guard_idxs = realloc(guard_idxs, sizeof *guard_idxs * guard_no);
  }

  int bar_idx = 0;
  for (int i = 0; i < symb.patterns; i++) {
    uint8_t pattern = barcode[i];
    int idx = indexof(guard_idxs, guard_no, i);
    if (idx != -1) {
      Guard guard = symb.guards[idx];
      for (int j = 0; j < guard.width; j++) {
        dest[bar_idx + j] = bar(&pattern);;
      }
      bar_idx += guard.width;
    } else {
      if (i > symb.invert) {
        pattern <<= 1;       // Eliminate the leading 0
        pattern = ~pattern;  // Invert pattern
        pattern >>= 1;       // Restore the leading 0 (and eliminate the trailing 1)
      }

      for (int j = 0; j < symb.digit_width; j++) {
        dest[bar_idx + j] = bar(&pattern);
      }
      bar_idx += symb.digit_width;
    }

  }
  free(guard_idxs);
  return 0;
}
