// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

int my_init(void)
{
	udelay(100);
	pr_info("We slept a long time!");
	return 10;
}

void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);
