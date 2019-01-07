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
 *      @file symb.c
 *      @author Elijah Schutz
 *      @date 12/12/18
 *      @brief Definitions of barcode symbologies and functions.
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "symb.h"
#include "util.h"

const char ctrl_strrepr[32][C128_MAX_STRREPR_SIZE] = {
    "\\0", "^A",  "^B",  "^C", "^D", "^E",  "^F",  "\\a", "\\b", "\\t", "\\n",
    "\\v", "\\f", "\\r", "^N", "^O", "^P",  "^Q",  "^R",  "^S",  "^T",  "^U",
    "^V",  "^W",  "^X",  "^Y", "^Z", "\\e", "^\\", "^]",  "^^",  "^_"};

const pattern C128_CODE[] = {
    0b101100110, 0b100110110, 0b100110011, 0b001001100, 0b001000110, 0b000100110, 0b001100100,
    0b001100010, 0b000110010, 0b100100100, 0b100100010, 0b100010010, 0b011001110, 0b001101110,
    0b001100111, 0b011100110, 0b001110110, 0b001110011, 0b100111001, 0b100101110, 0b100100111,
    0b101110010, 0b100111010, 0b110110111, 0b110100110, 0b110010110, 0b110010011, 0b110110010,
    0b110011010, 0b110011001, 0b101101100, 0b101100011, 0b100011011, 0b010001100, 0b000101100,
    0b000100011, 0b011000100, 0b000110100, 0b000110001, 0b101000100, 0b100010100, 0b100010001,
    0b011011100, 0b011000111, 0b000110111, 0b011101100, 0b011100011, 0b000111011, 0b110111011,
    0b101000111, 0b100010111, 0b101110100, 0b101110001, 0b101110111, 0b110101100, 0b110100011,
    0b110001011, 0b110110100, 0b110110001, 0b110001101, 0b110111101, 0b100100001, 0b111000101,
    0b010011000, 0b010000110, 0b001011000, 0b001000011, 0b000010110, 0b000010011, 0b011001000,
    0b011000010, 0b001101000, 0b001100001, 0b000011010, 0b000011001, 0b100001001, 0b100101000,
    0b111011101, 0b100001010, 0b000111101, 0b010011110, 0b001011110, 0b001001111, 0b011110010,
    0b001111010, 0b001111001, 0b111010010, 0b111001010, 0b111001001, 0b101101111, 0b101111011,
    0b111011011, 0b010111100, 0b010001111, 0b000101111, 0b011110100, 0b011110001, 0b111010100,
    0b111010001, 0b011101111, 0b011110111, 0b110101111, 0b111010111};

/**
 *      @detail Each pattern is represented by a 9-bit binary literal, meaning the theoretical
 *              numerical value of each pattern lies in the range [0, 511). An array of 512 uchars
 *              is used to map the numerical value of the pattern to the Code 128 value. In
 *              init_barcode() all values are initialised to NUL – of the 103 patterns, there is no
 *              empty pattern so NUL values should be checked for when using C128_CODE_INVERSE.
 */
uchar C128_CODE_INVERSE[512];

const uchar C128_A[] = {
    ' ',  '!',  '"',  '#', '$', '%', '&',   '\'',  '(',     ')',    '*',    '+',   ',',  '-',  '.',
    '/',  '0',  '1',  '2', '3', '4', '5',   '6',   '7',     '8',    '9',    ':',   ';',  '<',  '=',
    '>',  '?',  '@',  'A', 'B', 'C', 'D',   'E',   'F',     'G',    'H',    'I',   'J',  'K',  'L',
    'M',  'N',  'O',  'P', 'Q', 'R', 'S',   'T',   'U',     'V',    'W',    'X',   'Y',  'Z',  '[',
    '\\', ']',  '^',  '_', NUL, SOH, STX,   ETX,   EOT,     ENQ,    ACK,    '\a',  '\b', '\t', '\n',
    '\v', '\f', '\r', SO,  SI,  DLE, DC1,   DC2,   DC3,     DC4,    NAK,    SYN,   ETB,  CAN,  EM,
    SUB,  '\e', FS,   GS,  RS,  US,  AFNC3, AFNC2, AShiftB, ACodeC, ACodeB, AFNC4, AFNC1};

