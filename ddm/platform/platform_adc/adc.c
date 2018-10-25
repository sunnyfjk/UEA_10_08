/**
 * @Author: fjk
 * @Date:   2018-10-25T14:31:46+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: adc.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-25T16:24:08+08:00
 * @License: GPL
 */
#include "adc.h"

#define ADCCON 0x0
#define ADCDAT 0x4
#define ADCINTENB 0x8
#define ADCINTCLR 0xc
#define PRESCALERCON 0x10

void __iomem *adc_device_init(unsigned int addr, unsigned int size, int flag) {
  void __iomem *v = ioremap(addr, size);
  if (v == NULL)
    return NULL;
  if (flag) {
    /*初始化adc中断*/
    iowrite32(1, v + ADCINTENB);
  }
  iowrite32(8 << 6, v + ADCCON);
  iowrite32(((1 << 15) | 99), v + PRESCALERCON);
  return v;
}
void adc_device_exit(void __iomem *vir) {
  iowrite32(0, vir + ADCINTENB);
  iowrite32(1, vir + ADCINTCLR);
  iowrite32(0, vir + PRESCALERCON);
  iounmap(vir);
}

uint16_t adc_device_data(void __iomem *vir) {
  unsigned int reg = 0;
  reg = ioread32(vir + ADCDAT);
  return (reg & 0xfff);
}

void adc_device_start(void __iomem *vir) {

  unsigned int reg = 0;
  reg = ioread32(vir + ADCCON);
  reg &= ~(1 << 2);
  reg |= 1;
  iowrite32(reg, vir + ADCCON);
}

int adc_device_staus(void __iomem *vir) {
  unsigned int reg = 0;
  reg = ioread32(vir + ADCCON);
  return (reg & 1);
}

void adc_device_interrupt(void __iomem *vir) { iowrite32(1, vir + ADCINTENB); }

void adc_device_intclr(void __iomem *vir) { iowrite32(1, vir + ADCINTCLR); }
