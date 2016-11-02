#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "config.h"
#include "devinfo.h"
#include "optutils.h"
#include "raster.h"
#include "send.h"

int main(int argc, char **argv)
{
	pfan_opts opts;
	int ret;

	if (( ret = pfan_getopt(argc, argv, &opts) ) != PFAN_VALID_OPT)
		return ret;

	char    img_paths[PFAN_IMG_MAX][4096];
	uint8_t effects[PFAN_IMG_MAX][3];
	int     img_nbr;

	if (opts.cflag &&
		( img_nbr = pfan_read_cfg(opts.carg, img_paths, effects,
				opts.fflag) ) < 0) {
		fprintf(stderr, "Invalid config file.\n\n");
		return 1;
	}

	if (opts.dflag &&
		( img_nbr = pfan_read_dir(opts.darg, img_paths, effects,
				opts.fflag) ) < 0) {
		fprintf(stderr, "Can not open '%s' directory.\n\n", opts.darg);
		return 1;
	}

	if (!img_nbr) {
		fprintf(stdout, "No PBM image found.\n\n");
		return 0;
	}

	uint8_t **images = pfan_create_rasters(img_paths, img_nbr);

	if (!images)
		return 1;

	int pfan_fd = open(PFAN_DEVNAME, O_RDWR);

	if (pfan_fd <= 0) {
		pfan_free_rasters(images, img_nbr);
		fprintf(stderr, "Device can not be opened or found.\n");
		fprintf(stderr, "You may need permission.\n\n");
		return 1;
	}

	fprintf(stdout, "Device found.\n");
	fprintf(stdout, "Transfer is starting.\n");

	if (send(pfan_fd, img_nbr, images, effects) < sizeof(struct pfan_data)) {
		fprintf(stderr, "Transfer was aborted.\n\n");
		ret = 1;
	} else
		fprintf(stdout, "Transfer is complete.\n\n");

	close(pfan_fd);
	pfan_free_rasters(images, img_nbr);
	return ret;
}
