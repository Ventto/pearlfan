#ifndef CONVERT_H_
#define CONVERT_H_

unsigned long long pfan_convert_effect(const char id, const char effect[3]);

void pfan_convert_raster(char id, unsigned char *raster,
		unsigned short display[156]);

#endif /* !CONVERT_H_ */
