/*
 * @file       util.c
 * @brief      Assignments of definitions from util.h.
 * @author     Elijah Schutz
 * @date       12/12/18
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "util.h"

// Linear search
int indexof(char *arr, int len, char elem) {
    for (int i = 0; i < len; i++) {
        if (elem == arr[i]) {
            return i;
        }
    }
    return -1;
}

bool isdigits(char *str, int len) {
    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

char *slice(char *str, int start, int len) {

    char *res = malloc(len);
    VERIFY_NULL(res, len);

    memcpy(res, str + start, len); // memcpy to include nuls in the middle of the string

    return res;
}
