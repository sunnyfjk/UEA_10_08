/**
 * @Author: fjk
 * @Date:   2018-10-26T10:55:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-26T11:14:13+08:00
 * @License: GPL
 */
#include <fcntl.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    fprintf(stderr, "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

int read_input_dev(int fd) {

  int ret = 0;
  struct input_event event = {0};
  while ((ret = read(fd, &event, sizeof(event))) > 0) {
    switch (event.type) {
    case EV_SYN:
      PERR("SYN Event\n");
      break;
    case EV_KEY:
      PERR("KEY Event\n");
      if (event.value) {
        PERR("KEY down %d\n", event.code);
      } else {
        PERR("KEY up %d\n", event.code);
      }
      break;
    case EV_REL:
      PERR("REL Event\n");
      if (event.code == REL_X) {
        PERR("REL_X= %d\n", event.value);
      } else if (event.code == REL_Y) {
        PERR("REL_Y= %d\n", event.value);
      } else if (event.code == REL_Z) {
        PERR("REL_Z= %d\n", event.value);
      }
      break;
    case EV_ABS:
      PERR("ABS Event\n");
      if (event.code == ABS_X) {
        PERR("ABS_X= %d\n", event.value);
      } else if (event.code == ABS_Y) {
        PERR("ABS_Y= %d\n", event.value);
      } else if (event.code == ABS_Z) {
        PERR("ABS_Z= %d\n", event.value);
      }
      break;
    }
  }

  return 0;
}
int write_input_dev(int fd) { return 0; }
int main(int argc, char const *argv[]) {

  int fd = 0;
  /*
  argv[1] 要打开的设备
  argc[2] 读取或是写入
   */
  if (argc != 3)
    return -1;
  fd = open(argv[1], O_RDWR);

  if (!strcmp(argv[2], "read")) {
    read_input_dev(fd);

  } else if (!strcmp(argv[2], "write")) {
    write_input_dev(fd);
  }

  close(fd);

  return 0;
}
