// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hid.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
MODULE_DESCRIPTION("Keyboard alarm system");

int	kbd_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	pr_info("!!!!!!!!!!!!!!!!!!!!!!\n");
	pr_info("======= DANGER =======\n");
	pr_info("====== KEYBOARD ======\n");
	pr_info("===== CONNECTED! =====\n");
	pr_info("!!!!!!!!!!!!!!!!!!!!!!\n");
	return 0;
}

void	kbd_disconnect(struct usb_interface *intf)
{
	pr_info("!!!!!!!!!!!!!!!!!!!!!!!\n");
	pr_info("===== DANGER OVER =====\n");
	pr_info("====== KEYBOARD =======\n");
	pr_info("==== DISCONNECTED =====\n");
	pr_info("!!!!!!!!!!!!!!!!!!!!!!!\n");
}

// include/uapi/linux/hid.h
static struct usb_device_id kbd_table[] = {
	{
		USB_INTERFACE_INFO(
			USB_INTERFACE_CLASS_HID,
			USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_KEYBOARD)
	}, { }
};

static struct usb_driver kbd_driver = {
	.name = "keyboard alarm",
	.probe = kbd_probe,
	.disconnect = kbd_disconnect,
	.id_table = kbd_table
};

MODULE_DEVICE_TABLE(usb, kbd_table);
module_usb_driver(kbd_driver);
