/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: Kernel01.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-29T15:43:32+08:00
 * @License: GPL
 */
#include <linux/init.h>
#include <linux/module.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

void print_gpl(int num);

void my_print(int num) { print_gpl(num); }
EXPORT_SYMBOL(my_print);

static __init int Kernel01_init(void) {
  PERR("INIT\n");
  return 0;
}

static __exit void Kernel01_exit(void) {
  PERR("EXIT\n");
  return;
}

module_init(Kernel01_init);
module_exit(Kernel01_exit);
