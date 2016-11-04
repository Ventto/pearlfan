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
	int ret = 0;

	if ((ret = pfan_getopt(argc, argv, &opts)) != PFAN_VALID_OPT)
		return ret;

	char    img_paths[PFAN_IMG_MAX][4096];
	uint8_t effects[PFAN_IMG_MAX][3];
	int     img_nbr;

	if (opts.cflag &&
		(img_nbr = pfan_read_cfg(opts.carg, img_paths, effects,
				opts.fflag)) < 0) {
		fprintf(stderr, "Invalid config file.\n\n");
		return 1;
	}

	if (opts.dflag &&
		(img_nbr = pfan_read_dir(opts.darg, img_paths, effects,
				opts.fflag)) < 0) {
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

	int pfan_fd = open(PFAN_DEVNAME, O_WRONLY);

	if (pfan_fd <= 0) {
		pfan_free_rasters(images, img_nbr);
		fprintf(stderr, "Device can not be opened or found.\n");
		fprintf(stderr, "You may need permission.\n\n");
		return 1;
	}

	fprintf(stdout, "Device found.\n");
	fprintf(stdout, "Transfer is starting.\n");

	int expected_transfer = 320 * img_nbr;
	int bytes = send(pfan_fd, img_nbr, images, effects);

	ret = 0;
	if (bytes < expected_transfer) {
		ret = 1;
		fprintf(stderr, "Transfer aborted.\n");
		if (bytes >= 0)
			fprintf(stderr, "( %d / %d )\n\n", bytes, expected_transfer);
		else
			fprintf(stderr, "Transfer error (usberr = %d)\n\n", bytes);
	} else {
		fprintf(stdout, "Transfer is complete.\n\n");
	}

	close(pfan_fd);
	pfan_free_rasters(images, img_nbr);
	return ret;
}
