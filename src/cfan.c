/*
 * Linux driver for Led Fan by Dream Cheeky (version 0)
 */
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <asm/uaccess.h>

MODULE_AUTHOR("Thomas Venries, EPITA");
MODULE_DESCRIPTION("CheekyFan module, v0.1");
MODULE_LICENSE("GPL");

#define VENDOR_ID	0x0c45
#define PRODUCT_ID	0x7701

// Private structure
struct usb_cfan {
	struct usb_device	*udev;
	unsigned char		bmp[39][8];
	unsigned int		cfg[8];
};

// Forward declaration
static struct usb_driver cfan_driver;

/* Table of devices that work with this driver.
 * Informations for the Linux-Hotplug system to autoload the driver.
 */
static struct usb_device_id id_table [] = {
	{ USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
	{ },
};
MODULE_DEVICE_TABLE (usb, id_table);

// USB driver functions
static int cfan_probe (struct usb_interface *interface,
			const struct usb_device_id *id)
{
  printk (KERN_INFO "%s: USB Cheeky Fan has been connected\n",
		__FUNCTION__);
  return 0;
}

static void cfan_disconnect(struct usb_interface *interface)
{
	printk (KERN_INFO "%s: USB Cheeky Fan has been disconnected.\n",
		__FUNCTION__);
}

static struct usb_driver cfan_driver = {
	.name       =	"cfan",
	.probe      =	cfan_probe,
	.disconnect =	cfan_disconnect,
	.id_table   =	id_table,
};

static int __init usb_cfan_init(void)
{
	/* To register the USB driver with the USB subsystem. */
	return usb_register(&cfan_driver);
}

static void __exit usb_cfan_exit(void)
{
	/* deregister this driver with the USB subsystem */
 	usb_deregister(&cfan_driver);
}

module_init(usb_cfan_init);
module_exit(usb_cfan_exit);
