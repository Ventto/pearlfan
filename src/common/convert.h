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
#ifndef CONVERT_H_
#define CONVERT_H_

#define PFAN_IMG_W    156
#define PFAN_IMG_H    11

unsigned long long pfan_convert_effect(const char id,
                                       const char unsigned effect[3]);

void pfan_convert_raster(char id,
                         unsigned char *raster,
                         unsigned short display[PFAN_IMG_W]);

#endif /* !CONVERT_H_ */
