#ifndef DATA_H_
#define DATA_H_

/**
 * struct ventilo_data - designed to be the buffer as parameter for the module
 * driver's write function.
 * @n: number of displays, eight maximum, [0 ; 7]
 * @effects: the configuration effects for the eight fan displays,
 * contains three options for display effects.
 *	- 1: open effect option  [0 ; 5] <-- range of values
 *	- 2: close effect option [0 ; 5]
 *	- 3: turn effect option  [6 | c]
 * @displays: the eight bitmap displays, each one contains 1716 bytes or
 *	156 * 11 which represents 156 columns of 11 leds each.
 *	A 'display' means a whole ventilo's display.
 */
struct pfan_data {
	uint8_t n;
	uint8_t effects[8][3];
	unsigned char **images;
};

#endif /* DATA_H_ */
