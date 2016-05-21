#include <linux/limits.h>
#include <pbm.h>
#include <stdio.h>

#include "pbm_local.h"

void pbm_to_usbdata(unsigned char id,
			   unsigned char *raster,
			   uint16_t display[156])
{
	unsigned char i;
	unsigned char j;
	unsigned char col_end;

	for (i = 0; i < IMAGE_WIDTH; ++i) {
		col_end = IMAGE_WIDTH - i - 1;
		for (j = 0; j < IMAGE_HEIGHT; ++j)
			if (raster[j * IMAGE_WIDTH + i] == 1)
				display[col_end] &=
					pbm_mask[IMAGE_HEIGHT - j - 1];
	}
}

unsigned char *pbm_get_specific_raster(FILE *img)
{
	bit *raster = NULL;
	int cols = 0;
	int rows = 0;
	int format = 0;

	/* Getting the configuration from PBM image */
	pbm_readpbminit(img, &cols, &rows, &format);

	/* Invalid image's configuration for the ventilator */
	if (cols != IMAGE_WIDTH && rows != IMAGE_HEIGHT
	    && format != PBM_FORMAT) {
		pm_close(img);
		printf("Error: pbm_check() [cols=%d;rows=%d;format=%d]\n",
		       cols, rows, format);
		printf("Invalid Image Format Error\n");
		return NULL;
	}

	/* Allocation of the raster */
	raster = pbm_allocrow(IMAGE_WIDTH * IMAGE_HEIGHT);

	/* Cannot allocate */
	if (!raster) {
		printf("Error: pbm_allocrow()\n");
		return NULL;
	}

	/* Getting the raster from the PBM image */
	pbm_readpbmrow(img, raster, IMAGE_WIDTH * IMAGE_HEIGHT, format);

	return (unsigned char *)raster;
}
