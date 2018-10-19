/**
 * @Author: fjk
 * @Date:   2018-10-19T10:35:17+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: wdt.h
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-19T14:20:01+08:00
 * @License: GPL
 */
#ifndef __MY_WDT_H__
#define __MY_WDT_H__

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/clk.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/slab.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

#define WDT_ENABLE _IO('W', 0)
#define WDT_DISBALE _IO('W', 1)

#define WDT_BASE_ADDR 0xC0019000

#define WTCON 0x0
#define WTDAT 0x4
#define WTCNT 0x8
#define WTCLRINT 0xc

#define IP_RESET_REGISTER 0xC0012000
#define IP_RESET_REGISTER_1 0x4

void __iomem *wdt_init(void);
void wdt_exit(void __iomem *addr);
void wdt_set_cnt(void __iomem *addr, uint16_t value);
void wdt_set_enable(void __iomem *addr);
void wdt_set_disable(void __iomem *addr);
size_t wdt_get_clk(void __iomem *addr, unsigned int pclk);

struct char_wdt_t {
  void __iomem *vir;
  struct cdev dev;
  struct file_operations ops;
  dev_t num;
  struct class *wdt_class;
  struct device *wdt_device;
  struct clk *pclk;
};

#endif
