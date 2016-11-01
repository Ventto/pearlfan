#ifndef RASTER_H_
#define RASTER_H_

#include <stdint.h>

uint8_t **pfan_create_rasters(char image_paths[8][8193], int image_nbr);

void pfan_free_rasters(uint8_t **rasters, int n);

#endif /* !RASTER_H_ */
