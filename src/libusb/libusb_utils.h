#ifndef _LIBUSB_UTILS_H
# define _LIBUSB_UTILS_H

# include <libusb-1.0/libusb.h>

libusb_device_handle *open_device_with_vid_pid(libusb_context *ctx,
		int vid, int pid);

void release_usb(libusb_context *ctx, libusb_device_handle *dev_handle);

int send_usb_data(libusb_device_handle *dev_handle, void *data);

#endif /* ! _LIBUSB_UTILS_H */
