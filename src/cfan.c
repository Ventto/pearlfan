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
	u64	        	bmp[39][8];
	u64 		        cfg[8];
};

static struct usb_driver cfan_driver;

/* Add the device to this driver's supported device list */
static struct usb_device_id id_table[] = {
	{ USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table);

static int cfan_probe(struct usb_interface *interface,
		      const struct usb_device_id *id)
{
        /* interface_to_usbdev -> to_usb_device(intf->dev.parent) ->
         * container_of
         */
        struct usb_device *udev = interface_to_usbdev(interface);
        struct usb_cfan *dev;

        pr_info("cfan:%s: USB Cheeky Fan has been connected\n", __func__);

        dev = kmalloc(sizeof(struct usb_cfan), GFP_KERNEL);

        if (dev == NULL) {
            dev_err(&interface->dev, "cfan: Out of memory\n");
            return -ENOMEM;
        }

        memset(dev, 0x00, sizeof(*dev));
        /* Increment the reference count of the usb device structure
         * usb_get_dev -> get_dev -> kobj_to_dev -> kref_get ->
         * atomic_inc_return -> (asm)
         */
        dev->udev = usb_get_dev(udev);

        /* Increment the ref counter */
        usb_set_intfdata(interface, dev);

        pr_info("cfan:%s: [devnum=%d;bus_id=%d;devid=%d]\n",
                __func__,
                dev->udev->devnum,
                dev->udev->bus->busnum,
                dev->udev->dev.id);

	return 0;
}

static void cfan_disconnect(struct usb_interface *interface)
{
        struct usb_cfan *dev;

        pr_info("cfan:%s: USB Cheeky Fan has been disconnected.\n",
                __func__);

        dev = usb_get_intfdata (interface);
        usb_set_intfdata (interface, NULL);
        kfree(dev);
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
	    pr_info("cfan:%s(): unable to register USB driver.\n", __func__);
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
