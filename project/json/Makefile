# @Author: fjk
# @Date:   2018-10-30T09:53:16+08:00
# @Email:  sunnyfjk@gmai.com
# @Filename: Makefile
# @Last modified by:   fjk
# @Last modified time: 2018-10-30T09:57:55+08:00
# @License: GPL
TOOLCHAIN=
TAGET=json_demo
SRC=$(shell find ./ -name "*.c")
OBJ_O=${patsubst %.c,%.o,$(SRC)}
OBJ_D=${patsubst %.c,%.d,$(SRC)}
INC_H=-I$(shell pwd)/include
LIB_S=-lm
$(TAGET):$(OBJ_O)
	$(TOOLCHAIN)gcc -o $@ $^ $(LIB_S)
.c.o:
	$(TOOLCHAIN)gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<" $(INC_H)
clean:
	rm -rf $(OBJ_O) $(OBJ_D) $(TAGET)
