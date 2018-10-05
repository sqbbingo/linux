#Makefile2.6
	ifneq($(KERNELRELEASE),)
#kbuild syntax.dependency relationship of files and target modules are listed here.
	obj-m:=hello.o
else
PWD:= $(shell pwd)
KVER = 2.6.35.3
KDIR := /opt/linux-2.6.35.3
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.cmd *.mod.c *.ko *.tmp_versions
endif