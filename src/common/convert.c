#include <linux/kernel.h>

#ifndef EXPORT_SYMBOL
#define EXPORT_SYMBOL(sym)
#endif

#include "convert.h"

#define PFAN_IMG_H    11

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
