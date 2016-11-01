#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "config.h"
#include "devinfo.h"
#include "raster.h"
#include "send.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: pfan <config>\n");
		return 1;
	}

	char *config_file = argv[1];
	char image_paths[8][FILEPATH_MAX];
	uint8_t effects[8][3];
	int image_nbr;

	if ((image_nbr = pfan_read_config(config_file, image_paths, effects)) < 0) {
		printf("pfan: invalid config file.\n\n");
		return 1;
	}

	uint8_t **images = pfan_create_rasters(image_paths, image_nbr);

	if (!images)
		return 1;

	int pfan_fd = open(PFAN_DEVNAME, O_RDWR);

	if (pfan_fd <= 0) {
		pfan_free_rasters(images, image_nbr);
		printf("pfan: device cannot be opened or found.\n\n");
		return 1;
	}

	printf("pfan: device found.\n");
	printf("pfan: transfer is starting.\n");

	int ret = 0;

	if (send(pfan_fd, image_nbr, images, effects) < sizeof(struct pfan_data)) {
		printf("pfan: transfer was aborted.\n\n");
		ret = 1;
	} else
		printf("pfan: transfer is complete.\n\n");

	close(pfan_fd);
	pfan_free_rasters(images, image_nbr);
	return ret;
}
