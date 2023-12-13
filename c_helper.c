#include "internal.h"
#include <linux/printk.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

void helper_func(void)
{
	pr_info("helper_func called\n");
}

