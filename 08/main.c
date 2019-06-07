#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstand");
MODULE_DESCRIPTION("Reverse string module");

DEFINE_MUTEX(lock);

char rev_str[PAGE_SIZE];

static ssize_t myfd_read(struct file *fp, char __user *user,
		size_t size, loff_t *offs)
{
	ssize_t res;

	mutex_lock(&lock);
	res = simple_read_from_buffer(user, size, offs, rev_str, strlen(rev_str));
	mutex_unlock(&lock);
	return res >= 0 ? 0 : res;
}

static ssize_t myfd_write(struct file *fp, const char __user *user,
		size_t size, loff_t *offs)
{
	ssize_t res;
	int i;

	mutex_lock(&lock);
	memset(rev_str, PAGE_SIZE, 0);
	res = simple_write_to_buffer(rev_str, PAGE_SIZE - 1, offs, user, size);
	if (res > 0) {
		i = 0;
		while (i < res)
			rev_str[i++] = rev_str[--res]
	}
	mutex_unlock(&lock);
	return res >= 0 ? 0 : res;
}

static struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = myfd_read,
	.write = myfd_write
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};

module_misc_device(myfd_device);
