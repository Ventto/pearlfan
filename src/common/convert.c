#include <linux/kernel.h>

#ifndef EXPORT_SYMBOL
#define EXPORT_SYMBOL(sym)
#endif

#include "convert.h"

#define IMG_WIDTH     156
#define IMG_HEIGHT    11

unsigned long long pfan_convert_effect(const char id, const char effect[3])
{
	unsigned short opts = 0;

	opts |= effect[1];
	opts |= (effect[0] << 4);
	opts |= (id << 8);
	opts |= (effect[2] << 12);

	return 0x00000055000010A0 | (opts << 16);
}
EXPORT_SYMBOL(pfan_convert_effect);

void pfan_convert_raster(char id, unsigned char *raster,
		unsigned short display[156])
{
	unsigned char col_end;
	unsigned char i;
	unsigned char j;
	unsigned short mask[] = {
		0xFCFF, 0xFBFF, 0xF7FF, 0xFEFF, 0xCFFF, 0xBFFF,
		0x7FFF, 0xFFFE, 0xFFFD, 0xFFFB, 0xFFF7
	};

	for (i = 0; i < IMG_WIDTH; ++i) {
		col_end = IMG_WIDTH - i - 1;
		for (j = 0; j < IMG_HEIGHT; ++j)
			if (raster[j * IMG_WIDTH + i] == 1)
				display[col_end] &=
					mask[IMG_HEIGHT - j - 1];
	}
}
EXPORT_SYMBOL(pfan_convert_raster);
