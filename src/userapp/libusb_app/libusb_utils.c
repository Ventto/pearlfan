#include <string.h>
#include <stdio.h>

#include "libusb_utils.h"

/* Release the claimed interface and exit & close the USB device */
void release_usb(libusb_context *context,
			libusb_device_handle *dev_handle)
{
	int r = libusb_release_interface(dev_handle, 0);

	if (r != 0)
		printf("Cannot Release Interface\n");

	libusb_close(dev_handle);

	libusb_exit(context);

	printf("Released Interface\n");
}

/*
 * Sends a '8 bytes' control transfer's packet followed by
 * an 'interrupt tranfer'
 * return: result of one of the libusb's transfer functions
 */
int send_usb_data(libusb_device_handle *dev_handle,
			 unsigned char *data)
{
	unsigned char buf[8];
	int l = 0;
	int ret;

	memset((void *)buf, 0, 8);
	buf[7] = 0x2;

	ret = libusb_control_transfer(dev_handle,
				      0x21,
				      9,
				      0x200,
				      0,
				      data,
				      8,
				      1000);

	if (ret <= 0)
		return -1;

	ret = libusb_interrupt_transfer(dev_handle,
					0x81,
					buf,
					8,
					&l,
					1000);

	return ret;
}

/*
 * Perform the given USB session, get the targeted device and returns the last
 * one.
 */
libusb_device_handle *get_usb_device(libusb_context *context,
					    int vid,
					    int pid)
{
	int r;

	libusb_device_handle *dev_handle;

	/* Get the target device */
	dev_handle = libusb_open_device_with_vid_pid(context, vid, pid);
	if (dev_handle == NULL) {
		printf("=> Cannot open or find the USB device\n");
		printf("=> You may try again with 'sudo'.\n");
		libusb_exit(context);
		return NULL;
	}

	printf("Device Opened\n");

	/* Find out if kernel driver is attached */
	if (libusb_kernel_driver_active(dev_handle, 0) == 1) {
		printf("Kernel Driver Active\n");
		/* detach it */
		if (libusb_detach_kernel_driver(dev_handle, 0) == 0)
			printf("Kernel Driver Detached!\n");
	}

	/* Claim interface 0 (the first) of device */
	r = libusb_claim_interface(dev_handle, 0);
	if (r < 0) {
		release_usb(context, dev_handle);
		printf("Cannot Claim Interface\n");
		return NULL;
	}

	printf("Claimed Interface\n");

	return dev_handle;
}
