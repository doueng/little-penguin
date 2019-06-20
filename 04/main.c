// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hid.h>
#include <linux/usb.h>

static void usb_kbd_disconnect(struct usb_interface *intf)
{
	pr_info("KEYBOARD IS GONE!!!");
}

static int usb_kbd_probe(struct usb_interface *iface,
			 const struct usb_device_id *id)
{
	pr_info("KEYBOARD DETECTED");
	return (0);
}

static const struct usb_device_id usb_kbd_id_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{ }
};

MODULE_DEVICE_TABLE (usb, usb_kbd_id_table);

static struct usb_driver usb_kbd_driver = {
	.name = "keyboard alarm",
	.probe = usb_kbd_probe,
	.disconnect = usb_kbd_disconnect,
	.id_table = usb_kbd_id_table,
};

module_usb_driver(kbd_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
MODULE_DESCRIPTION("Keyboard alarm system");
