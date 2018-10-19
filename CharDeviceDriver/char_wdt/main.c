/**
 * @Author: fjk
 * @Date:   2018-10-18T14:18:04+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-19T14:32:09+08:00
 * @License: GPL
 */
#include <fcntl.h>
#include <sched.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WDT_ENABLE _IO('W', 0)
#define WDT_DISBALE _IO('W', 1)

int main(void) {
  int fd = 0;
  int ret = 0;
  int i = 10;
  size_t rate;
  uint16_t t;
  struct sched_param param;

  int max = sched_get_priority_max(SCHED_FIFO);
  param.sched_priority = max;
  sched_setscheduler(getpid(), SCHED_FIFO, &param);

  fd = open("/dev/mywdt", O_RDWR);
  if (fd < 0) {
    ret = fd;
    goto open_myled_err;
  }

  ret = read(fd, &rate, sizeof(rate));
  if (ret < 0) {
    perror("read");
  }

  t = rate * 3;

  ret = write(fd, &t, sizeof(t));
  ioctl(fd, WDT_ENABLE, NULL);
  while (i--) {
    ret = write(fd, &t, sizeof(t));
    sleep(2);
  }

  printf("%d\n", rate);
  close(fd);
  return 0;
open_myled_err:
  return ret;
}
