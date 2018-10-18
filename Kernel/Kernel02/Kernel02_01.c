/**
 * @Author: fjk
 * @Date:   2018-10-11T14:20:05+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: Kernel02_01.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-11T14:21:28+08:00
 * @License: GPL
 */
#include <linux/init.h>
#include <linux/module.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

static __init int Kernel02_01_init(void) {
  PERR("INIT\n");
  return 0;
}

static __exit void Kernel02_01_exit(void) {
  PERR("EXIT\n");
  return;
}

module_init(Kernel02_01_init);
module_exit(Kernel02_01_exit);
