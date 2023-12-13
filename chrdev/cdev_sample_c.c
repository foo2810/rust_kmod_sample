#include "internal.h"
#include <linux/printk.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");

#define MY_MAJOR 42
#define MY_MAX_MINORS 1

#define MAX_DEVICE_DATA 256

struct my_device_data {
	struct cdev cdev;
	char buffer[MAX_DEVICE_DATA];
	size_t size;
};

struct my_device_data my_device_data_devs[MY_MAX_MINORS];

static int my_open(struct inode *inode, struct file *file)
{
	struct my_device_data *my_data =
			container_of(inode->i_cdev, struct my_device_data, cdev);

	// pr_info("my_open called");

	/* validate access to device */
	file->private_data = my_data;

	/* initialize device */
	my_data->size = MAX_DEVICE_DATA;

	return 0;
}

static ssize_t my_read(struct file *file, char __user *user_buffer,
					size_t size, loff_t *offset)
{
	struct my_device_data *my_data = (struct my_device_data *) file->private_data;
	ssize_t len = min(my_data->size - *offset, size);

	// pr_info("my_read called");

	if (len <= 0)
		return 0;

	/* read data from my_data->buffer to user buffer */
	if (copy_to_user(user_buffer, my_data->buffer + *offset, len))
		return -EFAULT;

	*offset += len;
	return len;
}

static ssize_t my_write(struct file *file, const char __user *user_buffer,
					size_t size, loff_t * offset)
{
	struct my_device_data *my_data = (struct my_device_data *) file->private_data;
	ssize_t len = min(my_data->size - *offset, size);

	// pr_info("my_write called: size=%lld, len=%lld\n, offset=%d", size, len, *offset);

	if (len <= 0)
		return 0;

	/* read data from user buffer to my_data->buffer */
	if (copy_from_user(my_data->buffer + *offset, user_buffer, len))
		return -EFAULT;

	*offset += len;
	return len;
}

int my_release(struct inode *inode, struct file *file)
{
	struct my_device_data *my_data = (struct my_device_data *) file->private_data;

	// pr_info("my_release called");

	my_data->size = 0;

	return 0;
}

static struct file_operations cdev_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
};

void helper_func(void)
{
	pr_info("helper_func called\n");
}

int init_cdev(void)
{
	int err;
	struct cdev *cdev_ptr;

	err = register_chrdev_region(MKDEV(MY_MAJOR, 0), 1, "mycdev");

	if (err != 0)
		return err;
	
	cdev_ptr = &my_device_data_devs[0].cdev;
	cdev_init(cdev_ptr, &cdev_fops);
	cdev_add(cdev_ptr, MKDEV(MY_MAJOR, 0), 1);

	return 0;
}

void cleanup_cdev(void)
{
	struct cdev *cdev_ptr = &my_device_data_devs[0].cdev;;

	cdev_del(cdev_ptr);

	unregister_chrdev_region(MKDEV(MY_MAJOR, 0), 1);
}

