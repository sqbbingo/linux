ifneq ($(KERNELRELEASE),)
	obj-m = leds.o
else
	PWD := $(shell pwd)
#     KVER = 2.6.35.3
	KDIR := /home/bingo/work/lichee/linux
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions
endif

test:
	arm-linux-gnueabihf-gcc led_test.c -o led_test.out