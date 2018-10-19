/**
 * @Author: fjk
 * @Date:   2018-10-19T10:35:25+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: wdt.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-19T14:16:07+08:00
 * @License: GPL
 */
#include "wdt.h"
void *ip_vir;
void __iomem *wdt_init(void) {
  unsigned int reg = 0;
  void __iomem *v = ioremap(WDT_BASE_ADDR, SZ_4K);
  if (IS_ERR_OR_NULL(v)) {
    goto ioremap_wdt_register_err;
  }

  ip_vir = ioremap(IP_RESET_REGISTER, SZ_4K);
  /*打开 WDT ip 核*/
  reg = ioread32(ip_vir + IP_RESET_REGISTER_1);
  reg |= 1 << 26;
  iowrite32(reg, ip_vir + IP_RESET_REGISTER_1);

  /*
    设置 WTCON 寄存器
    时钟第一次分频为 200
    关闭看门狗
    设置第二次分频 128
    使能中断
    使能重启信号
   */

  reg = ioread32(v + WTCON);
  reg &= ~((0xff << 8) | (3 << 3) | (1 << 2) | (1 << 0) | (1 << 5));
  reg |= (199 << 8) | (3 << 3) | (1 << 2) | (1 << 0);
  iowrite32(reg, v + WTCON);
  reg = ioread32(v + WTCON);
  return v;
ioremap_wdt_register_err:
  return NULL;
}
void wdt_exit(void __iomem *addr) {
  unsigned int reg = 0;
  wdt_set_disable(addr);
  iounmap(addr);
  reg = ioread32(ip_vir + IP_RESET_REGISTER_1);
  reg &= ~(1 << 26);
  iowrite32(reg, ip_vir + IP_RESET_REGISTER_1);
  iounmap(ip_vir);
}

void wdt_set_cnt(void __iomem *addr, uint16_t value) {
  unsigned int reg = value;
  iowrite32(reg, addr + WTCNT);
}
void wdt_set_enable(void __iomem *addr) {
  unsigned int reg = 0;
  reg = ioread32(addr + WTCON);
  reg |= 1 << 5;
  iowrite32(reg, addr + WTCON);
}
void wdt_set_disable(void __iomem *addr) {
  unsigned int reg = 0;
  reg = ioread32(addr + WTCON);
  reg &= ~(1 << 5);
  iowrite32(reg, addr + WTCON);
}

size_t wdt_get_clk(void __iomem *addr, unsigned int pclk) {
  int clock_select[4] = {16, 32, 64, 128};
  unsigned int reg = 0;
  reg = ioread32(addr + WTCON);
  return pclk / (((reg >> 8) & 0xff) + 1) / clock_select[((reg >> 3) & 3)];
}
