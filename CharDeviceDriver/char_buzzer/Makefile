# @Author: fjk
# @Date:   2018-10-11T14:05:49+08:00
# @Email:  sunnyfjk@gmai.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-18T17:17:39+08:00
# @License: GPL
TAGET=bu
obj-m=$(TAGET).o
$(TAGET)-objs=buzzer.o char_buzzer.o
KERN_PATH=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/linux-3.4
CROSS_COMPILE=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/buildroot/output/host/usr/bin/arm-cortex_a9-linux-gnueabi-
PWD=$(shell pwd)
all:
	make -C $(KERN_PATH) M=$(PWD) modules
	$(CROSS_COMPILE)gcc main.c -o main
	cp $(PWD)/$(TAGET).ko $(PWD)/main /rootfs
clean:
	make -C  $(KERN_PATH) M=$(PWD) clean
	rm -fr main
