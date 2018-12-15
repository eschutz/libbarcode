/**
*     @file symb.h
*     @brief symb.h contains symbology and relevant function definitions.
*     @author Elijah Schutz
*     @date 12/12/18
*/

#ifndef SYMB_H
#define SYMB_H

#ifndef NUL
    #define NUL '\0'
#endif
#define SOH      1
#define STX      2
#define ETX      3
#define EOT      4
#define ENQ      5
#define ACK      6
#define SO       14
#define SI       15
#define DLE      16
#define DC1      17
#define DC2      18
#define DC3      19
#define DC4      20
#define NAK      21
#define SYN      22
#define ETB      23
#define CAN      24
#define EM       25
#define SUB      26
#define FS       28
#define GS       29
#define RS       30
#define US       31
#define DEL      127

#define C128_CODE_SIZE 103

#define C128_DATA_WIDTH       11
#define C128_STOP_WIDTH       13
#define C128_QUIET_WIDTH      10
#define C128_MAX_DATA_LEN     20
#define C128_MAX_PATTERN_SIZE 42

#define IN_C128_A(x) ((0 <= x && x <= 95) || (97 <= x && x <= 106))
#define IN_C128_B(x) (32 <= x && x <= 127)
#define USE_C128_C_FULL(x) (x > 1 && x % 2 == 0)
#define USE_C128_DGT(str, idx, len) (len % 2 == 0 && isdigits(slice(str, idx, len), len))
#define C128_C_MIN_DGT_MID 6
#define C128_C_MIN_DGT_END 4

#define C128_A_VALUE(c) (C128_A_INVERSE[c])
#define C128_B_VALUE(c) (c - 32)
#define C128_C_VALUE(c) (c)

#define START_A  0b11010000100
#define START_B  0b11010010000
#define START_C  0b11010011100
#define STOP     0b11000111010
#define RSTOP    0b11010111000
#define STOPPT   0b1100011101011

#define DEFAULT_CODESET B

// uchar to indicate we're still working with chars, not uint8
typedef unsigned char uchar;
typedef unsigned short pattern;
typedef struct Code128_Barcode Code128;
typedef enum Code128CodeSet Code128CodeSet;
typedef enum Code128Ctrl_A Code128Ctrl_A;
typedef enum Code128Ctrl_B Code128Ctrl_B;
typedef enum Code128Ctrl_C Code128Ctrl_C;

enum Code128CodeSet {
    A,
    B,
    C,
    Invalid
};

enum Code128Ctrl_A {
    AFNC1   = 102,
    AFNC2   = 97,
    AFNC3   = 96,
    AFNC4   = 101, // Unused
    AShiftB = 98,
    ACodeB  = 100,
    ACodeC  = 99,
    StartA = 103,
    AStop   = 106
};

// Take 32 from each to account for offset
enum Code128Ctrl_B {
    BFNC1   = 134,
    BFNC2   = 129,
    BFNC3   = 128,
    BFNC4   = 132,  // Unused
    BShiftA = 130,
    BCodeA  = 133,
    BCodeC  = 131,
    StartB = 136,
    BStop   = 138
};

enum Code128Ctrl_C {
    CFNC1   = 102,
    CCodeA  = 101,
    CCodeB  = 100,
    StartC = 105,
    CStop   = 106
};

struct Code128_Barcode {
    int     length;
    pattern data[];
};

const pattern C128_CODE[C128_CODE_SIZE];
const uchar C128_CODE_INVERSE[512];
const uchar C128_A[C128_CODE_SIZE];
const int C128_A_INVERSE[103];
const uchar C128_B[C128_CODE_SIZE];
const int C128_B_INVERSE[C128_CODE_SIZE];

int init_barcode(void);

int code_c_digit(uchar, uchar, int*);

int c128_checksum(pattern*, int, int*);
int c128_pattern_length(uchar*, int, int*);
int c128_encode(uchar*, int, Code128**);

#endif /* SYMB_H */
