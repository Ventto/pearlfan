#include <linux/limits.h>
#include <pbm.h>
#include <stdio.h>

#include "pbm_utils.h"

void pbm_to_usbdata(char id, uint8_t *raster, uint16_t display[156])
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

uint8_t *pbm_get_specific_raster(FILE *img)
{
	bit *raster = NULL;
	int cols, rows, format;

	pbm_readpbminit(img, &cols, &rows, &format);

	if (cols != IMAGE_WIDTH && rows != IMAGE_HEIGHT
			&& format != PBM_FORMAT) {
		printf("netpbm: invalid image file sizes.\n");
		printf("[cols=%d;rows=%d;format=%d]\n", cols, rows, format);
		return NULL;
	}

	raster = pbm_allocrow(IMAGE_WIDTH * IMAGE_HEIGHT);
	if (!raster) {
		printf("netpbm: cannot allocate the raster.\n");
		return NULL;
	}

	/* Getting the raster from the PBM image */
	pbm_readpbmrow(img, raster, IMAGE_WIDTH * IMAGE_HEIGHT, format);

	return (uint8_t *)raster;
}

void free_pbm_rasters(uint8_t **rasters, int n)
{
	if (!rasters)
		return;
	for (int i = 0; i < n; ++i)
		pbm_freerow(rasters[i]);
	free(rasters);
}
