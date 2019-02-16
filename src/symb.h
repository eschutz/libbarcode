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
 *     @file symb.h
 *     @brief Declarations for barcode representation data types and barcode generating functions.
 *     @detail Many functions defined here return externally-defined error codes, see errors.h for
 *             more information.
 *     @see errors.h
 *     @author Elijah Schutz
 *     @date 12/12/18
 */

#ifndef SYMB_H
#define SYMB_H

#include <stdbool.h>

// clang-format off

/**
 *      @defgroup  CtrlCharacters Control character macros
 */
/*@{*/
#ifndef NUL
    #define NUL '\0'
#endif
#define SOH 1
#define STX 2
#define ETX 3
#define EOT 4
#define ENQ 5
#define ACK 6
#define SO  14
#define SI  15
#define DLE 16
#define DC1 17
#define DC2 18
#define DC3 19
#define DC4 20
#define NAK 21
#define SYN 22
#define ETB 23
#define CAN 24
#define EM  25
#define SUB 26
#define FS  28
#define GS  29
#define RS  30
#define US  31
#define DEL 127
/*      @brief The total number of ASCII control characters */
#define ASCII_NUM_CTRL 32
/*@}*/

/**
 *      @brief Evaluates to true if @c c is an ASCII control character or @c DEL.
 */
#define IS_CTRL(c) (0 <= c && c < ASCII_NUM_CTRL || DEL == c)

/**
 *      @brief Length of a string representation of a control character (excludes null
 *             terminator).
 */
#define CTRL_STR_SIZE 2

/**
 *      @brief String representation of ASCII delete (127).
 *      @see ctrl_strrepr
 */
#define DEL_STRREPR "\\?"

/**
 *      @defgroup C128Properties Code 128 property macros
 */
/*@{*/

#define C128_CODE_SIZE        103
#define C128_DATA_WIDTH       11
#define C128_STOP_WIDTH       13
#define C128_QUIET_WIDTH      10
#define C128_MAX_DATA_LEN     20
// Use for defining the length of (null-terminated) strings prior to processing
#define C128_MAX_STRING_LEN   C128_MAX_DATA_LEN + 1
#define C128_MAX_PATTERN_SIZE 42
#define C128_MAX_STRREPR_SIZE (CTRL_STR_SIZE * C128_MAX_PATTERN_SIZE + 1)
#define C128_INVERSE_SIZE 512
/*@}*/

/**
 *      @defgroup C128LexUtils Macros to assist in converting a char array into a Code 128 barcode
 *      @ingroup C128Properties
 */
/*@{*/
#define IN_C128_A(x) ((0 <= x && x <= 95) || (97 <= x && x <= 106))
#define IN_C128_B(x) (ASCII_NUM_CTRL <= x && x <= DEL)
#define USE_C128_C_FULL(x) (x > 1 && x % 2 == 0)
#define USE_C128_DGT(str, idx, len) use_c128_dgt(str, idx, len)
#define CODE_CHANGE_NEEDED(code, chr) ((A == code && !IN_C128_A(chr)) || (B == code && !IN_C128_B(chr)))
#define C128_C_MIN_DGT_MID 6
#define C128_C_MIN_DGT_END 4

/* Macros to convert a char to its Code 128 value */
#define C128_A_VALUE(c) (C128_A_INVERSE[c])
#define C128_B_VALUE(c) (c - ASCII_NUM_CTRL)
#define C128_C_VALUE(c) (c)

/* Macros for special Code 128 patterns */
#define START_A 0b101000010
#define START_B 0b101001000
#define START_C 0b101001110
#define STOP    0b100011101
/*      @brief Reverse stop pattern for identifying if a barcode is upside-down */
#define RSTOP   0b101011100
/*      @brief Stop pattern with the trailing two bars added */
#define STOPPT  0b1100011101011

#define DEFAULT_C128_CODESET B
/*      @brief A macro that outputs to the default code switching pattern enum given a codeset */
#define DEFAULT_C128_SWITCH(c) c##CodeB
/*@}*/

// clang-format on

/**
 *      @brief An unsigned byte.
 *      @detail Used to represent characters to be converted to a barcode. Unsigned types are useful
 * as character codes greater than 127 may be used. The distinction from a uint8_t is purely
 *              semantic, as characters are being worked on, not unsigned bytes.
 */
typedef unsigned char uchar;

/**
 *      @brief Representation of a barcode pattern.
 *      @detail Barcode patterns are represented by binary literals, where a 0 represents a white
 * bar and a 1 represents a black bar. Two bytes are used to accommodate Code 128 barcodes, where
 * the pattern width is 11 bars (9 unique bars) (the stop pattern is 13 bars wide).
 */
typedef unsigned short pattern;

/**
 *      @brief Representation of a Code 128 barcode.
 */
typedef struct Code128_Barcode Code128;

/**
 *      @brief An enumeration of each of the Code 128 code sets – A, B, and C.
 */
typedef enum Code128CodeSet Code128CodeSet;

/**
 *      @brief Code 128A pattern control code values.
 */
typedef enum Code128Ctrl_A Code128Ctrl_A;

