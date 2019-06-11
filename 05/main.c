#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
MODULE_DESCRIPTION("ft_misc");

const static char *LOGIN = "dengstra";
const static size_t LOGIN_LEN = 8;

static ssize_t ft_read (struct file *filp,
		char __user *user, size_t len, loff_t *ppos)
{
	return simple_read_from_buffer(user, len, ppos, LOGIN, LOGIN_LEN);
}

static ssize_t ft_write (struct file *filp,
		const char __user *user, size_t len, loff_t *ppos)
{
	char to[9] = {0};
	ssize_t ret;

	ret = simple_write_to_buffer(to, LOGIN_LEN, ppos, user, len);
	return (0 == memcmp(LOGIN, user, LOGIN_LEN)
		? ret
		: -EINVAL);
}

const static struct file_operations fops = {
	.read = ft_read,
	.write = ft_write
};

static struct miscdevice miscdev = {
	.name = "fortytwo",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &fops
};

module_misc_device(miscdev);
