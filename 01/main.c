#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
MODULE_DESCRIPTION("Hello world");

int ft_init(void)
{
	pr_info("Hello world !\n");
	return 0;
}

void ft_exit(void)
{
	pr_info("Cleaning up module.\n");
}

module_init(ft_init);
module_exit(ft_exit);
