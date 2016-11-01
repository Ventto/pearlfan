#ifndef SEND_H_
#define SEND_H_

#include <stdint.h>

int send(int fd, int image_nbr, uint8_t **images, uint8_t effects[8][3]);

#endif /* !SEND_H_ */
