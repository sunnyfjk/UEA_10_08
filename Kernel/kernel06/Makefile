# @Author: fjk
# @Date:   2018-10-11T14:05:49+08:00
# @Email:  sunnyfjk@gmai.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-29T15:41:11+08:00
# @License: GPL
TAGET=Kernel_GPL
TAGET1=Kernel_no_GPL
TAGET2=Kernel_Proprietary
obj-m=$(TAGET).o $(TAGET1).o $(TAGET2).o
KERN_PATH=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/linux-3.4
PWD=$(shell pwd)
all:
	make -C $(KERN_PATH) M=$(PWD) modules
	cp $(PWD)/*.ko /rootfs
clean:
	make -C  $(KERN_PATH) M=$(PWD) clean
