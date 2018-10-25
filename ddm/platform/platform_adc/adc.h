/**
 * @Author: fjk
 * @Date:   2018-10-25T14:31:40+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: adc.h
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-25T15:21:02+08:00
 * @License: GPL
 */
#ifndef __ADC_H__
#define __ADC_H__
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

void __iomem *adc_device_init(unsigned int addr, unsigned int size);

void adc_device_exit(void __iomem *vir);

uint16_t adc_device_data(void __iomem *vir);

void adc_device_start(void __iomem *vir);

int adc_device_staus(void __iomem *vir);

void adc_device_interrupt(void __iomem *vir);

void adc_device_intclr(void __iomem *vir);

struct char_adc_t {
  void __iomem *v_adc;
  struct cdev cdev_adc;
  struct file_operations ops_adc;
  dev_t num_adc;
  struct class *class_adc;
  struct device *device_adc;
};

#endif
