#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "devinfo.h"
#include "send.h"

int send(int fd, int image_nbr, uint8_t **images, uint8_t effects[8][3])
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
