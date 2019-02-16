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
 *      @defgroup PSProperties Properties of PostScript barcode outputs
 */
/*@{*/
#define PS_HEADER                                                                                  \
    "%%!PS\n"                                                                                      \
    "/p {0 add} def\n"                                                                             \
    "/in {72 mul} def\n"                                                                           \
    "/mm {in 25.4 div} def\n"                                                                      \
    "/u {%s} def\n"                                                                                \
    "/lmargin %0.9f u def\n"                                                                       \
    "/rmargin %0.9f u def\n"                                                                       \
    "/tmargin %0.9f u def\n"                                                                       \
    "/bmargin %0.9f u def\n"                                                                       \
    "/BAR_W %0.9f u def\n"                                                                         \
    "/BAR_H %0.9f u def\n"                                                                         \
    "/PAD %0.9f u def\n"                                                                           \
    "/COL_W %0.9f u def\n"                                                                         \
    "/neg {-1 mul} def\n"                                                                          \
    "/x //lmargin def\n"                                                                           \
    "/y //bmargin //PAD add def\n"                                                                 \
    "/rpos {\n"                                                                                    \
    "  /y exch y add def\n"                                                                        \
    "  /x exch x add def\n"                                                                        \
    "} def\n"                                                                                      \
    "/reset_x {/x //lmargin def} def\n"                                                            \
    "/reset_y {/y //bmargin //PAD add def} def\n"                                                  \
    "/pad_x {//PAD 0 rpos} def\n"                                                                  \
    "/pad_y {0 //PAD rpos} def\n"                                                                  \
    "/bar {\n"                                                                                     \
    "  newpath\n"                                                                                  \
    "  x y moveto\n"                                                                               \
    "  0 //BAR_H rlineto\n"                                                                        \
    "  //BAR_W 0 rlineto\n"                                                                        \
    "  0 //BAR_H neg rlineto\n"                                                                    \
    "  closepath\n"                                                                                \
    "  0 setgray fill\n"                                                                           \
    "  /x x //BAR_W add def\n"                                                                     \
    "} def\n"
#define PS_HEADER_BUFSIZE 1024
#define PS_FOOTER "showpage\n"
#define PS_FOOTER_LEN 9
#define PS_TEXT                                                                                    \
    "/Helvetica findfont\n"                                                                        \
    "/fontsize %d def\n"                                                                           \
    "fontsize scalefont\n"                                                                         \
    "setfont\n"                                                                                    \
    "/str (%s) def\n"                                                                              \
    "newpath\n"                                                                                    \
    "x %d u sub y //PAD sub moveto\n"                                                              \
    "str stringwidth pop 2 div neg 0 rmoveto\n"                                                    \
    "str show\n"                                                                                   \
    "closepath\n"
#define PS_UNIT "mm"
#define PS_WIDTH 0.25  // mm
#define PS_HEIGHT 10   // mm
#define PS_MARGIN 6.35 // mm
#define PS_PAD 5       // mm
#define PS_COL_W 94    // mm
#define PS_BAR "bar\n"
#define PS_WSPACE "/x %d //BAR_W mul x add def\n"
#define PS_PADX "pad_x\n"
#define PS_PADY "pad_y\n"
#define PS_RESET_X "reset_x\n"
#define PS_RESET_Y "reset_y\n"
#define PS_COL_POS "/x //lmargin %0.9f u add %d mul def\n"
#define PS_RPOS "%0.9f u %0.9f u rpos\n"
#define PS_CMD_BUFSIZE 64
#define PS_FONT_SIZE 10
#define PS_TEXT_BUFSIZE 254
#define PS_MAX_POSITION_CMDS 3

// #define A4_WIDTH 210
// #define A4_HEIGHT 297
/*@}*/

/**
 *      @brief Enumeration of types of bars in a barcode – black or white
 */
typedef enum BarcodeColour BarType;

/**
 *      @brief Represents layout options for a PostScript Document
 *      @see PS_DEFAULT_PROPS
 */
typedef struct PSProperties PSProperties;

/**
 *      @brief Represents the layout of barcodes on a page.
 */
typedef struct PageLayout Layout;

enum BarcodeColour { White = 0, Black = 1 };

struct PSProperties {
    char units[3]; /**< The units of all values stored in the struct – one of "p", "mm", "in", "cm"
                    */
    float        lmargin;      /**< Left margin width */
    float        rmargin;      /**< Right margin width */
    float        tmargin;      /**< Top margin height */
    float        bmargin;      /**< Bottom margin height */
    float        bar_width;    /**< Width of a single bar in a barcode */
    float        bar_height;   /**< Height of a barcode */
    float        padding;      /**< Amount of padding between barcodes */
    float        column_width; /**< The maximum width of a column of barcodes*/
    unsigned int fontsize;     /**< Font size */
};

struct PageLayout {
    unsigned int rows;
    unsigned int cols;
};

/**
 *      @brief Useful default properties for PostScript layout
 */
extern const PSProperties PS_DEFAULT_PROPS;

/**
 *      @brief Returns the maximum amount of memory (in bytes) needed for a barcode SVG encoding the
 *             supplied number of @c rects.
 *      @param rects The number of rectangles (bars) in the barcode
 *      @return Maximum buffer size (in bytes)
 */
size_t svg_bufsize(int);

/**
 *      @brief Returns the maximum amount of memory (in bytes) needed for a barcode PS encoding the
 *             supplied number of @c rects.
 *      @param rects The number of rectangles (bars) in the barcode
 *      @return Maximum buffer size (in bytes)
 */
