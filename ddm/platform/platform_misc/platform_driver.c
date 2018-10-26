/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: platform_driver.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-26T10:06:49+08:00
 * @License: GPL
 */
#include "x6818_led.h"

MODULE_LICENSE("GPL");

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
  struct char_led_t *c = container_of(file->f_op, struct char_led_t, ops_led);
  file->private_data = c;
  return 0;
}
int char_led_release(struct inode *inode, struct file *file) { return 0; }
int led_probe(struct platform_device *device) {
  struct char_led_t *cl;
  int ret = 0;
  struct resource *led_device_resource =
      platform_get_resource(device, IORESOURCE_MEM, 0);
  if (IS_ERR_OR_NULL(led_device_resource))
    return -EBUSY;
  cl = kzalloc(sizeof(*cl), GFP_KERNEL);
  if (IS_ERR_OR_NULL(cl)) {
    ret = -ENOMEM;
    goto kzalloc_char_led_err;
  }
  /*初始化led设备*/
  cl->v_led =
      x6818_led_init(led_device_resource->start,
                     led_device_resource->end - led_device_resource->start + 1);
  if (IS_ERR_OR_NULL(cl->v_led)) {
    ret = -ENOMEM;
    goto x6818_led_init_err;
  }
  /*字符设备相关*/
  cl->ops_led.open = char_led_open;
  cl->ops_led.release = char_led_release;
  cl->ops_led.read = char_led_read;
  cl->ops_led.write = char_led_write;
  cl->ops_led.unlocked_ioctl = char_led_unlocked_ioctl;

  /*设备值misc设备的次设备号，自动分配*/
  cl->led_device.minor = MISC_DYNAMIC_MINOR;
  cl->led_device.name = device->name;
  cl->led_device.mode = 0644;
  cl->led_device.fops = &cl->ops_led;
  ret = misc_register(&cl->led_device);
  if (ret)
    goto misc_register_err;

  platform_set_drvdata(device, cl);
  return 0;
misc_register_err:
  x6818_led_exit(cl->v_led);
x6818_led_init_err:
  kfree(cl);
kzalloc_char_led_err:
  return ret;
}
int led_remove(struct platform_device *device) {
  struct char_led_t *cl = platform_get_drvdata(device);
  misc_deregister(&cl->led_device);
  /*注销 led 设备*/
  x6818_led_exit(cl->v_led);
  /*释放描述led设备的结构体*/
  kfree(cl);
  return 0;
}

struct platform_device_id id_table[] = {
    {"led_device", 0}, {"led_device1", 0}, {},
};

struct platform_driver led_driver = {
    .probe = led_probe,
    .remove = led_remove,
    .driver =
        {
            .name = "led_device_driver",
        },
    .id_table = id_table,
};

static __init int platform_driver_init(void) {
  PERR("INIT\n");
  return platform_driver_register(&led_driver);
}

static __exit void platform_driver_exit(void) {
  platform_driver_unregister(&led_driver);
  PERR("EXIT\n");
  return;
}

module_init(platform_driver_init);
module_exit(platform_driver_exit);
