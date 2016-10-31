#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/usb.h>

#include "convert.h"
#include "data.h"

#define VID  0x0c45
#define PID  0x7701

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Venries <thomas.venries@gmail.com>");
MODULE_DESCRIPTION("Pearl USB LED Fan module");
MODULE_VERSION("v0.7");

static struct usb_device_id id_table[] = {
	{ USB_DEVICE(VID, PID) },
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table);

static struct usb_device *pfan;

static int send(struct usb_device *udev, void *data)
{
	char buf[8];
	int l = 0;
	int ret;

	memset(buf, 0, 8);
	buf[7] = 0x02;

	ret = usb_control_msg(udev, usb_sndctrlpipe(udev, 0), 0x09, 0x21, 0x200,
			0x00, data, 0x008, 10 * HZ);

	if (ret <= 0)
		return ret;

	usb_interrupt_msg(udev, usb_rcvintpipe(udev, 1), buf, 8, &l, 10 * HZ);

	return 0;
}

static int pfan_open(struct inode *i, struct file *f)
{
	return 0;
}

static int pfan_release(struct inode *i, struct file *f)
{
	return 0;
}

static ssize_t pfan_write(struct file *f,
		const char __user *buffer,
		size_t cnt,
		loff_t *off)
{
	struct pfan_data *data = NULL;
	u16 display[156];
	int i;
	int j;

	if (cnt == 0) {
		pr_err("%s: buffer size is null !\n", __func__);
		return 0;
	}

	if (!buffer) {
		pr_err("%s: buffer is null but its size is not !\n", __func__);
		return -EINVAL;
	}

	if (cnt > sizeof(struct pfan_data)
			|| cnt < sizeof(struct pfan_data)) {
		pr_err("%s: the size's value is unexpected !\n", __func__);
		return -EINVAL;
	}

	data = (struct pfan_data *)buffer;

	for (i = 0; i < data->n; ++i) {
		u64 effect = pfan_convert_effect(i, data->effects[i]);
		send(pfan, &effect);
		for (j = 0; j < 39; j++) {
			memset(&display, 0xFF, 156 * 2);
			pfan_convert_raster(i, data->images[i], display);
			send(pfan, (u16 *)&display[j * 4]);
		}
	}

	return 1;
}

static struct file_operations pfan_fops = {
	.open    = pfan_open,
	.release = pfan_release,
	.write = pfan_write,
};

static struct usb_class_driver pfan_class = {
	.name = "usb/pfan0",
	.fops = &pfan_fops,
	.minor_base = 0
};

static int pfan_probe(struct usb_interface *interface,
		const struct usb_device_id *id)
{
	struct usb_device *udev = NULL;
	int ret;

	udev = interface_to_usbdev(interface);

	if (!udev) {
		pr_info("%s: device is null.\n", __func__);
		return -ENOMEM;
	}

	pfan = usb_get_dev(udev);

	pr_info("%s: device has been connected\n", __func__);

	ret = usb_register_dev(interface, &pfan_class);

	if (ret < 0) {
		pr_info("%s: unable to register the device.\n", __func__);
		return ret;
	}
	return 0;
}

static void pfan_disconnect(struct usb_interface *interface)
{
	usb_set_intfdata(interface, NULL);
	usb_deregister_dev(interface, &pfan_class);
	kfree(usb_get_intfdata(interface));
	pr_info("%s: the fan has been disconnected.\n", __func__);
}

static struct usb_driver pfan_driver = {
	.name       = "pfan",
	.probe      = pfan_probe,
	.disconnect = pfan_disconnect,
	.id_table   = id_table,
};

static int __init pfan_init(void)
{
	int ret = usb_register(&pfan_driver);

	if (ret < 0) {
		pr_info("%s: unable to register the driver.\n", __func__);
		return ret;
	}
	pr_info("%s: the driver has been registered.\n", __func__);
	return 0;
}

static void __exit pfan_exit(void)
{
	pr_info("%s: the driver has been unregistered.\n", __func__);
	usb_deregister(&pfan_driver);
}

module_init(pfan_init);
module_exit(pfan_exit);
