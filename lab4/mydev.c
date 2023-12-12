#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/string.h>

/*******************/
static ssize_t my_read(struct file *fp, char *buf, size_t count, loff_t *fpos);
static ssize_t my_write(struct file *fp,const char *buf, size_t count, loff_t *fpos);
static int my_open(struct inode *inode, struct file *fp);
static int my_init(void);
static void my_exit(void);
static void convert(char ch,int index);
struct file_operations my_fops = { 
	read: my_read, 
	write: my_write, 
	open: my_open 
}; 

char my_name [5][17]={};   //dANNY





char seg_for_c[27][17] = {
    "1111001100010001", // A
    "0000011100000101", // b
    "1100111100000000", // C
    "0000011001000101", // d
    "1000011100000001", // E
    "1000001100000001", // F
    "1001111100010000", // G
    "0011001100010001", // H
    "1100110001000100", // I
    "1100010001000100", // J
    "0000000001101100", // K
    "0000111100000000", // L
    "0011001110100000", // M
    "0011001110001000", // N
    "1111111100000000", // O
    "1000001101000001", // P
    "0111000001010000", //q
    "1110001100011001", //R
    "1101110100010001", //S
    "1100000001000100", //T
    "0011111100000000", //U
    "0000001100100010", //V
    "0011001100001010", //W
    "0000000010101010", //X
    "0000000010100100", //Y
    "1100110000100010", //Z
    "0000000000000000"
};


/*******************/



// File Operations
static ssize_t my_read(struct file *fp, char *buf, size_t count, loff_t *fpos) {
	static int k = 0;
	printk("call read\n");
	//count=16;
	if( copy_to_user(buf, &my_name[k], count) > 0) {
		pr_err("ERROR: Not all the bytes have been copied to user\n");
 	}
	if(k>=4){
		k=0;
	}
	else{
		k++;
	}
	
	return count;
}

static ssize_t my_write(struct file *fp,const char *buf, size_t count, loff_t *fpos){
	uint8_t rec_buf[10] = {0};
	printk("call write\n"); 
 	if( copy_from_user( rec_buf, buf, count ) > 0) {
		pr_err("ERROR: Not all the bytes have been copied from user\n");
	}
	
	for(int i=0;i<count;i++){
		convert(rec_buf[i],i);
	}
	
	return count; 
} 

static int my_open(struct inode *inode, struct file *fp){ 
	printk("call open\n"); 
	return 0; 
} 

static void convert(char ch,int index){
	int i;
	if((ch-'a') <0){
		i=ch-'A';
	}
	else{
		i=ch-'a';
	}
	
	strncpy(my_name[index],seg_for_c[i],17);		
}

#define MAJOR_NUM 237 
#define DEVICE_NAME "my_dev"
 
static int my_init(void) { 
	printk("call init\n");
	if(register_chrdev(MAJOR_NUM, DEVICE_NAME, &my_fops) < 0){ 
		printk("Can not get major %d\n", MAJOR_NUM); 
		return (-EBUSY); 
	} 
	printk("My device is started and the major is %d\n", MAJOR_NUM); return 0; 
} 

static void my_exit(void){ 
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME); 
	printk("call exit\n"); 	
} 
		
module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("1.32");

