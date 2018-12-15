/*
* @file         errors.h
* @brief        Contains error code definitions.
* @author       Elijah Schutz
* @date         12/12/18
*/

#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERR_GENERIC 1

#define ERR_ARGUMENT 2
#define ERR_CHAR_INVALID 3
#define ERR_DATA_LENGTH 4
#define ERR_INVALID_CODE_SET 5
#define ERR_ALREADY_INITIALISED 6

#define VERIFY_NULL(var, n)                                                                     \
    do {                                                                                        \
        if (var == NULL) {                                                                      \
            fprintf(                                                                            \
                stderr,                                                                         \
                "null pointer: could not allocate %d bytes for variable \"%s\" at %s:%d in %s", \
                n,                                                                              \
                #var,                                                                           \
                __FILE__,                                                                       \
                __LINE__ - 8,                                       /* 8 lines up until there */\
                __func__);                                                                      \
            exit(-1);                                                                           \
        }                                                                                       \
    } while(0)                                                                                  \


#endif /* ERRORS_H */
