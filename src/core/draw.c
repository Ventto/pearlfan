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

#define PFAN_LETTER_W         6
#define PFAN_MAX_CHAR         (PFAN_MAX_W / PFAN_LETTER_W - 1)

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

unsigned long long pfan_convert_effect(const char id,
                                       const unsigned char effect[3])
{
	unsigned short opts = 0;

	opts |= effect[PFAN_CLOSE];
	opts |= (effect[PFAN_OPEN] << 4);
	opts |= (id << 8);
	opts |= (effect[PFAN_BEFORECLOSE] << 12);

	return 0x00000055000010A0 | (opts << 16);
}

void pfan_draw_point(int xpos,
                     int ypos,
                     unsigned short display[PFAN_MAX_W])
{
	if (xpos < 0 || xpos >= PFAN_MAX_W
			|| ypos < 0 || ypos >= PFAN_MAX_H)
		return;

	display[155 - xpos] &= led_mask[ypos];
}

void pfan_draw_image(unsigned char *raster,
                     unsigned short display[PFAN_MAX_W])
{
	unsigned char i;
	unsigned char j;

	for (i = 0; i < PFAN_MAX_W; ++i) {
		for (j = 0; j < PFAN_MAX_H; ++j)
			if (raster[j * PFAN_MAX_W + i] == 1)
				pfan_draw_point(i, j, display);
	}
}

void pfan_draw_char(int xpos,
                    int c,
                    unsigned short display[PFAN_MAX_W])
{
	int i = 0;
	int j = 0;

	if (xpos < 0 || xpos >= PFAN_MAX_W)
		return;

	for (j = 0; j < PFAN_MAX_H; ++j)  {
		for (i = 0; i < PFAN_CHAR_W; ++i) {
			/* Because of little indian */
			if (pfan_6x11_font[c][j] & (0x01 << i))
				pfan_draw_point(xpos + i, j, display);
		}
	}
}

void pfan_draw_text(char *text, int length, unsigned short display[PFAN_MAX_W])
{
	int i = 0;

	if (length > PFAN_MAX_CHAR)
		return;

	for (i = 0; i < length; ++i)
		pfan_draw_char(i * 6, text[i], display);
}

int pfan_draw_paragraph(char *text,
                        unsigned short display[PFAN_MAX_DISPLAY][PFAN_MAX_W])
{
	int i;
	int count = 0;
	int chars = 0;
	char *itr = text;

	fprintf(stdout, "Displays:\n\n");
	for (i = 0; i < PFAN_MAX_DISPLAY; ++i) {
		if (*itr == '\0')
			break;
		for (;;) {
			if (*itr == '\0')
				break;
			if (*itr > 32) {
				chars = 0;
				do {
					chars++;
					itr++;
				} while (*itr > 32);
				if (count + chars > PFAN_MAX_CHAR) {
					itr -= chars;
					break;
				} else {
					count += chars;
				}
			} else {
				itr++;
				count++;
			}
			if (count == PFAN_MAX_CHAR)
				break;
		}
		if (count > 0) {
			pfan_draw_text(itr - count, count, display[i]);
			fprintf(stdout, "%d: [%.*s%*s]\n\n", i + 1, count, itr - count,
					PFAN_MAX_CHAR - count, "");
		} else {
			fprintf(stderr, "Error: a word is over the character limit of %d.\n\n",
					PFAN_MAX_CHAR);
			return 0;
		}
		count = 0;
	}

	if (*itr == '\0')
		return i;

	fprintf(stderr, "Error: the text is over the display limit of 8.\n\n");
	return 0;
}
