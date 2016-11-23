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
#include <pbm.h>
#include <stdio.h>

#include "defutils.h"
#include "raster.h"

static bit *create_img_raster(FILE *img)
{
	int cols;
	int rows;
	int format;
	bit *raster = NULL;

	pbm_readpbminit(img, &cols, &rows, &format);

	if (cols != PFAN_MAX_W && rows != PFAN_MAX_H) {
		fprintf(stdout, "Invalid image size, '156x11' is required.\n");
		return NULL;
	}

	if (format != PBM_FORMAT) {
		fprintf(stdout, "Invalid .PBM format.\n");
		return NULL;
	}

	raster = pbm_allocrow(PFAN_MAX_W * PFAN_MAX_H);
	if (!raster) {
		fprintf(stdout, "Can not allocate the raster.\n");
		return NULL;
	}

	pbm_readpbmrow(img, raster, PFAN_MAX_W * PFAN_MAX_H, format);

	return raster;
}

uint8_t **pfan_create_img_rasters(char img_paths[PFAN_MAX_DISPLAY][MAX_PATH],
                                  int n)
{
	pm_init("pfan", 0);

	uint8_t **rasters = malloc(sizeof(void *) * n);

	if (!rasters) {
		fprintf(stderr, "Can not allocate the raster array.\n");
		return NULL;
	}

	FILE *img = NULL;

	fprintf(stdout, "\nDisplays:\n");
	for (int i = 0; i < n; ++i) {
		img = pm_openr(img_paths[i]);
		if (!img) {
			pfan_free_rasters(rasters, i);
			fprintf(stderr, "Can not open '%s'\n", img_paths[i]);
			return NULL;
		}
		rasters[i] = create_img_raster(img);
		fprintf(stdout, "%d: '%s'\n", i + 1, img_paths[i]);
		pm_close(img);
	}

	return rasters;
}

void pfan_free_rasters(uint8_t **rasters, int n)
{
	for (int i = 0; i < n; ++i)
		pbm_freerow(rasters[i]);
	free(rasters);
}

void pfan_free_type_rasters(uint8_t **rasters, int n, uint8_t types[8])
{
	for (int i = 0; i < n; ++i)
		(types[PFAN_IS_IMG]) ? pbm_freerow(rasters[i]) : free(rasters[i]);
	free(rasters);
}
