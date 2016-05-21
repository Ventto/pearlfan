#ifndef CONFIG_H_
# define CONFIG_H_

#include <linux/limits.h>

int read_config_file(char *filename,
		     int *n,
		     char imgs[8][PATH_MAX],
		     char cfgs[8][3]);

#endif /* !CONFIG_H_ */
