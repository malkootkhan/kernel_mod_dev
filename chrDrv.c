#include <linux/fscrypt.h>
#include <linux/module.h>   // Required for all kernel modules
#include <linux/init.h>     // Needed for the macros module_init and module_exit
#include <linux/fs.h>       // For the character device operations
#include <linux/cdev.h>     // For registering and unregistering your device
#include <linux/types.h>    // For standard data types used in the kernel
#include <linux/uaccess.h>  // For transferring data between user and kernel space
#include <linux/kdev_t.h>   // For device numbers
#include <linux/device.h>   // For device creation in the sysfs
#include <linux/kernel.h>   // For printk and other kernel operations
#include <linux/slab.h>     // For memory allocation (if required)
#include <linux/ioctl.h>    // For ioctl commands (if required)


#define MINORBITS		(20)					/*20 lower bits of 32 bits device number*/
#define MINORMASK		( (1U << MINORBITS) - 1)		/*shifting to left with 20 bits and subtracting by one will give us 20bits number eg: 111111...111*/
#define MAJOR(dev)		((unsigned int)(dev >> MINORBITS))      /*shifting 20bits towards right will give us 12 significant bits which is major*/
#define MINOR(dev)		((unsigned int)(dev & MINORMASK))	/*it will give the lower 20 bits which is minor number*/
#define MKDEV(ma, mi)		((unsigned int)((ma << MINORBITS) | mi ))
#define DEVICE_NAME		"my_test_device_"
#define CLASS_NAME		"my_test_class"
#define DEFAULT_MINOR		(0)
#define DEV_COUNT		(2)
#define GET_DEV_NAME(x)		DEVICE_NAME "%d",x
//#define DEBUG

#ifdef DEBUG
	int test_point = 0;
#endif

int my_file_open(struct inode *iInstance, struct file *fInstance);
int my_file_release(struct inode *iInstance, struct file *fInstance); 
ssize_t my_file_read(struct file *src, char __user *dest, size_t size, loff_t *ptr);
ssize_t my_file_write(struct file *dest, const char __user *src, size_t size, loff_t *ptr);

typedef enum 
{
	SUCCESS,
	FAILURE = 1

}status_t;
struct my_cdev
{
	dev_t dev_num;
	struct class *class;
	struct cdev dev_cdev[DEV_COUNT];
	struct file_operations fop;
};
struct my_cdev my_ch_dev = 
{
	.dev_num = 0,
	.fop={
		.owner = THIS_MODULE,
		.open = my_file_open,
		.release = my_file_release,
		.read = my_file_read,
		.write = my_file_write,
	},
};

int my_file_open(struct inode *iInstance, struct file *fInstance)
{
	printk("File open system call is invoked on user side!\n");
	return 0;
}
int my_file_release(struct inode *iInstance, struct file *fInstance)
{
	printk("file release system call is invoked on user space\n");
	return  0;
}
ssize_t my_file_read(struct file *src, char __user *dest, size_t size, loff_t *ptr)
{
	printk("file read system call invoked on user space\n");
	return 0;
}
ssize_t my_file_write(struct file *dest, const char __user *src, size_t size, loff_t *ptr)
{
	printk("file write system call invoked on user space\n");
	return size;
}
/*main function that is called by kernel*/
/*the keywords: __init, __exit are the compiler attributes that tell the linker to put this symbol function name into .init.text section that is already known by the compiler when and why to free */
static int __init chr_drv_init(void)
{
	int ret;
	int index;
	dev_t curr_dev;
	
#ifdef DEBUG
	printk("test_point %d: Welcome to character drv\n", ++test_point);
#endif
	/*Step 01: Create class in the first step so that we can link the device this class you can see class in /sys/class/ and within class you can see your device what you linked*/
	my_ch_dev.class = class_create(CLASS_NAME);

	/*Step 02: create or assign device number it can be done in two ways either static or dynamic we will dynamic alloc*/
	ret = alloc_chrdev_region(&my_ch_dev.dev_num, DEFAULT_MINOR, DEV_COUNT, DEVICE_NAME);
	if(ret < 0)
	{
		printk("device number couldn't be allocated\n");
		return FAILURE; 
	}
	for(index = 0; index < DEV_COUNT; index++)
	{

		/*Step 03: initialize device here*/
#ifdef DEBUG
		printk("test_point %d.%d: cdev_init function is following\n", ++test_point,index);
#endif
		cdev_init(&my_ch_dev.dev_cdev[index], &my_ch_dev.fop);

		curr_dev = MKDEV( MAJOR(my_ch_dev.dev_num), (MINOR(my_ch_dev.dev_num)+index) );

		/*Step 04: add the device to system*/
#ifdef DEBUG
		printk("test_point %d.%d: cdev_add function is following\n", ++test_point,index);
#endif
		cdev_add(&my_ch_dev.dev_cdev[index], curr_dev, DEV_COUNT);

		/*Step 05: it will create a special file located in /dev direcory*/
#ifdef DEBUG
		printk("test_point %d.%d: device_create function is following\n", ++test_point,index);
#endif
		device_create(my_ch_dev.class, NULL, curr_dev, NULL, GET_DEV_NAME(index));
	}
	return SUCCESS;

}

static void __exit chr_drv_exit(void)
{
	int index;
	dev_t curr_dev;
#ifdef DEBUG
	printk("test_point %d: Good bye", ++test_point);
#endif

	for(index = 0; index < DEV_COUNT; index++)
	{

		curr_dev = MKDEV ( MAJOR(my_ch_dev.dev_num), (MINOR(my_ch_dev.dev_num)+index) );

		/*Step 01: device destroy means removing the device special file /dev*/
		device_destroy(my_ch_dev.class, curr_dev);

		/*Step 02: cdev_del makes the system forget about the character device*/
		cdev_del(&my_ch_dev.dev_cdev[index]);

		/*Step 04: this function will release the device number*/
		unregister_chrdev_region(curr_dev, DEV_COUNT);
	}
	
	/*Step 03: class_destroy and class_unregister will deregister and remove the class from the system*/
	class_unregister(my_ch_dev.class);
	class_destroy(my_ch_dev.class);
}



/*register the functions here with kernel*/
module_init(chr_drv_init);
module_exit(chr_drv_exit);

