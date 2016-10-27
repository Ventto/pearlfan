#include <string.h>
#include <stdio.h>

#include "libusb_utils.h"

/* Perform the given USB session, get the device and returns the last one */
libusb_device_handle *open_device_with_vid_pid(libusb_context *ctx,
		int vid, int pid)
{
	libusb_device_handle *dev_handle =
		libusb_open_device_with_vid_pid(ctx, vid, pid);

	if (!dev_handle) {
		printf("libusb: device cannot be opened or found.\n");
		libusb_exit(ctx);
		return NULL;
	}

	if (libusb_kernel_driver_active(dev_handle, 0) == 1) {
		printf("libusb: kernel driver attached on the device.\n");
		if (libusb_detach_kernel_driver(dev_handle, 0) == 0)
			printf("libusb: kernel driver dettached from the device.\n");
	}

	/* Claim interface 0 (the first) of device */
	if (libusb_claim_interface(dev_handle, 0) < 0) {
		release_usb(ctx, dev_handle);
		printf("libusb: cannot claim interface.\n");
		return NULL;
	}

	printf("libusb: claimed interface with success.\n");
	return dev_handle;
}

/* Release the claimed interface and exit & close the USB device */
void release_usb(libusb_context *ctx, libusb_device_handle *dev_handle)
{
	//int err = libusb_release_interface(dev_handle, 0);
	//if (err != 0)
	//	printf("libusb: cannot release the interface with code (%d).\n", err);
	libusb_close(dev_handle);
	libusb_exit(ctx);
	printf("libusb: release the interface with success.\n");
}

/*
 * Sends a '8 bytes' control transfer's packet followed by
 * an 'interrupt tranfer'
 * return: result of one of the libusb's transfer functions
 */
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
