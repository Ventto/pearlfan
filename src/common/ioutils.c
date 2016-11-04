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
#include <stdio.h>

#include "ioutils.h"

int pfan_isdir(char *path)
{
	DIR *dir = opendir(path);

	if (dir) {
		closedir(dir);
		return 1;
	}
	return 0;
}

int pfan_readaccess_file(char *path)
{
	FILE *file = fopen(path, "r");

	if (!file) {
		fprintf(stderr, "Can not open to '%s'.\n", path);
		return 0;
	}

	if (pfan_isdir(path)) {
		fclose(file);
		fprintf(stderr, "%s: is not a file.\n", path);
		return 0;
	}
	return 1;
}
