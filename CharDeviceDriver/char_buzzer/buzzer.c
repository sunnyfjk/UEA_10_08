/**
 * @Author: fjk
 * @Date:   2018-10-18T17:04:43+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: buzzer.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-18T17:18:44+08:00
 * @License: GPL
 */
#include "buzzer.h"

void __iomem *buzzer_init(void) {
  unsigned long reg;
  void __iomem *v = ioremap(GPIO_C_BASE_ADDR, SZ_4K);
  if (IS_ERR_OR_NULL(v)) {
    goto ioremap_gpio_c_err;
  }

  reg = ioread32(v + GPIOC_PULLENB_DISABLE_DEFAULT);
  reg |= (1 << 14);
  iowrite32(reg, v + GPIOC_PULLENB_DISABLE_DEFAULT);

  reg = ioread32(v + GPIOC_PULLENB);
  reg &= ~((1 << 14));
  iowrite32(reg, v + GPIOC_PULLENB);

  reg = ioread32(v + GPIOCOUTENB);
  reg |= (1 << 14);
  iowrite32(reg, v + GPIOCOUTENB);

  reg = ioread32(v + GPIOCOUT);
  reg &= ~(1 << 14);
  iowrite32(reg, v + GPIOCOUT);

  return v;
ioremap_gpio_c_err:
  return NULL;
}
void buzzer_exit(void __iomem *addr) {
  unsigned long reg = 0;
  reg = ioread32(addr + GPIOCOUT);
  reg &= ~(1 << 14);
  iowrite32(reg, addr + GPIOCOUT);
  iounmap(addr);
}

void buzzer_open(void __iomem *addr) {
  unsigned long reg = 0;
  reg = ioread32(addr + GPIOCOUT);
  reg |= (1 << 14);
  iowrite32(reg, addr + GPIOCOUT);
}

void buzzer_close(void __iomem *addr) {
  unsigned long reg = 0;
  reg = ioread32(addr + GPIOCOUT);
  reg &= ~(1 << 14);
  iowrite32(reg, addr + GPIOCOUT);
}

uint8_t buzzer_is_open(void __iomem *addr) {
  unsigned long reg = 0;
  reg = ioread32(addr + GPIOCOUT);
  return ((reg >> 14) & 1);
}
