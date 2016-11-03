#include <dirent.h>
#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "getopt.h"

#define OPEN           0
#define CLOSE          1
#define BEFORE_CLOSE   2

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
	return effect[OPEN] <= 6 && effect[CLOSE] <= 6
				&& (effect[BEFORE_CLOSE] == 0
					|| effect[BEFORE_CLOSE] == 4
					|| effect[BEFORE_CLOSE] == 6);
}

int pfan_read_cfg(char *path,
                  char img_paths[PFAN_IMG_MAX][4096],
                  uint8_t effects[PFAN_IMG_MAX][3],
                  int fastmode)
{
	FILE *file = fopen(path, "r");

	if (!file) {
		fprintf(stderr, "Can not open to '%s'.\n", path);
		return -1;
	}

	char *dirpath = dirname(path);
	char imgname[256];
	int n = 0;
	int matchs;

	while (n < PFAN_IMG_MAX
			&& (matchs = fscanf(file, "%s%*[ \t]+%hhu-%hhu-%hhu\n", imgname,
				&effects[n][OPEN],
				&effects[n][CLOSE],
				&effects[n][BEFORE_CLOSE])) == 4) {

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

	if (matchs != EOF && n < PFAN_IMG_MAX)
		return -1;
	return n;
}

int pfan_read_dir(char *path,
                  char img_paths[PFAN_IMG_MAX][4096],
                  uint8_t effects[PFAN_IMG_MAX][3],
                  int fastmode)
{
	memset(effects, (uint8_t)(fastmode) ? 6 : 0, 24);

	struct dirent *dp;
	DIR *dir = opendir(path);
	int n = 0;

	if (!dir)
		return -1;

	while ((dp = readdir(dir)) != NULL) {
		char *ext = strrchr(dp->d_name, '.');

		if (!ext)
			continue;

		if (strcmp(ext, ".pbm") == 0) {
			concat_path(img_paths[n], path, dp->d_name);
			fprintf(stdout, "%s\n", img_paths[n]);
			++n;
		}
	}
	fprintf(stdout, "\n");
	closedir(dir);
	return n;
}
