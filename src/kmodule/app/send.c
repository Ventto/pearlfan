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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "devinfo.h"
#include "send.h"

int pfan_send(int fd, int image_nbr, uint8_t **images, uint8_t effects[8][3])
{
	struct pfan_data *data = malloc(sizeof(struct pfan_data));

	if (!data)
		return 1;

	memset(data, 0, sizeof(struct pfan_data));
	data->n = image_nbr;
	memcpy(data->effects, effects, 24);
	data->images = images;

	int ret = write(fd, data, sizeof(struct pfan_data));

	free(data);
	return ret;
}
