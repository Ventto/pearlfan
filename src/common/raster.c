#include <linux/limits.h>
#include <pbm.h>
#include <stdio.h>

#include "raster.h"

#define IMAGE_WIDTH	156
#define IMAGE_HEIGHT	11
#define LEDS_NUMBER	11

static const uint16_t pbm_mask[] = {
	0xFCFF, 0xFBFF, 0xF7FF,
	0xFEFF, 0xCFFF, 0xBFFF,
	0x7FFF, 0xFFFE, 0xFFFD,
	0xFFFB, 0xFFF7
} ;

void pfan_convert_raster(char id, bit *raster, uint16_t display[156])
{
	uint8_t col_end;

	for (uint8_t i = 0; i < IMAGE_WIDTH; ++i) {
		col_end = IMAGE_WIDTH - i - 1;
		for (uint8_t j = 0; j < IMAGE_HEIGHT; ++j)
			if (raster[j * IMAGE_WIDTH + i] == 1)
				display[col_end] &=
					pbm_mask[IMAGE_HEIGHT - j - 1];
	}
}

bit *pfan_create_raster(FILE *img)
{
	int cols;
	int rows;
	int format;
	bit *raster = NULL;

	pbm_readpbminit(img, &cols, &rows, &format);

	if (cols != IMAGE_WIDTH && rows != IMAGE_HEIGHT) {
		printf("pfan: invalid image sizes, '11x156' is required.\n");
		return NULL;
	}

	if (format != PBM_FORMAT) {
		printf("pfan: invalid image file sizes.\n");
		return NULL;
	}

	raster = pbm_allocrow(IMAGE_WIDTH * IMAGE_HEIGHT);
	if (!raster) {
		printf("pfan: cannot allocate the raster.\n");
		return NULL;
	}

	pbm_readpbmrow(img, raster, IMAGE_WIDTH * IMAGE_HEIGHT, format);

	return raster;
}

void pfan_free_rasters(bit **rasters, int n)
{
	if (!rasters)
		return;
	for (int i = 0; i < n; ++i)
		pbm_freerow(rasters[i]);
	free(rasters);
}
