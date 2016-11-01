#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

#define FILEPATH_MAX        8193
#define PFAN_DISPLAY_MAX    8

int pfan_read_config(char *filename,
		char images[PFAN_DISPLAY_MAX][FILEPATH_MAX],
		uint8_t effects[PFAN_DISPLAY_MAX][3]);

#endif /* !CONFIG_H_ */
