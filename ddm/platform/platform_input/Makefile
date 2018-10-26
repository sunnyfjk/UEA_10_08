# @Author: fjk
# @Date:   2018-10-11T14:05:49+08:00
# @Email:  sunnyfjk@gmai.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-26T11:59:40+08:00
# @License: GPL
DEVICE=platform_input_device
DRIVER=platform_input_key_driver
obj-m=$(DEVICE).o $(DRIVER).o
$(DRIVER)-objs=platform_input_driver.o key.o
KERN_PATH=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/linux-3.4
CROSS_COMPILE=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/buildroot/output/host/usr/bin/arm-cortex_a9-linux-gnueabi-
PWD=$(shell pwd)
all:
	make -C $(KERN_PATH) M=$(PWD) modules
	cp $(PWD)/$(DEVICE).ko /rootfs
	cp $(PWD)/$(DRIVER).ko /rootfs
clean:
	make -C  $(KERN_PATH) M=$(PWD) clean
app:
	$(CROSS_COMPILE)gcc main.c -o main
	cp $(PWD)/main /rootfs
app-clean:
	rm -fr main
