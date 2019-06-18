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

struct list_head *head;

static void *mnt_start(struct seq_file *m, loff_t *pos) {
	head = current->nsproxy->mnt_ns->list;
	return head;
}

static int show_mnt(struct seq_file *m, void *v) {
	struct mount *mnt = v;
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };

	seq_dentry(m, mnt->mnt_root, " \t\n\\");
	//seq_path_root(m, &mnt_path, &p->root, " \t\n\\");
	return 0;
}

static void *next_mnt(struct seq_file *m, void *v, loff_t *pos) {
	struct mount *mnt = v;
	return mnt->next == head ? NULL : mnt->next;
}

static void stop_mnt(struct seq_file *m, void *v) {
}

static struct seq_operations ct_seq_ops = {
     .start = ft_start,
     .next  = ft_next,
     .stop  = ft_stop,
     .show  = ft_show
};

static int ft_open(struct inode *inode, struct file *file) {
     return seq_open(file, &ct_seq_ops);
};

const static struct file_operations ft_ops = {
	.owner = THIS_MODULE,
	.open = ft_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release
};

static int ft_init(void)
{
	proc_create("mymounts", 0660, NULL, &ft_ops);
	return 0;
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
