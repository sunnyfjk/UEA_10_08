# @Author: fjk
# @Date:   2018-10-11T14:05:49+08:00
# @Email:  sunnyfjk@gmai.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-24T11:43:48+08:00
# @License: GPL
DEVICE=mydevice
DRIVER=mydriver
BUSTYPE=mybustype
obj-m=$(DEVICE).o $(DRIVER).o $(BUSTYPE).o
KERN_PATH=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/linux-3.4
PWD=$(shell pwd)
all:
	make -C $(KERN_PATH) M=$(PWD) modules
	cp $(PWD)/$(DEVICE).ko /rootfs
	cp $(PWD)/$(DRIVER).ko /rootfs
	cp $(PWD)/$(BUSTYPE).ko /rootfs
clean:
	make -C  $(KERN_PATH) M=$(PWD) clean
