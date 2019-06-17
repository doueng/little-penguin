// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/syscalls.h>
#include <linux/jiffies.h>

struct dentry *debugfs_dentry;
static u8 buffer[PAGE_SIZE] = {0};
static u32 buffer_len = 0;

DEFINE_MUTEX(lock);

static ssize_t id_read(struct file *filp,
		char __user *user, size_t len, loff_t *ppos)
{
	int ret;

	mutex_lock(&lock);
	ret = simple_read_from_buffer(user, len, ppos, buffer, buffer_len);
	mutex_unlock(&lock);
	return ret;
}

static ssize_t id_write(struct file *filp,
		const char __user *user, size_t len, loff_t *ppos)
{
	int ret;

	mutex_lock(&lock);
	ret = simple_write_to_buffer(buffer, PAGE_SIZE, ppos, user, PAGE_SIZE);
	buffer_len = ret > 0 ? ret : buffer_len;
	mutex_unlock(&lock);
	return ret;
}

const static struct file_operations id_ops = {
	.read = id_read,
	.write = id_write
};

static ssize_t jiffies_read(struct file *filp,
		char __user *user, size_t len, loff_t *ppos)
{
	char jiffies[25] = {0};

	if (sprintf(jiffies, "%llu\n", get_jiffies_64()) < 0)
		return -1;
	return simple_read_from_buffer(user, len, ppos, jiffies,
			strlen(jiffies));
}

const static struct file_operations jiffies_ops = {
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

const static struct file_operations foo_ops = {
	.read = foo_read,
	.write = foo_write
};

#define my_syscall3(num, arg1, arg2, arg3)                                    \
({                                                                            \
	long _ret;                                                            \
	register long _num  asm("rax") = (num);                               \
	register long _arg1 asm("rdi") = (long)(arg1);                        \
	register long _arg2 asm("rsi") = (long)(arg2);                        \
	register long _arg3 asm("rdx") = (long)(arg3);                        \
									      \
	asm volatile (                                                        \
		"syscall\n"                                                   \
		: "=a" (_ret)                                                 \
		: "r"(_arg1), "r"(_arg2), "r"(_arg3),                         \
		  "0"(_num)                                                   \
		: "rcx", "r8", "r9", "r10", "r11", "memory", "cc"             \
	);                                                                    \
	_ret;                                                                 \
})


//static long (*chown)(const char __user *filename, uid_t user, gid_t group);

static int ft_init(void)
{
	//char dpath[101] = {0};
	char *path;
	//void **syscall_table;
	//syscall_table = (void*)0xffffffff820000e0;
	//chown = syscall_table[__NR_chown];
	//chown = (void*)kallsyms_lookup_name("sys_chwon");
	debugfs_dentry = debugfs_create_dir("fortytwo", NULL);
	if (!debugfs_dentry)
		return -1;
	//path = dentry_path_raw(debugfs_dentry, dpath, 100);
	//if (NULL == path)
	//	return -1;
	pr_info("%s\n", path);
	path = "/sys/kernel/debug/fortytwo";
	//chown(dpath, -1, 1000);
	//my_syscall3(92, path, -1, 1000);
	if (!debugfs_create_file("id", 0666, debugfs_dentry, NULL, &id_ops))
		return -1;
	if (!debugfs_create_file("jiffies", 0444, debugfs_dentry,
							 NULL, &jiffies_ops))
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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
