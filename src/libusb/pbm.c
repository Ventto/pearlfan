#include <errno.h>
#include <inttypes.h>	/* uintX_t types */
#include <pbm.h>
#include <stdio.h>	/* FILE */
#include <stdlib.h>
#include <string.h>	/* strtok() */

#include "ventilo_data.h"


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

void pbm_to_usbdata(unsigned char id,
		    unsigned char *raster,
		    uint16_t *display)
{
	unsigned char i;
	unsigned char j;
	unsigned char col_end;

	for (i = 0; i < 156; ++i) {
		col_end = 155 - i;
		for (j = 0; j < 11; ++j)
			if (raster[j * 156 + i] == 1)
				display[col_end] &= pbm_mask[10 - j];
	}
}

void print_raster(bit *raster)
{
	int i;
	int j = 0;

	printf("=------[ PBM Raster ]------=\n");
	for (i = 0; i < 1716; ++i) {
		printf("%d", raster[i]);
		if (j == 69) {
			j = 0;
			printf("\n");
		} else
			j++;
	}
}

unsigned char *pbm_get_specific_raster(FILE *img)
{
	bit *raster = NULL;
	int cols = 0;
	int rows = 0;
	int format = 0;

	/* Getting the configuration from PBM image */
	pbm_readpbminit(img, &cols, &rows, &format);

	/* Invalid image's configuration for the ventilator */
	if (cols != IMAGE_WIDTH && rows != IMAGE_HEIGHT
	    && format != PBM_FORMAT) {
		pm_close(img);
		printf("Error: pbm_check() [cols=%d;rows=%d;format=%d]\n",
		       cols, rows, format);
		printf("Invalid Image Format Error\n");
		return NULL;
	}

	/* Allocation of the raster */
	raster = pbm_allocrow(1716);

	/* Cannot allocate */
	if (!raster) {
		printf("Error: pbm_allocrow()\n");
		return NULL;
	}

	/* Getting the raster from the PBM image */
	pbm_readpbmrow(img, raster, 1716, format);

	return (unsigned char *)raster;
}

int main(int argc, char **argv)
{
	FILE *img = NULL;
	bit *raster = NULL;

	/* Initialization before importing PBM image*/
	pm_init(argv[0], 0);

	/* Missing argument */
	if (argc < 2) {
		printf("Error: a parameter is missing.\n");
		return EXIT_FAILURE;
	}

	/* Open a given PBM image as parameter */
	img = pm_openr(argv[1]);

	/* Cannot open file */
	if (!img) {
		printf("Error: pm_openr()\n");
		return EXIT_FAILURE;
	}

	/* Getting the configuration from PBM image */
	raster = pbm_get_specific_raster(img);

	/* Send data to USB ventilo (libusb | driver test) */

	pbm_freerow(raster);
	pm_close(img);
	return 0;
}
