#include <stdio.h>
#include <stdlib.h>
#include "generator.h"


int main(int argc, const char* argv[]) {
  uint8_t id[] = "471936178205";
  uint8_t *pattern = calloc(upc_a.patterns, sizeof *pattern);
  int dest_len = symb_width(upc_a);
  size_t dest_size = sizeof(int) * dest_len;
  int *dest = malloc(dest_size);

  encode(upc_a, id, upc_a.digits, pattern, upc_a.patterns);

  barcode_unicode(upc_a, pattern, upc_a.patterns, dest, dest_len);

  char *barcode = malloc(dest_len * 3);
  int stridx = 0;
  for (int i = 0; i < dest_len; i++) {
    printf("%x\n", dest[i] & 0xff);

    barcode[stridx] = dest[i] & 0xff;
    stridx++;
    printf("'%x'\n", barcode[stridx]);

    // for (int j = 3; j > 0; j--) {
    //   char chr = (dest[i] >> j * 8) & 0xff;
    //   if (chr == 0) {
    //     break;
    //   }
    //   printf("%c\n", chr);
    //   barcode[stridx] = chr;
    //   stridx++;
    // }
  }
  if (stridx < dest_len * 3) {
    barcode = realloc(barcode, stridx);
  }
  printf("%d\n", stridx);
  printf("%s\n", barcode);

  return 0;
}
