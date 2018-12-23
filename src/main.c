#include <stdio.h>
#include <string.h>

#include "symb.h"
#include "graphic.h"

char _str[] = "abc123HELLO";

int main(void) {
    init_barcode();
    int _len = strlen(_str);
    char str[_len];
    memcpy(str, _str, _len); // Remove null terminator
    Code128 *result;
    int status = c128_encode((uchar *)str, _len, &result);
    // for (int i = 0; i < result->length; i++) {
    //     printf("0x%x ", result->data[i]);
    // }

    char *svg;
    c128_svg(result, &svg);
    printf("%s\n", svg);
}
