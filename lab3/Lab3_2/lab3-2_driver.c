/***************************************************************************/
/*
**
* \file led_driver.c
* \details Simple GPIO driver explanation
* \author EmbeTronicX
* \Tested with Linux raspberrypi 5.4.51-v7l+
******************************************************************************
*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h> //copy_to/from_user()
#include <linux/gpio.h> //GPIO
//LED is connected to this GPIO
#define GPIO_21 (21)    //a
#define GPIO_22 (22)    //b 
#define GPIO_23 (23)    //c
#define GPIO_24 (24)    //d
#define GPIO_25 (25)	//e	
#define GPIO_26 (26)	//f
#define GPIO_27 (27)	//g

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
/*************** Driver functions **********************/
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static void seven_segment(char in);
/******************************************************/
//File operation structure 
static struct file_operations fops =
{
 .owner = THIS_MODULE,
 .read = etx_read,
 .write = etx_write,
 .open = etx_open,
 .release = etx_release,
};
/*
** This function will be called when we open the Device file
*/ 
static int etx_open(struct inode *inode, struct file *file)
{
 	pr_info("Device File Opened...!!!\n");
	return 0;
}
/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
 	pr_info("Device File Closed...!!!\n");
 	return 0;
}
/*
** This function will be called when we read the Device file
*/ 
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
 	uint8_t gpio_state_21 = 0;
 	uint8_t gpio_state_22 = 0;
 	uint8_t gpio_state_23 = 0;
 	uint8_t gpio_state_24 = 0;
 	uint8_t gpio_state_25 = 0;
 	uint8_t gpio_state_26 = 0;
 	uint8_t gpio_state_27 = 0;
 	//reading GPIO value
 	gpio_state_21 = gpio_get_value(GPIO_21);
 	gpio_state_22 = gpio_get_value(GPIO_22);
 	gpio_state_23 = gpio_get_value(GPIO_23);
 	gpio_state_24 = gpio_get_value(GPIO_24);
 	gpio_state_25 = gpio_get_value(GPIO_25);
 	gpio_state_26 = gpio_get_value(GPIO_26);
 	gpio_state_27 = gpio_get_value(GPIO_27);
 	//write to user
 	len = 1;
 	if( copy_to_user(buf, &gpio_state_21, len) > 0||copy_to_user(buf, &gpio_state_22, len) > 0||copy_to_user(buf, &gpio_state_23, len) > 0||copy_to_user(buf, &gpio_state_24, len) > 0||copy_to_user(buf, &gpio_state_25, len) > 0||copy_to_user(buf, &gpio_state_26, len) > 0||copy_to_user(buf, &gpio_state_27, len) > 0) {
		pr_err("ERROR: Not all the bytes have been copied to user\n");
 	}
 	pr_info("Read function : GPIO_21 = %d \n", gpio_state_21);
 	pr_info("Read function : GPIO_22 = %d \n", gpio_state_22);
 	pr_info("Read function : GPIO_23 = %d \n", gpio_state_23);
 	pr_info("Read function : GPIO_24 = %d \n", gpio_state_24);
 	pr_info("Read function : GPIO_25 = %d \n", gpio_state_25);
 	pr_info("Read function : GPIO_26 = %d \n", gpio_state_26);
 	pr_info("Read function : GPIO_27 = %d \n", gpio_state_27);
 	return 0;
}
/*
** This function will be called when we write the Device file
*/ 
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
 	uint8_t rec_buf[10] = {0};
 	if( copy_from_user( rec_buf, buf, len ) > 0) {
		pr_err("ERROR: Not all the bytes have been copied from user\n");
	}
 	
 	seven_segment(rec_buf[0]);
 	mdelay(1000);
 	seven_segment(rec_buf[1]);
 	mdelay(1000);
 	seven_segment(rec_buf[2]);
 	mdelay(1000);
 	seven_segment(rec_buf[3]);
 	mdelay(1000);
 	seven_segment(rec_buf[4]);
 	mdelay(1000);
 	seven_segment(rec_buf[5]);
 	mdelay(1000);
 	seven_segment(rec_buf[6]);
 	mdelay(1000);
 	seven_segment(rec_buf[7]);
 	mdelay(1000);
 	seven_segment(rec_buf[8]);
 	mdelay(1000);
	seven_segment('a');
 	return len;
}

