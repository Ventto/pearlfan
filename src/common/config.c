#include <stdio.h>
#include <unistd.h>

#include "config.h"

#define MAX_DISPLAYS	8
#define OPENING_EFFECT	0
#define CLOSING_EFFECT	1
#define DISPLAY_EFFECT	2

int read_config_file(char *filename, char imgs[8][PATH_MAX], char cfgs[8][3])
{
	FILE *file = fopen(filename, "r");

	if (!file) {
		printf("%s: permission denied.\n", filename);
		return -1;
	}

	int matchs;
	int n = 0;

	do {
		matchs = fscanf(file, "+%[^+]+%hhu/%hhu/%hhu\n", imgs[n],
				&cfgs[n][OPENING_EFFECT],
				&cfgs[n][CLOSING_EFFECT],
				&cfgs[n][DISPLAY_EFFECT]);

		if(access(imgs[n], F_OK ) == -1 ) {
			printf("config: '%s' does not exist.\n", imgs[n]);
			return -1;
		}

		n++;
	} while (matchs == 4 && n < MAX_DISPLAYS);

	fclose(file);

	if (matchs != 4 && matchs != EOF) {
		printf("%s: invalid config file.\n", filename);
		return -1;
	}

	return n;
}
