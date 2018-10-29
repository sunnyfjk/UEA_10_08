/**
 * @Author: fjk
 * @Date:   2018-10-29T14:17:48+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: timer01.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-29T14:30:30+08:00
 * @License: GPL
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

struct timer_list timer;
unsigned long data = 100;
void timer_fun(unsigned long data) {
  PERR("data=%ld\n", data);

  timer.expires = jiffies + 3 * HZ;
  add_timer(&timer);
}

static __init int timer01_init(void) {
  PERR("INIT\n");

  setup_timer(&timer, timer_fun, data);
  timer.expires = jiffies + 1 * HZ;
  add_timer(&timer);
  return 0;
}

static __exit void timer01_exit(void) {
  PERR("EXIT\n");
  del_timer(&timer);
  return;
}

module_init(timer01_init);
module_exit(timer01_exit);
