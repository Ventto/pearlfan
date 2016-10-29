#ifndef CONFIG_H_
# define CONFIG_H_

#include <linux/limits.h>

#define FILEPATH_MAX		(PATH_MAX * 2 + 1)
#define MAX_FAN_DISPLAYS	8
#define OPENING_EFFECT		0
#define CLOSING_EFFECT		1
#define DISPLAY_EFFECT		2

int pfan_read_config(char *filename, char imgs[MAX_FAN_DISPLAYS][FILEPATH_MAX],
		char effects[MAX_FAN_DISPLAYS][3]);

#endif /* !CONFIG_H_ */
