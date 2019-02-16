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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "graphic.h"
#include "symb.h"

#define STRINGS 18

char strings[STRINGS][C128_MAX_STRING_LEN] = {
    "CkORPNXLis6UMNbbKKkp", "v7CzHixDHE7P5PTupo14", "nXbF9rCcpIQVlzadujN7", "1rOeVTSwlekRgBIjLSe1",
    "NNg15hXW33esTzyxTnDm", "2cTxjvWcs3nlD63zcgjJ", "S4m80WoIdfiD316QSOvI", "1rUo5fF7FbtchAKefH8R",
    "dWXvrGYYLpmA4nZ9oVbo", "nqizcaueVkIKOrxlp6Bi", "6rNmAmyt9lUeMsoqf52o", "ie3QM3prlDbkv12vGpZO",
    "N1AEJq8AYaskDyMzGeqx", "FSXJzgaQhREHFi5n6ucB", "D7rcB0JBtVv5w0LMf7Y8", "gKo1rWVNPH4qzUt7fcmE",
    "kSEtBq3twMaJrfgq2mXa", "6QQ0fJNcWYOO52bLFiNc"};

int main(void) {
    init_barcode();
    char      texts[STRINGS][C128_MAX_DATA_LEN];
    Code128 **codes = calloc(1, sizeof codes * STRINGS);
    for (int i = 0; i < STRINGS; i++) {
        int len = strlen(strings[i]);
        memcpy(texts[i], strings[i], len); // Remove null terminator
        c128_encode((uchar *)texts[i], len, &codes[i]);
    }

    char * ps;
    Layout layout = {.rows = 3, .cols = 2};
    c128_ps_layout(codes, STRINGS, &ps, &PS_DEFAULT_PROPS, &layout);
    printf("%s\n", ps);
    free(ps);
    for (int i = 0; i < STRINGS; i++) {
        free(codes[i]);
    }
    free(codes);
    return 0;
}
