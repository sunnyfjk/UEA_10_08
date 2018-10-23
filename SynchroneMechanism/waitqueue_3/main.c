/**
 * @Author: fjk
 * @Date:   2018-10-22T16:48:15+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-22T17:17:47+08:00
 * @License: GPL
 */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  int fd = 0;
  char buf[101] = {0};
  if (argc < 3) {
    fprintf(stderr, "%s\n", "argument err");
    return -1;
  }
  if (!strcmp(argv[1], "O_NONBLOCK")) {
    fd = open("/dev/mystack_wait_queue", O_RDWR | O_NONBLOCK);
  } else {
    fd = open("/dev/mystack_wait_queue", O_RDWR);
  }
  if (fd < 0) {
    perror("open");
    return fd;
  }
  printf("argv[2]=%s\n", argv[2]);
  if (!strcmp(argv[2], "read")) {
    read(fd, buf, 100);
    printf("buf=%s\n", buf);
  } else {
    if (argc < 4) {
      fprintf(stderr, "%s\n", "argument err");
      close(fd);
      return 0;
    }
    write(fd, argv[3], strlen(argv[3]));
  }

  close(fd);

  return 0;
}
