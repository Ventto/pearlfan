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
#include <linux/kernel.h>

#include "defutils.h"
#include "draw.h"
#include "font.h"

#ifndef EXPORT_SYMBOL
# define EXPORT_SYMBOL(sym)
#endif

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
EXPORT_SYMBOL(pfan_convert_effect);

void pfan_draw_point(int xpos,
                     int ypos,
                     unsigned short display[PFAN_MAX_W])
{
	if (xpos < 0 || xpos >= PFAN_MAX_W
			|| ypos < 0 || ypos >= PFAN_MAX_H)
		return;

	display[155 - xpos] &= led_mask[ypos];
}
EXPORT_SYMBOL(pfan_draw_point);

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
EXPORT_SYMBOL(pfan_draw_image);

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
EXPORT_SYMBOL(pfan_draw_char);

void pfan_draw_text(char *text, int length, unsigned short display[PFAN_MAX_W])
{
	int i = 0;

	if (length > PFAN_MAX_CHAR)
		return;

	for (i = 0; i < length; ++i)
		pfan_draw_char(i * 6, text[i], display);
}
EXPORT_SYMBOL(pfan_draw_text);
