/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: mydriver.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-24T12:19:25+08:00
 * @License: GPL
 */
#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

int mydevice_driverprobe(struct device *dev) {
  PERR("dev->platform_data=%s\n", (char *)(dev->platform_data));
  return 0;
}
int mydevice_driveremove(struct device *dev) {
  PERR("dev->platform_data=%s\n", (char *)(dev->platform_data));
  return 0;
}

extern struct bus_type mybustype;
struct device_driver mydevice_driver = {
    .name = "myled_dev",
    .bus = &mybustype,
    .probe = mydevice_driverprobe,
    .remove = mydevice_driveremove,
};

static __init int mydriver_init(void) {
  PERR("INIT\n");
  return driver_register(&mydevice_driver);
}

static __exit void mydriver_exit(void) {
  PERR("EXIT\n");
  driver_unregister(&mydevice_driver);
}

module_init(mydriver_init);
module_exit(mydriver_exit);
