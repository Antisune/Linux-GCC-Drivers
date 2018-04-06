//-----Linux Kernel Module Programming - USB Device Driver 02:

/* */

/* */

  //mayflash_driver.c:

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

MODULE_LICENSE("GPLv3");
MODULE_AUTHOR("Samuel Whitty, Eric Romano");
MODULE_DESCRIPTION("USB Mayflash Registration Driver");

// probe function
/* called on device insertion if there is no other driver set up to handle it */
/* takes struct of usb_interface, which represents the logic? of the device */
/* "usb_device_id *id" checks if the device at the given id is able to be managed, and our driver can manage it */
static int mayflash_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
  printk(KERN_INFO "[*] Mayflash Device drive (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
  return 0; //indicates that the device will be managed
}

// usb_device_id
/* defines the driver with the given major/minor numbers */
/* { USB_DEVICE(v, p) }, */ /*can be used to support multiple devices */
static struct usb_device_id mayflash_table[] = {
  // note the major/minor number here
  // 0079:1843
  // check which represents vendor id, and which represents product id
  { USB_DEVICE(0x0781, 0x5406) },
  {} /* TERMINATING ENTRY */
};

static void mayflash_disconnect(struct usb_interface *interface){
  printk(KERN_INFO "[*] Mayflash Device drive removed\n");
}

static struct usb_driver mayflash_driver = {
  .name = "Mayflash Gamecube Adaptor", // contains USB driver name
  .id_table = mayflash_table, // of type usb_device_id, linked with major/minor numbers
  .probe = mayflash_probe, //checks when device (usb) is plugged into device, not called if another driver is using it
  .disconnect = mayflash_disconnect //cleans up memory
};

//display information, register the driver
static int __init mayflash_init(void){
  int ret = -1;
  printk(KERN_INFO "[*]Constructor of Mayflash driver");
  printk(KERN_INFO "\tRegistering Driver with Kernel");
  ret = usb_register(&mayflash_driver); //registers the driver
  printk(KERN_INFO "\nRegistration is complete");
  return ret;
}

//deregister and destructor calls
static void __exit mayflash_exit(void){
  printk(KERN_INFO "[*]Destructor of Mayflash driver");
  //usb_deregister()
  usb_deregister(&mayflash_driver);
  printk(KERN_INFO "\tderegistration complete");
}

module_init(mayflash_init);
module_exit(mayflash_exit);


/* */
