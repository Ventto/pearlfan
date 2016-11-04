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
#ifndef DATA_H_
#define DATA_H_

#define PFAN_CLASSNAME    "usb/pfan0"
#define PFAN_DEVNAME      "/dev/pfan0"
#define PFAN_VID           0x0c45
#define PFAN_PID           0x7701

/**
 * struct pfan_data - designed to the module driver's write
 * @n: number of displays (at most 8)
 * @effects: transition effects for displays:
 *   - open         [0 ; 5]
 *   - close        [0 ; 5]
 *   - before_close (0, 4, 6)
 * @images: PBM image rasters (156 x 11)
 */
struct pfan_data {
	unsigned char n;
	unsigned char effects[8][3];
	unsigned char **images;
};

#endif /* DATA_H_ */
