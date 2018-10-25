/**
 * @Author: fjk
 * @Date:   2018-10-18T14:18:04+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-25T15:32:55+08:00
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

struct user_led_con_t {
  uint8_t power;
  uint8_t num;
  uint8_t status;
};

int main(void) {
  int fd = 0;
  int ret = 0;
  uint16_t con = {0};
  fd = open("/dev/adc01", O_RDWR);
  if (fd < 0) {
    ret = fd;
    goto open_myled_err;
  }
  while (1) {
    ret = read(fd, &con, sizeof(con));
    if (ret < 0) {
      perror("read");
    }
    printf("%d\n", con);
    sleep(1);
  }

  close(fd);
  return 0;
open_myled_err:
  return ret;
}
