/**
 * @Author: fjk
 * @Date:   2018-10-18T17:10:47+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: char_buzzer.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-18T17:27:55+08:00
 * @License: GPL
 */

#include "buzzer.h"
MODULE_LICENSE("GPL");

struct buzzer_t *cl;

ssize_t char_buzzer_read(struct file *file, char __user *buffer, size_t size,
                         loff_t *pos) {
  struct buzzer_t *c = file->private_data;
  struct user_buzzer_t v;
  if (size != sizeof(v))
    return -EINVAL;

  v.power = buzzer_is_open(c->v_buzzer);

  if (copy_to_user(buffer, &v, size))
    return -EFAULT;

  return size;
}
ssize_t char_buzzer_write(struct file *file, const char __user *buffer,
                          size_t size, loff_t *pos) {

  struct buzzer_t *c = file->private_data;
  struct user_buzzer_t v;
  if (size != sizeof(v))
    return -EINVAL;

  if (copy_from_user(&v, buffer, size))
    return -EFAULT;

  if (v.power)
    buzzer_open(c->v_buzzer);
  else
    buzzer_close(c->v_buzzer);
  return size;
}

long char_buzzer_unlocked_ioctl(struct file *file, unsigned int cmd,
                                unsigned long data) {

  return 0;
}

int char_buzzer_open(struct inode *inode, struct file *file) {
  struct buzzer_t *c =
      container_of(inode->i_cdev, struct buzzer_t, cdev_buzzer);
  file->private_data = c;

  return 0;
}
int char_buzzer_release(struct inode *inode, struct file *file) { return 0; }

static __init int char_buzzer_init(void) {
  int ret = 0;
  PERR("INIT\n");
  cl = kzalloc(sizeof(*cl), GFP_KERNEL);
  if (IS_ERR_OR_NULL(cl)) {
    ret = -ENOMEM;
    goto kzalloc_char_buzzer_err;
  }
  cl->v_buzzer = buzzer_init();
  if (IS_ERR_OR_NULL(cl->v_buzzer)) {
    ret = -ENOMEM;
    goto buzzer_init_err;
  }
  ret = alloc_chrdev_region(&cl->num_buzzer, 0, 1, "buzzer driver");
  if (ret < 0) {
    goto alloc_chrdev_region_err;
  }

  cl->ops_buzzer.open = char_buzzer_open;
  cl->ops_buzzer.release = char_buzzer_release;
  cl->ops_buzzer.read = char_buzzer_read;
  cl->ops_buzzer.write = char_buzzer_write;
  cl->ops_buzzer.unlocked_ioctl = char_buzzer_unlocked_ioctl;

  cdev_init(&cl->cdev_buzzer, &cl->ops_buzzer);
  ret = cdev_add(&cl->cdev_buzzer, cl->num_buzzer, 1);
  if (ret < 0) {
    goto cdev_add_buzzer_err;
  }
  cl->class_buzzer = class_create(THIS_MODULE, "mybuzzer");
  if (IS_ERR_OR_NULL(cl->class_buzzer)) {
    ret = -ENOMEM;
    goto class_create_err;
  }
  cl->device_buzzer =
      device_create(cl->class_buzzer, NULL, cl->num_buzzer, NULL, "mybuzzer");
  if (IS_ERR_OR_NULL(cl->device_buzzer)) {
    goto device_create_err;
  }
  return 0;
device_create_err:
  class_destroy(cl->class_buzzer);
class_create_err:
  cdev_del(&cl->cdev_buzzer);
cdev_add_buzzer_err:
  unregister_chrdev_region(cl->num_buzzer, 1);
alloc_chrdev_region_err:
  buzzer_exit(cl->v_buzzer);
buzzer_init_err:
  kfree(cl);
kzalloc_char_buzzer_err:
  return ret;
}

static __exit void char_buzzer_exit(void) {

  device_destroy(cl->class_buzzer, cl->num_buzzer);
  class_destroy(cl->class_buzzer);
  cdev_del(&cl->cdev_buzzer);
  unregister_chrdev_region(cl->num_buzzer, 1);
  buzzer_exit(cl->v_buzzer);
  kfree(cl);
  PERR("EXIT\n");
  return;
}

module_init(char_buzzer_init);
module_exit(char_buzzer_exit);
