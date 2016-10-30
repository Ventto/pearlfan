#ifndef RASTER_H_
#define RASTER_H_

#include <stdint.h>

bit *pfan_create_raster(FILE *img);

void pfan_free_rasters(bit **rasters, int n);

#endif /* !RASTER_H_ */
