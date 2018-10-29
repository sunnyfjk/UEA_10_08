# @Author: fjk
# @Date:   2018-10-11T14:05:49+08:00
# @Email:  sunnyfjk@gmai.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-29T14:24:11+08:00
# @License: GPL
TAGET=timer01
obj-m=$(TAGET).o
KERN_PATH=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/linux-3.4
PWD=$(shell pwd)
all:
	make -C $(KERN_PATH) M=$(PWD) modules
	cp $(PWD)/$(TAGET).ko /rootfs
clean:
	make -C  $(KERN_PATH) M=$(PWD) clean
