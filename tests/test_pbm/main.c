#include <errno.h>
#include <inttypes.h>	/* uintX_t types */
#include <pbm.h>
#include <libpbm.h>
#include <stdio.h>	/* FILE */
#include <stdlib.h>
#include <string.h>	/* strtok() */



int main(int argc, char **argv)
{
	FILE *pbm = NULL;
	bit *raster = NULL;
	int cols = 0;
	int rows = 0;
	int format = 0;

	/* Initialization */
	pm_init(argv[0], 0);

	if (argc < 2) {
		printf("Error: no arg\n");
		return EXIT_FAILURE;
	}

	/* Open PBM image */
	pbm = pm_openr(argv[1]);

	if (!pbm) {
		printf("Error: pm_openr()\n");
		return EXIT_FAILURE;
	}

	pbm_readpbminit(pbm, &cols, &rows, &format);

	if (cols != 156 && rows != 11 && format != PBM_FORMAT) {
		pm_close(pbm);
		printf("Error: pbm_check() [cols=%d;rows=%d;format=%d]\n",
		       cols, rows, format);
		return EXIT_FAILURE;
	}

	raster = pbm_allocrow(1716);

	if (!raster) {
		printf("Error: pbm_allocrow()\n");
		return EXIT_FAILURE;
	}

	pbm_readpbmrow(pbm, raster, 1716, format);

	unsigned int i;
	unsigned int j = 0;

	for (i = 0; i < 1716; ++i) {
		printf("%d", raster[i]);
		if (j == 69) {
			j = 0;
			printf("\n");
		} else
			j++;
	}

	pbm_freerow(raster);
	pm_close(pbm);

	return EXIT_SUCCESS;
}
