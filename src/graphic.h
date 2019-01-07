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
 *      @file graphic.h
 *      @brief Function definitions relating to barcode graphic output from an internal
 *             representation.
 *      @detail Many functions defined here return externally-defined error codes, see errors.h for
 *              more information.
 *      @see errors.h
 *      @author Elijah Schutz
 *      @date 17/12/18
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "symb.h"
#include <string.h>

/**     @internal
 *      @brief Macro for stringification of macros
 *      @see STR
 */
#define XSTR(x) STR(x)

/**     @internal
 *      @brief Macro for stringification of an expression
 */
#define STR(x) #x

/**
 *      @defgroup SVGProperties Properties of SVG barcode outputs
 */
/*@{*/
#define SVG_HEIGHT 150
#define SVG_HEADER                                                                                 \
    "<?xml version=\"1.0\"?><svg xmlns=\"http://www.w3.org/2000/svg\" "                            \
    "xmlns:xlink=\"http://www.w3.org/1999/xlink\" "                                                \
    "height=\"" XSTR(SVG_HEIGHT) "\"><g fill=\"white\">"
#define SVG_FOOTER "</g></svg>"
#define SVG_FOOTER_LEN 10
#define SVG_COLOUR_LEN 7

#define SVG_RECT_BUFSIZE (55 + SVG_COLOUR_LEN)
#define SVG_TEXT_BUFSIZE 146

/*      @brief y-coordinate of barcode rectangles */
#define SVG_DEFAULT_Y 0
#define SVG_RECT_WIDTH 3
#define SVG_RECT_HEIGHT 130
#define SVG_FONT_SIZE 20
#define SVG_LINE_HEIGHT SVG_HEIGHT
/*@}*/

/**
 *      @brief Returns the maximum amount of memory (in bytes) needed for a barcode SVG encoding the
 *             supplied number of @c rects.
 *      @param rects The number of rectangles (bars) in the barcode
 *      @return Maximum buffer size (in bytes)
 */
size_t svg_bufsize(int);

/**
 *      @brief Generates an SVG rectangle with the given properties.
 *      @param x The x-coordinate of the top-left corner of the rectangle
 *      @param y The y-coordinate of the top-left corner of the rectangle
 *      @param w The width of the rectangle
 *      @param h The height of the rectangle
 *      @param colour The colour of the rectangle as a string (maximum length of SVG_COLOUR_LEN)
 *      @param dest A double pointer to a destination array of size SVG_RECT_BUFSIZE
 *      @return ERR_ARGUMENT or SUCCESS
 */
int svg_rect(int, int, int, int, char *, char[][SVG_RECT_BUFSIZE]);

/**
 *      @brief Generates an SVG text element for Code 128 barcodes.
 *      @param text The content of the SVG text element
 *      @param index The x-coordinate of the centre of the text (text-anchor="middle")
 *      @param dest A double pointer to a destination array of size SVG_TEXT_BUFSIZE
 *      @return SUCCESS
 */
int c128_text(char *, int, char[][SVG_TEXT_BUFSIZE]);

/**
 *      @brief Generates a black rectangle for Code 128 barcodes.
 *      @param x The x-coordinate of the top-left corner of the rectangle
 *      @param w The width of the rectangle
 *      @param dest A double pointer to a destination array of size SVG_RECT_BUFSIZE
 *      @return SUCCESS
 */
int c128_rect_black(int, int, char[][SVG_RECT_BUFSIZE]);

/**
 *      @brief Generates a white rectangle for Code 128 barcodes.
 *      @param x The x-coordinate of the top-left corner of the rectangle
 *      @param w The width of the rectangle
 *      @param dest A double pointer to a destination array of size SVG_RECT_BUFSIZE
 *      @return SUCCESS
 */
int c128_rect_white(int, int, char[][SVG_RECT_BUFSIZE]);

/**
 *      @brief Generates an SVG representation of a barcode pattern for Code 128 barcodes.
 *      @param pat The pattern to be represented
 *      @param width The number of bars comprising the pattern
 *      @param svg_x A pointer to an integer representing the x-coordinate from which to draw the
 *             pattern. This is automatically incremented within the function to facilitate drawing
 *             adjacent patterns.
 *      @param dest A double pointer to a destination string. There should still be
 *             <tt>SVG_RECT_BUFSIZE * width</tt> bytes of memory allocated from the end of the
 *             string, as the result is <tt>strncat</tt>ed with @c dest.
 *      @return SUCCESS
 */
int c128_pat2svg(pattern, int, int *, char **);

/**
 *      @brief Generates an SVG representation of a complete Code 128 barcode.
 *      @param code A pointer to a Code128 struct that contains the barcode to be used
 *      @param dest A double pointer to a destination string. Use svg_bufsize() to calculate the
 *             the size of @c dest
 *      @return SUCCESS
 *      @see svg_bufsize
 */
int c128_svg(Code128 *, char **);

#endif
