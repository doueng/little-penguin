#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/syscalls.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");

struct dentry *debugfs_dentry;
static u8 buffer[PAGE_SIZE];
static u32 buffer_len = 0;

DEFINE_MUTEX(lock);

static ssize_t id_read(struct file *filp,
		char __user *user, size_t len, loff_t *ppos)
{
	int ret;

	mutex_lock(&lock);
	ret = simple_read_from_buffer(user, len, ppos, buffer, buffer_len);
	mutex_unlock(&lock);
	return ret == 0 ? -EINVAL : ret;
}

static ssize_t id_write(struct file *filp,
		const char __user *user, size_t len, loff_t *ppos)
{
	int ret;

	mutex_lock(&lock);
	ret = simple_write_to_buffer(buffer, PAGE_SIZE, ppos, user, len);
	mutex_unlock(&lock);
	buffer_len = ret > 0 ? ret : buffer_len;
	return ret == 0 ? -EINVAL : ret;
}

static struct file_operations id_ops = {
	.read = id_read,
	.write = id_write
};

static ssize_t jiffies_read(struct file *filp,
		char __user *user, size_t len, loff_t *ppos)
{
	char jiffies[25] = {0};

	if (0 > sprintf(jiffies, "%llu\n", get_jiffies_64()))
		return -1;
	return simple_read_from_buffer(user, len, ppos, jiffies,
			strlen(jiffies));
}

static struct file_operations jiffies_ops = {
	.read = jiffies_read,
};

static ssize_t foo_read(struct file *filp,
		char __user *user, size_t len, loff_t *ppos)
{
	return 0;
}

static ssize_t foo_write(struct file *filp,
		const char __user *user, size_t len, loff_t *ppos)
{
	return 0;
}

static struct file_operations foo_ops = {
	.read = foo_read,
	.write = foo_write
};

static int ft_init(void)
{
	char dpath[101] = {0};

	//ft_chown chown = kallsyms_lookup_name("sys_chown");

	if (!(debugfs_dentry = debugfs_create_dir("fortytwo", NULL)))
		return -1;
	//if (NULL == dentry_path_raw(debugfs_dentry, dpath, 100))
	//	return -1;
	if (!debugfs_create_file("id", 0666, debugfs_dentry, NULL, &id_ops))
		return -1;
	if (!debugfs_create_file("jiffies", 0444, debugfs_dentry, NULL, &jiffies_ops))
		return -1;
	if (!debugfs_create_file("foo", 0644, debugfs_dentry, NULL, &foo_ops))
		return -1;
	return 0;
}

static void ft_exit(void)
{
	debugfs_remove_recursive(debugfs_dentry);
}

module_init(ft_init);
module_exit(ft_exit);
