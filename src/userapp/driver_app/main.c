#include <pbm.h>
#include <linux/limits.h>
#include <string.h>		/* memset() */
#include <stdio.h>		/* printf() */
#include <stdlib.h>		/* malloc() */

#include "pbm_utils.h"
#include "config.h"
#include "ventilo_data.h"

int main(int argc, char **argv)
{
	/* =--------------------------------= */
	/*     Open the configuration file    */
	/* =--------------------------------= */
	/* Missing argument */
	if (argc < 2) {
		printf("Error: a parameter is missing.\n");
		return EXIT_FAILURE;
	}

	char imgs[8][PATH_MAX];
	char cfgs[8][3];
	int n;

	if (read_config_file("config", &n, imgs, cfgs) < 0)
		return EXIT_FAILURE;

	/* =--------------------------------= */
	/*   Extract rasters from PBM images  */
	/* =--------------------------------= */
	FILE *img = NULL;
	unsigned char **rasters = malloc(sizeof(void *) * n);
	/* Initialization before importing PBM image*/
	pm_init(argv[0], 0);

	int i;
	/* Getting the configuration from PBM image */
	for (i = 0; i < n; i++) {
		img = pm_openr(imgs[i]);
		/* Cannot open file */
		if (!img) {
			free_pbm_rasters(rasters, i);
			printf("Cannot open PBM image\n");
			return EXIT_FAILURE;
		}
		rasters[i] = pbm_get_specific_raster(img);
		/* Close image file */
		pm_close(img);
	}

	/* =--------------------------------= */
	/* Send all data to the dev's driver  */
	/* =--------------------------------= */
	int fd;

	fd = open(DEVICE, O_RDWR);

	if (fd <= 0) {
		free_pbm_rasters(rasters, n);
		printf("Cannot open the device\n");
		return EXIT_FAILURE;
	}

	int r = EXIT_SUCCESS;

	struct ventilo_data data;

	data.n = n;
	memcpy(data.cfgs, cfgs, sizeof(cfgs));
	data.images = rasters;

	if (write(fd, &data, sizeof(struct ventilo_data)) <= 0) {
		printf("An error occured for write(). %s\n", strerror(errno));
		r = EXIT_FAILURE;
	}

	/* =--------------------------------= */
	/*          Free all used data        */
	/* =--------------------------------= */
	/* Close device */
	close(fd);
	/* Free PBM data */
	free_pbm_rasters(rasters, n);

	return r;
}
