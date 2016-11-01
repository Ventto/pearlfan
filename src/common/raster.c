#include <linux/limits.h>
#include <pbm.h>
#include <stdio.h>

#include "raster.h"

#define PFAN_IMG_W    156
#define PFAN_IMG_H    11

static bit *create_raster(FILE *img)
{
	int cols;
	int rows;
	int format;
	bit *raster = NULL;

	pbm_readpbminit(img, &cols, &rows, &format);

	if (cols != PFAN_IMG_W && rows != PFAN_IMG_H) {
		printf("pfan: invalid image sizes, '11x156' is required.\n");
		return NULL;
	}

	if (format != PBM_FORMAT) {
		printf("pfan: invalid image file sizes.\n");
		return NULL;
	}

	raster = pbm_allocrow(PFAN_IMG_W * PFAN_IMG_H);
	if (!raster) {
		printf("pfan: cannot allocate the raster.\n");
		return NULL;
	}

	pbm_readpbmrow(img, raster, PFAN_IMG_W * PFAN_IMG_H, format);

	return raster;
}

bit **pfan_create_rasters(char image_paths[8][8193], int image_nbr)
{
	pm_init("pfan", 0);

	bit **rasters = malloc(sizeof(void *) * image_nbr);

	if (!rasters)
		return NULL;

	FILE *img = NULL;

	for (int i = 0; i < image_nbr; ++i) {
		img = pm_openr(image_paths[i]);
		if (!img) {
			pfan_free_rasters(rasters, i);
			printf("pfan: can not open '%s'\n", image_paths[i]);
			return NULL;
		}
		rasters[i] = create_raster(img);
		pm_close(img);
	}

	return rasters;
}

void pfan_free_rasters(bit **rasters, int n)
{
	if (!rasters)
		return;
	for (int i = 0; i < n; ++i)
		pbm_freerow(rasters[i]);
	free(rasters);
}
