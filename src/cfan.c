/*
 * Linux driver for Led Fan by Dream Cheeky (version 0)
 */
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/usb.h>

#include "includes/cfan_data.h"

MODULE_AUTHOR("Thomas Venries, Franklin Mathieu");
MODULE_DESCRIPTION("CheekyFan module, v0.1");
MODULE_LICENSE("GPL");

#define VENDOR_ID	0x0c45
#define PRODUCT_ID	0x7701

struct usb_cfan {
	struct usb_device	*udev;
	u64     cfg[8];			/* displays configuration */
	u16	displays[8][156];	/* displays's buffer */
	u8	displays_nb;		/* number of displays */
};

static struct usb_driver cfan_driver;
static struct usb_cfan *cfan;

/* Add the device to this driver's supported device list */
static struct usb_device_id id_table[] = {
	{ USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table);

/* Set effect config for a fan view */
static u64 set_config(const u16 *cfg)
{
	/* Configuration:
	 * A0 10 <A><B> <C><N> 55 00 00 00
	 * [A] : OpenOption	[0 ; 5]
	 * [B] : CloseOption	[0 ; 5]
	 * [C] : TurnOption	(6 | c)
	 * [D] : Display ID	[0 ; 7]
	 * Warning: Reverse bytes sequence (stack behind) :
	 * 00 00 00 55 [CN] [AB] 10 A0
	 * [CNAB] : u16
	 */
	return 0x00000055000010A0 | (*cfg << 16);
}

static u8 write_letter(const unsigned char letter,
		       const unsigned char id,
		       const unsigned char column)
{
	unsigned char col = column;

	if (col)
		cfan->displays[id][col++] = 0xFFFF;

	pr_info("cfan:%s: detected letter: %x\n", __func__, letter);

	switch (letter) {
	case 'A':
		cfan->displays[id][col++] = 0x00FF;
		cfan->displays[id][col++] = 0x6EFF;
		cfan->displays[id][col++] = 0x6EFF;
		cfan->displays[id][col++] = 0x00FF;
		cfan->displays[id][col++] = 0xFEFF;
		break;
	case 'B':
		cfan->displays[id][col++] = 0x82FF;
		cfan->displays[id][col++] = 0x6CFF;
		cfan->displays[id][col++] = 0x6CFF;
		cfan->displays[id][col++] = 0x00FF;
		break;
	case 'C':
		cfan->displays[id][col++] = 0x7CFF;
		cfan->displays[id][col++] = 0x7CFF;
		cfan->displays[id][col++] = 0x7CFF;
		cfan->displays[id][col++] = 0x00FF;
		break;
	case 'D':
		cfan->displays[id][col++] = 0xC6FF;
		cfan->displays[id][col++] = 0xBAFF;
		cfan->displays[id][col++] = 0x7CFF;
		cfan->displays[id][col++] = 0x00FF;
		break;
	case 'P':
		cfan->displays[id][col++] = 0x9EFF;
		cfan->displays[id][col++] = 0x6EFF;
		cfan->displays[id][col++] = 0x6EFF;
		cfan->displays[id][col++] = 0x00FF;
		break;
	}

	return col;
}

/* Send a '8 bytes' packet followed by an INTERRUPT_IN msg */
static int send_data(struct usb_device *udev, void *data)
{
	char buf[8];
	int l = 0;
	int ret;

	memset(buf, 0, 8);
	buf[7] = 0x02;

	ret = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
			      0x09, 0x21, 0x200, 0x00, data, 0x008,
			      10 * HZ);

	if (ret <= 0)
		return ret;

	usb_interrupt_msg(udev, usb_rcvintpipe(udev, 1),
			  buf, 8, &l,
			  10 * HZ);

	return 0;
}

static int cfan_open(struct inode *i, struct file *f)
{
	return 0;
}

static int cfan_release(struct inode *i, struct file *f)
{
	return 0;
}

