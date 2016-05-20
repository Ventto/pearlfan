#include <libusb-1.0/libusb.h>
#include <stdio.h>		/* printf() */
#include <stdlib.h>		/* malloc() */

static void freedom(libusb_context *context, libusb_device_handle *dev_handle)
{
	int r;

	/* release the claimed interface */
	r = libusb_release_interface(dev_handle, 0);

	if (r != 0) {
		printf("Cannot Release Interface\n");
		return;
	}

	/* close the device we opened */
	libusb_close(dev_handle);

	/* needs to be called to end the */
	libusb_exit(context);

	/* delete the allocated memory for data */
	/* free(data); */

	printf("Released Interface\n");
}

int main(int argc, char **argv)
{
	libusb_device **devs;
	libusb_device_handle *dev_handle;
	libusb_context *context = NULL;
	int dev_nb;
	int r;

	/* Initialize the library for the session we just declared */
	r = libusb_init(&context);
	/* Error */
	if (r < 0) {
		printf("Init Error %d\n", r);
		return 1;
	}

	/* Set verbosity level to 3, as suggested in the documentation */
	libusb_set_debug(context, 3);

	/* Get the list of devices */
	dev_nb = libusb_get_device_list(context, &devs);
	/* Error */
	if (dev_nb < 0) {
		printf("Get Device Error\n");
		return 1;
	}

	printf("%d detected devices\n", dev_nb);

	/* Get the target device */
	dev_handle = libusb_open_device_with_vid_pid(context, 3141, 30465);
	/* Error */
	if (dev_handle == NULL) {
		printf("Cannot open device\n");
		/* needs to be called to end the */
		libusb_exit(context);
		return EXIT_FAILURE;
	}

	printf("Device Opened\n");

	/* free the list, unref the devices in it */
	libusb_free_device_list(devs, 1);

	/* find out if kernel driver is attached */
	if (libusb_kernel_driver_active(dev_handle, 0) == 1) {
		printf("Kernel Driver Active\n");
		/* detach it */
		if (libusb_detach_kernel_driver(dev_handle, 0) == 0)
			printf("Kernel Driver Detached!\n");
	}

	/* claim interface 0 (the first) of device (mine had jsut 1) */
	r = libusb_claim_interface(dev_handle, 0);
	/* Error */
	if (r < 0) {
		printf("Cannot Claim Interface\n");
		return 1;
	}
	/* just to see the data we want to write : abcd */
	printf("Claimed Interface\n");

	/* Send data */

	freedom(context, dev_handle);

	return 0;
}
