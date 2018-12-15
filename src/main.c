#include <stdio.h>
#include <string.h>

#include "symb.h"

/*

----|----|----|----|----|----|----|----|----|----|----|----|----|----
... 93   94   95   |-----------------------------|    103  104  105 ...
                   96   97   98   99   100  101  102
*/


char _str[] = "\1\2\38654321 he 12345";

int main(void) {
    init_barcode();
    printf("ASCII | INDEX\n");
    for (int i = 0; i < 103; i++) {
        printf("%d  | %d\n", C128_B[i], i);
    }
    return 0;

    /*
    int _len = strlen(_str);
    char str[_len];
    memcpy(str, _str, _len); // Remove null terminator
    Code128 **empty;
    int len = encode(str, _len, empty);
    printf("%d\n", len);
    */
}

/*
Start A
\1
\2
\3
Code C
86
54
32
1
Code B

h
e

Code C
12
34
5
Check
Stop
*/
