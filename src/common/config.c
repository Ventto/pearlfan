#include <stdio.h>
#include "config.h"

int read_config_file(char *filename,
			    unsigned char *n,
			    char imgs[8][PATH_MAX],
			    char cfgs[8][3])
{
	FILE *cfgfile = fopen(filename, "r+");

	if (!cfgfile) {
		printf("Cannot Open Config File: %s\n", filename);
		return -1;
	}

	*n = 0;

	int res;

	do {
		res = fscanf(cfgfile, "+%[^+]+%hhu/%hhu/%hhu\n", imgs[*n],
			     &cfgs[*n][0], &cfgs[*n][1], &cfgs[*n][2]);
		++*n;
	} while (res == 4 && *n < 8);

	fclose(cfgfile);

	if (res != 4 && res != EOF) {
		printf("Invalid Config File Error\n");
		return -1;
	}

	--*n;

	return 0;
}
