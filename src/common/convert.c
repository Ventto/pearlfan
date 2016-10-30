#include "convert.h"
#include "mask.h"

#define IMAGE_WIDTH	156
#define IMAGE_HEIGHT	11

unsigned long long pfan_convert_effect(const char id, const char effect[3])
{
	unsigned short opts = 0;

	opts |= effect[1];
	opts |= (effect[0] << 4);
	opts |= (id << 8);
	opts |= (effect[2] << 12);

	return 0x00000055000010A0 | (opts << 16);
}

void pfan_convert_raster(char id, unsigned char *raster,
		unsigned short display[156])
{
	unsigned char col_end;
	unsigned char i;
	unsigned char j;

	for (i = 0; i < IMAGE_WIDTH; ++i) {
		col_end = IMAGE_WIDTH - i - 1;
		for (j = 0; j < IMAGE_HEIGHT; ++j)
			if (raster[j * IMAGE_WIDTH + i] == 1)
				display[col_end] &=
					pfan_mask[IMAGE_HEIGHT - j - 1];
	}
}
