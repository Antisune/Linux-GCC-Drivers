
//-----Linux Kernel Module Programming - 04 Passing Arguments to Kernel Module

//Changes Designated by STEP #

//-create file called: "mayflashHello.c"

#include <linux/init.h>
#include <linux/module.h>
//step 1
/* add functionality for console input */
#include <linux/moduleparam.h> //accesses functionality for parameters given from the console

//step 2
/* declare variable that holds the value passed to the module */
int param_var = 0;

//step 3 register (macro)
//of form: "module_param(name_var, type, permissions"
/* register variable to module */
// types of permissions:
/*
	S_IRUSR -READING to USR (user)
	S_IWUSR	-WRITING to USR (user)
	S_IXUSR	-EXECUTION to USR (user)
	S_IWGRP	-WRITING to GRP (group)
	S_IRGRP	-READING to GRP (group)
*/

module_param(param_var, int S_IRUSR | S_IWUSR); //variable can be read AND written to

void display(){
	printk(KERN_ALERT "TEST: param = %d", param_var);
}

// create '.c' file to initialize, compile and use the module
/* entry point for the application */
static void hello_init(void){
	printk(KERN_ALERT "Hello world, this is the Mayflash driver"); //announces the driver
	display();
	return 0;
}

static void hello_exit(void){
	printk(KERN_ALERT "Test: Mayflash module removed");
}

module_init(hello_init); //loaded module
module_exit(hello_exit);

/*  */