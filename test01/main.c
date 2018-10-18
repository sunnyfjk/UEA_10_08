/**
 * @Author: fjk
 * @Date:   2018-10-08T12:29:11+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-10T14:57:13+08:00
 * @License: GPL
 */
#include <myadd.h>
#include <stdio.h>
#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    fprintf(stderr, "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

int main(void) {
  int ret = 0;
  ret = myadd(100, 2);
  PERR("ret=%d\n", ret);
  return 0;
}
