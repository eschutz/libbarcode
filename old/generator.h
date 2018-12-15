/**
*     @file generator.h
*     @brief generator.h contains Symbology and generator function definitions.
*
*     @author Elijah Schutz
*
*     @date 10/12/18
*/

#ifndef GENERATOR_H
#define GENERATOR_H

#define MAX_GUARDS 3

#define BAR_UTF_BYTES 3

#define DIGIT_CONSOLE_HEIGHT  20
#define GUARD_CONSOLE_HEIGHT DIGIT_CONSOLE_HEIGHT + 2

typedef unsigned short pattern;

/**
* @struct       Guard
*               Represents a guard pattern in a barcode
* @var          Guard::code
*               The binary representation of a barcode pattern, as outlined in Symbology1d::code.
* @var          Guard::width
*               The width of the guard pattern in bars
* @var          Guard::index
*               The index within a barcode at which the guard appears
*/
typedef struct Guard Guard;

struct Guard {
  uint8_t code;
  int width;
  int index;
};

const struct Guard null_guard;

/**
* @struct       Symbology1D
*               Represents a 1-dimensional barcode symbology format
* @var          Symbology1D::patterns
*               The total number of patterns constituting a barcode. Includes digits and guards.
* @var          Symbology1D::digits
*               The total number of digits contained in the barcode.
* @var          Symbology1D::digit_width
*               The width of each digit in bars.
* @var          Symbology1D::code
*               ASCII char to bar pattern
*               A char at index n represents the corresponding bar pattern in
*               binary with a leading 0. 0 represents white, 1 represents black.
*               For example, in UPC-A: 0 => □□□■■□■ => 0b01110010 => 0x72 => 'r'
* @var          Symbology1D::guards
*               Array of Guard structs, placed at a given index in the barcode.
* @var          Symbology1D::invert
*               The index from which to invert (black <-> white) barcode patterns. -1 for no inversion.
* @attention    10/12/18 May remove Symbology1D -> Symbology alias in future if higher dimensional symbologies must be added.
*/
typedef struct Symbology1D Symbology;

struct Symbology1D {
  int patterns;
  int digits;
  int digit_width;
  int invert;
  pattern code[128];
  Guard guards[MAX_GUARDS];
};

const struct Symbology1D upc_a;

int guard_eql(Guard, Guard);

/**
*     symb_width – returns the total width of all vertical bars in a barcode.
*     @param symb     The symbology standard to use
*     @return         The total width of the barcode
*/
int symb_width(Symbology);

/**
*     encode – encode an identification number as a barcode.
*     @param symb     The symbology to use
*     @param id       The identification number as an array of bytes in the range [0, 9] – should be symb.digits long
*     @param dest     The destination byte array – should be symb.patterns long
*     @return         0 if encoding was successful, -1 if unsuccessful
*/
int encode(Symbology, char*, size_t, uint8_t*, size_t);


int barcode_unicode(Symbology, pattern*, size_t, int*, int);

#endif /* GENERATOR_H */
