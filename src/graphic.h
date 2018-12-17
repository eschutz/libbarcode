/**
 *     @file graphic.h
 *     @brief Function definitions relating to barcode graphic output from an internal
 * representation.
 *     @author Elijah Schutz
 *     @date 17/12/18
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "symb.h"

#define XML_VERSION 1.0
#define SVG_HEIGHT 100
#define SVG_HEADER                                                                                 \
    "<?xml version=\"" #XML_VERSION                                                                \
    "\"?><svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" "  \
    "height=\"" #HEIGHT "\">"
#define SVG_FOOTER "</svg>"

#define RECT_WIDTH 1
#define RECT_HEIGHT (SVG_HEIGHT - FONT_SIZE)
#define FONT_SIZE 20

typedef enum Format Format;

enum Format {
    SVG
};

int c128_write(Code128, char*, Format);

#endif
