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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "defutils.h"
#include "devinfo.h"
#include "optutils.h"
#include "raster.h"

int main(int argc, char **argv)
{
	pfan_opts opts;
	int ret = 0;

	if ((ret = pfan_getopt(argc, argv, &opts)) != PFAN_VALID_OPT)
		return ret;

	char   img_paths[PFAN_MAX_DISPLAY][4096];
	struct pfan_mldata data;

	memset(&data, 0x00, sizeof(struct pfan_mldata));

	if (opts.cflag &&
		(data.display_nbr = pfan_read_cfg(opts.carg, img_paths,
										  data.effects)) < 0) {
		fprintf(stderr, "Invalid config file.\n\n");
		return 1;
	}

	if (opts.dflag &&
		(data.display_nbr = pfan_read_dir(opts.darg, img_paths,
										  data.effects)) < 0) {
		fprintf(stderr, "Can not open '%s' directory.\n\n", opts.darg);
		return 1;
	}

	if (!opts.mflag) {
		if (!data.display_nbr) {
			fprintf(stdout, "No .PBM image found.\n\n");
			return 0;
		}
		data.rasters = pfan_create_img_rasters(img_paths, data.display_nbr);
		if (!data.rasters)
			return 1;
	}

	if (opts.mflag) {
		if (strlen(opts.marg) > 26) {
			fprintf(stderr, "Over the character limit of 26.\n\n");
			return 1;
		}
		data.rasters = malloc(sizeof (void *));
		data.rasters[0] = malloc(strlen(opts.marg) + 1);
		memcpy(data.rasters[0], opts.marg, strlen(opts.marg) + 1);
		data.types[0] = 1;
		data.display_nbr = 1;
	}

	if (opts.fflag)
		memset(data.effects, 0x06, sizeof(data.effects));

	int pfan_fd = open(PFAN_DEVNAME, O_WRONLY);

	if (pfan_fd <= 0) {
		pfan_free_type_rasters(data.rasters, data.display_nbr, data.types);
		fprintf(stderr, "Device can not be opened or found.\n");
		fprintf(stderr, "You may need permission.\n\n");
		return 1;
	}

	fprintf(stdout, "Device found.\n");
	fprintf(stdout, "Transfer is starting.\n");

	int expected_transfer = 320 * data.display_nbr;
	int bytes = write(pfan_fd, &data, sizeof (struct pfan_mldata));

	ret = 0;
	if (bytes < expected_transfer) {
		ret = 1;
		fprintf(stderr, "Transfer aborted.\n");
		if (bytes >= 0)
			fprintf(stderr, "( %d / %d )\n\n", bytes, expected_transfer);
		else
			fprintf(stderr, "Transfer error (usberr = %d)\n\n", bytes);
	} else {
		fprintf(stdout, "Transfer is complete.\n");
		fprintf(stdout, "( %d / %d )\n\n", bytes, expected_transfer);
	}

	close(pfan_fd);
	pfan_free_type_rasters(data.rasters, data.display_nbr, data.types);
	return ret;
}
