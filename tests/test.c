#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#define DEVICE		("/dev/cfan0")

int main(int argc, char **argv)
{
	int fd;

	fd = open(DEVICE, O_WRONLY);

	if (fd <= 0)
		return 2;

	if (!write(fd, "hello", 5)) {
		close(fd);
		return 3;
	}

	close(fd);

	return EXIT_SUCCESS;
}
