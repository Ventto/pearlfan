#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

#define PFAN_IMG_MAX   8

int pfan_read_cfg(char *path,
                  char img_paths[PFAN_IMG_MAX][4096],
                  uint8_t effects[PFAN_IMG_MAX][3],
                  int fastmode);

int pfan_read_dir(char *path,
                  char img_paths[PFAN_IMG_MAX][4096],
                  uint8_t effects[PFAN_IMG_MAX][3],
                  int fastmode);

#endif /* !CONFIG_H_ */
