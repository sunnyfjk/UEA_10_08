/**
 * @Author: fjk
 * @Date:   2018-10-17T11:45:32+08:00
 * @Email:  sunnyfjkgmail.com
 * @Filename: char02.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-17T15:07:57+08:00
 * @License: GPL
 */
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

struct set_value_t {
  int a;
  int b;
  int c;
};

#define SET_STATE _IOW('R', 1, int)
#define SET_VALUE _IOW('R', 2, struct set_value_t *)

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

struct char02_t {
  dev_t char02_num;
  struct cdev char02_dev;
  struct file_operations char02_ops;
  struct class *char02_class;
  struct device *char02_device;
};

ssize_t char02_read(struct file *file, char __user *buffer, size_t size,
                    loff_t *pos) {
  return 0;
}
ssize_t char02_write(struct file *file, const char __user *buffer, size_t size,
                     loff_t *pos) {
  return 0;
}

long char02_unlocked_ioctl(struct file *file, unsigned int cmd,
                           unsigned long data) {
  int ret = 0;
  struct char02_t *c = file->private_data;
  struct set_value_t value;
  switch (cmd) {
  case SET_STATE:
    PERR("data=%ld\n", data);
    break;
  case SET_VALUE:
    ret = copy_from_user(&value, (void *)data, sizeof(value));
    PERR("value.a=%d,value.b=%d,value.c=%d\n", value.a, value.b, value.c);
    break;
  default:
    break;
  }
  return 0;
}

int char02_open(struct inode *inode, struct file *file) {
  struct char02_t *c = container_of(inode->i_cdev, struct char02_t, char02_dev);
  file->private_data = c;
  return 0;
}
int char02_release(struct inode *inode, struct file *file) { return 0; }

struct char02_t *char02 = NULL;
static __init int char02_init(void) {
  int ret = 0;
  PERR("INIT\n");

  char02 = kzalloc(sizeof(*char02), GFP_KERNEL);
  if (IS_ERR_OR_NULL(char02)) {
    PERR("kzalloc char02 dev mem err\n");
    ret = -ENOMEM;
    goto kzalloc_char02_dev_mem_err;
  }
  /*
    申请设备号并注册
  */
  ret = alloc_chrdev_region(&char02->char02_num, 0, 1, "char02");
  if (ret) {
    PERR("alloc char02 chrdev region err\n");
    goto alloc_char02_chrdev_region_err;
  }

  PERR("MAJOR=%d MINOR=%d\n", MAJOR(char02->char02_num),
       MINOR(char02->char02_num));

  cdev_init(&char02->char02_dev, &char02->char02_ops);

  char02->char02_ops.open = char02_open;
  char02->char02_ops.release = char02_release;
  char02->char02_ops.read = char02_read;
  char02->char02_ops.write = char02_write;
  char02->char02_ops.unlocked_ioctl = char02_unlocked_ioctl;

  ret = cdev_add(&char02->char02_dev, char02->char02_num, 1);
  if (ret) {
    PERR("cdev_add_char02_device_err\n");
    goto cdev_add_char02_device_err;
  }

  char02->char02_class = class_create(THIS_MODULE, "char02");
  if (IS_ERR_OR_NULL(char02->char02_class)) {
    PERR("char02_class_create_err\n");
    ret = -ENOMEM;
    goto char02_class_create_err;
  }
  char02->char02_device = device_create(char02->char02_class, NULL,
                                        char02->char02_num, NULL, "mychar");
  if (IS_ERR_OR_NULL(char02->char02_device)) {
    PERR("char02_device_create_err\n");
    ret = -ENOMEM;
    goto char02_device_create_err;
  }
  return 0;
char02_device_create_err:
  class_destroy(char02->char02_class);
char02_class_create_err:
  cdev_del(&char02->char02_dev);
cdev_add_char02_device_err:
  unregister_chrdev_region(char02->char02_num, 1);
alloc_char02_chrdev_region_err:
  kfree(char02);
kzalloc_char02_dev_mem_err:
  return ret;
}
static __exit void char02_exit(void) {
  device_destroy(char02->char02_class, char02->char02_num);
  class_destroy(char02->char02_class);
  cdev_del(&char02->char02_dev);
  unregister_chrdev_region(char02->char02_num, 1);
  kfree(char02);
  PERR("EXIT\n");
  return;
}

module_init(char02_init);
module_exit(char02_exit);
