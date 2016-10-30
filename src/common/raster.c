#include <linux/limits.h>
#include <pbm.h>
#include <stdio.h>

#include "raster.h"

#define IMAGE_WIDTH	156
#define IMAGE_HEIGHT	11

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
