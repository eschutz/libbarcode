#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "graphic.h"
#include "symb.h"

#define STRINGS 6

char strings[STRINGS][C128_MAX_DATA_LEN] = {"abc123HELLO", "ThisIsABarcode!",
                                            "~/Documents/Program", "Thu 9:25 pm",
                                            "alterations to reci", "int c128_ps_header"};

int main(void) {
    init_barcode();
    char texts[STRINGS][C128_MAX_DATA_LEN];
    // Code128 *codes[STRINGS];
    Code128 **codes = calloc(1, sizeof **codes);
    for (int i = 0; i < STRINGS; i++) {
        int len = strlen(strings[i]);
        memcpy(texts[i], strings[i], len); // Remove null terminator
        c128_encode((uchar *)texts[i], len, &codes[i]);
    }

    char * ps;
    Layout layout = {.rows = 6, .cols = 1};
    c128_ps_layout(codes, STRINGS, &ps, &PS_DEFAULT_PROPS, &layout);
    printf("%s\n", ps);
    free(ps);
    free(codes);
    return 0;
}
