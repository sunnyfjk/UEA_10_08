/**
 * @Author: fjk
 * @Date:   2018-10-18T10:57:02+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: x6818_led.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-18T14:14:24+08:00
 * @License: GPL
 */
#include "x6818_led.h"
void __iomem *x6818_led_init(void) {
  void __iomem *v = NULL;
  unsigned int reg = 0;
  v = ioremap(LED_GPIO_BASE_ADDR, SZ_64K);
  if (v == NULL)
    goto ioreamp_LED_GPIO_BASE_ADDR_err;

  /*设置 B26 的GPIO*/
  reg = ioread32(v + GPIOB_PULLENB_DISABLE_DEFAULT);
  reg |= (1 << 26);
  iowrite32(reg, v + GPIOB_PULLENB_DISABLE_DEFAULT);

  reg = ioread32(v + GPIOB_PULLENB);
  reg &= ~(1 << 26);
  iowrite32(reg, v + GPIOB_PULLENB);

  reg = ioread32(v + GPIOBOUTENB);
  reg |= (1 << 26);
  iowrite32(reg, v + GPIOBOUTENB);

  reg = ioread32(v + GPIOBOUT);
  reg |= (1 << 26);
  iowrite32(reg, v + GPIOBOUT);
  /*设置 C11 C7 C12 的GPIO*/
  reg = ioread32(v + GPIOC_PULLENB_DISABLE_DEFAULT);
  reg |= (1 << 11) | (1 << 7) | (1 << 12);
  iowrite32(reg, v + GPIOC_PULLENB_DISABLE_DEFAULT);

  reg = ioread32(v + GPIOC_PULLENB);
  reg &= ~((1 << 11) | (1 << 7) | (1 << 12));
  iowrite32(reg, v + GPIOC_PULLENB);

  reg = ioread32(v + GPIOCOUTENB);
  reg |= (1 << 11) | (1 << 7) | (1 << 12);
  iowrite32(reg, v + GPIOCOUTENB);

  reg = ioread32(v + GPIOCOUT);
  reg |= (1 << 11) | (1 << 7) | (1 << 12);
  iowrite32(reg, v + GPIOCOUT);
  return v;
ioreamp_LED_GPIO_BASE_ADDR_err:
  return NULL;
}
void x6818_led_exit(void __iomem *addr) {
  unsigned int reg = 0;
  reg = ioread32(addr + GPIOCOUT);
  reg |= (1 << 11) | (1 << 7) | (1 << 12);
  iowrite32(reg, addr + GPIOCOUT);
  reg = ioread32(addr + GPIOBOUT);
  reg |= (1 << 26);
  iowrite32(reg, addr + GPIOBOUT);
  iounmap(addr);
}

void x6818_led_open(void __iomem *addr, int num) {
  x6818_led_con(addr, num, 1);
}
void x6818_led_close(void __iomem *addr, int num) {
  x6818_led_con(addr, num, 0);
}
void x6818_led_con(void __iomem *addr, int num, int flag) {
  unsigned int reg = 0;
  switch (num) {
  case 0:
    reg = ioread32(addr + GPIOBOUT);
    if (flag) {
      reg &= ~(1 << 26);
    } else {
      reg |= (1 << 26);
    }
    iowrite32(reg, addr + GPIOBOUT);
    break;
  case 1:
    reg = ioread32(addr + GPIOCOUT);
    if (flag) {
      reg &= ~((1 << 11));
    } else {
      reg |= (1 << 11);
    }
    iowrite32(reg, addr + GPIOCOUT);
    break;
  case 2:
    reg = ioread32(addr + GPIOCOUT);
    if (flag) {
      reg &= ~((1 << 7));
    } else {
      reg |= (1 << 7);
    }
    iowrite32(reg, addr + GPIOCOUT);
    break;
  case 3:
    reg = ioread32(addr + GPIOCOUT);
    if (flag) {
      reg &= ~((1 << 12));
    } else {
      reg |= (1 << 12);
    }
    iowrite32(reg, addr + GPIOCOUT);
    break;
  }
}
void x6818_led_status(void __iomem *addr, uint8_t *status) {
  unsigned int reg = 0;
  (*status) = 0;
  reg = ioread32(addr + GPIOBOUT);
  if (!(reg & (1 << 26)))
    (*status) |= (1 << 0);

  reg = ioread32(addr + GPIOCOUT);
  if (!(reg & (1 << 11)))
    (*status) |= (1 << 1);

  if (!(reg & (1 << 7)))
    (*status) |= (1 << 2);

  if (!(reg & (1 << 12)))
    (*status) |= (1 << 3);
}
