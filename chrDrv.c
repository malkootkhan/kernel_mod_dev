#include <init.h>
#include <module.h>
#include <linux/cdev.h>


#define MINOR			(0)
#define DEVICE_COUNT		(8)
#define DEVICE_NAME		("test_device")
#define DEVICE_CLASS		("test_class")

typedef enum 
{
	SUCCESS,
	FAILURE

}

static struct class *dev_class;
static struct cdev dev_cdev[DEVICE_COUNT];
static dev_t dev_num;


/*main function that is called by kernel*/

static int __init drv_init(void)
{
	int i;
	dev_t curr_dev;
	/*dynamic allocation of device number specifically the major one the minor we set already*/
	alloc_chrdev_region(&dev_num, MINOR, DEVICE_COUNT, DEVICE_NAME); 
	dev_class = class_create(THIS_MODULE, dev_class);
	
	char *kbuf;
	copy_from_user(kbuf, buff, count);	
	
	

	return SUCCESS;

}

