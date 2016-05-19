#ifndef __CFAN_API_H
#define __CFAN_API_H

# include <inttypes.h>

/*-------------------------------------------------------------------------*/

/*
 * Cfan API (over the module driver) is designed to make the fan management
 * easier for developpers.
 */

/*---------------------*/

/*
 * Sets the display's configuration according to different effect options
 * @id: the display number [0;7]
 * @open_option: effect before displaying
 * @close_option: effect after displaying
 * @turn_option: effect during the display
 */
uint16_t set_config(const unsigned char id,
		const unsigned char open_option,
		const unsigned char close_option,
		const unsigned char turn_option);

/*
 * Switch two bytes each other according to a given u16.
 * For the fan context, it converts a binary leds column into data in order,
 * intented to be sent into an URB.
 * That function helps you to avoid headaches to turn on leds
 * and to be understandable by the fan
 * @leds_column: a given binary leds column. On the fan, there are eleven leds.
 *	Keep in mind the fan turns. Therefore, conceptuely there are eleven leds
 *	per column for a 360° display. So to turn on some of them,
 *	be only focus on the eleven first bits, so that (1: off, 0: on):
 *	'1111 1011 1110 1101', leds 2, 5, 11 will be turned on.
 */
uint16_t u16_rook(const uint16_t x);

void reverse_u16array_order(uint16_t *arr, const unsigned int width);

#endif /* __CFAN_API_H */
