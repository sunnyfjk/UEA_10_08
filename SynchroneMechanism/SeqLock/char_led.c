/**
 * @Author: fjk
 * @Date:   2018-10-18T11:58:13+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: char_led.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-22T14:32:13+08:00
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
  /*
  从file结构体中取出私有数据，私有数据为 open 时写入的
  */
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
  int ret = 0;
  struct char_led_t *c =
      container_of(inode->i_cdev, struct char_led_t, cdev_led);
  spin_lock(&c->lock);
  if (c->flag == 1) {
    ret = -EBUSY;
    goto device_busy;
  }
  c->flag = 1;
  spin_unlock(&c->lock);
  file->private_data = c;
  return 0;
device_busy:
  spin_unlock(&c->lock);
  return ret;
}
int char_led_release(struct inode *inode, struct file *file) {
  int ret = 0;
  struct char_led_t *c = file->private_data;
  spin_lock(&c->lock);
  if (c->flag == 0) {
    ret = -ENODEV;
    goto device_nodev;
  }
  c->flag = 0;
  spin_unlock(&c->lock);
  return 0;
device_nodev:
  spin_unlock(&c->lock);
  return ret;
}

static __init int char_led_init(void) {
  int ret = 0;
  PERR("INIT\n");
  cl = kzalloc(sizeof(*cl), GFP_KERNEL);
  if (IS_ERR_OR_NULL(cl)) {
    ret = -ENOMEM;
    goto kzalloc_char_led_err;
  }
  spin_lock_init(&cl->lock);
  /*初始化led设备*/
  cl->v_led = x6818_led_init();
  if (IS_ERR_OR_NULL(cl->v_led)) {
    ret = -ENOMEM;
    goto x6818_led_init_err;
  }
  /*申请设备号并注册*/
  ret = alloc_chrdev_region(&cl->num_led, 0, 1, "led driver");
  if (ret < 0) {
    goto alloc_chrdev_region_err;
  }
  /*初始化函数操作集*/
  cl->ops_led.open = char_led_open;
  cl->ops_led.release = char_led_release;
  cl->ops_led.read = char_led_read;
  cl->ops_led.write = char_led_write;
  cl->ops_led.unlocked_ioctl = char_led_unlocked_ioctl;
  /*初始化字符设别*/
  cdev_init(&cl->cdev_led, &cl->ops_led);
  /*注册字符设备*/
  ret = cdev_add(&cl->cdev_led, cl->num_led, 1);
  if (ret < 0) {
    goto cdev_add_led_err;
  }
  /*注册类*/
  cl->class_led = class_create(THIS_MODULE, "myled");
  if (IS_ERR_OR_NULL(cl->class_led)) {
    ret = -ENOMEM;
    goto class_create_err;
  }
  /*注册设备*/
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
  /*注销设备*/
  device_destroy(cl->class_led, cl->num_led);
  /*注销类*/
  class_destroy(cl->class_led);
  /*注销 char 设备*/
  cdev_del(&cl->cdev_led);
  /*注销 设备号*/
  unregister_chrdev_region(cl->num_led, 1);
  /*注销 led 设备*/
  x6818_led_exit(cl->v_led);
  /*释放描述led设备的结构体*/
  kfree(cl);
  PERR("EXIT\n");
  return;
}

module_init(char_led_init);
module_exit(char_led_exit);
