#include <stdio.h>
#include "config.h"

int read_config_file(char *filename, int *n, char imgs[8][PATH_MAX],
		char cfgs[8][3])
{
	FILE *file = fopen(filename, "r+");

	if (!file) {
		printf("%s: cannot be opened.\n", filename);
		return -1;
	}

	*n = 0;

	int res;

	do {
		res = fscanf(file, "+%[^+]+%hhu/%hhu/%hhu\n", imgs[*n],
				&cfgs[*n][0], &cfgs[*n][1], &cfgs[*n][2]);
		printf("%s\n", imgs[*n]);
		(*n)++;
	} while (res == 4 && *n < 8);

	fclose(file);

	if (res != 4 && res != EOF) {
		printf("%s: invalid config file.\n", filename);
		return -1;
	}

	return 0;
}
