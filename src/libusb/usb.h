#ifndef USB_H_
#define USB_H_

#include <libusb-1.0/libusb.h>
#include <stdint.h>

libusb_device_handle *pfan_open(libusb_context *ctx, int vid, int pid);

void pfan_close(libusb_context *ctx, libusb_device_handle *dev_handle);

int pfan_send(libusb_device_handle *dev_handle, int img_n,
              uint8_t effects[8][3],
              uint16_t displays[8][156]);

#endif /* !USB_H_ */
