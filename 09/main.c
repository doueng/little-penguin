#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mount.h>

#include <linux/dcache.h>
#include <fs/mount.h>

#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
MODULE_DESCRIPTION("List mount points");

static struct proc_dir_entry *pent;

static ssize_t ft_read(struct file *filp,
		char __user *buf,
		size_t len,
		loff_t *ppos)
{
	// struct task_struct == typeof(current)
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	struct mount *mnt;

	list_for_each_entry(mnt, &ns->list, mnt_list)
	{
		print_d("%s is mounted", mnt->mnt_mountpoint->d_name.name);
	}
	return 0;

	/*struct dentry *curdentry;*/

	/*list_for_each_entry(curdentry, &current->fs->root.mnt->mnt_root->d_subdirs, d_child)*/
	/*{*/
		/*if (curdentry->d_flags & DCACHE_MOUNTED)*/
			/*printk("%s is mounted", curdentry->d_name.name);*/
	/*}*/
	/*return 0;*/
}

static struct file_operations ft_ops = {
	.owner = THIS_MODULE,
	.read = ft_read,
};

static int ft_init(void)
{
	pent=proc_create("mymounts",0660,NULL,&ft_ops);
	return pent ? 0 : -1;
}

static void ft_exit(void)
{
	proc_remove(pent);
}

module_init(ft_init);
module_exit(ft_exit);
