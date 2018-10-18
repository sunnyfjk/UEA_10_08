/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: char01.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-16T17:38:10+08:00
 * @License: GPL
 */
#include "char01.h"
MODULE_LICENSE("GPL");

dev_t dev_num;
static struct cdev mycdev;

struct array_queue_t *au;

struct array_queue_t *array_queue_alloc(void) {
  struct array_queue_t *au = NULL;
  au = kzalloc(sizeof(*au), GFP_KERNEL);
  if (IS_ERR_OR_NULL(au)) {
    PERR("kzalloc_array_queue_err\n");
    goto kzalloc_array_queue_err;
  }
  return au;
kzalloc_array_queue_err:
  return NULL;
}
void array_queue_free(struct array_queue_t *au) { kfree(au); }

/*出队*/
int dequeue(struct array_queue_t *au, char __user *data, size_t size) {
  int i = 0;
  int ret = 0;
  if (IS_ERR_OR_NULL(au) || IS_ERR_OR_NULL(data) || !size)
    return 0;

  if (au->count == 0 || au->head == au->tail)
    return 0;
  if (size > au->count)
    size = au->count;
  for (i = 0; i < size; i++) {
    ret =
        copy_to_user(&(data[i]), &(au->queue[(au->tail + i) % QUEUE_SIZE]), 1);
    if (ret)
      break;
  }
  size = i;
  au->tail = (au->tail + size) % QUEUE_SIZE;
  au->count -= size;
  return size;
}

/*入队*/
int enqueue(struct array_queue_t *au, const char __user *data, size_t size) {
  int i = 0;
  int ret = 0;
  if (IS_ERR_OR_NULL(au) || IS_ERR_OR_NULL(data) || !size)
    return 0;
  if ((au->count >= (QUEUE_SIZE - 1)) ||
      ((au->tail + 1) % QUEUE_SIZE == au->head))
    return 0;

  if (size > (QUEUE_SIZE - 1 - au->count))
    size = QUEUE_SIZE - 1 - au->count;
  for (i = 0; i < size; i++) {
    ret = copy_from_user(&(au->queue[(au->head + i) % QUEUE_SIZE]), &(data[i]),
                         1);
    if (ret)
      break;
  }
  size = i;
  au->count += size;
  au->head = (au->head + size) % QUEUE_SIZE;
  return size;
}

ssize_t mycdev_read(struct file *file, char __user *buffer, size_t size,
                    loff_t *pos) {

  return dequeue(au, buffer, size);
}
ssize_t mycdev_write(struct file *file, const char __user *buffer, size_t size,
                     loff_t *pos) {
  return enqueue(au, buffer, size);
}
int mycdev_open(struct inode *inode, struct file *file) { return 0; }
int mycdev_release(struct inode *inode, struct file *file) { return 0; }
long mycdev_unlocked_ioctl(struct file *file, unsigned int cmd,
                           unsigned long data) {
  return 0;
}

static struct file_operations mydev_ops = {
    .open = mycdev_open,
    .release = mycdev_release,
    .read = mycdev_read,
    .write = mycdev_write,
    .unlocked_ioctl = mycdev_unlocked_ioctl,
};

static __init int char01_init(void) {
  int ret = 0;
  PERR("INIT\n");

  au = array_queue_alloc();
  if (IS_ERR_OR_NULL(au))
    return -ENOMEM;

  dev_num = MKDEV(243, 0);
  ret = register_chrdev_region(dev_num, 1, "mycdev");
  if (ret) {
    PERR("register_chrdev_region_err\n");
    goto register_chrdev_region_err;
  }
  cdev_init(&mycdev, &mydev_ops);

  ret = cdev_add(&mycdev, dev_num, 1);
  if (ret) {
    PERR("register cdev err\n");
    goto register_cdev_err;
  }

  return 0;
register_cdev_err:
  unregister_chrdev_region(dev_num, 1);
register_chrdev_region_err:
  array_queue_free(au);
  return ret;
}

static __exit void char01_exit(void) {
  unregister_chrdev_region(dev_num, 1);
  cdev_del(&mycdev);
  array_queue_free(au);
  PERR("EXIT\n");
  return;
}

module_init(char01_init);
module_exit(char01_exit);
