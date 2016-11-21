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
#ifndef RASTER_H_
#define RASTER_H_

#include <stdint.h>

uint8_t **pfan_create_img_rasters(char image_paths[8][4096], int n);

void pfan_free_rasters(uint8_t **rasters, int n);

void pfan_free_type_rasters(uint8_t **rasters, int n, uint8_t types[8]);

#endif /* !RASTER_H_ */
