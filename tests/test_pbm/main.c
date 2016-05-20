#include <errno.h>
#include <inttypes.h>	/* uintX_t types */
#include <pbm.h>
#include <libpbm.h>
#include <stdio.h>	/* FILE */
#include <stdlib.h>
#include <string.h>	/* strtok() */

#include "cfan_data.h"

#define DEBUG	0

static uint16_t pbm_mask[11];

static void pbm_masks_init(void)
{
	pbm_mask[10] = 0xFFF7;
	pbm_mask[9] = 0xFFFB;
	pbm_mask[8] = 0xFFFD;
	pbm_mask[7] = 0xFFFE;
	pbm_mask[6] = 0xBFFF;
	pbm_mask[5] = 0xDFFF;
	pbm_mask[4] = 0xEFFF;
	pbm_mask[3] = 0xF7FF;
	pbm_mask[2] = 0xFBFF;
	pbm_mask[1] = 0xFDFF;
	pbm_mask[0] = 0xFEFF;
}

static void print_raster(bit *raster)
{
	int i;
	int j = 0;

	printf("RASTER:\n");
	for (i = 0; i < 1716; ++i) {
		printf("%d", raster[i]);
		if (j == 69) {
			j = 0;
			printf("\n");
		} else
			j++;
	}
}

static void pbm_to_display(unsigned char id,
			   unsigned char *raster,
			   uint16_t *display)
{
	unsigned char i;
	unsigned char j;
	unsigned char col_end;

	if (DEBUG)
		print_raster(raster);

	for (i = 0; i < 156; ++i) {
		col_end = 155 - i;
		for (j = 0; j < 11; ++j)
			if (raster[j * 156 + i] == 1)
				display[col_end] &= pbm_mask[10 - j];
	}
}

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

	if (DEBUG)
		print_raster(raster);

	struct cfan_data *cfan = NULL;

	cfan = malloc(sizeof(*cfan));

	if (!cfan)
		return -ENOMEM;

	/* Test conversion PBM data to fan display */
	uint16_t display[8][156];

	if (DEBUG) {
		for (i = 0; i < 8; ++i)
			for (j = 0; j < 156; ++j)
				display[i][j] = 0xFFFF;
	}

	if (DEBUG) {
		pbm_masks_init();
		pbm_to_display(0, raster, display[0]);
	}

	if (DEBUG)
		printf("\nDISPLAY 0:\n");

	if (DEBUG)
		for (i = 0; i < 156; ++i)
			printf("%x\n", display[0][i]);


	/* Stock PBM data in cfan_data structure */
	cfan->n = 1;
	cfan->cfg[0][0] = 2;
	cfan->cfg[0][1] = 3;
	cfan->cfg[0][2] = 6;
	cfan->bitmaps = malloc(sizeof(unsigned char *));
	cfan->bitmaps[0] = raster;

	/* Send the data to the driver */
	int fd = open("/dev/cfan0", O_WRONLY);
	int ret = EXIT_SUCCESS;

	if (fd <= 0)
		ret = -EBADF;

	ret = write(fd, cfan, sizeof(*cfan));
	printf("Info: write(): code %d\n", ret);
	printf("Info: cfan->bitmaps addr: %p\n", cfan->bitmaps);
	ret = (ret <= 0) ? EXIT_FAILURE : EXIT_SUCCESS;

	close(fd);
	pbm_freerow(raster);
	pm_close(pbm);
	free(cfan->bitmaps);
	free(cfan);

	return ret;
}
