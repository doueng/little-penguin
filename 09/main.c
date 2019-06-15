// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mount.h>
#include <linux/limits.h> // PATH_MAX
#include <linux/path.h>


#include <linux/sched.h>
#include <linux/nsproxy.h>
//#include <../fs/mount.h>
#include <linux/mount.h>
#include <linux/string.h>

#include <linux/dcache.h>

#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define read_string(s) (simple_read_from_buffer(user, len, ppos, s, strlen(s)))
#define x(ret) if (ret < 0) { return ret; } else { ret; }

static struct proc_dir_entry *pent;

static ssize_t ft_read(struct file *filp, char __user *user, size_t len, loff_t *ppos)
{
	// struct task_struct == typeof(current)
	struct mount *mnt;
	struct path;
	//char mount_name[PATH_MAX+1];
	//char r[1000] = {0};
	//char *d_name;
	ssize_t num_read;

	list_for_each_entry(mnt, &current->nsproxy->mnt_ns->list, mnt_list) {
		//memset(mount_name, 0, ARRAY_SIZE(mount_name));
		//d_name = mnt->mnt_mountpoint->d_iname;
		num_read += x(read_string(mnt->mnt_mountpoint->d_iname));
		path = {.mnt = &mnt->mnt,
			.dentry = mnt->mnt_mountpoint}
		num_read += x(read_string("\t"));
		num_read += x(read_string(path_get(&path)));
		num_read += x(read_string("\n"));
		//num_read += read_string(mnt->mnt_mountpoint->d_iname);
		//snprintf(mount_name, PATH_MAX, "%s\t%s\n", d_name, path_get(&path));
		//sprintf(mount_name, "%s\t%s\n", d_name, mnt->mnt_devname);
		//strcat(r, mount_name);
	}
	return num_read;
}

const static struct file_operations ft_ops = {
	.owner = THIS_MODULE,
	.read = ft_read,
};

static int ft_init(void)
{
	pent = proc_create("mymounts", 0660, NULL, &ft_ops);
	return pent ? 0 : -1;
}

static void ft_exit(void)
{
	proc_remove(pent);
}

module_init(ft_init);
module_exit(ft_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
MODULE_DESCRIPTION("List mount points");
