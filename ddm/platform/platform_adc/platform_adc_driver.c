/**
 * @Author: fjk
 * @Date:   2018-10-25T14:59:26+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: platform_adc_driver.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-25T15:30:20+08:00
 * @License: GPL
 */

#include "adc.h"
MODULE_LICENSE("GPL");

ssize_t char_adc_read(struct file *file, char __user *buffer, size_t size,
                      loff_t *pos) {
  struct char_adc_t *c = file->private_data;
  uint16_t val = 0;
  if (size != sizeof(uint16_t))
    return -EINVAL;
  adc_device_start(c->v_adc);
  while (adc_device_staus(c->v_adc))
    ;
  val = adc_device_data(c->v_adc);
  if (copy_to_user(buffer, &val, size))
    return -EFAULT;
  return size;
}
ssize_t char_adc_write(struct file *file, const char __user *buffer,
                       size_t size, loff_t *pos) {
  /*
  从file结构体中取出私有数据，私有数据为 open 时写入的
  */
  struct char_adc_t *c = file->private_data;

  return size;
}
long char_adc_unlocked_ioctl(struct file *file, unsigned int cmd,
                             unsigned long data) {

  return 0;
}
int char_adc_open(struct inode *inode, struct file *file) {
  struct char_adc_t *c =
      container_of(inode->i_cdev, struct char_adc_t, cdev_adc);
  file->private_data = c;
  return 0;
}
int char_adc_release(struct inode *inode, struct file *file) { return 0; }
int adc_probe(struct platform_device *device) {
  struct char_adc_t *cl;
  int ret = 0;
  struct resource *adc_device_resource =
      platform_get_resource(device, IORESOURCE_MEM, 0);
  if (IS_ERR_OR_NULL(adc_device_resource))
    return -EBUSY;
  cl = kzalloc(sizeof(*cl), GFP_KERNEL);
  if (IS_ERR_OR_NULL(cl)) {
    ret = -ENOMEM;
    goto kzalloc_char_adc_err;
  }
  /*初始化adc设备*/
  cl->v_adc = adc_device_init(adc_device_resource->start,
                              adc_device_resource->end -
                                  adc_device_resource->start + 1);
  if (IS_ERR_OR_NULL(cl->v_adc)) {
    ret = -ENOMEM;
    goto adc_device_init_err;
  }
  /*申请设备号并注册*/
  ret = alloc_chrdev_region(&cl->num_adc, 0, 1, "adc driver");
  if (ret < 0) {
    goto alloc_chrdev_region_err;
  }
  /*初始化函数操作集*/
  cl->ops_adc.open = char_adc_open;
  cl->ops_adc.release = char_adc_release;
  cl->ops_adc.read = char_adc_read;
  cl->ops_adc.write = char_adc_write;
  cl->ops_adc.unlocked_ioctl = char_adc_unlocked_ioctl;
  /*初始化字符设别*/
  cdev_init(&cl->cdev_adc, &cl->ops_adc);
  /*注册字符设备*/
  ret = cdev_add(&cl->cdev_adc, cl->num_adc, 1);
  if (ret < 0) {
    goto cdev_add_adc_err;
  }
  /*注册类*/
  cl->class_adc = class_create(THIS_MODULE, "myadc");
  if (IS_ERR_OR_NULL(cl->class_adc)) {
    ret = -ENOMEM;
    goto class_create_err;
  }
  /*注册设备*/
  cl->device_adc =
      device_create(cl->class_adc, NULL, cl->num_adc, NULL, device->name);
  if (IS_ERR_OR_NULL(cl->device_adc)) {
    goto device_create_err;
  }
  platform_set_drvdata(device, cl);
  return 0;
device_create_err:
  class_destroy(cl->class_adc);
class_create_err:
  cdev_del(&cl->cdev_adc);
cdev_add_adc_err:
  unregister_chrdev_region(cl->num_adc, 1);
alloc_chrdev_region_err:
  adc_device_exit(cl->v_adc);
adc_device_init_err:
  kfree(cl);
kzalloc_char_adc_err:
  return ret;
}
int adc_remove(struct platform_device *device) {
  struct char_adc_t *cl = platform_get_drvdata(device);
  /*注销设备*/
  device_destroy(cl->class_adc, cl->num_adc);
  /*注销类*/
  class_destroy(cl->class_adc);
  /*注销 char 设备*/
  cdev_del(&cl->cdev_adc);
  /*注销 设备号*/
  unregister_chrdev_region(cl->num_adc, 1);
  /*注销 adc 设备*/
  adc_device_exit(cl->v_adc);
  /*释放描述adc设备的结构体*/
  kfree(cl);
  return 0;
}

struct platform_device_id id_table[] = {
    {"adc01", 0}, {"adc_device1", 0}, {},
};

struct platform_driver adc_driver = {
    .probe = adc_probe,
    .remove = adc_remove,
    .driver =
        {
            .name = "adc_device_driver",
        },
    .id_table = id_table,
};

static __init int platform_driver_init(void) {
  PERR("INIT\n");
  return platform_driver_register(&adc_driver);
}

static __exit void platform_driver_exit(void) {
  platform_driver_unregister(&adc_driver);
  PERR("EXIT\n");
  return;
}

module_init(platform_driver_init);
module_exit(platform_driver_exit);
