ifneq ($(KERNELRELEASE),)
	obj-m = mx28_gpio.o
else
	PWD := $(shell pwd)
	KVER = 2.6.35.3
	KDIR := $(KERNEL_PATH)
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions
endif
