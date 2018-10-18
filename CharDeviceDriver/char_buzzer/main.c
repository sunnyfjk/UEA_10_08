/**
 * @Author: fjk
 * @Date:   2018-10-18T14:18:04+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-18T17:28:10+08:00
 * @License: GPL
 */
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct user_buzzer_t {
  /*设置 读取 buzzer的状态*/
  uint8_t power;
};

int main(void) {
  int fd = 0;
  int ret = 0;
  int i = 0;
  struct user_buzzer_t con = {0};
  fd = open("/dev/mybuzzer", O_RDWR);
  if (fd < 0) {
    ret = fd;
    goto open_myled_err;
  }
  for (i = 0; i < 15; i++) {
    con.power = !con.power;
    ret = write(fd, &con, sizeof(con));
    if (ret < 0) {
      perror("write");
    }
    sleep(1);
  }

  ret = read(fd, &con, sizeof(con));
  if (ret < 0) {
    perror("read");
  }

  printf("%#x\n", con.power);
  close(fd);
  return 0;
open_myled_err:
  return ret;
}
