/**
 * @Author: fjk
 * @Date:   2018-10-10T14:55:07+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: myadd.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-10T15:03:01+08:00
 * @License: GPL
 */
#include <myadd.h>
#include <stdio.h>
#define _GNU_SOURCE      /* See feature_test_macros(7) */
#include <sys/syscall.h> /* For SYS_xxx definitions */
#include <unistd.h>
#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    fprintf(stderr, "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

long myadd(int a, int b) { return syscall(378, a, b); }
