/**
 * @Author: fjk
 * @Date:   2018-10-11T14:20:05+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: Kernel05_01.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-11T14:39:14+08:00
 * @License: GPL
 */
#include <linux/init.h>
#include <linux/module.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

void my_printk(void) {
  int i = 0;
  for (i = 0; i < 10; i++)
    PERR("\n");
}
EXPORT_SYMBOL(my_printk);

static __init int Kernel05_01_init(void) {
  PERR("INIT\n");
  return 0;
}

static __exit void Kernel05_01_exit(void) {
  PERR("EXIT\n");
  return;
}

module_init(Kernel05_01_init);
module_exit(Kernel05_01_exit);
