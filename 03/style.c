// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

int delay(int delays)
{
	size_t udelays;

	udelays = 10 * delays;
	udelay(udelays);
	return udelays;
}

int my_init(void)
{
	return delay(10);
}

void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);
