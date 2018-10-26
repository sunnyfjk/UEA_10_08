/**
 * @Author: fjk
 * @Date:   2018-10-26T11:42:35+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: platform_input_device.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-26T12:14:26+08:00
 * @License: GPL
 */
#include "key.h"

MODULE_LICENSE("GPL");

struct resource my_key_resource[] = {
        [0] =
            {
                .start = 0xC0010000,
                .end = 0xC0010000 + SZ_64K - 1,
                .name = "my_key",
                .flags = IORESOURCE_MEM,
            },
        [1] =
            {
                .start = IRQ_GPIO_A_START + 28,
                .end = IRQ_GPIO_A_START + 28,
                .name = "UP",
                .flags = IORESOURCE_IRQ,
            },
        [2] =
            {
                .start = IRQ_GPIO_B_START + 30,
                .end = IRQ_GPIO_B_START + 30,
                .name = "DOWN",
                .flags = IORESOURCE_IRQ,
            },
        [3] =
            {
                .start = IRQ_GPIO_B_START + 31,
                .end = IRQ_GPIO_B_START + 31,
                .name = "SPACE",
                .flags = IORESOURCE_IRQ,
            },
        [4] =
            {
                .start = IRQ_GPIO_B_START + 9,
                .end = IRQ_GPIO_B_START + 9,
                .name = "ENTER",
                .flags = IORESOURCE_IRQ,
            },
};

struct key_info_t key_info[] = {
        [0] =
            {
                .name = "UP", .offset = 0xA018, .gpio = 28, .code = KEY_UP,
            },
        [1] =
            {
                .name = "UP", .offset = 0xB018, .gpio = 30, .code = KEY_DOWN,
            },
        [2] =
            {
                .name = "UP", .offset = 0xB018, .gpio = 31, .code = KEY_SPACE,
            },
        [3] =
            {
                .name = "UP", .offset = 0xB018, .gpio = 9, .code = KEY_ENTER,
            },
};
struct my_key_t my_key = {
    .key_info = key_info, .key_info_num = ARRAY_SIZE(key_info),
};
void my_key_release(struct device *dev) {}
struct platform_device key_device = {
    .name = "my_key",
    .id = -1,
    .dev =
        {
            .init_name = "my_key",
            .release = my_key_release,
            .platform_data = &my_key,
        },
    .num_resources = ARRAY_SIZE(my_key_resource),
    .resource = my_key_resource,
};

static __init int platform_input_device_init(void) {
  PERR("INIT\n");

  return platform_device_register(&key_device);
}

static __exit void platform_input_device_exit(void) {
  platform_device_unregister(&key_device);
  PERR("EXIT\n");
  return;
}

module_init(platform_input_device_init);
module_exit(platform_input_device_exit);