const int C128_A_INVERSE[] = {
    64, 65, 66, 67, 68, 69, 7,  71, 72, 73, 74, 75, 76, 77, 78, 79, 8,  81,  82, 83, 84,
    85, 86, 87, 88, 89, 9,  91, 92, 93, 94, 95, 0,  1,  2,  3,  4,  5,  6,   7,  8,  9,
    1,  11, 12, 13, 14, 15, 16, 17, 18, 19, 2,  21, 22, 23, 24, 25, 26, 27,  28, 29, 3,
    31, 32, 33, 34, 35, 36, 37, 38, 39, 4,  41, 42, 43, 44, 45, 46, 47, 48,  49, 5,  51,
    52, 53, 54, 55, 56, 57, 58, 59, 6,  61, 62, 63, 96, 97, 98, 99, 10, 101, 102};

const uchar C128_B[] = {
    ' ',  '!', '"', '#', '$', '%', '&',   '\'',  '(',     ')',    '*',   '+',    ',',  '-', '.',
    '/',  '0', '1', '2', '3', '4', '5',   '6',   '7',     '8',    '9',   ':',    ';',  '<', '=',
    '>',  '?', '@', 'A', 'B', 'C', 'D',   'E',   'F',     'G',    'H',   'I',    'J',  'K', 'L',
    'M',  'N', 'O', 'P', 'Q', 'R', 'S',   'T',   'U',     'V',    'W',   'X',    'Y',  'Z', '[',
    '\\', ']', '^', '_', '`', 'a', 'b',   'c',   'd',     'e',    'f',   'g',    'h',  'i', 'j',
    'k',  'l', 'm', 'n', 'o', 'p', 'q',   'r',   's',     't',    'u',   'v',    'w',  'x', 'y',
    'z',  '{', '|', '}', '~', DEL, BFNC3, BFNC2, BShiftA, BCodeC, BFNC4, BCodeA, BFNC1};

/**
 *      @detail Initalises C128_CODE_INVERSE by iterating through C128_CODE and swapping the index
 * and value.
 */
int init_barcode(void) {
    static bool initialised = false;
    if (initialised) {
        return ERR_ALREADY_INITIALISED;
    }
    initialised = true;
    // Sets all empty values to 0
    memset(C128_CODE_INVERSE, 0, sizeof C128_CODE_INVERSE);
    // for (int i = 0; i < 512; i++) {
    //     C128_CODE_INVERSE[i] = NUL;
    // }
    /**
     * Uses the Code 128 value (C128_CODE[i]) as the index of C128_CODE_INVERSE, and the index (i)
     * as the value, to produce an inverse mapping.
     * Null values should be checked for in practice.
     */
    for (int i = 0; i < C128_CODE_SIZE; i++) {
        C128_CODE_INVERSE[C128_CODE[i]] = i;
    }
    return SUCCESS;
}

/**
 *      @detail Iterates through @c data to replacing every instance of a control character with its
 *              its string representation supplied by ctrl_strrepr and DEL_STRREPR.
 *      @see ctrl_strrepr
 *      @see DEL_STRREPR
 */
int c128_strrepr(uchar *data, int data_len, char **dest) {
    if (data_len > C128_MAX_DATA_LEN) {
        fprintf(stderr, "data length exceeds maximum of %d\n", C128_MAX_DATA_LEN);
        return ERR_DATA_LENGTH;
    }

    *dest = calloc(1, C128_MAX_STRREPR_SIZE);
    VERIFY_NULL(*dest, C128_MAX_STRREPR_SIZE);

    for (int i = 0; i < data_len; i++) {
        char c = (char)data[i];
        if (IS_CTRL(c)) {
            if (DEL == c) {
                strncat(*dest, DEL_STRREPR, CTRL_STR_SIZE);
            } else {
                strncat(*dest, ctrl_strrepr[(int)c], CTRL_STR_SIZE);
            }
        } else {
            char str[2];
            str[0] = c;
            str[1] = '\0';
            strncat(*dest, str, 1);
        }
    }
    return SUCCESS;
}

