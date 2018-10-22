/**
 * @Author: fjk
 * @Date:   2018-10-18T10:40:24+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: x6818_led.h
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-22T14:24:23+08:00
 * @License: GPL
 */
#ifndef __X6818_LED_H__
#define __X6818_LED_H__
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
/*
GPIO_B26
GPIO_C11
GPIO_C7
GPIO_C12
 */
#define LED_GPIO_BASE_ADDR 0xC0010000

/*设置输入输出功能*/
#define GPIOBOUTENB 0xB004
/*设置输出电平*/
#define GPIOBOUT 0xB000
/*设置是否启用开漏输出或推挽输出*/
#define GPIOB_PULLENB 0xB060
/*设置是否启用GPIOB_PULLENB的设置*/
#define GPIOB_PULLENB_DISABLE_DEFAULT 0xB064

#define GPIOCOUTENB 0xC004
#define GPIOCOUT 0xC000
#define GPIOC_PULLENB 0xC060
#define GPIOC_PULLENB_DISABLE_DEFAULT 0xC064

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)
/**
 * [x6818_led_init 初始化led设备]
 * @return  [成功：led设备的虚拟地址 失败：NULL]
 */
void __iomem *x6818_led_init(void);
/**
 * [x6818_led_exit 释放字符设备，释放led设备对应的虚拟地址]
 * @param addr [led设备的虚拟地址]
 */
void x6818_led_exit(void __iomem *addr);
/**
 * [x6818_led_open 点亮led设备]
 * @param addr [led设备虚拟地址]
 * @param num  [led设备序号]
 */
void x6818_led_open(void __iomem *addr, int num);
/**
 * [x6818_led_close 熄灭led设备]
 * @param addr [led设备虚拟地址]
 * @param num  [led设备序号]
 */
void x6818_led_close(void __iomem *addr, int num);
/**
 * [x6818_led_con 设置led设备的状态]
 * @param addr [led设备虚拟地址]
 * @param num  [led设备序号]
 * @param flag [led设备的状态 0 关 1 开]
 */
void x6818_led_con(void __iomem *addr, int num, int flag);
/**
 * [x6818_led_status 获取led设备的状态]
 * @param addr   [led设备虚拟地址]
 * @param status [led设备的状态]
 */
void x6818_led_status(void __iomem *addr, uint8_t *status);

struct char_led_t {
  void __iomem *v_led;
  struct cdev cdev_led;
  struct file_operations ops_led;
  dev_t num_led;
  struct class *class_led;
  struct device *device_led;
  spinlock_t lock;
  int flag;
};

struct user_led_con_t {
  uint8_t power;
  uint8_t num;
  uint8_t status;
};

#endif
