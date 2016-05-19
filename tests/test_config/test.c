#include <errno.h>
#include <fcntl.h>	/* write() */
#include <inttypes.h>	/* uintX_t types */
#include <pbm.h>
#include <libpbm.h>
#include <stdio.h>	/* FILE */
#include <stdlib.h>
#include <string.h>	/* strtok() */
#include <unistd.h>	/* read() */

#include "includes/cfan_data.h"

#define MAX_LINE	264

static int is_validstring(char *str)
{
	char i = 0;
	char *ptr = str;

	while (*ptr != '+' && i++ <= 26) {
		if ((*ptr < 65 && *ptr != 32) || *ptr > 90)
			return 0;
		ptr++;
	}

	return (*ptr == '+') ? i : 0;
}

static int read_config(struct cfan_data *cfan, char *str, unsigned char n)
{
	unsigned char i = 0;
	/*
	 * Parses the display's configuration string (ex: '1/2/3')
	 * Stocks 1,2,3 in cfg.
	 */
	while (i < 3) {
		if (*str >= 48 && *str <= 53) {
			cfan->cfg[n][i++] = *(str++) - 48;
			if (*(str++) != '/')
				return 1;
		} else
			return 1;
	}
	return 0;
}

static int read_backup_file(FILE *file, struct cfan_data *cfan)
{
	char line[MAX_LINE];
	char words[8][27];
	unsigned char n = 0;
	unsigned char size = 0;
	char *filename = NULL;

	while (fgets(line, sizeof(line), file)) {
		if (line[0] == '+') {
			if (line[1] == '+') {
				printf("PBM image - [%d] - ", n);
				filename = strtok(line + 2, "+");
				if (filename)
					printf("[%s]\n", filename);
				else
					printf("[FAILED: OPEN]\n");
			} else if (line[1] != '\n') {
				/* String validity */
				size = is_validstring(line + 1);
				if (!size) {
					printf("bad word !\n");
					return EXIT_FAILURE;
				}
				/* String copy */
				strncpy(words[n], line + 1, size);
				words[n][size] = '\0';
				printf("word: %s - [%d]\n",
				       words[n], n);
				/* display configuration */
				read_config(cfan, line + size + 2, n);
				printf("\t=>configuration\n");
				printf("\t=> %d, %d, %d\n", cfan->cfg[n][0],
				       cfan->cfg[n][1],
				       cfan->cfg[n][2]);
				n++;
			} else
				printf("error: bad string\n");
		}
	}
	return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	struct cfan_data *cfan = NULL;
	FILE *file = NULL;

	cfan = malloc(sizeof(*cfan));

	if (!cfan)
		return -ENOMEM;

	if (argc < 2)
		printf("no arg\n");
	else if (argc == 2) {
		file = fopen(argv[1], "r+");

		if (read_backup_file(file, cfan))
			return EXIT_FAILURE;

		if (!file)
			return -ENOENT;
	} else
		printf("error: bad file\n");

	free(cfan);

	return EXIT_SUCCESS;
}
