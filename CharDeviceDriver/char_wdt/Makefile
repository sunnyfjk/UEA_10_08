# @Author: fjk
# @Date:   2018-10-11T14:05:49+08:00
# @Email:  sunnyfjk@gmai.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-19T14:31:49+08:00
# @License: GPL
TAGET=mywdt
obj-m=$(TAGET).o
$(TAGET)-objs=wdt.o char_wdt.o
KERN_PATH=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/linux-3.4
CROSS_COMPILE=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/buildroot/output/host/usr/bin/arm-cortex_a9-linux-gnueabi-
PWD=$(shell pwd)
all:
	make -C $(KERN_PATH) M=$(PWD) modules
	cp $(PWD)/$(TAGET).ko  /rootfs
clean:
	make -C  $(KERN_PATH) M=$(PWD) clean
main:
	$(CROSS_COMPILE)gcc main.c -o main
	cp $(PWD)/main /rootfs
main_clean:
	rm -fr main
