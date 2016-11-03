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
