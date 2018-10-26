/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: platform_device.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-24T15:46:51+08:00
 * @License: GPL
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

/*

struct platform_device {
        const char      * name;
        int             id;
        struct device   dev;
        u32             num_resources;
        struct resource * resource;

        const struct platform_device_id *id_entry;

        struct mfd_cell *mfd_cell;

        struct pdev_archdata    archdata;
};
 */

struct resource led_device_resource[] = {
        [0] =
            {
                .start = 0xC0010000,
                .end = 0xC0010000 + SZ_64K - 1,
                .name = "led_addrs",
                .flags = IORESOURCE_MEM,
            },

};

void led_device_release(struct device *dev) {}
struct platform_device led_device = {
    .name = "led_device",
    .id = -1,
    .dev =
        {
            .init_name = "led_device", .release = led_device_release,
        },
    .num_resources = ARRAY_SIZE(led_device_resource),
    .resource = led_device_resource,
};

static __init int platform_device_init(void) {
  PERR("INIT\n");

  return platform_device_register(&led_device);
}

static __exit void platform_device_exit(void) {
  platform_device_unregister(&led_device);
  PERR("EXIT\n");
  return;
}

module_init(platform_device_init);
module_exit(platform_device_exit);
