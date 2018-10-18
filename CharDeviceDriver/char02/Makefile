# @Author: fjk
# @Date:   2018-10-17T10:22:17+08:00
# @Email:  sunnyfjkgmail.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-17T11:45:42+08:00
# @License: GPL

TAGET=char02
obj-m=$(TAGET).o
KERN_SRC=/lib/modules/`uname -r`/build
PWD=$(shell pwd)
all:
	make -C $(KERN_SRC) M=$(PWD) modules
	cp $(PWD)/$(TAGET).ko /rootfs
clean:
	make -C $(KERN_SRC) M=$(PWD) clean