size_t ps_bufsize(int);

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
 *      @brief Generates a PostScript text element for Code 128 barcodes.
 *      @param text The content of the text element
 *      @param fsize The font size of the text
 *      @param index The x-coordinate of the centre of the text (centre-justified)
 *      @param dest A double pointer to a destination array of size PS_TEXT_BUFSIZE
 *      @return SUCCESS
 */
int c128_ps_text(char *, int, int, char[][PS_TEXT_BUFSIZE]);

/**
 *      @brief Generates a black SVG rectangle for Code 128 barcodes.
 *      @param x The x-coordinate of the top-left corner of the rectangle
 *      @param w The width of the rectangle
 *      @param dest A double pointer to a destination array of size SVG_RECT_BUFSIZE
 *      @return SUCCESS
 */
int c128_rect_black(int, int, char[][SVG_RECT_BUFSIZE]);

/**
 *      @brief Generates a white SVG rectangle for Code 128 barcodes.
 *      @param x The x-coordinate of the top-left corner of the rectangle
 *      @param w The width of the rectangle
 *      @param dest A double pointer to a destination array of size SVG_RECT_BUFSIZE
 *      @return SUCCESS
 */
int c128_rect_white(int, int, char[][SVG_RECT_BUFSIZE]);

/**
 *      @brief Generates a black PostScript rectangle for Code 128 barcodes.
 *      @param dest A double pointer to a destination array of size PS_CMD_BUFSIZE
 *      @return SUCCESS
 */
int c128_ps_rect_black(char[][PS_CMD_BUFSIZE]);

/**
 *      @brief Generates a white PostScript rectangle for Code 128 barcodes.
 *      @param w The width of the rectangle (only available on white rectangles - for quiet zone)
 *      @param dest A double pointer to a destination array of size PS_CMD_BUFSIZE
 *      @return SUCCESS
 */
int c128_ps_rect_white(int, char[][PS_CMD_BUFSIZE]);

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
 *      @brief Generates a PostScript representation of a barcode pattern for Code 128 barcodes.
 *      @param pat The pattern to be represented
 *      @param width The number of bars comprising the pattern
 *      @param ps_x A pointer to an integer representing the x-coordinate from which to draw the
 *             pattern. This is automatically incremented within the function to facilitate drawing
 *             adjacent patterns.
 *      @param dest A double pointer to a destination string. There should still be
 *             <tt>PS_CMD_BUFSIZE * width</tt> bytes of memory allocated from the end of the
 *             string, as the result is <tt>strncat</tt>ed with @c dest.
 *      @return SUCCESS
 */
int c128_pat2ps(pattern, int, float *, char **, const PSProperties *);

/**
 *      @brief Generates an SVG representation of a complete Code 128 barcode.
 *      @param code A pointer to a Code128 struct that contains the barcode to be used
 *      @param dest A double pointer to a destination string. Use svg_bufsize() to calculate the
 *             the size of @c dest
 *      @return SUCCESS
 *      @see svg_bufsize
 */
int c128_svg(Code128 *, char **);

/**
 *      @brief Initialises a string to be encoded with a PostScript barcode(s)
 *      @param dest A double pointer to a destination string – memory is allocated inside the
 *             function
 *      @param barcodes The number of barcodes that will be encoded
 *      @return SUCCESS
 */
int c128_ps_init(char **, int);

/**
 *      @brief Adds the PostScript header to a string initialised by c128_ps_init()
 *      @param dest The destination string initialised by c128_ps_init()
 *      @param props A PSProperties struct containing the PostScript layout properties
 *      @return SUCCESS
 *      @see c128_ps_init()
 */
int c128_ps_header(char **, const PSProperties *);

/**
 *      @brief Adds a PostScript footer to a string representing a PostScript file, after use in
 *             c128_ps()
 *      @param dest The destination string
 *      @return SUCCESS
 *      @see c128_ps
 */
int c128_ps_footer(char **);

/**
 *      @brief Generates a PostScript representation of a complete Code 128 barcode, following
 *             c128_ps_init() and c128_ps_header()
 *      @param codes A double pointer to a Code128 struct containing the barcodes to be printed
 *      @param num_codes The number of barcodes contained in @c codes
 *      @param dest A double pointer to a destination string – its memory is allocated in
 *             c128_ps_init
 *      @param props A PSProperties struct containing the properties of the page in the PostScript
 *             file
 *      @return SUCCESS
 *      @see c128_ps_init
 *      @see c128_ps_header
 *      @see c128_ps_footer
 *      @see ps_bufsize
 */
int c128_ps(Code128 *, char **, const PSProperties *);

/**
 *      @brief Generates a PostScript file containing multiple barcodes
 *      @param codes A double pointer to a Code128 struct containing the barcodes to be printed
 *      @param num_codes The number of barcodes contained in @c codes
 *      @param dest A double pointer to a destination string, whose memory is allocated internally
 *      @param props A PSProperties struct containing the properties of the page in the PostScript
 *             file
 *      @param layout A pointer to a Layout struct containing the number of rows and columns in
 *             which the barcodes should be arranged
 *      @return SUCCESS, or ERR_INVALID_LAYOUT when num_codes exceeds
 *              <tt>(*layout)->cols * (*layout)->rows</tt>
 *      @see PS_DEFAULT_PROPS
 */
int c128_ps_layout(Code128 **, int, char **, const PSProperties *, Layout *);

#endif
