/**
 * @Author: fjk
 * @Date:   2018-10-11T14:25:03+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: Kernel_main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-11T14:31:46+08:00
 * @License: GPL
 */
#include <linux/init.h>
#include <linux/module.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

extern void my_printk(void);

static __init int Kernel03_init(void) {
  PERR("INIT\n");
  my_printk();
  return 0;
}

static __exit void Kernel03_exit(void) {
  PERR("EXIT\n");
  return;
}

module_init(Kernel03_init);
module_exit(Kernel03_exit);
