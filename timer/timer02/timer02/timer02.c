/**
 * @Author: fjk
 * @Date:   2018-10-29T14:34:45+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: timer02.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-29T15:06:22+08:00
 * @License: GPL
 */

#include <linux/hrtimer.h>
#include <linux/init.h>
#include <linux/module.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

struct hrtimer hrtimer;

enum hrtimer_restart hrtimer_function(struct hrtimer *hr) {
  PERR("\n");
  hrtimer_forward_now(hr, ktime_set(3, 0));
  return HRTIMER_RESTART;
}

static __init int timer02_init(void) {
  PERR("INIT\n");
  struct timespec ts;
  getnstimeofday(&ts);
  hrtimer_init(&hrtimer, CLOCK_REALTIME, HRTIMER_MODE_ABS);
  hrtimer.function = hrtimer_function;
  hrtimer_start(&hrtimer, ktime_set(ts.tv_sec + 3, 0), HRTIMER_MODE_ABS);
  return 0;
}

static __exit void timer02_exit(void) {
  PERR("EXIT\n");
  hrtimer_cancel(&hrtimer);
  return;
}

module_init(timer02_init);
module_exit(timer02_exit);