/* control LED*/
static void seven_segment(char in)
{
	if(in=='0'){
		gpio_set_value(GPIO_21, 1);
		gpio_set_value(GPIO_22, 1);
		gpio_set_value(GPIO_23, 1);
		gpio_set_value(GPIO_24, 1);
		gpio_set_value(GPIO_25, 1);
		gpio_set_value(GPIO_26, 1);
		gpio_set_value(GPIO_27, 0);
	}
	else if(in=='1'){
		gpio_set_value(GPIO_21, 0);
		gpio_set_value(GPIO_22, 1);
		gpio_set_value(GPIO_23, 1);
		gpio_set_value(GPIO_24, 0);
		gpio_set_value(GPIO_25, 0);
		gpio_set_value(GPIO_26, 0);
		gpio_set_value(GPIO_27, 0);
	}
	else if(in=='2'){
		gpio_set_value(GPIO_21, 1);
		gpio_set_value(GPIO_22, 1);
		gpio_set_value(GPIO_23, 0);
		gpio_set_value(GPIO_24, 1);
		gpio_set_value(GPIO_25, 1);
		gpio_set_value(GPIO_26, 0);
		gpio_set_value(GPIO_27, 1);
	}
	else if(in=='3'){
		gpio_set_value(GPIO_21, 1);
		gpio_set_value(GPIO_22, 1);
		gpio_set_value(GPIO_23, 1);
		gpio_set_value(GPIO_24, 1);
		gpio_set_value(GPIO_25, 0);
		gpio_set_value(GPIO_26, 0);
		gpio_set_value(GPIO_27, 1);
	}
	else if(in=='4'){
		gpio_set_value(GPIO_21, 0);
		gpio_set_value(GPIO_22, 1);
		gpio_set_value(GPIO_23, 1);
		gpio_set_value(GPIO_24, 0);
		gpio_set_value(GPIO_25, 0);
		gpio_set_value(GPIO_26, 1);
		gpio_set_value(GPIO_27, 1);
	}
	else if(in=='5'){
		gpio_set_value(GPIO_21, 1);
		gpio_set_value(GPIO_22, 0);
		gpio_set_value(GPIO_23, 1);
		gpio_set_value(GPIO_24, 1);
		gpio_set_value(GPIO_25, 0);
		gpio_set_value(GPIO_26, 1);
		gpio_set_value(GPIO_27, 1);
	}
	else if(in=='6'){
		gpio_set_value(GPIO_21, 1);
		gpio_set_value(GPIO_22, 0);
		gpio_set_value(GPIO_23, 1);
		gpio_set_value(GPIO_24, 1);
		gpio_set_value(GPIO_25, 1);
		gpio_set_value(GPIO_26, 1);
		gpio_set_value(GPIO_27, 1);
	}
	else if(in=='7'){
		gpio_set_value(GPIO_21, 1);
		gpio_set_value(GPIO_22, 1);
		gpio_set_value(GPIO_23, 1);
		gpio_set_value(GPIO_24, 0);
		gpio_set_value(GPIO_25, 0);
		gpio_set_value(GPIO_26, 0);
		gpio_set_value(GPIO_27, 0);
	}
	else if(in=='8'){
		gpio_set_value(GPIO_21, 1);
		gpio_set_value(GPIO_22, 1);
		gpio_set_value(GPIO_23, 1);
		gpio_set_value(GPIO_24, 1);
		gpio_set_value(GPIO_25, 1);
		gpio_set_value(GPIO_26, 1);
		gpio_set_value(GPIO_27, 1);
	}
	else if(in=='9'){
		gpio_set_value(GPIO_21, 1);
		gpio_set_value(GPIO_22, 1);
		gpio_set_value(GPIO_23, 1);
		gpio_set_value(GPIO_24, 0);
		gpio_set_value(GPIO_25, 0);
		gpio_set_value(GPIO_26, 1);
		gpio_set_value(GPIO_27, 1);
	}
	else{
		gpio_set_value(GPIO_21, 0);
		gpio_set_value(GPIO_22, 0);
		gpio_set_value(GPIO_23, 0);
		gpio_set_value(GPIO_24, 0);
		gpio_set_value(GPIO_25, 0);
		gpio_set_value(GPIO_26, 0);
		gpio_set_value(GPIO_27, 0);
	}	
}



