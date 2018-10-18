# @Author: fjk
# @Date:   2018-10-17T10:22:17+08:00
# @Email:  sunnyfjkgmail.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-18T09:46:51+08:00
# @License: GPL

TAGET=char01
obj-m=$(TAGET).o
KERN_PATH=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/linux-3.4
PWD=$(shell pwd)
all:
	make -C $(KERN_PATH) M=$(PWD) modules
	cp $(PWD)/$(TAGET).ko /rootfs
clean:
	make -C  $(KERN_PATH) M=$(PWD) clean
