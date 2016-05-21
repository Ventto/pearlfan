#ifndef __VENTILO_DATA_H
#define __VENTILO_DATA_H

#define VENDOR_ID	3141
#define PRODUCT_ID	30465
#define DEVICE		"/dev/ventilo0"

/**
 * struct ventilo_data - designed to be the buffer as parameter for the module
 * driver's write function.
 * @n: number of displays, eight maximum, [0 ; 7]
 * @cfg: the configuration for eight different displays, contains three options
 *	for display effects.
 *	- 1: open effect option  [0 ; 5] <-- range of values
 *	- 2: close effect option [0 ; 5]
 *	- 3: turn effect option  [6 | c]
 * @displays: the eight bitmap displays, each one contains 1716 bytes or
 *	156 * 11 which represents 156 columns of 11 leds each.
 *	A 'display' means a whole ventilo's display.
 */
struct ventilo_data {
	uint8_t n;
	uint8_t cfgs[8][3];
	unsigned char **images;
};

#endif /* __VENTILO_DATA_H */
