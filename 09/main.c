#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mount.h>


#include <linux/sched.h>
#include <linux/nsproxy.h>
#include <../fs/mount.h>
#include <linux/string.h>

#include <linux/dcache.h>

#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
MODULE_DESCRIPTION("List mount points");

static struct proc_dir_entry *pent;

static ssize_t ft_read(struct file *filp, char __user *user, size_t len, loff_t *ppos)
{
	// struct task_struct == typeof(current)
	struct mount *mnt;
	char mount_name[100];
	char path[100];
	char r[1000] = {0};
	char *d_name;

	list_for_each_entry(mnt, &current->nsproxy->mnt_ns->list, mnt_list)
	{
		memset(mount_name, 0, ARRAY_SIZE(mount_name));
		memset(path, 0, ARRAY_SIZE(path));
		d_name = mnt->mnt_mountpoint->d_iname;
		sprintf(mount_name, "%s\t%s\n", d_name, dentry_path_raw(mnt->mnt_mountpoint, path, ARRAY_SIZE(path)));
		//sprintf(mount_name, "%s\t%s\n", d_name, mnt->mnt_devname);
		strcat(r, mount_name);
	}
	return simple_read_from_buffer(user, len, ppos, r, strlen(r));
}

static struct file_operations ft_ops = {
	.owner = THIS_MODULE,
	.read = ft_read,
};

static int ft_init(void)
{
	pr_info("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	pent = proc_create("mymounts",0660,NULL,&ft_ops);
	return (pent ? 0 : -1);
}

static void ft_exit(void)
{
	pr_info("?????????????????????????????????");
	proc_remove(pent);
}

module_init(ft_init);
module_exit(ft_exit);
