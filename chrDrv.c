#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define MINORBITS		(20)					/*20 lower bits of 32 bits device number*/
#define MINORMASK		( (1U << MINORBITS) - 1)		/*shifting to left with 20 bits and subtracting by one will give us 20bits number eg: 111111...111*/
#define MAJOR(dev)		((unsigned int)(dev >> MINORBITS))      /*shifting 20bits towards right will give us 12 significant bits which is major*/
#define MINOR(dev)		((unsigned int)(dev & MINORMASK))	/*it will give the lower 20 bits which is minor number*/
#define DEVICE_NAME		("my_test_device")
#define CLASS_NAME		("my_test_class")
#define DEFAULT_MINOR		(0)
#define DEV_COUNT		(1)

typedef enum 
{
	SUCCESS,
	FAILURE = 1

};

struct class *class;
struct cdev dev_cdev;
static dev_t dev_num;
struct file_operations fop;

/*main function that is called by kernel*/
/*the keywords: __init, __exit are the compiler attributes that tell the linker to put this symbol function name into .init.text section that is already known by the compiler when and why to free */
static int __init chr_drv_init(void)
{
	int ret;

	/*Step 01: Create class in the first step so that we can link the device this class*/
	class = class_create(CLASS_NAME);

	/*Step 02: create or assign device number it can be done in two ways either static or dynamic we will dynamic alloc*/
	ret = alloc_chrdev_region(&dev_num, DEFAULT_MINOR, DEV_COUNT, DEVICE_NAME);
	if(ret < 0)
	{
		printk("The device number couldn't be allocated\n");
		return FAILURE; 
	}
	
	/*Step 03: initialize device here*/
	cdev_init(&dev_cdev, &fop);

	/*Step 04: add the device to system*/
	cdev_add(&dev_cdev, dev_num, DEV_COUNT);

	/*Step 05: it will create a special file located in /dev direcory*/
	device_create(class, NULL, dev_num, NULL, DEVICE_NAME);
	return SUCCESS;

}

/*register the drv_init function here with kernel*/
module_init(chr_drv_init);
