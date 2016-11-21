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
#ifndef USB_H_
#define USB_H_

#include <libusb-1.0/libusb.h>
#include <stdint.h>

libusb_device_handle *pfan_open(int vid, int pid);

void pfan_close(libusb_device_handle *dev_handle);

int pfan_send(libusb_device_handle *dev_handle, int img_n,
              uint8_t effects[8][3],
              uint16_t displays[8][156]);

#endif /* !USB_H_ */
