/**
 * @Author: fjk
 * @Date:   2018-10-25T14:59:34+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: platform_adc_device.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-25T15:16:24+08:00
 * @License: GPL
 */
#include <linux/init.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <mach/platform.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

struct resource adc_device_resource[] = {
        [0] =
            {
                .start = 0xC0053000,
                .end = 0xC0053000 + SZ_4K - 1,
                .name = "adc_register",
                .flags = IORESOURCE_MEM,
            },
        [1] =
            {
                .start = IRQ_PHY_ADC,
                .end = IRQ_PHY_ADC,
                .name = "adc_irq",
                .flags = IORESOURCE_IRQ,
            },

};

void platform_adc_device_release(struct device *dev) {}

struct platform_device platform_adc_device = {
    .name = "adc01",
    .id = -1,
    .dev =
        {
            .init_name = "adc01", .release = platform_adc_device_release,
        },
    .num_resources = ARRAY_SIZE(adc_device_resource),
    .resource = adc_device_resource,
};

static __init int platform_adc_device_init(void) {
  PERR("INIT\n");

  return platform_device_register(&platform_adc_device);
}

static __exit void platform_adc_device_exit(void) {
  platform_device_unregister(&platform_adc_device);
  PERR("EXIT\n");
  return;
}

module_init(platform_adc_device_init);
module_exit(platform_adc_device_exit);
