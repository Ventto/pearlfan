#include <libusb-1.0/libusb.h>
#include <string.h>		/* memset() */
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

/* Send a '8 bytes' packet followed by an INTERRUPT_IN msg */
static int send_data(libusb_device_handle *dev_handle, unsigned char *data)
{
	unsigned char buf[8];
	int l = 0;
	int ret;
	uint8_t bRequest = 9;
	uint16_t wValue = 0x200;
	uint16_t wIndex = 0;
	uint16_t wLength = 8;

	buf[7] = 0x2;

	ret = libusb_control_transfer(dev_handle,
				      0x21,
				      bRequest,
				      wValue,
				      wIndex,
				      data,
				      wLength,
				      1000);

	if (ret <= 0)
		return ret;

	memset((void *)buf, 0, 8);

	ret = libusb_interrupt_transfer(dev_handle,
					0x81,
					buf,
					8,
					&l,
					1000);

	return ret;
}

int main(int argc, char **argv)
{
	libusb_device_handle *dev_handle;
	libusb_context *context = NULL;
	int r;

	/* Initialize the library for the session we just declared */
	r = libusb_init(&context);
	/* Error */
	if (r < 0) {
		printf("Init Error %d\n", r);
		return 1;
	}

	/* Get the target device */
	dev_handle = libusb_open_device_with_vid_pid(context, 3141, 30465);
	/* Error */
	if (dev_handle == NULL) {
		printf("Cannot open device\n");
		/* needs to be called to end the */
		libusb_exit(context);
		return 1;
	}

	printf("Device Opened\n");

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
		return r;
	}
	/* just to see the data we want to write : abcd */
	printf("Claimed Interface\n");

	/* Send data */
	uint64_t cfg = 0x00000055602210A0;
	unsigned char data[8];

	data[0] = 0x00;
	data[1] = 0x00;
	data[2] = 0xFF;
	data[3] = 0x00;
	data[4] = 0xFF;
	data[5] = 0xFF;
	data[6] = 0x00;
	data[7] = 0xFF;

	r = 0;

	r = send_data(dev_handle, (unsigned char *)&cfg);

	if (r <= 0)
		printf("send_data(): configure\n");

	r = send_data(dev_handle, (unsigned char *)data);

	if (r <= 0)
		printf("send_data(): interrupt\n");

	freedom(context, dev_handle);

	return r;
}
