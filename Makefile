obj-m += mayflashModule.o
KERNEL_DIR = /lib/modules/4.15.14-1-ARCH/build
PWD = $(shell pwd)
all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
#all:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	rm -rf mayflashModule.o

