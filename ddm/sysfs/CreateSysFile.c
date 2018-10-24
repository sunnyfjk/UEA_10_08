/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: CreateSysFile.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-24T12:21:26+08:00
 * @License: GPL
 */
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sysfs.h>

struct kset *kset;
struct kobject led1, led2;

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

ssize_t led_show(struct kobject *kobject, struct attribute *attribute,
                 char *buff) {
  return sprintf(buff, "kobject->name=%s\tattribute->name=%s\n", kobject->name,
                 attribute->name);
}
ssize_t led_store(struct kobject *kobject, struct attribute *attribute,
                  const char *buffer, size_t size) {
  PERR("kobject->name=%s\tattribute->name=%s\t buffer=%s\n", kobject->name,
       attribute->name, buffer);
  return size;
}

struct sysfs_ops led_sysfs_ops = {
    .show = led_show, .store = led_store,
};

struct attribute led_on = {
    .name = "on", .mode = 0644,
};
struct attribute led_off = {
    .name = "off", .mode = 0644,
};

struct attribute *led_attribute[] = {
    &led_on, &led_off, NULL,
};
void led_type_release(struct kobject *kobj) {}
struct kobj_type led_type = {
    .release = led_type_release,
    .sysfs_ops = &led_sysfs_ops,
    .default_attrs = led_attribute,
};

static __init int CreateSysFile_init(void) {
  int ret = 0;
  PERR("INIT\n");
  kset = kset_create_and_add("myled", NULL, NULL);
  ret = kobject_init_and_add(&led1, &led_type, &kset->kobj, "led1");
  ret = kobject_init_and_add(&led2, &led_type, &kset->kobj, "led2");

  return 0;
}

static __exit void CreateSysFile_exit(void) {
  kobject_del(&led1);
  kobject_del(&led2);
  kset_unregister(kset);

  PERR("EXIT\n");
  return;
}

module_init(CreateSysFile_init);
module_exit(CreateSysFile_exit);
