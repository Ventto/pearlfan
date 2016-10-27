#include <linux/limits.h>	/* PATH_MAX */
#include <pbm.h>
#include <stdio.h>			/* printf() */
#include <stdlib.h>			/* malloc() */
#include <string.h>			/* memset() */

#include "config.h"
#include "pbm_utils.h"
#include "ventilo.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Missing argument.\n");
		return EXIT_FAILURE;
	}

	char *cfgfile = argv[1];
	char imgs[8][PATH_MAX];
	char cfgs[8][3];
	int n;

	if (read_config_file(cfgfile, &n, imgs, cfgs) < 0)
		return EXIT_FAILURE;

	FILE *img = NULL;
	unsigned char **rasters = malloc(sizeof(void *) * n);

	pm_init(argv[0], 0);

	for (int i = 0; i < n; i++) {
		img = pm_openr(imgs[i]);
		if (!img) {
			free_pbm_rasters(rasters, i);
			printf("Cannot open PBM image.\n");
			return EXIT_FAILURE;
		}
		rasters[i] = pbm_get_specific_raster(img);
		pm_close(img);
	}

	int fan_fd = open(DEVICE, O_RDWR);

	if (fan_fd <= 0) {
		free_pbm_rasters(rasters, n);
		printf("Cannot open the USB device.\n");
		return EXIT_FAILURE;
	}

	struct ventilo_data data;
	int ret = EXIT_SUCCESS;

	data.n = n;
	memcpy(data.cfgs, cfgs, sizeof(cfgs));
	data.images = rasters;

	if (write(fan_fd, &data, sizeof(struct ventilo_data)) <= 0) {
		printf("Error occured during the data sending. %s\n", strerror(errno));
		ret = EXIT_FAILURE;
	}

	close(fan_fd);
	free_pbm_rasters(rasters, n);
	return ret;
}
