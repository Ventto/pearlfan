/*
 * The GPL License (GPLv3)
 *
 * Copyright © 2016 Thomas "Ventto" Venriès  <thomas.venries@gmail.com>
 *
 * Pearlfan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pearlfan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pearlfan.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <libusb-1.0/libusb.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "draw.h"
#include "defutils.h"
#include "tests.h"
#include "usb.h"

int main(int argc, char **argv)
{
	uint16_t displays[PFAN_MAX_DISPLAY][PFAN_MAX_W];
	uint8_t effects[PFAN_MAX_DISPLAY][3];
	int img_nbr = 0;
	int ret = 0;

	memset(displays, 0xFF, sizeof(displays));
	memset(effects, 0x00, sizeof(effects));

	Test_columns(displays[0]);
	Test_rows(displays[1]);
	Test_square(displays[2]);
	Test_26chars(displays[3]);
	Test_oneChar(displays[4]);
	Test_textOneChar(displays[5]);
	Test_textLimit(displays[6]);
	Test_tinyText(displays[7]);
	img_nbr = 8;

	libusb_device_handle *usb_handle = NULL;

	if (libusb_init(NULL) < 0) {
		fprintf(stderr, "Libusb initialization failed.\n\n");
		return 1;
	}

	usb_handle = pfan_open(PFAN_VID, PFAN_PID);
	if (!usb_handle) {
		libusb_exit(NULL);
		return 1;
	}

	fprintf(stdout, "Device found.\n");
	fprintf(stdout, "Transfer is starting.\n");

	int expected_transfer = 320 * img_nbr;
	int bytes = pfan_send(usb_handle, img_nbr, effects, displays);

	ret = 0;

	if (bytes < expected_transfer) {
		fprintf(stderr, "Transfer aborted.\n");
		if (bytes < 0)
			fprintf(stderr, "Transfer error (libusberr = %d)\n\n", bytes);
		fprintf(stderr, "( %d / %d bytes)\n\n", bytes, expected_transfer);
		ret = 1;
	}

	fprintf(stdout, "Transfer is complete.\n\n");
	fprintf(stdout, "( %d / %d bytes)\n\n", bytes, expected_transfer);
	pfan_close(usb_handle);
	return ret;
}
