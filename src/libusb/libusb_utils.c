#include <string.h>
#include <stdio.h>

#include "libusb_utils.h"

libusb_device_handle *open_device_with_vid_pid(libusb_context *ctx,
		int vid, int pid)
{
	libusb_device_handle *dev_handle =
		libusb_open_device_with_vid_pid(ctx, vid, pid);

	if (!dev_handle) {
		printf("libusb: device cannot be opened or found.\n");
		return NULL;
	}

	if (libusb_kernel_driver_active(dev_handle, 0) == 1) {
		if (libusb_detach_kernel_driver(dev_handle, 0) != 0) {
			libusb_close(dev_handle);
			printf("libusb: can not dettach the device from the driver.\n");
			return NULL;
		}
	}

	if (libusb_claim_interface(dev_handle, 0) < 0) {
		libusb_close(dev_handle);
		printf("libusb: cannot claim interface.\n");
		return NULL;
	}

	return dev_handle;
}

void release_usb(libusb_context *ctx, libusb_device_handle *dev_handle)
{
	libusb_close(dev_handle);
	libusb_exit(ctx);
	printf("libusb: release the interface with success.\n");
}

int send_usb_data(libusb_device_handle *dev_handle, void *data)
{
	uint8_t buf[8];
	int l = 0;

	memset((void *)buf, 0, 8);
	buf[7] = 0x2;

	if (libusb_control_transfer(dev_handle, 0x21, 9, 0x200, 0, data, 8,
				1000) <= 0)
		return -1;
	return libusb_interrupt_transfer(dev_handle, 0x81, buf, 8, &l, 1000);
}
