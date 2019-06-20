// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/syscalls.h>
#include <linux/jiffies.h>

struct dentry *debugfs_dir;

const static char *LOGIN = "dengstra";
const static size_t LOGIN_LEN = 8;

static u8 buffer[PAGE_SIZE] = {0};
static u32 buffer_len;

DEFINE_MUTEX(lock);

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

static ssize_t foo_read(struct file *filp,
		char __user *user, size_t len, loff_t *ppos)
{
	ssize_t ret;

	mutex_lock(&lock);
	ret = simple_read_from_buffer(user, len, ppos, buffer, buffer_len);
	mutex_unlock(&lock);
	return ret;
}

static ssize_t foo_write(struct file *filp,
		const char __user *user, size_t len, loff_t *ppos)
{
	ssize_t ret;

	if (len > PAGE_SIZE)
		return -EINVAL;
	mutex_lock(&lock);
	ret = simple_write_to_buffer(buffer, len, ppos, user, PAGE_SIZE);
	if (ret > 0)
		buffer_len = ret;
	mutex_unlock(&lock);
	return ret;
}

const static struct file_operations foo_ops = {
	.read = foo_read,
	.write = foo_write
};

static int ft_init(void)
{
	struct dentry *ret;

	debugfs_dir = debugfs_create_dir("fortytwo", NULL);
	if (IS_ERR(debugfs_dir))
		goto err_dir;
	ret = debugfs_create_file("id", 0666, debugfs_dir, NULL, &id_ops);
	if (IS_ERR(ret))
		goto err_file;
	ret = debugfs_create_u64("jiffies", 0444, debugfs_dir, (u64 *)&jiffies);
	if (IS_ERR(ret))
		goto err_file;
	ret = debugfs_create_file("foo", 0644, debugfs_dir, NULL, &foo_ops);
	if (IS_ERR(ret))
		goto err_file;
	return 0;
err_dir:
	return PTR_ERR(debugfs_dir);
err_file:
	return PTR_ERR(ret);
}

static void ft_exit(void)
{
	debugfs_remove_recursive(debugfs_dir);
}

module_init(ft_init);
module_exit(ft_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
