#include <pbm.h>
#include <linux/limits.h>
#include <string.h>		/* memset() */
#include <stdio.h>		/* printf() */
#include <stdlib.h>		/* malloc() */

#include "config.h"
#include "libusb_utils.h"
#include "pbm_utils.h"
#include "ventilo_data.h"

/* Set effect config for a fan view */
static uint64_t set_config(unsigned char id,
			   unsigned char open_option,
			   unsigned char close_option,
			   unsigned char turn_option)
{
	/* Configuration:
	 * A0 10 <A><B> <C><D> 55 00 00 00
	 * [A] : OpenOption	[0 ; 5]
	 * [B] : CloseOption	[0 ; 5]
	 * [C] : TurnOption	(6 | c)
	 * [D] : Display ID	[0 ; 7]
	 * Warning: Reverse bytes sequence (stack behind ?) :
	 * 00 00 00 55 [CD] [AB] 10 A0
	 * [CDAB] : u16
	 */
	uint16_t cdab = 0;

	printf("Config: ID: %d\n", id);
	printf("Config: open option: %d\n", open_option);
	printf("Config: close option: %d\n", close_option);
	printf("Config: turn option: %d\n", turn_option);

	cdab |= close_option;
	cdab |= (open_option << 4);
	cdab |= (id << 8);
	cdab |= (turn_option << 12);

	return 0x00000055000010A0 | (cdab << 16);
}

int main(int argc, char **argv)
{
	/* LIBUSB */
	libusb_device_handle *dev_handle;
	libusb_context *context = NULL;

	/* Initialize the library for the session we just declared */
	int r = libusb_init(&context);

	if (r < 0) {
		printf("Init Error %d\n", r);
		return 1;
	}

	/* Performing the given usb session and trying to get the device */
	dev_handle = get_usb_device(context, VENDOR_ID, PRODUCT_ID);
	/* Error */
	if (!dev_handle)
		return 1;

	/* =--------------------------------= */
	/*     Open the configuration file    */
	/* =--------------------------------= */
	/* Missing argument */
	if (argc < 2) {
		printf("Error: a parameter is missing.\n");
		return EXIT_FAILURE;
	}

	char imgs[8][PATH_MAX];
	char cfgs[8][3];
	int n;

	if (read_config_file("config", &n, imgs, cfgs) < 0)
		return EXIT_FAILURE;

	/* =--------------------------------= */
	/*   Extract rasters from PBM images  */
	/* =--------------------------------= */
	FILE *img = NULL;
	bit **rasters = malloc(sizeof(bit) * n);

	/* Initialization before importing PBM image*/
	pm_init(argv[0], 0);

	unsigned char i;
	/* Getting the configuration from PBM image */
	for (i = 0; i < n; i++) {
		img = pm_openr(imgs[i]);
		/* Cannot open file */
		if (!img) {
			printf("Cannot open PBM image\n");
			return EXIT_FAILURE;
		}
		rasters[i] = pbm_get_specific_raster(img);
		/* Close image file */
		pm_close(img);
	}

	/* =--------------------------------= */
	/*     Convertion PBM to USB data      */
	/* =--------------------------------= */
	uint16_t fan_displays[FAN_DISPLAY_MAX_NUMBER][156];
	uint64_t fan_configs[FAN_DISPLAY_MAX_NUMBER];

	memset(fan_displays, 0xFF, FAN_DISPLAY_MAX_NUMBER *
	       FAN_DISPLAY_8BYTES_PACKET_NUMBER * 4);

	for (i = 0; i < n ; i++) {
		fan_configs[i] = set_config(i, cfgs[i][0], cfgs[i][1],
					    cfgs[i][2]);
		pbm_to_usbdata(i, rasters[i], fan_displays[i]);
	}

	/* =--------------------------------= */
	/*    Send all data to the debvice    */
	/* =--------------------------------= */
	unsigned char j;

	for (i = 0; i < n; i++) {
		send_usb_data(dev_handle, (unsigned char *)&fan_configs[i]);
		for (j = 0; j < FAN_DISPLAY_8BYTES_PACKET_NUMBER; ++j)
			send_usb_data(dev_handle,
				      (unsigned char *)
				      (&fan_displays[i][j * 4]));
	}

	/* Free USB resources */
	release_usb(context, dev_handle);
	return r;
}
