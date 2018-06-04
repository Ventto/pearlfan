/*
 * The GPL License (GPLv3)
 *
 * Copyright © 2016 Thomas "Ventto" Venriès  <thomas.venries@gmail.com>
 *
 * Pearlfan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pearlfan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pearlfan.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>

#include "draw.h"
#include "font.h"

#define PFAN_LSPACE        1
#define PFAN_CHAR_SPACE    (PFAN_CHAR_W + PFAN_LSPACE)
#define PFAN_MAX_CHAR      (PFAN_MAX_W / PFAN_CHAR_SPACE)

static unsigned short led_mask[] = {
    0xFFF7,/* [0]  LED10*/
    0xFFFB,/* [1]  LED9 */
    0xFFFD,/* [2]  LED8 */
    0xFFFE,/* [3]  LED7 */
    0x7FFF,/* [4]  LED6 */
    0xBFFF,/* [5]  LED5 */
    0xDFFF,/* [6]  LED4 */
    0xEFFF,/* [7]  LED3 */
    0xF7FF,/* [8]  LED2 */
    0xFBFF,/* [9]  LED1 */
    0xFCFF,/* [10] LED0 */
};

void pfan_draw_point(int xpos,
                     int ypos,
                     unsigned short display[PFAN_MAX_W])
{
    if (xpos < 0 || xpos >= PFAN_MAX_W || ypos < 0 || ypos >= PFAN_MAX_H) {
        return;
    }

    display[PFAN_MAX_W - 1 - xpos] &= led_mask[ypos];
}

void pfan_draw_image(unsigned char *raster,
                     unsigned short display[PFAN_MAX_W])
{
    for (unsigned char i = 0; i < PFAN_MAX_W; ++i) {
        for (unsigned char j = 0; j < PFAN_MAX_H; ++j) {
            if (raster[j * PFAN_MAX_W + i] == 1) {
                pfan_draw_point(i, j, display);
            }
        }
    }
}

void pfan_draw_char(int xpos,
                    int c,
                    unsigned short display[PFAN_MAX_W])
{
    if (xpos < 0 || xpos >= PFAN_MAX_W)
        return;

    for (int j = 0; j < PFAN_MAX_H; ++j)  {
        for (int i = 0; i < PFAN_CHAR_W; ++i) {
            /* Because of little indian */
            if (pfan_font[c][j] & (0x01 << i))
                pfan_draw_point(xpos + i, j, display);
        }
    }
}

void pfan_draw_text(char *text,
                    int lenght,
                    int lspace,
                    unsigned short display[PFAN_MAX_W])
{
    int i = 0;

    if (lenght == 0 || lenght > PFAN_MAX_CHAR) {
        return;
    }

    pfan_draw_char(0, text[i], display);

    /* From 1 because of first led column is disabled when it is rotating */
    for (i = 1; i < lenght; ++i) {
        pfan_draw_char(i * (PFAN_CHAR_W + lspace), text[i], display);
    }
}

int pfan_draw_paragraph(char *text,
                        unsigned short display[PFAN_MAX_DISPLAY][PFAN_MAX_W])
{
    char *itr = text;
    int count = 0;
    int chars = 0;
    int i;

    fprintf(stdout, "\nDisplays:\n");

    for (i = 0; i < PFAN_MAX_DISPLAY; ++i) {
        if (*itr == '\0') {
            break;
        }
        for (;;) {
            if (*itr == '\0') {
                break;
            }
            if (*itr > 32) {
                chars = 0;
                do {
                    chars++;
                    itr++;
                } while (*itr > 32);
                if (count + chars > PFAN_MAX_CHAR) {
                    /* Big word */
                    if (!count) {
                        count = PFAN_MAX_CHAR;
                        itr -= chars - PFAN_MAX_CHAR;
                    } else if (chars > PFAN_MAX_CHAR) {
                        itr -= chars - (PFAN_MAX_CHAR - count);
                        count = PFAN_MAX_CHAR;
                    }   else
                        itr -= chars;
                    break;
                } else {
                    count += chars;
                }
            } else {
                itr++;
                count++;
            }
            if (count == PFAN_MAX_CHAR) {
                break;
            }
        }
        pfan_draw_text(itr - count, count, PFAN_LSPACE, display[i]);
        fprintf(stdout, "%d: [%.*s%*s]\n", i + 1, count, itr - count,
                PFAN_MAX_CHAR - count, "");
        count = 0;
    }

    if (*itr == '\0') {
        return i;
    }

    fprintf(stderr, "Error: the text is over the display limit of 8.\n\n");
    return 0;
}
