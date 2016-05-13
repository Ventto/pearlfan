#ifndef __CFAN_DATA_H
#define __CFAN_DATA_H

/**
 * struct cfan_data - designed to be the buffer as parameter for the module
 * driver's write function.
 * @cfg: the configuration for eight different displays, written on two bytes
 *	[A][B] [C][D], divided in four blocks of four bits
 *	- [A]: turn effect option [0 ; 5] <-- range of values
 *	- [B]: the display number [0 ; 7]
 *	- [C]: open effect option [0 ; 5]
 *	- [D]: close effect option (6 | c)
 * @data: the eight displays, each one contains 156 x columns of leds,
 *	whose each column, its eleven first bits represent the fan's leds
 *	(1: off, 0: on)
 *
 */
struct cfan_data {
	uint16_t cfg[8];
	uint16_t data[8][156];
};

#endif /* __CFAN_DATA_H */
