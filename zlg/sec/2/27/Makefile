ifneq ($(KERNELRELEASE),)
	obj-m := char_misc.o
else
	PWD := $(shell pwd)
	KVER = 2.6.35.3
	KDIR := $(KERNEL_PATH)
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf .*cmd *.o *.mod.c *.ko .tmp_versions
endif