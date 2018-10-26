/**
 * @Author: fjk
 * @Date:   2018-10-26T11:59:51+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: platform_input_driver.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-26T12:30:11+08:00
 * @License: GPL
 */
#include "key.h"

MODULE_LICENSE("GPL");
int my_key_driver_probe(struct platform_device *dev) { return 0; }
int my_key_driver_remove(struct platform_device *dev) { return 0; }

struct platform_device_id id_table[] = {
    {"my_key", 0}, {},
};

struct platform_driver my_key_driver = {
    .probe = my_key_driver_probe,
    .remove = my_key_driver_remove,
    .driver =
        {
            .name = "my_key_driver",
        },
    .id_table = id_table,
};

static __init int platform_input_driver_init(void) {
  PERR("INIT\n");
  return platform_driver_register(&platform_driver_register);
}

static __exit void platform_input_driver_exit(void) {
  platform_driver_unregister(&platform_driver_register);
  PERR("EXIT\n");
  return;
}

module_init(platform_input_driver_init);
module_exit(platform_input_driver_exit);
