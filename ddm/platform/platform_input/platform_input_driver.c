/**
 * @Author: fjk
 * @Date:   2018-10-26T11:59:51+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: platform_input_driver.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-26T15:24:47+08:00
 * @License: GPL
 */
#include "key.h"
#include <linux/io.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
int is_down(struct driver_key_info_t *dk) {
  unsigned int reg = ioread32((dk->vir) + (dk->key_info->offset));
  return (!((reg >> dk->key_info->gpio) & 1));
}

irqreturn_t key_input_irq_handler(int irq, void *data) {
  struct driver_key_info_t *dk = data;

  input_report_key(dk->key_input, dk->key_info->code, is_down(dk));
  input_sync(dk->key_input);

  return IRQ_HANDLED;
}

int my_key_driver_probe(struct platform_device *dev) {
  int ret = 0;
  struct resource *key_mem, *key_irq;
  int i = 0, j = 0;
  struct my_key_driver_t *my_key_driver;
  struct my_key_t *my_key = dev->dev.platform_data;
  /*
  获取按键内存资源
   */
  key_mem = platform_get_resource(dev, IORESOURCE_MEM, 0);
  if (key_mem == NULL)
    return -EBUSY;
  /*申请存放表述设备的资源/*/
  my_key_driver = kzalloc(sizeof(*my_key_driver), GFP_KERNEL);
  if (IS_ERR_OR_NULL(my_key_driver)) {
    ret = -ENOMEM;
    goto kzalloc_my_key_driver_err;
  }
  /*取出设备中keyinfo 数量*/
  my_key_driver->driver_key_info_num = my_key->key_info_num;
  /*根据 keyinfo 的数量。申请驱动中需要存储的设比信息的内存空间*/
  my_key_driver->driver_key_info = kzalloc(
      sizeof(*(my_key_driver->driver_key_info)) * (my_key->key_info_num),
      GFP_KERNEL);
  if (IS_ERR_OR_NULL(my_key_driver->driver_key_info)) {
    ret = -ENOMEM;
    goto kzalloc_driver_key_info_err;
  }
  /*获取 key irq 资源*/
  for (i = 0; i < my_key_driver->driver_key_info_num; i++) {
    key_irq = platform_get_resource(dev, IORESOURCE_IRQ, i);
    if (IS_ERR_OR_NULL(key_irq)) {
      ret = -EBUSY;
      goto platform_get_key_irq_resource_err;
    }
    /*将irq 存放到 驱动信息当中去*/
    my_key_driver->driver_key_info[i].irq = key_irq->start;
    my_key_driver->driver_key_info[i].name = key_irq->name;
    for (j = 0; j < my_key_driver->driver_key_info_num; j++) {
      /*根据 irq 资源的名字，取出其他的设备信息。并保存到驱动信息的嗯中启用*/
      if (!strcmp((key_irq->name), (my_key->key_info[j].name))) {
        my_key_driver->driver_key_info[i].key_info = &(my_key->key_info[j]);
      }
    }
  }
  my_key_driver->driver_key_info[0].vir =
      ioremap(key_mem->start, key_mem->end - key_mem->start + 1);
  if (my_key_driver->driver_key_info[0].vir == NULL) {
    ret = -ENOMEM;
    goto ioremap_key_mem_err;
  }
  for (i = 0; i < my_key_driver->driver_key_info_num; i++) {
    ret = request_irq(
        my_key_driver->driver_key_info[i].irq, key_input_irq_handler,
        IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_SAMPLE_RANDOM,
        my_key_driver->driver_key_info[i].name,
        &(my_key_driver->driver_key_info[i]));
    if (ret) {
      goto request_irq_err;
    }
  }
  my_key_driver->driver_key_info[0].key_input = input_allocate_device();
  if (IS_ERR_OR_NULL(my_key_driver->driver_key_info[0].key_input)) {
    /*i=3 是为了释放注册的中断*/
    i = 4;
    ret = -ENOMEM;
    goto request_irq_err;
  }
  for (i = 0; i < my_key_driver->driver_key_info_num; i++) {
    /*将vir复制给其他的按键的驱动信息*/
    my_key_driver->driver_key_info[i].vir =
        my_key_driver->driver_key_info[0].vir;
    /*将input设备信息复制给其他的按键的驱动信息*/
    my_key_driver->driver_key_info[i].key_input =
        my_key_driver->driver_key_info[0].key_input;
  }
  my_key_driver->driver_key_info[0].key_input->name = dev->name;
  my_key_driver->driver_key_info[0].key_input->phys = "uplooking phys";
  my_key_driver->driver_key_info[0].key_input->uniq = "uplooking uniq";
  my_key_driver->driver_key_info[0].key_input->id.bustype = BUS_HOST;
  /*设置input 设备支持的事件*/
  set_bit(EV_KEY, my_key_driver->driver_key_info[0].key_input->evbit);
  set_bit(EV_SYN, my_key_driver->driver_key_info[0].key_input->evbit);
  /*设置input 设备支持的键码*/
  for (i = 0; i < my_key_driver->driver_key_info_num; i++) {
    set_bit(my_key_driver->driver_key_info[i].key_info->code,
            my_key_driver->driver_key_info[i].key_input->keybit);
  }

  ret = input_register_device(my_key_driver->driver_key_info[0].key_input);
  if (ret) {
    goto input_register_device_err;
  }
  platform_set_drvdata(dev, my_key_driver);
  return 0;
input_register_device_err:
  input_free_device(my_key_driver->driver_key_info[0].key_input);
request_irq_err:
  while (i--) {
    free_irq(my_key_driver->driver_key_info[i].irq,
             &(my_key_driver->driver_key_info[i]));
  }
  iounmap(my_key_driver->driver_key_info[0].vir);
ioremap_key_mem_err:
platform_get_key_irq_resource_err:
  kfree(my_key_driver->driver_key_info);
kzalloc_driver_key_info_err:
  kfree(my_key_driver);
kzalloc_my_key_driver_err:
  return ret;
}
int my_key_driver_remove(struct platform_device *dev) {

  struct my_key_driver_t *my_key_driver = platform_get_drvdata(dev);
  int i = my_key_driver->driver_key_info_num;

  input_unregister_device(my_key_driver->driver_key_info[0].key_input);
  input_free_device(my_key_driver->driver_key_info[0].key_input);
  while (i--) {
    free_irq(my_key_driver->driver_key_info[i].irq,
             &(my_key_driver->driver_key_info[i]));
  }
  iounmap(my_key_driver->driver_key_info[0].vir);
  kfree(my_key_driver->driver_key_info);
  kfree(my_key_driver);
  return 0;
}

struct platform_device_id id_table[] = {
    {"my_key", 0}, {},
};

struct platform_driver my_key_driver = {
    .probe = my_key_driver_probe,
    .remove = my_key_driver_remove,
    .driver =
        {
            .name = "my_key_driver",
        },
    .id_table = id_table,
};

static __init int platform_input_driver_init(void) {
  PERR("INIT\n");
  return platform_driver_register(&my_key_driver);
}

static __exit void platform_input_driver_exit(void) {
  platform_driver_unregister(&my_key_driver);
  PERR("EXIT\n");
  return;
}

module_init(platform_input_driver_init);
module_exit(platform_input_driver_exit);
