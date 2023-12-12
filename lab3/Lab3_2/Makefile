obj-m += lab3-2_driver.o 
KDIR = /home/ynnad132/linux

all: 
	make -C $(KDIR) ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- M=$(shell pwd) modules 
clean: 
	make -C $(KDIR) M=$(shell pwd) clean
