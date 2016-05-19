#include <inttypes.h>

uint16_t set_config(const unsigned char id,
		const unsigned char open_option,
		const unsigned char close_option,
		const unsigned char turn_option)
{
	return 0x0000 | close_option
		| (open_option << 4)
		| (id << 8)
		| (turn_option << 12);
}

uint16_t u16_rook(const uint16_t x)
{
	return 0x0000 | (x << 8) | (x >> 8);
}

void reverse_u16array_order(uint16_t *arr, const unsigned int width)
{
	uint16_t swap;
	unsigned int i;
	unsigned int count = (width - 1) / 2;
	unsigned int acc = width - 1;

	for (i = 0; i <= count; ++i) {
		swap = *(arr + i);
		*(arr + i) = *(arr + acc - i);
		*(arr + acc - i) = swap;
	}
}
