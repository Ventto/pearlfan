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
#include <dirent.h>
#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"

static void concat_path(char buf[4096], const char *dir, const char *imgname)
{
	buf[0] = '\0';
	strncat(buf, dir, strlen(dir));
	if (dir[strlen(dir) - 1] != '/')
		strncat(buf, "/", 1);
	strncat(buf, imgname, strlen(imgname));
}

static int isvalid_effect(uint8_t effect[3])
{
	return effect[PFAN_OPEN] <= 6 && effect[PFAN_CLOSE] <= 5
				&& (effect[PFAN_BEFORECLOSE] == 0
					|| effect[PFAN_BEFORECLOSE] == 2
					|| effect[PFAN_BEFORECLOSE] == 6);
}

int pfan_read_cfg(char *path,
                  char img_paths[PFAN_MAX_DISPLAY][MAX_PATH],
                  uint8_t effects[PFAN_MAX_DISPLAY][3])
{
	FILE *file = fopen(path, "r");

	if (!file) {
		fprintf(stderr, "Can not open '%s'.\n", path);
		return -1;
	}

	char *dirpath = dirname(path);
	char imgname[256];
	int n = 0;
	int matchs;

	while (n < PFAN_MAX_DISPLAY
			&& (matchs = fscanf(file, "%s%*[ \t]+%hhu-%hhu-%hhu\n", imgname,
				&effects[n][PFAN_OPEN],
				&effects[n][PFAN_CLOSE],
				&effects[n][PFAN_BEFORECLOSE])) == 4) {

		char *ext = strrchr(imgname, '.');

		if (!ext || strcmp(ext, ".pbm") != 0) {
			fprintf(stderr, "%s: invalid extension, expected '.pbm'.\n",
					imgname);
			break;
		}

		concat_path(img_paths[n], dirpath, imgname);

		if (access(img_paths[n], F_OK) == -1) {
			fprintf(stderr, "Image '%s' does not exist.\n", img_paths[n]);
			break;
		}

		if (!isvalid_effect(effects[n])) {
			fprintf(stderr, "Invalid effect value for '%s'.\n", img_paths[n]);
			break;
		}

		n++;
	}

	fclose(file);

	if (matchs != EOF && n < PFAN_MAX_DISPLAY)
		return -1;
	return n;
}

int pfan_read_dir(char *path,
                  char img_paths[PFAN_MAX_DISPLAY][MAX_PATH],
                  uint8_t effects[PFAN_MAX_DISPLAY][3])
{
	struct dirent *dp;
	DIR *dir = opendir(path);
	int n = 0;

	if (!dir)
		return -1;

	while (n < PFAN_MAX_DISPLAY && (dp = readdir(dir)) != NULL) {
		char *ext = strrchr(dp->d_name, '.');

		if (!ext)
			continue;

		if (strcmp(ext, ".pbm") == 0) {
			concat_path(img_paths[n], path, dp->d_name);
			++n;
		}
	}

	fprintf(stdout, "\n");
	closedir(dir);
	return n;
}
