/****************************************************************************
 * Copyright (C) 2018  Elijah Schutz
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
 *      @file graphic.c
 *      @brief Definitions of barcode graphic output functions.
 *      @author Elijah Schutz
 *      @date 17/12/18
 */

#include <string.h>

#include "errors.h"
#include "graphic.h"
#include "symb.h"

size_t svg_bufsize(int rects) {
    // Add 1 to allow for the null terminator
    return strlen(SVG_HEADER) + SVG_RECT_BUFSIZE * rects + strlen(SVG_FOOTER) + 1;
}

int svg_rect(int x, int y, int w, int h, char *colour, char dest[][SVG_RECT_BUFSIZE]) {
    if (strlen(colour) > SVG_COLOUR_LEN) {
        fprintf(stderr, "colour code '%s' too long", colour);
        return ERR_ARGUMENT;
    }

    snprintf(*dest, SVG_RECT_BUFSIZE,
             "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"%s\"/>", x, y, w, h,
             colour);
    return SUCCESS;
}

int c128_text(char *text, int index, char dest[][SVG_TEXT_BUFSIZE]) {
    snprintf(*dest, SVG_TEXT_BUFSIZE,
             "<text x=\"%d\" y=\"%d\" text-anchor=\"middle\" font-family=\"Helvetica\" "
             "font-size=\"%d\" fill=\"black\">%s</text>",
             index, SVG_LINE_HEIGHT, SVG_FONT_SIZE, text);
    return SUCCESS;
}

int c128_rect_black(int x, int w, char dest[][SVG_RECT_BUFSIZE]) {
    svg_rect(x, SVG_DEFAULT_Y, w, SVG_RECT_HEIGHT, "black", dest);
    return SUCCESS;
}

int c128_rect_white(int x, int w, char dest[][SVG_RECT_BUFSIZE]) {
    svg_rect(x, SVG_DEFAULT_Y, w, SVG_RECT_HEIGHT, "white", dest);
    return SUCCESS;
}

/**
 *      @detail Black and white bars in patterns are represented by binary numerals, where a 0
 *              represents a white bar and a 1 represents a black bar. For example, let a pattern
 *              <tt>p = 0b1101010</tt>. @c p has a width of 7. <tt>p & 1 == 0</tt>, so the last
 *              bar is white. <tt>p >> 1 == 0b110101</tt> and <tt>0b110101 & 1 == 1</tt> so the
 *              second-to-last bar is black. <tt>(p >> 2) & 1 == 0</tt> so the third-to-last bar is
 *              black, and so on until the first digit is reached. Using this process (in reverse),
 *              black and white bars are added to string, generating a barcode SVG from the internal
 *              representation.
 */
int c128_pat2svg(pattern pat, int width, int *svg_x, char **dest) {
    /**
     * A decrementing counter is used so iteration begins from the left. Using the example above,
     * <tt>i == 6</tt> on the first iteration, <tt>(p >> 6) & 1 == 1</tt>, yielding a black bar.
     */
    for (int i = width - 1; i >= 0; i--) {
        if (((pat >> i) & 1) == 1) {
            char bar[SVG_RECT_BUFSIZE];
            c128_rect_black(*svg_x, SVG_RECT_WIDTH, &bar);
            strncat(*dest, bar, strlen(bar));
            // For a white bar, nothing is added as the group fill is white
            // See SVG_HEADER in graphic.h
        }
        *svg_x += SVG_RECT_WIDTH;
    }
    return SUCCESS;
}

int c128_svg(Code128 *code, char **dest) {
    /**
     * Code 128 barcodes have whitespace 'quiet zone' of a prescribed width preceding and following
     * the barcode, which is required for it to be properly readable.
     */
    static const int quiet_width = C128_QUIET_WIDTH * SVG_RECT_WIDTH;

    // +2 to account for the extra two bars at the end of the code
    int    rects     = code->length * C128_DATA_WIDTH + 2 * C128_QUIET_WIDTH + 2;
    size_t dest_size = svg_bufsize(rects);
    *dest            = calloc(1, dest_size);
    VERIFY_NULL(*dest, dest_size);

    strncpy(*dest, SVG_HEADER, strlen(SVG_HEADER));

    // x-coordinate
    // As the background is white, the leading quiet zone is implemented by having quiet_width
    // whitespace before the rectangles are drawn
    int svg_x = quiet_width;
    for (int i = 0; i < code->length; i++) {
        /**
         * All Code 128 patterns are preceded by a black bar and followed by a white bar, so this is
         * removed from the internal representation and added when generating a barcode image.
         */
        pattern pat = code->data[i];
        if (i + 1 != code->length) {
            // Add leading black bar
            char bbar[SVG_RECT_BUFSIZE];
            c128_rect_black(svg_x, SVG_RECT_WIDTH, &bbar);
            strncat(*dest, bbar, strlen(bbar));
            svg_x += SVG_RECT_WIDTH;

            // - 2 to account for leading and trailing bars
            c128_pat2svg(pat, C128_DATA_WIDTH - 2, &svg_x, dest);

            // Add trailing white bar
            svg_x += SVG_RECT_WIDTH;
        } else {
            // Stop code, as i + 1 == code->length (last index)
            c128_pat2svg(pat, C128_STOP_WIDTH, &svg_x, dest);
        }
    }

    // Trailing quiet zone
    svg_x += quiet_width;

    // Add the barcode text beneath the barcode at its centre
    char *text;
    c128_strrepr(code->text, code->length, &text);

    char svg_text[SVG_TEXT_BUFSIZE];
    c128_text(text, svg_x / 2, &svg_text);

    strncat(*dest, svg_text, SVG_TEXT_BUFSIZE);

    strncat(*dest, SVG_FOOTER, SVG_FOOTER_LEN);

    free(text);

    return SUCCESS;
}
