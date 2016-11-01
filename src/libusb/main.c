#include <libusb-1.0/libusb.h>
#include <pbm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "convert.h"
#include "devinfo.h"
#include "raster.h"
#include "usb.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: pearlfan <config_file>\n");
		return EXIT_FAILURE;
	}

	char *config_file = argv[1];

	if(access(config_file, F_OK ) == -1 ) {
		printf("pfan: '%s' does not exist.\n", config_file);
		return EXIT_FAILURE;
	}

	char image_paths[PFAN_DISPLAY_MAX][FILEPATH_MAX];
	uint8_t effects[PFAN_DISPLAY_MAX][3];
	int image_nbr;

	if ((image_nbr = pfan_read_config(config_file, image_paths, effects)) < 0) {
		printf("pfan: invalid config file.\n\n");
		return EXIT_FAILURE;
	}

	if (!image_nbr)
		return EXIT_SUCCESS;

	bit **rasters = pfan_create_rasters(image_paths, image_nbr);

	if (!rasters)
		return EXIT_FAILURE;

	uint16_t displays[PFAN_DISPLAY_MAX][PFAN_IMG_W];

	memset(displays, 0xFF, sizeof(displays));

	for (int i = 0; i < image_nbr ; i++)
		pfan_convert_raster(i, rasters[i], displays[i]);

	libusb_device_handle *usb_handle = NULL;
	libusb_context *usb_ctx = NULL;

	if (libusb_init(&usb_ctx) < 0) {
		pfan_free_rasters(rasters, image_nbr);
		printf("pfan: libusb initialization failed.\n\n");
		return EXIT_FAILURE;
	}

	usb_handle = pfan_open(usb_ctx, PFAN_VID, PFAN_PID);
	if (!usb_handle) {
		pfan_free_rasters(rasters, image_nbr);
		libusb_exit(usb_ctx);
		return EXIT_FAILURE;
	}

	printf("pfan: device found.\n");
	printf("pfan: transfer is starting.\n");
	if (pfan_send(usb_handle, image_nbr, effects, displays) != 0) {
		printf("pfan: transfer is not complete.\n\n");
		return EXIT_FAILURE;
	}
	printf("pfan: transfer is complete.\n\n");
	pfan_close(usb_ctx, usb_handle);
	pfan_free_rasters(rasters, image_nbr);

	return EXIT_SUCCESS;
}
