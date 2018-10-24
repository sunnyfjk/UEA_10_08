/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: mybustype.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-24T12:24:47+08:00
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

int mybustype_match(struct device *dev, struct device_driver *drv) {

  if (!strcmp((char *)(dev->platform_data), drv->name))
    return 1;
  return 0;
}

struct bus_type mybustype = {
    .name = "mybus", .match = mybustype_match,
};

EXPORT_SYMBOL(mybustype);

static __init int mybustype_init(void) {
  PERR("INIT\n");
  bus_register(&mybustype);
  return 0;
}

static __exit void mybustype_exit(void) {
  bus_unregister(&mybustype);
  PERR("EXIT\n");
  return;
}

module_init(mybustype_init);
module_exit(mybustype_exit);
