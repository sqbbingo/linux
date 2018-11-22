#!/bin/sh
cd ..
rmmod led_drv
insmod led_drv.ko
cd test/
./led_test
