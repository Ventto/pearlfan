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
#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

#include "defutils.h"

int pfan_read_cfg(char *path,
                  char img_paths[PFAN_MAX_DISPLAY][MAX_PATH],
                  uint8_t effects[PFAN_MAX_DISPLAY][3]);

int pfan_read_dir(char *path,
                  char img_paths[PFAN_MAX_DISPLAY][MAX_PATH],
                  uint8_t effects[PFAN_MAX_DISPLAY][3]);

#endif /* !CONFIG_H_ */
