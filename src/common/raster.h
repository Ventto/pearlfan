#ifndef PBM_H_
#define PBM_H_

#include <stdint.h>

void pfan_convert_raster(char id, bit *raster, uint16_t display[156]);

bit *pfan_create_raster(FILE *img);

void pfan_free_rasters(bit **rasters, int n);

#endif /* !PBM_H_ */
