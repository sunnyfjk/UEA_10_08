/**
 * @Author: fjk
 * @Date:   2018-10-11T14:25:18+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: Kernel_other.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-11T14:32:03+08:00
 * @License: GPL
 */
#include <linux/init.h>
#include <linux/module.h>
#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

void my_printk(void) {
  int i = 0;
  for (i = 0; i < 10; i++)
    PERR("\n");
}
EXPORT_SYMBOL(my_printk);
