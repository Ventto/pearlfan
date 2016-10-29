#include <libusb-1.0/libusb.h>
#include <linux/limits.h>
#include <pbm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "usb.h"
#include "raster.h"
#include "ventilo.h"

#define FAN_MAX_IMG_NBR			8
#define FAN_MAX_DATA			(FAN_MAX_IMG_NBR * 2 * 156)

static uint64_t effect_to_usbdata(const char id, const char effect[3])
{
	uint16_t opts = 0;

	opts |= effect[CLOSING_EFFECT];
	opts |= (effect[OPENING_EFFECT] << 4);
	opts |= (id << 8);
	opts |= (effect[DISPLAY_EFFECT] << 12);

	return 0x00000055000010A0 | (opts << 16);
}

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

	char imgs[MAX_FAN_DISPLAYS][FILEPATH_MAX];
	char effects[MAX_FAN_DISPLAYS][3];
	int img_nbr;

	if ((img_nbr = pfan_read_config(config_file, imgs, effects)) < 0) {
		printf("pfan: '%s' invalid config file.\n", config_file);
		return EXIT_FAILURE;
	}

	if (!img_nbr)
		return EXIT_SUCCESS;

	FILE *img = NULL;
	bit **rasters = malloc(sizeof(void *) * img_nbr);

	pm_init(argv[0], 0);

	for (int i = 0; i < img_nbr; i++) {
		img = pm_openr(imgs[i]);
		if (!img) {
			printf("pfan: can not open '%s'\n", imgs[i]);
			return EXIT_FAILURE;
		}
		rasters[i] = pfan_create_raster(img);
		pm_close(img);
	}

	uint16_t fan_displays[FAN_MAX_IMG_NBR][156];
	memset(fan_displays, 0xFF, FAN_MAX_DATA);

	for (int i = 0; i < img_nbr ; i++)
		pfan_convert_raster(i, rasters[i], fan_displays[i]);

	libusb_device_handle *usb_handle = NULL;
	libusb_context *usb_ctx = NULL;

	if (libusb_init(&usb_ctx) < 0) {
		pfan_free_rasters(rasters, img_nbr);
		printf("pfan: libusb initialization failed.\n");
		return EXIT_FAILURE;
	}

	usb_handle = pfan_open(usb_ctx, VENDOR_ID, PRODUCT_ID);
	if (!usb_handle) {
		pfan_free_rasters(rasters, img_nbr);
		libusb_exit(usb_ctx);
		return EXIT_FAILURE;
	}

	for (uint8_t i = 0; i < img_nbr; i++) {
		uint64_t effect = effect_to_usbdata(i, effects[i]);

		pfan_send(usb_handle, &effect);
		for (uint8_t j = 0; j < 39; ++j)
			pfan_send(usb_handle, &fan_displays[i][j * 4]);
	}

	libusb_close(usb_handle);
	libusb_exit(usb_ctx);
	pfan_free_rasters(rasters, img_nbr);

	return EXIT_SUCCESS;
}
