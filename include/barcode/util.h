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
char *slice(char *, int, int);
