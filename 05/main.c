// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

const static char *LOGIN = "dengstra";
const static size_t LOGIN_LEN = 8;

static ssize_t id_read(struct file *filp,
		char __user *user, size_t len, loff_t *ppos)
{
	return simple_read_from_buffer(user, len, ppos, LOGIN, LOGIN_LEN);
}

static ssize_t id_write(struct file *filp,
		const char __user *user, size_t len, loff_t *ppos)
{
	char to[8] = {0};
	ssize_t ret;

	if (len != LOGIN_LEN)
		return -EINVAL;
	ret = simple_write_to_buffer(to, LOGIN_LEN, ppos, user, len);
	return memcmp(LOGIN, user, LOGIN_LEN) ? -EINVAL : ret;
}

const static struct file_operations id_ops = {
	.read = id_read,
	.write = id_write
};

static struct miscdevice miscdev = {
	.name = "fortytwo",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &id_ops
};

module_misc_device(miscdev);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
MODULE_DESCRIPTION("ft_misc");
