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
#include <linux/limits.h>
#include <pbm.h>
#include <stdio.h>

#include "raster.h"

#define PFAN_IMG_W    156
#define PFAN_IMG_H    11

static bit *create_img_raster(FILE *img)
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

uint8_t **pfan_create_img_rasters(char img_paths[8][4096], int n)
{
	pm_init("pfan", 0);

	uint8_t **rasters = malloc(sizeof(void *) * n);

	if (!rasters) {
		fprintf(stderr, "Can not allocate raster container.\n");
		return NULL;
	}

	FILE *img = NULL;

	for (int i = 0; i < n; ++i) {
		img = pm_openr(img_paths[i]);
		if (!img) {
			pfan_free_rasters(rasters, i);
			fprintf(stderr, "Can not open '%s'\n", img_paths[i]);
			return NULL;
		}
		rasters[i] = create_img_raster(img);
		pm_close(img);
	}

	return rasters;
}

void pfan_free_rasters(uint8_t **rasters, int n)
{
	if (!rasters)
		return;
	for (int i = 0; i < n; ++i)
		pbm_freerow(rasters[i]);
	free(rasters);
}
