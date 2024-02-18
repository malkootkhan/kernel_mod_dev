/*These headers are necessary for module as they hold APIs for module and initialization*/
#include <linux/module.h>
#include <linux/init.h>

/*
 *These macros are giving some important minimal info about the module
 */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is just a hello custom module for learning purposes");
MODULE_AUTHOR("Malkoot Khan");
//MODULE_INFO("Test_module", "for learning purpose");

typedef enum{FALSE, TRUE}bool_t;

static int __init hello_world(void)
{
	printk("Hello World!\n");
	return TRUE;
}

/*Register module with kernel calling tree*/
module_init(hello_world);
