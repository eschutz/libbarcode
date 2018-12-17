#include <stdio.h>
#include <string.h>

#include "symb.h"

char _str[] = "12345678";

int main(void) {
    init_barcode();
    int _len = strlen(_str);
    char str[_len];
    memcpy(str, _str, _len); // Remove null terminator
    Code128 *result;
    int status = c128_encode((uchar *)str, _len, &result);
    printf("Status: %d\n", status);
    for (int i = 0; i < result->length; i++) {
        printf("0x%x ", result->data[i]);
    }
    printf("\n");
}