/**
 *      @detail Uses atoi() to convert two digits into a Code 128C values. Code C has a one-to-one
 *              digit-to-value mapping. Passing the two digits in "57" to the function will result
 *              in 57, and passing "06" to the function will result in 6.
 */
int c128_c_digit(uchar d0, uchar d1, int *dest) {
    char dd[3] = {d0, d1, '\0'};
    if (isdigit(d0) && isdigit(d1)) {
        *dest = atoi((char *)dd);
    } else {
        fprintf(stderr, "argument \"%s\" does not consist of digits\n", dd);
        return ERR_ARGUMENT;
    }
    return SUCCESS;
}

/**
 *      @detail Code 128 C is only used on a string if it is even-lengthed and is made up completely
 *              of digits. use_c128_dgt() utilises slice() and isdigits() defined in util.h to
 *              achieve this.
 */
bool use_c128_dgt(char *str, int idx, int len) {
    if (len % 2 != 0) {
        return false;
    }
    char *substr = slice(str, idx, len);

    bool res = isdigits(substr, len) ? true : false;

    free(substr);
    return res;
}

/**
 *      @detail The Code 128 checksum algorithm works as follows:
 *              Add the value of the start code to an accumulator.
 *              Starting from 1, iterate through the data section of the barcode and add the product
 *              of each value and its index to the accumulator.
 *              Reduce the accumulator modulo 103.
 *              See <a href="https://en.wikipedia.org/wiki/Code_128#Check_digit_calculation"
 * Wikipedia</a> for more details.
 */
int c128_checksum(int *values, int val_len, int *dest) {
    // Algorithm:
    // Add the start value to the sum of the encoded values multiplied by their
    // index The checksum is this sum modulo the length of Code 128 codes (103)
    *dest = values[0]; // Start value
    for (int i = 1; i < val_len; i++) {
        *dest += values[i] * i;
    }
    *dest %= C128_CODE_SIZE;
    return SUCCESS;
}

/**
 *      @detail The Code 128 algorithm is comprised of 3 'codes' allowing it to represent all 128
 *              ASCII characters. Code A represents characters 0 - 95 (ASCII control characters are
 *              are allowed), code B represents 32 - 127, and code C represents pairs of adjacent
 *              digits, allowing highly efficient compression of numerical data. Each code contains
 *              103 values, including Code 128 control symbols (FNC1-4, Shifts, Code changes, etc.).
 *              Code 128 has 107 barcode patterns, including starts and the stop pattern. Each
 *              pattern has a value 0-106. Within each code, characters or strings are assigned
 *              values that map to a specific pattern (Start and stop codes are common to A, B, and
 *              C).
 *              <a href="https://en.wikipedia.org/wiki/Code_128#Specification">Wikipedia</a> has a
 *              full explanation of the algorithm.
 */
