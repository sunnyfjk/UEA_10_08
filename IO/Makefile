# @Author: fjk
# @Date:   2018-10-11T14:05:49+08:00
# @Email:  sunnyfjk@gmai.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-23T14:39:50+08:00
# @License: GPL
TAGET=waitqueue
obj-m=$(TAGET).o
KERN_PATH=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/linux-3.4
PWD=$(shell pwd)
CROSS_COMPILE=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/buildroot/output/host/usr/bin/arm-cortex_a9-linux-gnueabi-
all:
	make -C $(KERN_PATH) M=$(PWD) modules
	cp $(PWD)/$(TAGET).ko /rootfs
clean:
	make -C  $(KERN_PATH) M=$(PWD) clean
app:
	$(CROSS_COMPILE)gcc poll_read.c -o poll_read
	cp $(PWD)/poll_read /rootfs
	$(CROSS_COMPILE)gcc poll_write.c -o poll_write
	cp $(PWD)/poll_write /rootfs
app_clean:
	rm -fr poll_read poll_write
