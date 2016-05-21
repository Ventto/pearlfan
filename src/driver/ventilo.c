/*
 * Linux driver for USB Leds Fan (by Dream Cheeky)
 */
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/usb.h>

#include "includes/ventilo_data.h"

MODULE_AUTHOR("Thomas Venries, Franklin Mathieu");
MODULE_DESCRIPTION("CheekyFan module, v0.1");
MODULE_LICENSE("GPL");

#define VENDOR_ID	0x0c45
#define PRODUCT_ID	0x7701

struct usb_ventilo {
	struct 	usb_device	*udev;
	u64     cfg[8];			/* displays configuration */
	u16	displays[8][156];	/* displays's buffer */
	u8	displays_nb;		/* number of displays */
};

static struct usb_driver ventilo_driver;
static struct usb_ventilo *ventilo;

/* Add the device to this driver's supported device list */
static struct usb_device_id id_table[] = {
	{ USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table);

/*
 * Global array which helps to convert a given PBM raster to
 * a ventilator's display (see pbm_to_display function below).
 */
static u16 pbm_mask[11];

/* Initializes masks which represent eleven LEDs's activation */
static void pbm_masks_init(void)
{
	pbm_mask[10] = 0xFFF7;	/* led 11 */
	pbm_mask[9] = 0xFFFB;	/* led 10 */
	pbm_mask[8] = 0xFFFD;	/* led 9 */
	pbm_mask[7] = 0xFFFE;	/* led 8 */

	pbm_mask[6] = 0x7FFF;	/* led 7 */
	pbm_mask[5] = 0xBFFF;	/* led 6 */
	pbm_mask[4] = 0xCFFF;	/* led 5 */

	pbm_mask[3] = 0xFEFF;	/* led 4 */
	pbm_mask[2] = 0xF7FF;	/* led 3 */
	pbm_mask[1] = 0xFBFF;	/* led 2 */
	pbm_mask[0] = 0xFCFF;	/* led 1 */
}

/* Converts a given PBM raster to a ventilator's display */
static void pbm_to_display(unsigned char id,
			   char *raster,
			   uint16_t *display)
{
	unsigned char i;
	unsigned char j;
	unsigned char col_end;

	for (i = 0; i < 156; ++i) {
		col_end = 155 - i;
		for (j = 0; j < 11; ++j)
			if (raster[j * 156 + i] == 1)
				display[col_end] &= pbm_mask[10 - j];
	}
	for (i = 0; i < 156; ++i)
		pr_info("%x\n", display[i]);
}

/* Set effect config for a fan view */
static u64 set_config(unsigned char id,
		      unsigned char open_option,
		      unsigned char close_option,
		      unsigned char turn_option)
{
	/* Configuration:
	 * A0 10 <A><B> <C><D> 55 00 00 00
	 * [A] : OpenOption	[0 ; 5]
	 * [B] : CloseOption	[0 ; 5]
	 * [C] : TurnOption	(6 | c)
	 * [D] : Display ID	[0 ; 7]
	 * Warning: Reverse bytes sequence (stack behind ?) :
	 * 00 00 00 55 [CD] [AB] 10 A0
	 * [CDAB] : u16
	 */
	u16 cdab = 0;

	pr_info("ventilo:%s: ID: %d\n", __func__, id);
	pr_info("ventilo:%s: open option: %d\n", __func__, open_option);
	pr_info("ventilo:%s: close option: %d\n", __func__, close_option);
	pr_info("ventilo:%s: turn option: %d\n", __func__, turn_option);

	cdab |= close_option;
	cdab |= (open_option << 4);
	cdab |= (id << 8);
	cdab |= (turn_option << 12);

	return 0x00000055000010A0 | (cdab << 16);
}

static u8 write_letter(const unsigned char letter,
		       const unsigned char id,
		       const unsigned char column)
{
	unsigned char col = column;

	if (col)
		ventilo->displays[id][col++] = 0xFFFF;

	pr_info("ventilo:%s: detected letter: %x\n", __func__, letter);

	switch (letter) {
	case 'A':
		ventilo->displays[id][col++] = 0x00FF;
		ventilo->displays[id][col++] = 0x6EFF;
		ventilo->displays[id][col++] = 0x6EFF;
		ventilo->displays[id][col++] = 0x00FF;
		ventilo->displays[id][col++] = 0xFEFF;
		break;
	case 'B':
		ventilo->displays[id][col++] = 0x82FF;
		ventilo->displays[id][col++] = 0x6CFF;
		ventilo->displays[id][col++] = 0x6CFF;
		ventilo->displays[id][col++] = 0x00FF;
		break;
	case 'C':
		ventilo->displays[id][col++] = 0x7CFF;
		ventilo->displays[id][col++] = 0x7CFF;
		ventilo->displays[id][col++] = 0x7CFF;
		ventilo->displays[id][col++] = 0x00FF;
		break;
	case 'D':
		ventilo->displays[id][col++] = 0xC6FF;
		ventilo->displays[id][col++] = 0xBAFF;
		ventilo->displays[id][col++] = 0x7CFF;
		ventilo->displays[id][col++] = 0x00FF;
		break;
	case 'P':
		ventilo->displays[id][col++] = 0x9EFF;
		ventilo->displays[id][col++] = 0x6EFF;
		ventilo->displays[id][col++] = 0x6EFF;
		ventilo->displays[id][col++] = 0x00FF;
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

static int ventilo_open(struct inode *i, struct file *f)
{
	return 0;
}

static int ventilo_release(struct inode *i, struct file *f)
{
	return 0;
}

static ssize_t ventilo_write(struct file *f,
			  const char __user *buffer,
			  size_t cnt,
			  loff_t *off)
{
	struct ventilo_data *ventilo_data = NULL;
	unsigned char i;

	if (cnt == 0) {
		pr_err("ventilo:%s: buffer size is null !\n", __func__);
		return 0;
	}

	if (!buffer) {
		pr_err("ventilo:%s: buffer is null but its size is not !\n",
		       __func__);
		return -EINVAL;
	}

	if (cnt > sizeof(struct ventilo_data)
	    || cnt < sizeof(struct ventilo_data)) {
		pr_err("ventilo:%s: the size's value is unexpected !\n",
		       __func__);
		return -EINVAL;
	}

	ventilo_data = (struct ventilo_data *)buffer;
	ventilo->displays_nb = ventilo_data->n;
	ventilo->cfg[0] = set_config(0, ventilo_data->cfg[0][0],
				  ventilo_data->cfg[0][1],
				  ventilo_data->cfg[0][2]);

	pr_info("ventilo:%s: Number of displays: %d\n", __func__,
		ventilo->displays_nb);

	/* Clear the ventilator */
	/*
	* for (i = 0; i < 156; i += 4)
	*	send_data(ventilo->udev, (u16 *)ventilo->displays + i);
	* return 2;
	*/

	pbm_masks_init();
	pbm_to_display(0, ventilo_data->bitmaps[0], ventilo->displays[0]);

	/* Send config */
	send_data(ventilo->udev, &ventilo->cfg[0]);

	/* Send display */
	for (i = 0; i < 156; i += 4)
		send_data(ventilo->udev, (u16 *)ventilo->displays + i);

	return 1;
}

const struct file_operations ventilo_fops = {
	.open    = ventilo_open,
	.release = ventilo_release,
	.write = ventilo_write,
};

static struct usb_class_driver ventilo_class_driver = {
	.name = "usb/ventilo0",
	.fops = &ventilo_fops,
	.minor_base = 0
};

static int ventilo_probe(struct usb_interface *interface,
		      const struct usb_device_id *id)
{
	struct usb_device *udev = interface_to_usbdev(interface);
	int i;
	int j;
	int ret;

	ventilo = kmalloc(sizeof(*ventilo), GFP_KERNEL);

	if (!ventilo)
		return -ENOMEM;

	memset(ventilo, 0x00, sizeof(*ventilo));

	for (j = 0; j < 8; ++j)
		for (i = 0; i < 156; i++)
			ventilo->displays[j][i] = 0xFFFF;

	/* Increment the reference count of the usb device structure */
	ventilo->udev = usb_get_dev(udev);

	/* Increment the ref counter */
	usb_set_intfdata(interface, ventilo);

	pr_info("ventilo:%s: USB Cheeky Fan has been connected\n", __func__);
	pr_info("ventilo:%s: [devnum=%d;bus_id=%d]\n",
		__func__,
		ventilo->udev->devnum,
		ventilo->udev->bus->busnum);

	ret = usb_register_dev(interface, &ventilo_class_driver);

	if (ret < 0) {
		pr_info("ventilo: %s(): unable to register the device.\n",
			__func__);
		return ret;
	}

	return 0;
}

static void ventilo_disconnect(struct usb_interface *interface)
{
	struct usb_ventilo *dev;

	dev = usb_get_intfdata(interface);
	usb_set_intfdata(interface, NULL);
	usb_deregister_dev(interface, &ventilo_class_driver);
	kfree(dev);

	pr_info("ventilo: %s(): USB Cheeky Fan has been disconnected.\n",
		__func__);
}

static struct usb_driver ventilo_driver = {
	.name       =	"ventilo",
	.probe      =	ventilo_probe,
	.disconnect =	ventilo_disconnect,
	.id_table   =	id_table,
};

static int __init usb_ventilo_init(void)
{
	int retval = 0;
	/* Register the USB driver. */
	retval = usb_register(&ventilo_driver);
	if (retval < 0) {
		pr_info("ventilo: %s(): unable to register USB driver.\n",
			__func__);
		return retval;
	}
	return 0;
}

static void __exit usb_ventilo_exit(void)
{
	/* Un-register this USB driver. */
	usb_deregister(&ventilo_driver);
}

module_init(usb_ventilo_init);
module_exit(usb_ventilo_exit);