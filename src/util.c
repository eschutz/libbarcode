/* Copyright (C) 2019 Elijah Schutz */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

/**
 *      @file util.c
 *      @brief Definitions of general utility functions used in this project.
 *      @author Elijah Schutz
 *      @date 12/12/18
 */

#include "barcode/util.h"

#include "barcode/errors.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool isdigits(char * str, int len) {
    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

/**
 *      @detail Since some barcode symbologies allow for NUL characters, slice() really operates on
 *              char arrays, not strings. @c strncpy is thus forgone in favour of @c memcpy.
 */
char * slice(char * str, int start, int len) {
    char * res = malloc(len);
    VERIFY_NULL(res, len);

    memcpy(res, str + start, len);

    return res;
}
