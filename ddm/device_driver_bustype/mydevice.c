/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: mydevice.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-24T12:28:00+08:00
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

extern struct bus_type mybustype;

void mydevice_release(struct device *dev) {}
struct device mydevice = {
    .init_name = "myled_dev",
    .release = mydevice_release,
    .bus = &mybustype,
    .platform_data = "myled_dev",
};

static __init int mydevice_init(void) {
  PERR("INIT\n");
  return device_register(&mydevice);
}

static __exit void mydevice_exit(void) {
  device_unregister(&mydevice);
  PERR("EXIT\n");
  return;
}

module_init(mydevice_init);
module_exit(mydevice_exit);
