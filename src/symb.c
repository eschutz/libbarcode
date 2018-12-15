/**
*     @file symb.c
*     @brief Assignments of barcode symbologies and functions defined in symb.h.
*     @author Elijah Schutz
*     @date 12/12/18
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "symb.h"
#include "util.h"
#include "errors.h"

// Each code always starts with a 1, and ends with a 0
const pattern C128_CODE[] = {
      0b101100110,
      0b100110110,
      0b100110011,
      0b001001100,
      0b001000110,
      0b000100110,
      0b001100100,
      0b001100010,
      0b000110010,
      0b100100100,
      0b100100010,
      0b100010010,
      0b011001110,
      0b001101110,
      0b001100111,
      0b011100110,
      0b001110110,
      0b001110011,
      0b100111001,
      0b100101110,
      0b100100111,
      0b101110010,
      0b100111010,
      0b110110111,
      0b110100110,
      0b110010110,
      0b110010011,
      0b110110010,
      0b110011010,
      0b110011001,
      0b101101100,
      0b101100011,
      0b100011011,
      0b010001100,
      0b000101100,
      0b000100011,
      0b011000100,
      0b000110100,
      0b000110001,
      0b101000100,
      0b100010100,
      0b100010001,
      0b011011100,
      0b011000111,
      0b000110111,
      0b011101100,
      0b011100011,
      0b000111011,
      0b110111011,
      0b101000111,
      0b100010111,
      0b101110100,
      0b101110001,
      0b101110111,
      0b110101100,
      0b110100011,
      0b110001011,
      0b110110100,
      0b110110001,
      0b110001101,
      0b110111101,
      0b100100001,
      0b111000101,
      0b010011000,
      0b010000110,
      0b001011000,
      0b001000011,
      0b000010110,
      0b000010011,
      0b011001000,
      0b011000010,
      0b001101000,
      0b001100001,
      0b000011010,
      0b000011001,
      0b100001001,
      0b100101000,
      0b111011101,
      0b100001010,
      0b000111101,
      0b010011110,
      0b001011110,
      0b001001111,
      0b011110010,
      0b001111010,
      0b001111001,
      0b111010010,
      0b111001010,
      0b111001001,
      0b101101111,
      0b101111011,
      0b111011011,
      0b010111100,
      0b010001111,
      0b000101111,
      0b011110100,
      0b011110001,
      0b111010100,
      0b111010001,
      0b011101111,
      0b011110111,
      0b110101111,
      0b111010111
};

const uchar C128_CODE_INVERSE[C128_CODE_SIZE];

const uchar C128_A[] = {
    ' ',
    '!',
    '"',
    '#',
    '$',
    '%',
    '&',
    '\'',
    '(',
    ')',
    '*',
    '+',
    ',',
    '-',
    '.',
    '/',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    ':',
    ';',
    '<',
    '=',
    '>',
    '?',
    '@',
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    '[',
    '\\',
    ']',
    '^',
    '_',
    NUL,
    SOH,
    STX,
    ETX,
    EOT,
    ENQ,
    ACK,
    '\a',
    '\b',
    '\t',
    '\n',
    '\v',
    '\f',
    '\r',
    SO,
    SI,
    DLE,
    DC1,
    DC2,
    DC3,
    DC4,
    NAK,
    SYN,
    ETB,
    CAN,
    EM,
    SUB,
    '\e',
    FS,
    GS,
    RS,
    US,
    AFNC3,
    AFNC2,
    AShiftB,
    ACodeC,
    ACodeB,
    AFNC4,
    AFNC1
};

const int C128_A_INVERSE[] = {
    64,
    65,
    66,
    67,
    68,
    69,
    7,
    71,
    72,
    73,
    74,
    75,
    76,
    77,
    78,
    79,
    8,
    81,
    82,
    83,
    84,
    85,
    86,
    87,
    88,
    89,
    9,
    91,
    92,
    93,
    94,
    95,
    ,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    1,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    2,
    21,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    29,
    3,
    31,
    32,
    33,
    34,
    35,
    36,
    37,
    38,
    39,
    4,
    41,
    42,
    43,
    44,
    45,
    46,
    47,
    48,
    49,
    5,
    51,
    52,
    53,
    54,
    55,
    56,
    57,
    58,
    59,
    6,
    61,
    62,
    63,
    96,
    97,
    98,
    99,
    10,
    101,
    102
};

const uchar C128_B[] = {
    ' ',
    '!',
    '"',
    '#',
    '$',
    '%',
    '&',
    '\'',
    '(',
    ')',
    '*',
    '+',
    ',',
    '-',
    '.',
    '/',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    ':',
    ';',
    '<',
    '=',
    '>',
    '?',
    '@',
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    '[',
    '\\',
    ']',
    '^',
    '_',
    '`',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z',
    '{',
    '|',
    '}',
    '~',
    DEL,
    BFNC3,
    BFNC2,
    BShiftA,
    BCodeC,
    BFNC4,
    BCodeA,
    BFNC1
};

int init_barcode(void) {
    static bool initialised = false;
    if (initialised) {
        return ERR_ALREADY_INITIALISED;
    }
    initialised = true;
    for (int i = 0; i < 512; i++) { // Set
        C128_CODE_INVERSE = NULL;
    }
    for (int i = 0; i < C128_CODE_SIZE; i++) {
        C128_CODE_INVERSE[C128_CODE[i]] = i;
    }
    return SUCCESS;
}

int code_c_digit(uchar d0, uchar d1, int *dst) {
    char dd[2] = { d0, d1 };
    if (isdigit(d0) && isdigit(d1)) {
        *dst = atoi((char*) dd);
    } else {
        fprintf(stderr, "argument \"%s\" does not consist of digits\n", dd);
        return ERR_ARGUMENT;
    }
    return SUCCESS;
}

int c128_checksum(int *val, int val_len, int *dest) {
    // Algorithm:
    // Add the start value to the sum of the encoded values multiplied by their index
    // The checksum is this sum modulo 103
    *dest = val[0]; // Start value
    for (int i = 1; i < val_len; i++) {
        *dest += val[i] * i;
    }
    *dest %= 103;
    return SUCCESS;
}

// Lexical analysis of input number - deprecated
int c128_pattern_length(uchar *data, int data_len, int *dest) {
    /*
    * Analyse string to see if code A, B, or C should be used.
    * - If the first character is a control character, use code A (ASCII < 32)
    * - Code C is used under the following conditions:
    *   | Location of Consecutive Digits | Number of Digits |
    *   |       beginning of data        |        4+        |
    *   |          end of data           |        4+        |
    *   |         middle of data         |        6+        |
    *   |          entire data           |  2 or 4+ (not 3) |
    * - Otherwise, use code B
    * - While building the barcode, if any characters out of the range of the
    *   current code are found, change to the required code until it is
    *   necessary to change back.
    *
    * Using this scheme, loop through and calculate the length of the resulting pattern
    */
    int pat_len = 0;
    Code128CodeSet code = Invalid;

    if (USE_C128_C_FULL(data_len) && isdigits((char*) data, data_len)) {
        *dest = CEILDIV(data_len, 2) + 3; // Code C compresses 2x, add 3 for start, stop, and check
        return SUCCESS;
    }

    uchar init = data[0];

    if (USE_C128_DGT((char*) data, 0, C128_C_MIN_DGT_END)) {
        code = C;
        pat_len += CEILDIV(C128_C_MIN_DGT_END, 2);
    } else if (IN_C128_B(init)) {
        code = B;
        pat_len++;
    } else if (IN_C128_A(init)) {
        code = A;
        pat_len++;
    } else {
        fprintf(stderr, "no code set available for character '%c'", init);
        return ERR_INVALID_CODE_SET;
    }

    for (int i = 1; i < data_len; i++) {
        uchar chr = data[i];
        if (C == code) {
            if (isdigit(chr) && i + 1 < data_len && isdigit(data[i + 1])) {
                // Code C has a compression factor of 2x, so we skip the next char if they're both digits
                i++;
            } else {
                code = DEFAULT_CODESET;
                // Decrement, so we redo this index on the next iteration
                // Note: could be reimplemented recursively
                i--;
            }
            pat_len++; // Whether chr is a digit or not, length increases from the addition of a digit or a code change
            continue;
        } else if (isdigit(chr) &&
                   ((i + C128_C_MIN_DGT_END <= data_len && USE_C128_DGT((char*) data, i, data_len - i)) ||
                    (i + C128_C_MIN_DGT_MID <= data_len && USE_C128_DGT((char*) data, i, C128_C_MIN_DGT_MID)))) {
            // Check if enough digits to switch to Code C
            code = C;
            pat_len++;
            i--; // Retry with code as C
            continue;
        } else if ((A == code && !IN_C128_A(chr)) || (B == code && !IN_C128_B(chr))) {
            if (i + 1 < data_len) {
                uchar next = data[i + 1];
                if ((A == code && IN_C128_B(next)) || (B == code && IN_C128_A(next))) {
                    // This checks if the next character matches the new code set
                    // If it is in the new code set, encode a codeX character, otherwise encode
                    // XshiftY (for one character)
                    // If next is in the new code set, change the code
                    code = code == A ? B : A;
                }
            }
            pat_len++; // Either way, a new character increases pattern length
        } else { // Error handling
            if (!IN_C128_A(chr) && !IN_C128_B(chr)) {
                fprintf(stderr, "no code set available for character '%c'", init);
                return ERR_INVALID_CODE_SET;
            }
        }
        pat_len++;
    }

    // Allow for start, stop, and check digit
    pat_len += 3;
    *dest = pat_len;
    return SUCCESS;
}

