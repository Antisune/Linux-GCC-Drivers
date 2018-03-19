
//-----Linux Kernel Module Programmin - 07 Coding the Char Device

//mayflashModule.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> 		//file operations struction: open/close/read/write to device
#include <linux/cdev.h>		//char driver; makes cdev available
#include <linux/semaphore>	//used to access semaphores
#include <asm/uaccess.h>	//copy_to_user;copy_from_user

//create structure
struct mayflash_device {
		char data[100];
		struct semaphore sem;
} virtual_device;

//registering the device requires a cdev object and other useful variables, defined in a space outside of the stack to prevent memory issues

struct cdev *mcdev; // my char device
int major_number;	// stores major number, which is extracted from dev_t using - mknod /director/file c major minor
int ret; //holds return values of functions, used in error correction

dev_t dev_num;		// holds major number given to the user by the kernel

#define DEVICE_NAME "mayflashdevice"

//now need to write an entry point for the device driver
//registers the capabilities of the 'dd' to the system

static int driver_entry(void){

	//(1) registering a device with the system
	//register device with the system
	//	-first allocate chrdev_region
	//	-chrdev_region(dev_t*, uint fminor, uint count, char* name)
	ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if (ret < 0) {
		printk(KERN_ALERT "mayflashdriver: failed to allocate a major number");
		return ret; //propagate error
	}
	major_number = MAJOR(dev_num); //extract major number and store in our variable (MACRO)
	printk(KERN_INFO  "mayflashdevice: major number is %d",major_number);
	printk(KERN_INFO "\tuse \"mknod /dev/%s c %d 0\" for device file", DEVICE_NAME, major_number);

	//(2) create and initialize cdev structure, initialize cdev
	mcdev = cdev_alloc();
	mcdev->ops = &fops; //not defined yet! contains file_operations for the cdev structure
	mcdev->owner = THIS_MODULE;
	//adding cdev to the kernel with:
	//	int cdev_add(struct cdev* dev, dev_t num, unsigned int count)
	ret = cdev_add(mcdev, dev_num, 1)
	if(ret < 0) {
		printk(KERN_ALERT "mayflashdevice: unable to add cdev to kernel");
		return ret;
	}


	//initialize semaphore
	sema_init(&virtual_device.sem,1); //initial value of 1 for the semaphore
	return 0;
}

//deinitialization function: undos everything in reverse order
//	1 - 
static void driver_exit(void){
	cdev_del(mcdev);

	unregister_chrdev_region(dev_num,1);
	printk(KERN_ALERT "mayflashdevice: unloaded module");
}

//-----Linux Kernel Module Programmin - 08 Coding the Char Device part 2

//insert the following code on line 287, below the definition of the DEVICE_NAME

//allow one process to open the device as mutual exclusive lock
//	-uses mutex to share access to thread

ssize_t device_open(struct inode *inode, struct file *filp){
	if (down_interruptible(&virtual_device.sem) !=0){
		printk(KERN_ALERT "mayflashcode: could not lock device during open");
		return -1;
	}

	printk(KERN_INFO "mayflashdevice: opened device");
	return 0;
}

ssize_t device_read(struct fuke* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset){
	//take data from kernel transfer to user
	//	-done with copy_to_user(destination, sizeOfTransfer)
	printk(KERN_INFO "mayflashdevice: Reading from device");
	ret = copy_to_user(bufStoreData, virtual_device.data, bufCount);
	return ret;
}

ssize_t device_write(struct file* filp, const char* bufSourceData, size_t bufCount, loff_t* curOffset){
	//send data from user to kernel
	//copy_from_user (dest, source, count)

	printk(KERN_INFO "mayflashdevice: writing to device");
	ret = copy_from_user(virtual_device.data, bufSourceData, bufCount)
}

//release mutex lock
int device_close(struct inode * inode, struct file *filp){
	
	//calling up, opposite of semaphore down, to release mutex
	//allows other processes to use the device
	up(&virtual_device.sem);
	printk(KERN_INFO "mayflashdevice: closed device");
	return 0;
}

//which functions to call on user operation on a given device file
struct file_operations fops = {
	.owner = THIS_MODULE, 		// prevent unloading of module when operations in use
	.open = device_open,		// point to the method to call when opening the device
	.release = device_close,	// point to the method to call when closing the device
	.write = device_write,		//... writing
	.read = device_read			//... reading
}


/*  */
module_init(driver_entry);
module_exit(driver_exit);