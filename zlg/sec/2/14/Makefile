#Makefile2.6
ifneq ($(KERNELRELEASE),)
	obj-m := char_null_udev.o
else
	PWD := $(shell pwd)
	KVER = 2.6.35.3
	KDIR := $(KERNEL_PATH)
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions
endif
