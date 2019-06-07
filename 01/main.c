#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Douglas Engstrand");
MODULE_DESCRIPTION("Hello world");

int init_module(void)
{
	print_d("Hello world !\n");
	return 0;
}

void cleanup_module(void)
{
	print_d("Cleaning up module.\n");
}
