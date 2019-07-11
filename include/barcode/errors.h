/* Copyright (C) 2019 Elijah Schutz */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

/*
 *      @file errors.h
 *      @brief Error handling macros.
 *      @author Elijah Schutz
 *      @date 12/12/18
 */

#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdlib.h>

/**
 *      @defgroup Errors Error numbers returned by functions
 */
/*@{*/
// clang-format off
#define SUCCESS                 0
#define ERR_GENERIC             1
#define ERR_ARGUMENT            2
#define ERR_CHAR_INVALID        3
#define ERR_DATA_LENGTH         4
#define ERR_INVALID_CODE_SET    5
#define ERR_ALREADY_INITIALISED 6
#define ERR_NULL_PATTERN        7
#define ERR_INVALID_LAYOUT      8
#define BARCODE_MAX_ERR         ERR_INVALID_LAYOUT
/*@}*/

// clang-format on
/**
 *      @brief Verify if a pointer is null.
 *      @detail If a null pointer is supplied, an error message is printed and the program exits.
 *      @param var A pointer to check
 *      @param n The size of the memory allocated to the pointer (used in the error message)
 */
// clang-format off
#define VERIFY_NULL(var, n)                                                                     \
    do {                                                                                        \
        if (var == NULL) {                                                                      \
            fprintf(                                                                            \
                stderr,                                                                         \
                "null pointer: could not allocate %d bytes for variable \"%s\" at %s:%d in %s", \
                (int) n,                                                                        \
                #var,                                                                           \
                __FILE__,                                                                       \
                __LINE__ - 8,                                       /* 8 lines up until there */\
                __func__);                                                                      \
            exit(-1);                                                                           \
        }                                                                                       \
    } while(0)                                                                                  \
// clang-format on

#endif /* ERRORS_H */
