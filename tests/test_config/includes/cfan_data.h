#ifndef __CFAN_DATA_H
#define __CFAN_DATA_H

/**
 * struct cfan_data - designed to be the buffer as parameter for the module
 * driver's write function.
 * @n: number of displays, eight maximum, [0 ; 7]
 * @cfg: the configuration for eight different displays, written on two bytes
 *	[A][B] [C][D], divided in four blocks of four bits
 *	- [A]: turn effect option [0 ; 5] <-- range of values
 *	- [B]: the display number [0 ; 7]
 *	- [C]: open effect option [0 ; 5]
 *	- [D]: close effect option (6 | c)
 * @displays_data: the eight displays, each one contains 156 x columns of leds,
 *	whose each column, its eleven first bits represent the fan's leds
 *	(1: off, 0: on)
 *
 */
struct cfan_data {
	uint8_t n;
	uint8_t cfg[8][3];
	uint16_t displays[8][156];
};

#endif /* __CFAN_DATA_H */
