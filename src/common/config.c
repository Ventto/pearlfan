#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"

static void concat_path(char buf[FILEPATH_MAX],
		const char *dir,
		const char *relpath)
{
	buf[0] = '\0';
	strncat(buf, dir, strlen(dir));
	strncat(buf, "/", 1);
	strncat(buf, relpath, strlen(relpath));
}

int pfan_read_config(char *filename,
		char imgs[MAX_FAN_DISPLAYS][FILEPATH_MAX],
		char effects[MAX_FAN_DISPLAYS][3])
{
	FILE *file = fopen(filename, "r");

	if (!file) {
		printf("%s: permission denied.\n", filename);
		return -1;
	}

	char *dir = dirname(filename);
	char relpath[FILEPATH_MAX];
	int n = 0;
	int matchs;

	while (n < MAX_FAN_DISPLAYS && (matchs =
			fscanf(file, "%[^+]+%hhu/%hhu/%hhu\n", relpath,
					&effects[n][OPENING_EFFECT],
					&effects[n][CLOSING_EFFECT],
					&effects[n][DISPLAY_EFFECT])) == 4) {

		concat_path(imgs[n], dir, relpath);

		if(access(imgs[n], F_OK ) == -1 ) {
			printf("config: '%s' does not exist.\n", imgs[n]);
			break;
		}

		printf("config: '%s' ; [%d, %d, %d]\n", imgs[n],
				effects[n][OPENING_EFFECT],
				effects[n][CLOSING_EFFECT],
				effects[n][DISPLAY_EFFECT]);

		n++;
	}

	fclose(file);

	if (matchs != EOF && n < MAX_FAN_DISPLAYS)
		return -1;
	return n;
}
