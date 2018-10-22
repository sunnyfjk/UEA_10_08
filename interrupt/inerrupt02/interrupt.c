/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: interrupt.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-22T11:31:37+08:00
 * @License: GPL
 */
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <mach/platform.h>

struct key_irq_t {
  int irq_num[4];
  struct tasklet_struct task;
  struct tasklet_struct hi_task;
  char *irq_name[4];
} key_irq;

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

irqreturn_t key_irq_handler(int irq, void *dev) {

  struct key_irq_t *d = dev;
  PERR("irq=%d\n", irq);
  tasklet_schedule(&d->task);
  tasklet_hi_schedule(&d->hi_task);

  if (in_interrupt()) {
    PERR("in_interrupt\n");
  } else {
    PERR("no_interrupt\n");
  }
  /*执行成功*/
  return IRQ_HANDLED;
  /*
   执行失败返回
   return IRQ_NONE
   */
}

void key_task_func(unsigned long data) {

  if (in_interrupt()) {
    PERR("in_interrupt\n");
  } else {
    PERR("no_interrupt\n");
  }
  PERR("data=%p\n", (void *)data);
}
void key_hi_task_func(unsigned long data) {

  if (in_interrupt()) {
    PERR("in_interrupt\n");
  } else {
    PERR("no_interrupt\n");
  }
  PERR("data=%p\n", (void *)data);
}

static __init int interrupt_init(void) {
  int ret = 0;
  int i = 0;
  PERR("INIT\n");

  key_irq.irq_num[0] = IRQ_GPIO_A_START + 28;
  key_irq.irq_num[1] = IRQ_GPIO_B_START + 30;
  key_irq.irq_num[2] = IRQ_GPIO_B_START + 31;
  key_irq.irq_num[3] = IRQ_GPIO_B_START + 9;
  key_irq.irq_name[0] = "key_1";
  key_irq.irq_name[1] = "key_2";
  key_irq.irq_name[2] = "key_3";
  key_irq.irq_name[3] = "key_4";

  tasklet_init(&key_irq.task, key_task_func, (unsigned long)&key_irq);
  tasklet_init(&key_irq.hi_task, key_hi_task_func, (unsigned long)&key_irq);

  for (i = 0; i < 4; i++) {
    ret = request_irq(key_irq.irq_num[i], key_irq_handler,
                      IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                      key_irq.irq_name[i], &key_irq);
    if (ret) {
      goto request_irq_err;
    }
  }

  if (in_interrupt()) {
    PERR("in_interrupt\n");
  } else {
    PERR("no_interrupt\n");
  }

  return 0;
request_irq_err:
  while (i--) {
    free_irq(key_irq.irq_num[i], &key_irq);
  }
  return ret;
}

static __exit void interrupt_exit(void) {

  int i = 4;
  while (i--) {
    free_irq(key_irq.irq_num[i], &key_irq);
  }
  PERR("EXIT\n");
  return;
}

module_init(interrupt_init);
module_exit(interrupt_exit);
