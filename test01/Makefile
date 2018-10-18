# @Author: fjk
# @Date:   2018-10-08T12:31:57+08:00
# @Email:  sunnyfjk@gmai.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-10T16:36:46+08:00
# @License: GPL
CROSS_COMPILE=/opt/UEA_10_08/s5p6818sdk_lzy1/Linux/src/buildroot/output/host/usr/bin/arm-cortex_a9-linux-gnueabi-
all:
	$(CROSS_COMPILE)gcc main.c -o main -I./ -L./ -lmyadd
libs:
	$(CROSS_COMPILE)gcc -c myadd.c -o myadd.o -I ./
	$(CROSS_COMPILE)ar rcs -o libmyadd.a myadd.o
	$(CROSS_COMPILE)gcc -c -fPIC -shared myadd.c -o libmyadd.so -I./
clean:
	rm -fr main myadd.o libmyadd.a libmyadd.so
CP:
	cp libmyadd.so main /rootfs
