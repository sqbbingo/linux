#Makefile2.6
ifneq ($(KERNELRELEASE),)
#kbuild syntax.dependency relationship of files and target modules are listed here.
#gpiodrv-objs := gpiodrv.c
obj-m := char_cdev.o 
else
PWD := $(shell pwd)
KVER = 2.6.35.3
KDIR := $(KERNEL_PATH)
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions
endif