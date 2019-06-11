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
 *      @file graphic.c
 *      @brief Definitions of barcode graphic output functions.
 *      @author Elijah Schutz
 *      @date 17/12/18
 */

#include <string.h>

#include "barcode/errors.h"
#include "barcode/graphic.h"
#include "barcode/symb.h"

const PSProperties PS_DEFAULT_PROPS = {.units = PS_UNIT,
                                       // .p_width = A4_WIDTH,
                                       // .p_height = A4_HEIGHT,
                                       .lmargin      = PS_MARGIN,
                                       .rmargin      = PS_MARGIN,
                                       .tmargin      = 2 * PS_MARGIN,
                                       .bmargin      = 2 * PS_MARGIN,
                                       .bar_width    = PS_WIDTH,
                                       .bar_height   = PS_HEIGHT,
                                       .padding      = PS_PAD,
                                       .column_width = PS_COL_W,
                                       .fontsize     = PS_FONT_SIZE};

size_t svg_bufsize(int rects) {
    // Add 1 to allow for the null terminator
    return strlen(SVG_HEADER) + SVG_RECT_BUFSIZE * rects + SVG_TEXT_BUFSIZE + strlen(SVG_FOOTER) +
           1;
}

size_t ps_bufsize(int rects) {
    // Add 1 for the null terminator
    return PS_CMD_BUFSIZE * rects + PS_TEXT_BUFSIZE + 1;
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

int c128_ps_text(char *text, int fsize, int index, char dest[][PS_TEXT_BUFSIZE]) {
    snprintf(*dest, PS_TEXT_BUFSIZE, PS_TEXT, fsize, text, index);
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

int c128_ps_rect_black(char dest[][PS_CMD_BUFSIZE]) {
    strncpy(*dest, PS_BAR, strlen(PS_BAR));
    return SUCCESS;
}

int c128_ps_rect_white(int w, char dest[][PS_CMD_BUFSIZE]) {
    snprintf(*dest, PS_CMD_BUFSIZE, PS_WSPACE, w);
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
        if (((pat >> i) & 1) == Black) {
            char bar[SVG_RECT_BUFSIZE];
            c128_rect_black(*svg_x, SVG_RECT_WIDTH, &bar);
            strncat(*dest, bar, SVG_RECT_BUFSIZE);
            // For a white bar, nothing is added as the group fill is white
            // See SVG_HEADER in graphic.h
        }
        *svg_x += SVG_RECT_WIDTH;
    }
    return SUCCESS;
}

/**
 *      @detail See c128_pat2svg() for details on the algorithim used to print barcodes
 *      @see c128_pat2svg
 */
int c128_pat2ps(pattern pat, int width, float *ps_x, char **dest, const PSProperties *props) {
    char bar[PS_CMD_BUFSIZE];
    memset(&bar, 0, PS_CMD_BUFSIZE);
    for (int i = width - 1; i >= 0; i--) {
        if (((pat >> i) & 1) == Black) {
            c128_ps_rect_black(&bar);
        } else {
            // 1, as there is a single bar
            c128_ps_rect_white(1, &bar);
        }
        strncat(*dest, bar, PS_CMD_BUFSIZE);
        *ps_x += props->bar_width;
        memset(&bar, 0, PS_CMD_BUFSIZE);
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
    int    rects     = code->datalen * C128_DATA_WIDTH + 2 * C128_QUIET_WIDTH + 2;
    size_t dest_size = svg_bufsize(rects);
    *dest            = calloc(1, dest_size);
    VERIFY_NULL(*dest, dest_size);

    strncpy(*dest, SVG_HEADER, strlen(SVG_HEADER));

    // x-coordinate
    // As the background is white, the leading quiet zone is implemented by having quiet_width
    // whitespace before the rectangles are drawn
    int svg_x = quiet_width;
    for (int i = 0; i < code->datalen; i++) {
        /**
         * All Code 128 patterns are preceded by a black bar and followed by a white bar, so this is
         * removed from the internal representation and added when generating a barcode image.
         */
        char    bbar[SVG_RECT_BUFSIZE];
        pattern pat = code->data[i];
        if (i + 1 != code->datalen) {
            // Add leading black bar

            memset(&bbar, 0, SVG_RECT_BUFSIZE);
            c128_rect_black(svg_x, SVG_RECT_WIDTH, &bbar);
            strncat(*dest, bbar, strlen(bbar));
            svg_x += SVG_RECT_WIDTH;

            // - 2 to account for leading and trailing bars
            c128_pat2svg(pat, C128_DATA_WIDTH - 2, &svg_x, dest);

            // Add trailing white bar
            svg_x += SVG_RECT_WIDTH;
        } else {
            // Stop code, as i + 1 == code->datalen (last index)
            c128_pat2svg(pat, C128_STOP_WIDTH, &svg_x, dest);
        }
    }

    // Trailing quiet zone
    svg_x += quiet_width;

    // Add the barcode text beneath the barcode at its centre
    char *text;
    c128_strrepr(code->text, code->textlen, &text);

    char svg_text[SVG_TEXT_BUFSIZE];
    c128_text(text, svg_x / 2, &svg_text);

    strncat(*dest, svg_text, SVG_TEXT_BUFSIZE);

    strncat(*dest, SVG_FOOTER, SVG_FOOTER_LEN);

    free(text);

    return SUCCESS;
}

/**
 *      @detail This function is used internally, so you're probably looking for c128_ps_layout()
 *      @see c128_ps_layout()
 */
int c128_ps_init(char **dest, int barcodes) {
    // + 1 for null terminator
    size_t dest_size = PS_HEADER_BUFSIZE +
                       ps_bufsize(C128_MAX_PATTERN_SIZE * C128_DATA_WIDTH + 2 * C128_QUIET_WIDTH +
                                  PS_MAX_POSITION_CMDS * PS_CMD_BUFSIZE) *
                           barcodes +
                       strlen(PS_FOOTER) + 1;
    *dest = calloc(1, dest_size);
    VERIFY_NULL(*dest, dest_size);

    return SUCCESS;
}

/**
 *      @detail This function is used internally, so you're probably looking for c128_ps_layout()
 *      @see c128_ps_layout()
 */
int c128_ps_header(char **dest, const PSProperties *props) {
    char header[PS_HEADER_BUFSIZE];
    snprintf(header, PS_HEADER_BUFSIZE, PS_HEADER, props->units, props->lmargin, props->rmargin,
             props->tmargin, props->bmargin, props->bar_width, props->bar_height, props->padding,
             props->column_width);

    strncpy(*dest, header, PS_HEADER_BUFSIZE);

    return SUCCESS;
}

/**
 *      @detail This function is used internally, so you're probably looking for c128_ps_layout()
 *      @see c128_ps_layout()
 */
int c128_ps_footer(char **dest) {
    strncat(*dest, PS_FOOTER, PS_FOOTER_LEN);
    return SUCCESS;
}

/**
 *      @detail This function is used internally, so you're probably looking for c128_ps_layout()
 *      @see c128_ps_layout()
 */
int c128_ps(Code128 *code, char **dest, const PSProperties *props) {
    char quiet_zone[PS_CMD_BUFSIZE];
    c128_ps_rect_white(C128_QUIET_WIDTH, &quiet_zone);
    strncat(*dest, quiet_zone, PS_CMD_BUFSIZE);

    int   quiet_width = C128_QUIET_WIDTH * props->bar_width;
    float ps_x        = quiet_width;

    for (int i = 0; i < code->datalen; i++) {
        pattern pat = code->data[i];

        char bar[PS_CMD_BUFSIZE];
        memset(&bar, 0, PS_CMD_BUFSIZE);

        if (i + 1 != code->datalen) {
            c128_ps_rect_black(&bar);
            strncat(*dest, bar, PS_CMD_BUFSIZE);
            ps_x += props->bar_width;

            c128_pat2ps(pat, C128_DATA_WIDTH - 2, &ps_x, dest, props);

            memset(&bar, 0, PS_CMD_BUFSIZE);
            c128_ps_rect_white(1, &bar);
            strncat(*dest, bar, PS_CMD_BUFSIZE);
            ps_x += props->bar_width;
        } else {
            c128_pat2ps(pat, C128_STOP_WIDTH, &ps_x, dest, props);
        }
    }

    ps_x += props->bar_width;
    strncat(*dest, quiet_zone, PS_CMD_BUFSIZE);
    ps_x += quiet_width;

    char *text;
    c128_strrepr(code->text, code->textlen, &text);

    char ps_text[PS_TEXT_BUFSIZE];
    c128_ps_text(text, props->fontsize, ps_x / 2, &ps_text);

    strncat(*dest, ps_text, PS_TEXT_BUFSIZE);

    free(text);

    return SUCCESS;
}

int c128_ps_layout(Code128 **codes, int num_codes, char **dest, const PSProperties *props,
                   Layout *layout) {
    unsigned int max_codes = layout->cols * layout->rows;
    if ((unsigned int)num_codes > max_codes || max_codes == 0) {
        return ERR_INVALID_LAYOUT;
    }

    c128_ps_init(dest, num_codes);
    c128_ps_header(dest, props);

    int row, col, lrow = -1, lcol = -1;
    for (int i = 0; i < num_codes; i++) {
        row = i / layout->cols;
        col = i % layout->cols;

        if (lrow != -1 && row != lrow) {
            char rpos[PS_CMD_BUFSIZE];
            snprintf(rpos, PS_CMD_BUFSIZE, PS_RPOS, 0.0, props->bar_height + props->fontsize);
            strncat(*dest, rpos, PS_CMD_BUFSIZE);
            strncat(*dest, PS_PADY, PS_CMD_BUFSIZE);
            strncat(*dest, PS_RESET_X, PS_CMD_BUFSIZE);
        }
        if (lcol != -1 && col != lcol) {
            char col_pos[PS_CMD_BUFSIZE];
            snprintf(col_pos, PS_CMD_BUFSIZE, PS_COL_POS, props->column_width, col);
            strncat(*dest, col_pos, PS_CMD_BUFSIZE);
            strncat(*dest, PS_PADX, PS_CMD_BUFSIZE);
            // strncat(*dest, PS_RESET_Y, PS_CMD_BUFSIZE);
        }

        c128_ps((codes)[i], dest, props);

        lrow = row;
        lcol = col;
    }

    c128_ps_footer(dest);

    return SUCCESS;
}
