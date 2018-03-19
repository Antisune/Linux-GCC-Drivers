
obj-m := mayflashHello.o

KDIR = /usr/src/linux-headers-3.2.0-24-generic-pae
PWD = $(shell pwd) 
all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order