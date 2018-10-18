/**
 * @Author: fjk
 * @Date:   2018-10-18T17:01:25+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: buzzer.h
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-18T17:22:41+08:00
 * @License: GPL
 */
#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

/*
GPIOC14
 */

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

#define GPIO_C_BASE_ADDR 0xC001C000
#define GPIOCOUTENB 0x04
#define GPIOCOUT 0x00
#define GPIOC_PULLENB 0x60
#define GPIOC_PULLENB_DISABLE_DEFAULT 0x64

void __iomem *buzzer_init(void);
void buzzer_exit(void __iomem *addr);

void buzzer_open(void __iomem *addr);

void buzzer_close(void __iomem *addr);

uint8_t buzzer_is_open(void __iomem *addr);

struct buzzer_t {
  void __iomem *v_buzzer;
  struct cdev cdev_buzzer;
  struct file_operations ops_buzzer;
  dev_t num_buzzer;
  struct class *class_buzzer;
  struct device *device_buzzer;
};
struct user_buzzer_t {
  /*设置 读取 buzzer的状态*/
  uint8_t power;
};

#endif