// Instead allocate max memory and have running sum of value length
int c128_encode(uchar *data, int data_len, Code128 **dest) {
    if (data_len > C128_MAX_DATA_LEN) {
        fprintf(stderr, "data length exceeds maximum of %d\n", C128_MAX_DATA_LEN);
        return ERR_DATA_LENGTH;
    }
    /* ALGORITHM - could be made much more efficient using a map
     * Calculate the length of the result first and malloc
     *
     * Loop through the string, adding the relevant pattern to the result and
     * changing codes as necessary
     *
     * Calculate the check symbol and add it to the result
     * Add the stop pattern to the result
     */
     int status;
     int pat_len;
     status = pattern_length(data, data_len, &pat_len);
     if (SUCCESS != status) {
         return status;
     }

     // Stores the values, not the patterns, of the barcode
     int values_len = pat_len - 2;
     int values[values_len];

     size_t dest_size = sizeof(pattern) * pat_len;
     pattern *dest_pat = malloc(dest_size);
     VERIFY_NULL(dest_pat, dest_size);

     // Assign stop code
     dest_pat[pat_len - 1] = STOPPT;

     Code128CodeSet code = Invalid;

     if (USE_C128_C_FULL(data_len) && isdigits((char*) data, data_len)) {
         code = C;
         values[0] = Code128Ctrl_C.StartC;
         dest_pat[0] = START_C;
         for (int i = 0; i < data_len; i += 2) {
             // data_len will always be 2 * (pat_len - 3), so we use i / 2 for the index accounting
             // for this, and 1 + to offset for the start code
             int dest_idx = i / 2 + 1;
             status = code_c_digit(data[i], data[i + 1], &value[dest_idx]);
             if (SUCCESS != status) {
                 return status;
             }
             dest_pat[dest_idx] = C128_CODE[C128_C_VALUE(value[dest_idx])];
         }
         int check;
         status = c128_checksum(values, values_len, &check);
         if (SUCCESS != status) {
             return status;
         }
         // Second-to-last value is checksum
         dest_pat[pat_len - 2] = C128_CODE[C128_C_VALUE(check)];
         return SUCCESS;
     }

     char init = data[0];

     if (USE_C128_DGT(data, 0, C128_C_MIN_DGT_END)) {
         code = C;
         values[0] = Code128Ctrl_C.StartC;
         dest_pat[0] = START_C;

         for (int i = 0; i < C128_C_MIN_DGT_END; i += 2) {
             int dest_idx = i / 2 + 1; // See the previous if-statement
             status = code_c_digit(data[i], data[i + 1], &value[dest_idx]);
             if (SUCCESS != status) {
                 return status;
             }
             dest_pat[dest_idx] = C128_CODE[C128_C_VALUE(value[dest_idx])];
         }
     } else if (IN_C128_B(init)){
         code = B;
         values[0] = Code128Ctrl_B.StartB;
         dest_pat[0] = START_B;
         values[1] = C128_B_VALUE(init);
         dest_pat[1] = C128_CODE[values[1]];
     } else if (IN_C128_A(init)) {
         code = A;
         values[0] = Code128Ctrl_A.StartA;
         dest_pat[0] = START_A;
         values[1] = C128_A_VALUE(init);
         dest_pat[1] = C128_CODE[values[1]];
     } else {
         fprintf(stderr, "no code set available for character '%c'", init);
         return ERR_INVALID_CODE_SET;
     }

     for (int i = 1; i < data_len; i++) {
         char chr = data[i];
         if (C == code) {
         }
     }


     /*

     for (int i = 1; i < data_len; i++) {
         char chr = data[i];
         if (C == code) {
             if (isdigit(chr)) {
                 int next = i + 1;
                 if (next < data_len && isdigit(data[next])) {
                     // Code C has a compression factor of 2x, so we skip the next char if they're both digits
                     i++;
                 }
             } else {
                 code = DEFAULT_CODESET;
                 // Decrement, so we redo this index on the next iteration
                 // Note: could be reimplemented recursively
                 i--;
             }
             pat_len++; // Whether chr is a digit or not, length increases from the addition of a digit or a code change
             continue;
         } else if (isdigit(chr)) {
             // Check if enough digits to switch to Code C
             if ((i + C128_C_MIN_DGT_END <= data_len && USE_C128_DGT(data, i, data_len - i)) ||
                 (i + C128_C_MIN_DGT_MID <= data_len && USE_C128_DGT(data, i, C128_C_MIN_DGT_MID))) {
                 code = C;
                 pat_len++;
                 i--; // Retry with code as C
                 continue;
             }
         } else if ((A == code && !IN_C128_A(chr)) || (B == code && !IN_C128_B(chr))) {
             if (i + 1 < data_len) {
                 char next = data[i + 1];
                 if ((A == code && IN_C128_B(next)) || (B == code && IN_C128_A(next))) {
                     // This checks if the next character matches the new code set
                     // If it is in the new code set, encode a codeX character, otherwise encode
                     // XshiftY (for one character)
                     // If next is in the new code set, change the code
                     code = code == A ? B : A;
                 }
             }
             pat_len++; // Either way, a new character increases pattern length
         } else { // Error handling
             if (!isdigit(chr) && !IN_C128_A(chr) && !IN_C128_B(chr)) {
                 fprintf(stderr, "no code set available for character '%c'", init);
                 return ERR_INVALID_CODE_SET;
             }
         }
         pat_len++;
     }

     // Allow for start, stop, and check digit
     pat_len += 3;
     *dest = pat_len;
     return SUCCESS; */
}
