// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hid.h>
#include <linux/usb.h>

static struct usb_driver kbd_driver = {
	.name = "keyboard_alarm",
};

module_usb_driver(kbd_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
MODULE_DESCRIPTION("Keyboard alarm system");
