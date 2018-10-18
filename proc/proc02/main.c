/**
 * @Author: fjk
 * @Date:   2018-10-12T14:23:00+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-12T14:27:40+08:00
 * @License: GPL
 */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

  char buf[100] = {0};
  int fd = 0;
  int ret = 0, pos = 0;
  fd = open("/proc/mydir/onlyRead", O_RDONLY);
  if (fd < 0) {
    return fd;
  }
  while ((ret = read(fd, buf + pos, 1)) > 0) {
    pos += ret;
  }
  close(fd);
  printf("buf=%s\n", buf);
}
