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

MODULE_AUTHOR("Thomas Venries, EPITA");
MODULE_DESCRIPTION("CheekyFan module, v0.1");
MODULE_LICENSE("GPL");

#define VENDOR_ID	0x0c45
#define PRODUCT_ID	0x7701

struct usb_cfan {
	struct usb_device	*udev;
	u64	displays[39][8];
	u64     cfg[8];
};

static struct usb_driver cfan_driver;
static struct usb_cfan *cfan;

/* Add the device to this driver's supported device list */
static struct usb_device_id id_table[] = {
	{ USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table);

static void init_cfan_cfg(struct usb_cfan *cfan)
{
	/* Configuration:
	 * A0 10 <A><B> <C><N> 55 00 00 00
	 * [A] : OpenOption	[0 ; 5]
	 * [B] : CloseOption	[0 ; 5]
	 * [C] : TurnOption	(6 | c)
	 * [D] : Display ID	[0 ; 7]
	 * Warning: Reverse the data bloc
	 */
	cfan->cfg[0] = 0x000000550000010A;
	cfan->cfg[1] = 0x000000551000010A;
	cfan->cfg[2] = 0x000000552000010A;
	cfan->cfg[3] = 0x000000553000010A;
	cfan->cfg[4] = 0x000000554000010A;
	cfan->cfg[5] = 0x000000555000010A;
	cfan->cfg[6] = 0x000000556000010A;
	cfan->cfg[7] = 0x000000557000010A;
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
	if (!cfan) {
		dev_warn(&cfan->udev->dev,
			 "cfan: cfan_write() : cfan is NULL !\n");
		return -1;
	}

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
	init_cfan_cfg(cfan);

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
