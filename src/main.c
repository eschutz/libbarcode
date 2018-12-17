#include <stdio.h>
#include <string.h>

#include "symb.h"


char _str[] = "abc123";

int main(void) {
    init_barcode();
    return 0;
    int _len = strlen(_str);
    char str[_len];
    memcpy(str, _str, _len); // Remove null terminator
    Code128 **empty;
    c128_encode((uchar*) str, _len, empty);
}
