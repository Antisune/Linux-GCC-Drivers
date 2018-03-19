
//-----Linux Kernel Module Programming - 03 Coding, Compiling the Module

//-create file called: "mayflashHello.c"

#include <linux/init.h>
#include <linux/module.h>

// create '.c' file to initialize, compile and use the module
/* entry point for the application */
static void hello_init(void){
	printk(KERN_ALERT "Hello world, this is the Mayflash driver"); //announces the driver

	return 0;
}

static void hello_exit(void){
	printk(KERN_ALERT "Test: Mayflash module removed");
}

module_init(hello_init); //loaded module
module_exit(hello_exit);


/* */