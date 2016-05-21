#ifndef _LIBUSB_UTILS_H
# define _LIBUSB_UTILS_H

#include <libusb-1.0/libusb.h>

/* Release the claimed interface and exit & close the USB device */
void release_usb(libusb_context *context,
			libusb_device_handle *dev_handle);

/*
 * Sends a '8 bytes' control transfer's packet followed by
 * an 'interrupt tranfer'
 * return: result of one of the libusb's transfer functions
 */
int send_usb_data(libusb_device_handle *dev_handle,
			 unsigned char *data);

/*
 * Perform the given USB session, get the targeted device and returns the last
 * one.
 */
libusb_device_handle *get_usb_device(libusb_context *context,
					    int vid,
					    int pid);

#endif /* _LIBUSB_UTILS_H */
