#ifndef USB_H_
#define USB_H_

#include <libusb-1.0/libusb.h>

libusb_device_handle *pfan_open(libusb_context *ctx, int vid, int pid);

int pfan_send(libusb_device_handle *dev_handle, void *data);

#endif /* !USB_H_ */
