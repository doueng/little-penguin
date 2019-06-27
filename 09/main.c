// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs_struct.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <../fs/mount.h>
#include <linux/kallsyms.h>

static struct proc_dir_entry *pent;

static void print_mnt_rec(struct seq_file *s, struct mount *mnt)
{
	if (mnt_has_parent(mnt->mnt_parent))
		print_mnt_rec(s, mnt->mnt_parent);
	seq_dentry(s, mnt->mnt_mountpoint, " \t\n\\");
}

static int ft_show(struct seq_file *s, void *v)
{
	struct mount *mnt = v;

	if (IS_ERR(mnt))
		return PTR_ERR(mnt);
	seq_printf(s, "%-20s", mnt->mnt_devname);
	print_mnt_rec(s, mnt);
	seq_puts(s, "\n");
	return 0;
}

static void *ft_next(struct seq_file *s, void *v, loff_t *pos)
{
	struct mount *mnt = v;
	struct mount *next;

	if (IS_ERR(mnt))
		return mnt;
	next = list_next_entry(mnt, mnt_list);
	if (next == real_mount(s->private))
		goto end;
	return next;
end:
	*pos = 1337;
	return NULL;
}

static void *ft_start(struct seq_file *s, loff_t *pos)
{
	struct path root;
	struct vfsmount *(*collect_mounts)(const struct path *p);
	void (*drop_collected_mounts)(struct vfsmount *m);

	if (*pos == 1337)
		goto end;
	collect_mounts = (void *)kallsyms_lookup_name("collect_mounts");
	get_fs_root(current->fs, &root);
	s->private = collect_mounts(&root);
	if (IS_ERR(s->private))
		return NULL;
	return real_mount(s->private);
end:
	drop_collected_mounts = (void *)kallsyms_lookup_name("drop_collected_mounts");
	drop_collected_mounts(s->private);
	return NULL;

}

static void ft_stop(struct seq_file *s, void *v)
{
}

const struct seq_operations seq_ops = {
	.start = ft_start,
	.next = ft_next,
	.show = ft_show,
	.stop = ft_stop
};

static int ft_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &seq_ops);
}

const struct file_operations ft_ops = {
	.owner		= THIS_MODULE,
	.open		= ft_open,
	.read		= seq_read
};

static int ft_init(void)
{
	pent = proc_create("mymounts", 0660, NULL, &ft_ops);
	if (IS_ERR(pent))
		return PTR_ERR(pent);
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
