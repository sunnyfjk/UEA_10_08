/**
 * @Author: fjk
 * @Date:   2018-10-19T10:35:33+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: char_wdt.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-19T14:21:11+08:00
 * @License: GPL
 */
#include "wdt.h"

MODULE_LICENSE("GPL");

ssize_t wdt_read(struct file *file, char __user *buffer, size_t size,
                 loff_t *pos) {
  struct char_wdt_t *cw = file->private_data;
  size_t value = 0;
  unsigned long rate = 0;
  if (size != sizeof(size_t))
    return -EINVAL;
  rate = clk_get_rate(cw->pclk);
  PERR("rate=%ld\n", rate);
  value = wdt_get_clk(cw->vir, rate);
  if (copy_to_user(buffer, &value, size))
    return -EFAULT;

  return size;
}
ssize_t wdt_write(struct file *file, const char __user *buffer, size_t size,
                  loff_t *pos) {
  struct char_wdt_t *cw = file->private_data;
  uint16_t value;
  if (size != sizeof(uint16_t))
    return -EINVAL;

  if (copy_from_user(&value, buffer, size))
    return -EFAULT;

  wdt_set_cnt(cw->vir, value);

  return size;
}
long wdt_unlocked_ioctl(struct file *file, unsigned int cmd,
                        unsigned long arg) {
  struct char_wdt_t *cw = file->private_data;
  switch (cmd) {
  case WDT_ENABLE:
    wdt_set_enable(cw->vir);
    break;
  case WDT_DISBALE:
    wdt_set_disable(cw->vir);
    break;
  }
  return 0;
}
int wdt_open(struct inode *inode, struct file *file) {
  struct char_wdt_t *cw = container_of(file->f_op, struct char_wdt_t, ops);
  file->private_data = cw;

  return 0;
}
int wdt_release(struct inode *inode, struct file *file) { return 0; }

struct char_wdt_t *cw = NULL;

static __init int char_wdt_init(void) {
  int ret = 0;
  PERR("INIT\n");

  cw = kzalloc(sizeof(*cw), GFP_KERNEL);
  if (IS_ERR_OR_NULL(cw)) {
    ret = -ENOMEM;
    goto kzalloc_char_wdt_err;
  }

  cw->pclk = clk_get(NULL, "pclk");
  if (IS_ERR_OR_NULL(cw->pclk)) {
    PERR("get_pclk_err\n");
    ret = -ENODEV;
    goto get_pclk_err;
  }
  ret = clk_enable(cw->pclk);
  if (ret) {
    goto pclk_enable_err;
  }

  cw->vir = wdt_init();
  if (IS_ERR_OR_NULL(cw->vir)) {
    ret = -ENOMEM;
    goto wdt_init_err;
  }

  ret = alloc_chrdev_region(&cw->num, 0, 1, "my wdt");
  if (ret) {
    goto alloc_wdt_chrdev_region_err;
  }

  cw->ops.open = wdt_open;
  cw->ops.release = wdt_release;
  cw->ops.read = wdt_read;
  cw->ops.write = wdt_write;
  cw->ops.unlocked_ioctl = wdt_unlocked_ioctl;

  cdev_init(&cw->dev, &cw->ops);

  ret = cdev_add(&cw->dev, cw->num, 1);
  if (ret) {
    goto wdt_cdev_add_err;
  }

  cw->wdt_class = class_create(THIS_MODULE, "mywdt");
  if (IS_ERR_OR_NULL(cw->wdt_class)) {
    ret = -ENOMEM;
    goto mywdt_class_create_err;
  }

  cw->wdt_device = device_create(cw->wdt_class, NULL, cw->num, NULL, "mywdt");
  if (IS_ERR_OR_NULL(cw->wdt_device)) {
    ret = -ENOMEM;
    goto mywdt_device_create_err;
  }

  return 0;
mywdt_device_create_err:
  class_destroy(cw->wdt_class);
mywdt_class_create_err:
  cdev_del(&cw->dev);
wdt_cdev_add_err:
  unregister_chrdev_region(cw->num, 1);
alloc_wdt_chrdev_region_err:
  wdt_exit(cw->vir);
wdt_init_err:
  clk_disable(cw->pclk);
pclk_enable_err:
  clk_put(cw->pclk);
get_pclk_err:
  kfree(cw);
kzalloc_char_wdt_err:
  return ret;
}

static __exit void char_wdt_exit(void) {
  device_destroy(cw->wdt_class, cw->num);
  class_destroy(cw->wdt_class);
  cdev_del(&cw->dev);
  unregister_chrdev_region(cw->num, 1);
  wdt_exit(cw->vir);
  clk_disable(cw->pclk);
  clk_put(cw->pclk);
  kfree(cw);
  PERR("EXIT\n");
  return;
}

module_init(char_wdt_init);
module_exit(char_wdt_exit);
