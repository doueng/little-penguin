#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hid.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
MODULE_DESCRIPTION("Keyboard alarm system");

int	kbd_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	print_d("!!!!!!!!!!!!!!!!!!!!!!\n");
	print_d("======= DANGER =======\n");
	print_d("====== KEYBOARD ======\n");
	print_d("===== CONNECTED! =====\n");
	print_d("!!!!!!!!!!!!!!!!!!!!!!\n");
	return 0;
}

void	kbd_disconnect(struct usb_interface *intf)
{
	print_d("!!!!!!!!!!!!!!!!!!!!!!!\n");
	print_d("===== DANGER OVER =====\n");
	print_d("====== KEYBOARD =======\n");
	print_d("==== DISCONNECTED =====\n");
	print_d("!!!!!!!!!!!!!!!!!!!!!!!\n");
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
