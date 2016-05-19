#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEVICE		("/dev/cfan0")

int main(int argc, char **argv)
{
	int fd;

	fd = open(DEVICE, O_WRONLY);

	if (fd <= 0)
		return -EBADF;

	unsigned char *buffer = NULL;

	/* Buffer:
	 * - Number of displays : 1 bytes
	 * - Configuration : 2 bytes,
	 * - Display : 26 bytes max (letters)
	 */
	buffer = malloc(6);

	if (!buffer)
		return -ENOMEM;

	/* Configuration:
	 * 00 00 00 55 [C][D] [A][B] 10 A0
	 * [A] : OpenOption	[0 ; 5]
	 * [B] : CloseOption	[0 ; 5]
	 * [C] : TurnOption	(6 | c)
	 * [D] : Display ID	[0 ; 7]
	 * Warning: Reverse bytes sequence (stack behind)
	 */
	buffer[0] = 0x01; /* Number of displays */
	buffer[1] = 0x44; /* Open & Close options */
	buffer[2] = 0x00; /* Turn option & Display ID */

	buffer[3] = 'A';
	buffer[4] = 'P';
	buffer[5] = 'C';
	buffer[6] = 'D';
	buffer[7] = 'D';
	buffer[8] = '\0';

	if (!write(fd, buffer, 9)) {
		close(fd);
		free(buffer);
		return -ENOSPC;
	}

	free(buffer);
	close(fd);

	return EXIT_SUCCESS;
}
