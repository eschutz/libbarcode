/****************************************************************************
 * Copyright (C) 2019  Elijah Schutz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ****************************************************************************/

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
