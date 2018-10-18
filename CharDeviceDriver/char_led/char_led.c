/**
 * @Author: fjk
 * @Date:   2018-10-18T11:58:13+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: char_led.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-18T15:24:24+08:00
 * @License: GPL
 */
#include "x6818_led.h"

MODULE_LICENSE("GPL");

struct char_led_t *cl;

ssize_t char_led_read(struct file *file, char __user *buffer, size_t size,
                      loff_t *pos) {
  struct char_led_t *c = file->private_data;
  struct user_led_con_t con;
  if (size != sizeof(con))
    return -EINVAL;

  x6818_led_status(c->v_led, &con.status);

  if (copy_to_user(buffer, &con, size)) {
    return -EFAULT;
  }
  return size;
}
ssize_t char_led_write(struct file *file, const char __user *buffer,
                       size_t size, loff_t *pos) {
  struct char_led_t *c = file->private_data;
  struct user_led_con_t con;
  if (size != sizeof(con))
    return -EINVAL;
  if (copy_from_user(&con, buffer, size)) {
    return -EFAULT;
  }
  PERR("con.power=%d\tcon.num=%d\n", con.power, con.num);
  if (con.power) {
    x6818_led_open(c->v_led, con.num);
  } else {
    x6818_led_close(c->v_led, con.num);
  }
  return size;
}

long char_led_unlocked_ioctl(struct file *file, unsigned int cmd,
                             unsigned long data) {

  return 0;
}

int char_led_open(struct inode *inode, struct file *file) {
  struct char_led_t *c =
      container_of(inode->i_cdev, struct char_led_t, cdev_led);
  file->private_data = c;
  /*
  假设 0 地址为 struct char_led_t 结构体的首地址
  struct char_led_t *a=(struct char_led_t *)0;
  得出 struct char_led_t 中 cdev 相对与 0 的地址
  void *b=&(a->cdev_led);

  计算 得出 struct char_led_t 中 cdev 相对首地址的偏移
  unsigned long pos=((char *)a)-((char *)b);

  知道 struct char_led_t 中 cdev 真实地址，
  真实地址 减去 相对偏移 可以得出 struct char_led_t 结构体的真实的首地址
  ((char *)(inode->i_cdev)-pos)==cl

   */
  return 0;
}
int char_led_release(struct inode *inode, struct file *file) { return 0; }

static __init int char_led_init(void) {
  int ret = 0;
  PERR("INIT\n");
  cl = kzalloc(sizeof(*cl), GFP_KERNEL);
  if (IS_ERR_OR_NULL(cl)) {
    ret = -ENOMEM;
    goto kzalloc_char_led_err;
  }
  cl->v_led = x6818_led_init();
  if (IS_ERR_OR_NULL(cl->v_led)) {
    ret = -ENOMEM;
    goto x6818_led_init_err;
  }
  ret = alloc_chrdev_region(&cl->num_led, 0, 1, "led driver");
  if (ret < 0) {
    goto alloc_chrdev_region_err;
  }

  cl->ops_led.open = char_led_open;
  cl->ops_led.release = char_led_release;
  cl->ops_led.read = char_led_read;
  cl->ops_led.write = char_led_write;
  cl->ops_led.unlocked_ioctl = char_led_unlocked_ioctl;

  cdev_init(&cl->cdev_led, &cl->ops_led);
  ret = cdev_add(&cl->cdev_led, cl->num_led, 1);
  if (ret < 0) {
    goto cdev_add_led_err;
  }
  cl->class_led = class_create(THIS_MODULE, "myled");
  if (IS_ERR_OR_NULL(cl->class_led)) {
    ret = -ENOMEM;
    goto class_create_err;
  }
  cl->device_led =
      device_create(cl->class_led, NULL, cl->num_led, NULL, "myled");
  if (IS_ERR_OR_NULL(cl->device_led)) {
    goto device_create_err;
  }
  return 0;
device_create_err:
  class_destroy(cl->class_led);
class_create_err:
  cdev_del(&cl->cdev_led);
cdev_add_led_err:
  unregister_chrdev_region(cl->num_led, 1);
alloc_chrdev_region_err:
  x6818_led_exit(cl->v_led);
x6818_led_init_err:
  kfree(cl);
kzalloc_char_led_err:
  return ret;
}

static __exit void char_led_exit(void) {

  device_destroy(cl->class_led, cl->num_led);
  class_destroy(cl->class_led);
  cdev_del(&cl->cdev_led);
  unregister_chrdev_region(cl->num_led, 1);
  x6818_led_exit(cl->v_led);
  kfree(cl);
  PERR("EXIT\n");
  return;
}

module_init(char_led_init);
module_exit(char_led_exit);
