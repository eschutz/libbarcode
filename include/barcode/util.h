/* Copyright (C) 2019 Elijah Schutz */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

/**
 *      @file util.h
 *      @brief Declarations for utility macros and functions used throughout this project.
 *      @author Elijah Schutz
 *      @date 12/12/18
 */
#include <stdbool.h>

/**
 *      @brief Ceiling integer division - 5 / 2 == 3
 *      @detail Example usage:
 *              @code
 *              CEILDIV(5, 2)
 *                  => 3
 *              @endcode
 */
#define CEILDIV(x, y) ((x + y - 1) / y)

/**
 *      @brief Checks if a string is comprised only of digits
 *      @param str A string to be tested on
 *      @param len The length of str
 *      @return true or false
 */
bool isdigits(char *, int);

/**
 *      @brief Returns the contents of @c str starting from @c start to <tt>start + len</tt>
 *      @param str A string to be sliced
 *      @param start The index from which to begin slicing
 *      @param len The length of the resulting substring
 *      @return A string of length len
 */
char * slice(char *, int, int);
