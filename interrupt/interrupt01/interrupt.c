/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: interrupt.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-19T15:41:44+08:00
 * @License: GPL
 */
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <mach/platform.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

irqreturn_t key_irq_handler(int irq, void *dev) {

  PERR("irq=%d\n", irq);

  /*执行成功*/
  return IRQ_HANDLED;
  /*
   执行失败返回
   return IRQ_NONE
   */
}
static __init int interrupt_init(void) {
  int ret = 0;
  PERR("INIT\n");
  /*
  GPIOA_28
  GPIOB_30
  GPIOB_31
  GPIOB_9
   */
  ret = request_irq(gpio_to_irq(PAD_GPIO_A + 28), key_irq_handler,
                    IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "key_1", NULL);
  if (ret < 0) {
    goto key_1_request_irq_err;
  }
  ret = request_irq(IRQ_GPIO_B_START + 30, key_irq_handler,
                    IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "key_2", NULL);
  if (ret < 0) {
    goto key_2_request_irq_err;
  }
  ret = request_irq(IRQ_GPIO_B_START + 31, key_irq_handler,
                    IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "key_3", NULL);
  if (ret < 0) {
    goto key_3_request_irq_err;
  }
  ret = request_irq(IRQ_GPIO_B_START + 9, key_irq_handler,
                    IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "key_4", NULL);
  if (ret < 0) {
    goto key_4_request_irq_err;
  }

  return 0;
key_4_request_irq_err:
  free_irq(IRQ_GPIO_B_START + 31, NULL);
key_3_request_irq_err:
  free_irq(IRQ_GPIO_B_START + 30, NULL);
key_2_request_irq_err:
  free_irq(gpio_to_irq(PAD_GPIO_A + 28), NULL);
key_1_request_irq_err:
  return ret;
}

static __exit void interrupt_exit(void) {
  PERR("EXIT\n");
  free_irq(IRQ_GPIO_B_START + 9, NULL);
  free_irq(IRQ_GPIO_B_START + 31, NULL);
  free_irq(IRQ_GPIO_B_START + 30, NULL);
  free_irq(gpio_to_irq(PAD_GPIO_A + 28), NULL);
  return;
}

module_init(interrupt_init);
module_exit(interrupt_exit);
