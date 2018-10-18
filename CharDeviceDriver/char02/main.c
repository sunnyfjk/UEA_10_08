/**
 * @Author: fjk
 * @Date:   2018-10-17T14:21:53+08:00
 * @Email:  sunnyfjkgmail.com
 * @Filename: main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-17T15:28:36+08:00
 * @License: GPL
 */
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SET_STATE _IOW('R', 1, int)
#define SET_VALUE _IOW('R', 2, struct set_value_t *)
#define SET_OPEN _IO('R', 3)
#define GET_STATE _IOR('R', 4, int *)
#define GET_VALUE _IOR('R', 5, struct set_value_t *)

#define SET_GET _IOWR('R', 6, struct set_value_t *)

struct set_value_t {
  int a;
  int b;
  int c;
};

int main(void) {
  struct set_value_t v = {0};
  int fd = 0;
  int ret = 0;
  fd = open("/dev/mychar", O_RDWR);
  if (fd < 0) {
    return fd;
  }
  v.a = 10;
  v.b = 100;
  v.c = 1000;
  ret = ioctl(fd, SET_STATE, 89);
  printf("ret=%d\n", ret);

  ret = ioctl(fd, SET_VALUE, &v);
  printf("ret=%d\n", ret);

  close(fd);
  return 0;
}
