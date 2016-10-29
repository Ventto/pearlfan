#include <libusb-1.0/libusb.h>
#include <linux/limits.h>
#include <pbm.h>
#include <stdio.h>		/* printf() */
#include <stdlib.h>		/* malloc() */
#include <string.h>		/* memset() */
#include <unistd.h>		/* access() */

#include "config.h"
#include "libusb_utils.h"
#include "pbm_utils.h"
#include "ventilo.h"

#define FAN_MAX_IMG_NBR			8
#define FAN_MAX_DATA			(FAN_MAX_IMG_NBR * 2 * 156)

/* Set effect config for a fan view */
static uint64_t set_config(uint8_t id,
		uint8_t open_opt,
		uint8_t close_opt,
		uint8_t turn_opt)
{
	uint16_t opts = 0;

	printf("Config: ID: %d\n", id);
	printf("Config: open option:\t%d\n", open_opt);
	printf("Config: close option:\t%d\n", close_opt);
	printf("Config: turn option:\t%d\n", turn_opt);

	opts |= close_opt;
	opts |= (open_opt << 4);
	opts |= (id << 8);
	opts |= (turn_opt << 12);

	return 0x00000055000010A0 | (opts << 16);
}

int main(int argc, char **argv)
{

	if (argc != 2) {
		printf("Usage: dreamyfan <config_file>\n");
		return EXIT_FAILURE;
	}

	char *config_file = argv[1];

	if(access(config_file, F_OK ) == -1 ) {
		printf("%s: does not exist.\n", config_file);
		return EXIT_FAILURE;
	}

	char imgs[8][PATH_MAX];
	char cfgs[8][3];
	int img_nbr;

	if ((img_nbr = read_config_file(config_file, imgs, cfgs)) < 0) {
		return EXIT_FAILURE;
	}

	FILE *img = NULL;
	bit **rasters = malloc(sizeof(void*) * img_nbr);

	pm_init(argv[1], 0);

	for (int i = 0; i < img_nbr; i++) {
		img = pm_openr(imgs[i]);
		if (!img) {
			printf("Cannot open PBM image\n");
			return EXIT_FAILURE;
		}
		rasters[i] = pbm_get_specific_raster(img);
		pm_close(img);
	}

	uint16_t fan_displays[FAN_MAX_IMG_NBR][156];

	memset(fan_displays, 0xFF, FAN_MAX_DATA);

	for (int i = 0; i < img_nbr ; i++)
		pbm_to_usbdata(i, rasters[i], fan_displays[i]);

	libusb_device_handle *usb_handle = NULL;
	libusb_context *usb_ctx = NULL;

	if (libusb_init(&usb_ctx) < 0) {
		printf("libusb: initialization failed.\n");
		return EXIT_FAILURE;
	}

	usb_handle = open_device_with_vid_pid(usb_ctx, VENDOR_ID, PRODUCT_ID);
	if (!usb_handle) {
		libusb_exit(usb_ctx);
		return EXIT_FAILURE;
	}

	for (int i = 0; i < img_nbr; i++) {
		uint64_t fconfig = set_config(i, cfgs[i][0], cfgs[i][1], cfgs[i][2]);
		send_usb_data(usb_handle, &fconfig);
		for (uint8_t j = 0; j < 39; ++j) {
			void *raw_data = &fan_displays[i][j * 4];
			send_usb_data(usb_handle, raw_data);
		}
	}

	libusb_close(usb_handle);
	libusb_exit(usb_ctx);
	free_pbm_rasters(rasters, img_nbr);
	return EXIT_SUCCESS;
}
