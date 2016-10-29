#include <linux/limits.h>
#include <pbm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "pbm_utils.h"
#include "ventilo.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Missing argument.\n");
		return EXIT_FAILURE;
	}

	char *config_file = argv[1];
	char imgs[MAX_FAN_DISPLAYS][FILEPATH_MAX];
	char effects[MAX_FAN_DISPLAYS][3];
	int img_nbr;

	if ((img_nbr = read_config_file(config_file, imgs, effects)) < 0) {
		printf("%s: invalid config file.\n", config_file);
		return EXIT_FAILURE;
	}

	FILE *img = NULL;
	unsigned char **rasters = malloc(sizeof(void *) * img_nbr);

	pm_init(argv[0], 0);

	for (int i = 0; i < img_nbr; i++) {
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
		free_pbm_rasters(rasters, img_nbr);
		printf("Cannot open the USB device.\n");
		return EXIT_FAILURE;
	}

	struct ventilo_data data;
	int ret = EXIT_SUCCESS;

	data.n = img_nbr;
	memcpy(data.cfgs, effects, sizeof(effects));
	data.images = rasters;

	if (write(fan_fd, &data, sizeof(struct ventilo_data)) <= 0) {
		printf("Error occured during the data sending. %s\n", strerror(errno));
		ret = EXIT_FAILURE;
	}

	close(fan_fd);
	free_pbm_rasters(rasters, img_nbr);
	return ret;
}
