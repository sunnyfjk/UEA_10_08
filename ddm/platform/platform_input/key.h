/**
 * @Author: fjk
 * @Date:   2018-10-26T11:40:46+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: key.h
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-26T12:23:59+08:00
 * @License: GPL
 */
#ifndef __KEY_H__
#define __KEY_H__
#include <linux/init.h>
#include <linux/input.h>
#include <linux/module.h>
/*中断号的头文件*/
#include <linux/platform_device.h>
#include <mach/platform.h>

#include <linux/input.h>
#include <linux/interrupt.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

struct key_info_t {
  const char *name;
  unsigned int offset;
  uint8_t gpio;
  int code;
};

struct my_key_t {
  struct key_info_t *key_info;
  size_t key_info_num;
};
struct driver_key_info_t {
  struct key_info_t *key_info;
  struct input_dev *key_input;
  void __iomem *vir;
  int irq;
  const char *name;
};

struct my_key_driver_t {
  struct driver_key_info_t *driver_key_info;
  size_t driver_key_info_num;
};

#endif