int c128_encode(uchar *data, int data_len, Code128 **dest) {
    if (data_len > C128_MAX_DATA_LEN) {
        fprintf(stderr, "data length exceeds maximum of %d\n", C128_MAX_DATA_LEN);
        return ERR_DATA_LENGTH;
    }
    int status;
    int values_len = 0;

    // values stores the numerical values of the symbols
    size_t values_size = sizeof(int) * C128_MAX_PATTERN_SIZE;
    int *  values      = malloc(values_size);
    VERIFY_NULL(values, values_size);

    // dest_pat stores the patterns of the symbols, i.e. C128_CODE[values]
    size_t   dest_size = sizeof(pattern) * C128_MAX_PATTERN_SIZE;
    pattern *dest_pat  = malloc(dest_size);
    VERIFY_NULL(dest_pat, dest_size);

    Code128CodeSet code = Invalid;

    /**
     * If the full data has an even length and is solely numeric, code C can be used for the whole
     * thing. As code C encodes 2 digits per value, this method yields 2x compression.
     */
    if (USE_C128_C_FULL(data_len) && isdigits((char *)data, data_len)) {
        code        = C;
        values[0]   = StartC;
        dest_pat[0] = START_C;
        values_len++;

        // Code C encodes 2 digits per value, so the character index increases by 2 each iteration
        for (int i = 0; i < data_len; i += 2) {
            status = c128_c_digit(data[i], data[i + 1], &values[values_len]);
            if (SUCCESS != status) {
                return status;
            }
            dest_pat[values_len] = C128_CODE[C128_C_VALUE(values[values_len])];
            values_len++;
        }
    } else {
        char init = data[0];

        // If the first C128_C_MIN_DGT_END characters are digits, code C can be used initially
        if (USE_C128_DGT((char *)data, 0, C128_C_MIN_DGT_END)) {
            code        = C;
            values[0]   = StartC;
            dest_pat[0] = START_C;
            values_len++;

            for (int i = 0; i < C128_C_MIN_DGT_END; i += 2) {
                status = c128_c_digit(data[i], data[i + 1], &values[values_len]);
                if (SUCCESS != status) {
                    return status;
                }
                dest_pat[values_len] = C128_CODE[C128_C_VALUE(values[values_len])];
                values_len++;
            }
        } else if (IN_C128_B(init)) {
            code        = B;
            values[0]   = StartB;
            dest_pat[0] = START_B;
            values[1]   = C128_B_VALUE(init);
            dest_pat[1] = C128_CODE[values[1]];
            values_len += 2;
        } else if (IN_C128_A(init)) {
            code        = A;
            values[0]   = StartA;
            dest_pat[0] = START_A;
            values[1]   = C128_A_VALUE((int)init);
            dest_pat[1] = C128_CODE[values[1]];
            values_len += 2;
        } else {
            fprintf(stderr, "no code set available for character '%c'", init);
            return ERR_CHAR_INVALID;
        }

        for (int i = 1; i < data_len; i++) {
            char  chr      = data[i];
            int   next     = i + 1;
            uchar next_chr = data[next];

            // Code C is just digits (single digits can be encoded in A and B), so if chr is not
            // present in either A or B, it is unsupported.
            if (!IN_C128_A(chr) && !IN_C128_B(chr)) {
                fprintf(stderr, "no code set available for character '%c'", init);
                return ERR_CHAR_INVALID;
            }

            if (C == code) {
                if (isdigit(chr) && next < data_len && isdigit(next_chr)) {
                    status = c128_c_digit(chr, next_chr, &values[values_len]);
                    if (SUCCESS != status) {
                        return status;
                    }
                    dest_pat[values_len] = C128_CODE[C128_C_VALUE(values[values_len])];
                    // Code C has a compression factor of 2x, so we skip the next
                    // char as they're both digits.
                    i++;
                } else {
                    // If the default code set is B, DEFAULT_C128_SWITCH(C) generates the enum
                    // property CcodeB of Code128Ctrl_C.
                    Code128Ctrl_C val    = DEFAULT_C128_SWITCH(C);
                    values[values_len]   = val;
                    dest_pat[values_len] = C128_CODE[C128_C_VALUE(val)];
                    code                 = DEFAULT_C128_CODESET;
                    // Decrement, so we redo this index on the next iteration.
                    i--;
                }
                values_len++;
            } else if (isdigit(chr) && ((i + C128_C_MIN_DGT_END <= data_len &&
                                         USE_C128_DGT((char *)data, i, data_len - i)) ||
                                        (i + C128_C_MIN_DGT_MID <= data_len &&
                                         USE_C128_DGT((char *)data, i, C128_C_MIN_DGT_MID)))) {
                /**
                 * There happens to be an optimum number of digits that minimises the number of
                 * patterns in the barcode when switching to code C.
                 * This is as follows (from Wikipedia)
                 * | Location of Digits | Number of consecutive digits |
                 * |  beginning of data |              4+              |
                 * |     end of data    |              4+              |
                 * |   middle of data   |              6+              |
                 * |     entire data    |  either 2 or 4+ (but not 3)  |
                 *
                 * The bottom case has already handled (USE_C128_C_FULL), as has the first case.
                 * The middle two cases are handled in the above statement using C128_C_MIN_DGT_END
                 * (4) and C128_C_MIN_DGT_MID (6).
                 *
                 * If this section is evaluated, a code transition pattern to code C (e.g.ACodeC)
                 * is encoded, and the current character re-encoded.
                 */

                if (A == code) {
                    values[values_len]   = ACodeC;
                    dest_pat[values_len] = C128_CODE[C128_A_VALUE(ACodeC)];
                } else if (B == code) {
                    values[values_len]   = BCodeC;
                    dest_pat[values_len] = C128_CODE[C128_B_VALUE(BCodeC)];
                } else {
                    fprintf(stderr, "invalid code set '%d'", code);
                    return ERR_INVALID_CODE_SET;
                }
                values_len++;
                code = C;
                i--; // Retry with code as C
                continue;
            } else if ((A == code && !IN_C128_A(chr)) || (B == code && !IN_C128_B(chr))) {
                // If an A<->B code change is necessary
                if (next < data_len &&
                    ((A == code && IN_C128_B(next_chr)) || (B == code && IN_C128_A(next_chr)))) {
                    /**
                     * We check if the next character matches the alternative code set.
                     * If it is in the new code set, encode an XcodeY pattern,
                     * otherwise encode XshiftY (for one character).
                     */
                    if (A == code) {
                        code                 = B;
                        values[values_len]   = ACodeB;
                        dest_pat[values_len] = C128_CODE[C128_A_VALUE(ACodeB)];
                    } else {
                        code                 = A;
                        values[values_len]   = BCodeA;
                        dest_pat[values_len] = C128_CODE[C128_B_VALUE(BCodeA)];
                    }
                } else {
                    if (A == code) {
                        code                 = B;
                        values[values_len]   = AShiftB;
                        dest_pat[values_len] = C128_CODE[C128_A_VALUE(AShiftB)];
                    } else {
                        code                 = A;
                        values[values_len]   = BShiftA;
                        dest_pat[values_len] = C128_CODE[C128_B_VALUE(BShiftA)];
                    }
                }
                values_len++;
                i--; // Retry with changed code
            } else {
                // Handles the cases where the code does not change
                switch (code) {
                    int val;
                    case B:
                        val                  = C128_B_VALUE(chr);
                        values[values_len]   = val;
                        dest_pat[values_len] = C128_CODE[val];
                        break;
                    case A:
                        val                  = C128_A_VALUE((int)chr);
                        values[values_len]   = val;
                        dest_pat[values_len] = C128_CODE[C128_A_INVERSE[val]];
                        break;
                    default:
                        fprintf(stderr, "invalid code set '%d'", code);
                        return ERR_INVALID_CODE_SET;
                }
                values_len++;
            }
        }
    }

    /*
     * The checksum is evaluated based on the start and all data patterns encoded so far. The
     * pattern resulting from the checksum value is appended to the end of the barcode, and then the
     * stop pattern is added, yielding a complete, valid Code 128 barcode.
     */
    int checksum;
    status = c128_checksum(values, values_len, &checksum);

    if (SUCCESS != status) {
        return status;
    }

    switch (code) {
        case A: dest_pat[values_len] = C128_CODE[C128_A_INVERSE[C128_A_VALUE(checksum)]]; break;
        case B: dest_pat[values_len] = C128_CODE[C128_B_VALUE(checksum)]; break;
        case C: dest_pat[values_len] = C128_CODE[C128_C_VALUE(checksum)]; break;
        default: fprintf(stderr, "invalid code set '%d'", code); return ERR_INVALID_CODE_SET;
    }

    // Allow for the checksum and stop pattern in the pattern length
    int pat_len = values_len + 2;

    dest_pat[pat_len - 1] = STOPPT;

    size_t total_size = sizeof **dest + dest_size;

    *dest = calloc(1, total_size);
    VERIFY_NULL(dest, total_size);

    (*dest)->length = pat_len;
    memcpy((*dest)->text, data, data_len);
    memcpy((*dest)->data, dest_pat, dest_size);

    free(values);
    free(dest_pat);

    return SUCCESS;
}
