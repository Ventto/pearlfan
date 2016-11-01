#include <pbm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "devinfo.h"
#include "raster.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: pfan <config>\n");
		return EXIT_FAILURE;
	}

	char *config_file = argv[1];
	char image_paths[PFAN_DISPLAY_MAX][FILEPATH_MAX];
	uint8_t effects[PFAN_DISPLAY_MAX][3];
	int image_nbr;

	if ((image_nbr = pfan_read_config(config_file, image_paths, effects)) < 0) {
		printf("%s: invalid config file.\n", config_file);
		return EXIT_FAILURE;
	}

	bit **rasters = pfan_create_rasters(image_paths, image_nbr);

	if (!rasters)
		return EXIT_FAILURE;

	int pfan_fd = open(PFAN_DEVNAME, O_RDWR);

	if (pfan_fd <= 0) {
		pfan_free_rasters(rasters, image_nbr);
		printf("Cannot open the USB device.\n");
		return EXIT_FAILURE;
	}

	struct pfan_data *data = malloc(sizeof(struct pfan_data));
	int ret = EXIT_SUCCESS;

	memset(data, 0, sizeof(struct pfan_data));

	data->n = image_nbr;
	memcpy(data->effects, effects, sizeof(effects));
	data->images = rasters;

	if (write(pfan_fd, data, sizeof(struct pfan_data)) <= 0) {
		printf("Error occured during the data sending. %s\n", strerror(errno));
		ret = EXIT_FAILURE;
	}

	close(pfan_fd);
	free(data);
	pfan_free_rasters(rasters, image_nbr);
	return ret;
}