/*
** Module Init function
*/ 
static int __init etx_driver_init(void)
{
 /*Allocating Major number*/
	if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
		pr_err("Cannot allocate major number\n");
		goto r_unreg;
	}
 	pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 	/*Creating cdev structure*/
 	cdev_init(&etx_cdev,&fops);
 	/*Adding character device to the system*/
 	if((cdev_add(&etx_cdev,dev,1)) < 0){
		pr_err("Cannot add the device to the system\n");
		goto r_del;
 	}
 /*Creating struct class*/
 	if((dev_class = class_create(THIS_MODULE,"etx_class")) == NULL){
		pr_err("Cannot create the struct class\n");
		goto r_class;
 	}
 	/*Creating device*/
 	if((device_create(dev_class,NULL,dev,NULL,"etx_device")) == NULL){
		pr_err( "Cannot create the Device \n");
		goto r_device;
 	}
 	//Checking the GPIO is valid or not
 	if(gpio_is_valid(GPIO_21) == false||gpio_is_valid(GPIO_22) == false||gpio_is_valid(GPIO_23) == false||gpio_is_valid(GPIO_24) == false||gpio_is_valid(GPIO_25) == false||gpio_is_valid(GPIO_26) == false||gpio_is_valid(GPIO_27) == false){
		pr_err("GPIO is not valid\n");
		goto r_device;
 	}
 	//Requesting the GPIO
 	if(gpio_request(GPIO_21,"GPIO_21") < 0||gpio_request(GPIO_22,"GPIO_22") < 0||gpio_request(GPIO_23,"GPIO_23") < 0||gpio_request(GPIO_24,"GPIO_24") < 0||gpio_request(GPIO_25,"GPIO_25") < 0||gpio_request(GPIO_26,"GPIO_26") < 0||gpio_request(GPIO_27,"GPIO_27") < 0){
		pr_err("ERROR: GPIO request\n");
		goto r_gpio;
 	}
 //configure the GPIO as output
 gpio_direction_output(GPIO_21, 0);
 gpio_direction_output(GPIO_22, 0);
 gpio_direction_output(GPIO_23, 0);
 gpio_direction_output(GPIO_24, 0);
 gpio_direction_output(GPIO_25, 0);
 gpio_direction_output(GPIO_26, 0);
 gpio_direction_output(GPIO_27, 0); 
 /* Using this call the GPIO 21 will be visible in /sys/class/gpio/
 ** Now you can change the gpio values by using below commands also.
 ** echo 1 > /sys/class/gpio/gpio21/value (turn ON the LED)
 ** echo 0 > /sys/class/gpio/gpio21/value (turn OFF the LED)
 ** cat /sys/class/gpio/gpio21/value (read the value LED)
 ** 
 ** the second argument prevents the direction from being changed.
 */
 	gpio_export(GPIO_21, false);
 	gpio_export(GPIO_22, false);
 	gpio_export(GPIO_23, false);
 	gpio_export(GPIO_24, false);
 	gpio_export(GPIO_25, false);
 	gpio_export(GPIO_26, false);
 	gpio_export(GPIO_27, false);
 	pr_info("Device Driver Insert...Done!!!\n");
 	return 0;
r_gpio:
 gpio_free(GPIO_21);
 gpio_free(GPIO_22);
 gpio_free(GPIO_23);
 gpio_free(GPIO_24);
 gpio_free(GPIO_25);
 gpio_free(GPIO_26);
 gpio_free(GPIO_27);
r_device:
 device_destroy(dev_class,dev);
r_class:
 class_destroy(dev_class);
r_del:
 cdev_del(&etx_cdev);
r_unreg:
 unregister_chrdev_region(dev,1);
 return -1;
}
/*
** Module exit function
*/ 
static void __exit etx_driver_exit(void)
{
 gpio_unexport(GPIO_21);
 gpio_unexport(GPIO_22);
 gpio_unexport(GPIO_23);
 gpio_unexport(GPIO_24);
 gpio_unexport(GPIO_25);
 gpio_unexport(GPIO_26);
 gpio_unexport(GPIO_27);
 gpio_free(GPIO_21);
 gpio_free(GPIO_22);
 gpio_free(GPIO_23);
 gpio_free(GPIO_24);
 gpio_free(GPIO_25);
 gpio_free(GPIO_26);
 gpio_free(GPIO_27);
 device_destroy(dev_class,dev);
 class_destroy(dev_class);
 cdev_del(&etx_cdev);
 unregister_chrdev_region(dev, 1);
 pr_info("Device Driver Remove...Done!!\n");
}
module_init(etx_driver_init);
module_exit(etx_driver_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <danny19990721@gmail.com>");
MODULE_DESCRIPTION("A simple device driver for LED - GPIO Driver");
MODULE_VERSION("1.32");

