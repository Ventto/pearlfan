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
#ifndef EXPORT_SYMBOL
#define EXPORT_SYMBOL(sym)
#endif

#include <linux/kernel.h>

#include "convert.h"

#define OPEN            0
#define CLOSE           1
#define BEFORE_CLOSE    2

unsigned long long pfan_convert_effect(const char id,
                                       const unsigned char effect[3])
{
	unsigned short opts = 0;

	opts |= effect[CLOSE];
	opts |= (effect[OPEN] << 4);
	opts |= (id << 8);
	opts |= (effect[BEFORE_CLOSE] << 12);

	return 0x00000055000010A0 | (opts << 16);
}
EXPORT_SYMBOL(pfan_convert_effect);

void pfan_convert_raster(char id,
                         unsigned char *raster,
                         unsigned short display[PFAN_IMG_W])
{
	unsigned char col_end;
	unsigned char i;
	unsigned char j;
	unsigned short mask[] = {
		0xFCFF, 0xFBFF, 0xF7FF, 0xFEFF, 0xCFFF, 0xBFFF,
		0x7FFF, 0xFFFE, 0xFFFD, 0xFFFB, 0xFFF7
	};

	for (i = 0; i < PFAN_IMG_W; ++i) {
		col_end = PFAN_IMG_W - i - 1;
		for (j = 0; j < PFAN_IMG_H; ++j)
			if (raster[j * PFAN_IMG_W + i] == 1)
				display[col_end] &=
					mask[PFAN_IMG_H - j - 1];
	}
}
EXPORT_SYMBOL(pfan_convert_raster);
