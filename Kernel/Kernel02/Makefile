# @Author: fjk
# @Date:   2018-10-11T14:05:49+08:00
# @Email:  sunnyfjk@gmai.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-11T14:22:45+08:00
# @License: GPL
TAGET=Kernel02
TAGET01=Kernel02_01
obj-m=$(TAGET).o $(TAGET01).o
KERN_PATH=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/linux-3.4
PWD=$(shell pwd)
all:
	make -C $(KERN_PATH) M=$(PWD) modules
	cp $(PWD)/*.ko /rootfs
clean:
	make -C  $(KERN_PATH) M=$(PWD) clean
