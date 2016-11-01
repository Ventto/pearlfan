#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"

#define OPEN           0
#define CLOSE          1
#define BEFORE_CLOSE   2

static void concat_path(char buf[FILEPATH_MAX],
		const char *dir,
		const char *relpath)
{
	buf[0] = '\0';
	strncat(buf, dir, strlen(dir));
	strncat(buf, "/", 1);
	strncat(buf, relpath, strlen(relpath));
}

static inline int check_open(uint8_t value)
{
	return value >= 0 && value <= 6;
}

static inline int check_close(uint8_t value)
{
	return check_open(value);
}

static inline int check_beforeclose(uint8_t value)
{
	return value == 0 || value == 6;
}

int pfan_read_config(char *filename,
		char image_paths[PFAN_DISPLAY_MAX][FILEPATH_MAX],
		uint8_t effects[PFAN_DISPLAY_MAX][3])
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

	while (n < PFAN_DISPLAY_MAX && (matchs =
			fscanf(file, "%[^+]+%hhu/%hhu/%hhu\n", relpath,
					&effects[n][OPEN],
					&effects[n][CLOSE],
					&effects[n][BEFORE_CLOSE])) == 4) {

		concat_path(image_paths[n], dir, relpath);

		if(access(image_paths[n], F_OK ) == -1 ) {
			printf("pfan: image '%s' does not exist.\n", image_paths[n]);
			break;
		}

		if (!check_open(effects[n][OPEN])
				|| !check_close(effects[n][CLOSE])
				|| !check_beforeclose(effects[n][BEFORE_CLOSE])) {
			printf("pfan: bad effect values for '%s'.\n", image_paths[n]);
			break;
		}

		n++;
	}

	fclose(file);

	if (matchs != EOF && n < PFAN_DISPLAY_MAX)
		return -1;
	return n;
}
