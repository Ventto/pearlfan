#ifndef CONFIG_H_
# define CONFIG_H_

#include <linux/limits.h>

#define FILEPATH_MAX		(PATH_MAX * 2 + 1)
#define MAX_FAN_DISPLAYS	8
#define OPENING_EFFECT		0
#define CLOSING_EFFECT		1
#define DISPLAY_EFFECT		2

int read_config_file(char *filename,
		char imgs[8][FILEPATH_MAX],
		char cfgs[8][3]);

#endif /* !CONFIG_H_ */