/**
 *      @brief Code 128B pattern control code values.
 */
typedef enum Code128Ctrl_B Code128Ctrl_B;

/**
 *      @brief Code 128C pattern control code values.
 */
typedef enum Code128Ctrl_C Code128Ctrl_C;

/**
 *      @brief Control character string representation mapping for ASCII 0-31. Using a char as the
 *             index will return its string representation.
 *             For example <tt>ctrl_strrepr[(int) '\0'] == "\\0"</tt>
 *      @see DEL_STRREPR
 *      @see c128_strrepr
 */
extern const char ctrl_strrepr[ASCII_NUM_CTRL][C128_MAX_STRREPR_SIZE];

/**
 *      @detail Stores the length of the barcode stored in data[], the text it represents, and the
 *              barcode representation itself. Note that the text field is *not* a string, so a null
 *              terminator must be appended. For a string representation of barcode data, see
 *              c128_strrepr().
 *      @see c128_strrepr
 */
struct Code128_Barcode {
    int     datalen;
    int     textlen;
    uchar   text[C128_MAX_DATA_LEN];
    pattern data[];
};

enum Code128CodeSet { A, B, C, Invalid };

enum Code128Ctrl_A {
    AFNC1   = 102,
    AFNC2   = 97,
    AFNC3   = 96,
    AFNC4   = 101,
    AShiftB = 98,
    ACodeB  = 100,
    ACodeC  = 99,
    StartA  = 103,
    AStop   = 106
};

enum Code128Ctrl_B {
    BFNC1   = 134,
    BFNC2   = 129,
    BFNC3   = 128,
    BFNC4   = 132,
    BShiftA = 130,
    BCodeA  = 133,
    BCodeC  = 131,
    StartB  = 136,
    BStop   = 138
};

enum Code128Ctrl_C { CFNC1 = 102, CCodeA = 101, CCodeB = 100, StartC = 105, CStop = 106 };

/**
 *     @brief Value-to-pattern mapping for generating Code 128 barcodes.
 *     @see C128_CODE_INVERSE
 */
extern const pattern C128_CODE[C128_CODE_SIZE];

/**
 *      @brief Pattern-to-value mapping for reading Code 128 barcodes. Inverse of C128_CODE.
 *      @see C128_CODE
 */
extern uchar C128_CODE_INVERSE[];

/**
 *      @brief Value-to-character mapping for reading Code 128 barcodes in Code A.
 *      @see C128_A_INVERSE
 *      @see C128_A_VALUE
 */
extern const uchar C128_A[C128_CODE_SIZE];

/**
 *      @brief Character-to-value mapping for generating Code 128 barcodes in Code A. Inverse of
 * C128_A.
 *      @see C128_A
 *      @see C128_A_VALUE
 */
extern const int C128_A_INVERSE[C128_CODE_SIZE];

/**
 *      @brief Value-to-character mapping for reading Code 128 barcodes in Code B.
 *      @see C128_B_VALUE
 */
extern const uchar C128_B[C128_CODE_SIZE];

/**
 *      @brief Necessary initialisations for barcode usage. <b>Always call before using any function
 * in symb.h</b>
 *      @return SUCCESS or ERR_ALREADY_INITIALISED
 */
int init_barcode(void);

/**
 *      @brief Generate a string representation of data as encoded by code 128.
 *      @param data The data to be stringified.
 *      @param data_len The length of @c data.
 *      @param dest The destination string. Should be of size C128_MAX_STRREPR_SIZE.
 *      @returns SUCCESS or ERR_DATA_LENGTH
 */
int c128_strrepr(uchar *, int, char **);

/**
 *      @brief Converts two digit characters into a Code 128C value.
 *      @param digit1 First base-10 digit.
 *      @param digit2 Second base-10 digit.
 *      @param dest Destination integer pointer.
 *      @return SUCCESS or ERR_ARGUMENT if one of the characters is not a digit.
 */
int c128_c_digit(uchar, uchar, int *);

/**
 *      @brief Assess whether to use on a substring of digits.
 *      @param str The full string
 *      @param idx The index from which the substring begins
 *      @param len The length of the substring
 *      @return true if Code 128 should be used, false otherwise
 */
bool use_c128_dgt(char *str, int idx, int len);

/**
 *      @brief Calculates the Code 128 checksum of the supplied values.
 *      @param values An integer array of Code 128 values.
 *      @param val_len Length of values
 *      @param dest Destingation integer pointer.
 *      @return SUCCESS
 */
int c128_checksum(int *, int, int *);

/**
 *      @brief Encodes a uchar array into a Code 128 barcode.
 *      @param data The data to be encoded.
 *      @param data_len The length of the data array.
 *      @param dest A double pointer to a Code128 structure.
 *      @return SUCCESS or ERR_ARGUMENT
 *      @warning data is interpreted as a uchar array, @e not a
 *               string. The null terminator is a valid Code 128 character, so must be removed from
 *               a string beforehand.
 *      @warning Memory is allocated to dest inside the function, so ideally it should be unassigned
 *               before use.
 */
int c128_encode(uchar *, int, Code128 **);

#endif /* SYMB_H */
