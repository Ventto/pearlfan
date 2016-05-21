#include <errno.h>
#include <inttypes.h>	/* uintX_t types */
#include <pbm.h>
#include <stdio.h>	/* FILE */
#include <stdlib.h>
#include <string.h>	/* strtok() */

#include "ventilo_data.h"

#define DEVICE		"/dev/ventilo0"

#define IMAGE_WIDTH	156
#define IMAGE_HEIGHT	11

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

static void free_data(FILE *pbm,
		      bit *raster,
		      struct ventilo_data *ventilo) {
	pbm_freerow(raster);
	pm_close(pbm);
	if (!ventilo)
		return;
	if (ventilo->bitmaps)
		free(ventilo->bitmaps);
	free(ventilo);
}

int main(int argc, char **argv)
{
	FILE *pbm = NULL;
	bit *raster = NULL;
	int cols = 0;
	int rows = 0;
	int format = 0;
	int fd;
	int ret;
	unsigned int i;
	unsigned int j = 0;

	/* Initialization before importing PBM image*/
	pm_init(argv[0], 0);

	/* Missing argument */
	if (argc < 2) {
		printf("Error: a parameter is missing.\n");
		return EXIT_FAILURE;
	}

	/* Open a given PBM image as parameter */
	pbm = pm_openr(argv[1]);

	/* Cannot open file */
	if (!pbm) {
		printf("Error: pm_openr()\n");
		return EXIT_FAILURE;
	}

	/* Getting the configuration from PBM image */
	pbm_readpbminit(pbm, &cols, &rows, &format);

	/* Invalid image's configuration for the ventilator */
	if (cols != IMAGE_WIDTH && rows != IMAGE_HEIGHT
	    && format != PBM_FORMAT) {
		pm_close(pbm);
		printf("Error: pbm_check() [cols=%d;rows=%d;format=%d]\n",
		       cols, rows, format);
		return EXIT_FAILURE;
	}

	/* Allocation of the raster */
	raster = pbm_allocrow(1716);

	/* Cannot allocate */
	if (!raster) {
		printf("Error: pbm_allocrow()\n");
		return EXIT_FAILURE;
	}

	/* Getting the raster from the PBM image */
	pbm_readpbmrow(pbm, raster, 1716, format);

	if (DEBUG)
		print_raster(raster);

	struct ventilo_data *ventilo = NULL;

	ventilo = malloc(sizeof(*ventilo));

	if (!ventilo)
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
		printf("\nVentilator Data:\n");

	/* Print the data for the ventilator */
	if (DEBUG)
		for (i = 0; i < 156; ++i)
			printf("%x\n", display[0][i]);

	/* Set the ventilator configuration */
	ventilo->n = 1;
	ventilo->cfg[0][0] = 2;
	ventilo->cfg[0][1] = 3;
	ventilo->cfg[0][2] = 6;
	ventilo->bitmaps = malloc(sizeof(unsigned char *));

	/* Stock PBM data in ventilo_data structure */
	ventilo->bitmaps[0] = raster;

	/* Target the ventialtor device */
	fd = open(DEVICE, O_WRONLY);

	/* Error: Cannot open file */
	if (fd <= 0) {
		free_data(pbm, raster, ventilo);
		printf("AppPBM: Error: Cannot open the ventilator device.\n");
		return -EBADF;
	}

	/* Send the data to the driver */
	ret = write(fd, ventilo, sizeof(*ventilo));

	if (ret <= 0) {
		printf("AppPBM: Error: write error.\n");
		ret = EXIT_FAILURE;
	}
	ret = EXIT_SUCCESS;

	close(fd);
	free_data(pbm, raster, ventilo);
	return ret;
}