static ssize_t cfan_write(struct file *f,
			  const char __user *buffer,
			  size_t cnt,
			  loff_t *off)
{
	u64 config;
	unsigned char i;
	unsigned char j = 0;
	unsigned char count = 0;
	unsigned char column_index = 0;

	if (!cfan)
		return -ENODEV;

	if (cnt == 0)
		return 0;

	if (!buffer || cnt > 233)
		return -EINVAL;

	pr_info("cfan:%s: =---------[ WRITE ] :\n", __func__);
	pr_info("cfan:%s: displays_nb: %d\n", __func__, buffer[0]);

	/* Get the number of displays */
	cfan->displays_nb = buffer[0];
	j++;

	/* Set the configuration */
	config = set_config((u16 *)(buffer + j));
	j += 2;

	pr_info("cfan:%s: 'j' before string: %d\n", __func__, j);

	/* Strlen with the acc 'j' */
	while (buffer[j++] != '\0' && count < 26)
		count++;
	j--;
	j--;

	pr_info("cfan:%s: 'j' after string: %d\n", __func__, j);
	pr_info("cfan:%s: nb of characters: %d\n", __func__, count);
	pr_info("cfan:%s: config: %x\n", __func__, (unsigned int)config);

	i = j - count;
	/* Write letter in the buffer */
	for (; j > i; j--) {
		pr_info("cfan:%s: 'j' reverse: %d\n", __func__, j);
		column_index = write_letter(buffer[j], 0, column_index);
	}

	pr_info("cfan:%s: column_index: %d\n", __func__, column_index);

	/* Send config */
	send_data(cfan->udev, &config);
	/* Send display */
	for (i = 0; i < column_index; i += 4)
		send_data(cfan->udev, (u16 *)cfan->displays + i);

	return 1;
}

const struct file_operations cfan_fops = {
	.open    = cfan_open,
	.release = cfan_release,
	.write = cfan_write,
};

static struct usb_class_driver cfan_class_driver = {
	.name = "usb/cfan0",
	.fops = &cfan_fops,
	.minor_base = 0
};

static int cfan_probe(struct usb_interface *interface,
		      const struct usb_device_id *id)
{
	struct usb_device *udev = interface_to_usbdev(interface);
	int ret;

	cfan = kmalloc(sizeof(*cfan), GFP_KERNEL);

	if (!cfan)
		return -ENOMEM;

	memset(cfan, 0x00, sizeof(*cfan));

	/* Increment the reference count of the usb device structure */
	cfan->udev = usb_get_dev(udev);

	/* Increment the ref counter */
	usb_set_intfdata(interface, cfan);

	pr_info("cfan:%s: USB Cheeky Fan has been connected\n", __func__);
	pr_info("cfan:%s: [devnum=%d;bus_id=%d;devid=%d]\n",
		__func__,
		cfan->udev->devnum,
		cfan->udev->bus->busnum,
		cfan->udev->dev.id);

	ret = usb_register_dev(interface, &cfan_class_driver);

	if (ret < 0) {
		pr_info("cfan: %s(): unable to register the device.\n",
			__func__);
		return ret;
	}

	return 0;
}

static void cfan_disconnect(struct usb_interface *interface)
{
	struct usb_cfan *dev;

	dev = usb_get_intfdata(interface);
	usb_set_intfdata(interface, NULL);
	usb_deregister_dev(interface, &cfan_class_driver);
	kfree(dev);

	pr_info("cfan: %s(): USB Cheeky Fan has been disconnected.\n",
		__func__);
}

static struct usb_driver cfan_driver = {
	.name       =	"cfan",
	.probe      =	cfan_probe,
	.disconnect =	cfan_disconnect,
	.id_table   =	id_table,
};

static int __init usb_cfan_init(void)
{
	int retval = 0;
	/* Register the USB driver. */
	retval = usb_register(&cfan_driver);
	if (retval < 0) {
		pr_info("cfan: %s(): unable to register USB driver.\n",
			__func__);
		return retval;
	}
	return 0;
}

static void __exit usb_cfan_exit(void)
{
	/* Un-register this USB driver. */
	usb_deregister(&cfan_driver);
}

module_init(usb_cfan_init);
module_exit(usb_cfan_exit);
