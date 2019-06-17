// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
#include <linux/module.h>
#include <linux/kernel.h>
#include <../fs/mount.h>
#include <linux/limits.h> // PATH_MAX
#include <linux/slab.h> // kzalloc


#include <linux/path.h>

#include <linux/sched.h>
#include <linux/nsproxy.h>
#include <linux/mount.h>
#include <linux/string.h>

#include <linux/dcache.h>

#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

static struct proc_dir_entry *pent;

// vi proc_namespace.c +318

static ssize_t ft_read(struct file *filp, char __user *user, size_t len, loff_t *ppos)
{
	struct mount *mnt;
	struct path path;
	char *path_name;
	char *tmp;

	path_name = kzalloc(100, GFP_KERNEL);
	list_for_each_entry(mnt, &current->nsproxy->mnt_ns->list, mnt_list) {
		path.mnt = &mnt->mnt;
		path.dentry = mnt->mnt_mountpoint;
		tmp = d_path(&path, path_name, 100);
		pr_info("%s\n", tmp);
		//pr_info("%s\n", tmp);
	}
	return 0;
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
