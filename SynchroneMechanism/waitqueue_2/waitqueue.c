/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: waitqueue.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-23T09:45:53+08:00
 * @License: GPL
 */
#include "waitqueue.h"
MODULE_LICENSE("GPL");

struct stack_cdev_t *dev;

struct MyStack_t *stack_init(size_t size) {
  struct MyStack_t *s = NULL;
  if (size == 0) {
    goto argument_err;
  }
  if (in_interrupt())
    s = kzalloc(sizeof(*s), GFP_ATOMIC);
  else
    s = kzalloc(sizeof(*s), GFP_KERNEL);
  if (IS_ERR_OR_NULL(s)) {
    goto argument_err;
  }
  if (in_interrupt())
    s->stack = kzalloc(size, GFP_ATOMIC);
  else
    s->stack = kzalloc(size, GFP_KERNEL);
  if (IS_ERR_OR_NULL(s->stack)) {
    goto kzalloc_stack_err;
  }
  s->stack_size = size;
  return s;
kzalloc_stack_err:
  kfree(s);
argument_err:
  return NULL;
}
void stack_exit(struct MyStack_t *s) {
  kfree(s->stack);
  kfree(s);
}

size_t stack_pop(struct MyStack_t *s, char __user *buffer, size_t count) {

  int i = 0;
  if (s->top == 0)
    return 0;
  if (s->top < count)
    count = s->top;
  for (i = 0; i < count; i++) {
    if (copy_to_user(&buffer[i], &(s->stack[s->top - 1]), 1))
      return -EFAULT;
    s->top--;
  }
  return count;
}
size_t stack_push(struct MyStack_t *s, const char __user *buffer,
                  size_t count) {
  int i = 0;
  if (s->top == s->stack_size)
    return 0;
  if (count > (s->stack_size - s->top))
    count = s->stack_size - s->top;

  for (i = 0; i < count; i++) {
    if (copy_from_user(&(s->stack[s->top]), &buffer[i], 1))
      return -EFAULT;
    s->top++;
  }
  return count;
}

ssize_t my_stack_read(struct file *file, char __user *buffer, size_t size,
                      loff_t *pos) {
  int ret = 0;
  struct stack_cdev_t *s = file->private_data;
  if (file->f_flags & O_NONBLOCK) {
    if (s->stack->top == 0)
      return 0;
  } else {
    if (s->stack->top == 0) {
      ret = wait_event_interruptible(s->ro, (s->stack->top > 0));
      if (ret)
        return ret;
    }
  }
  ret = stack_pop(s->stack, buffer, size);
  wake_up(&s->wo);
  return ret;
}
ssize_t my_stack_write(struct file *file, const char __user *buffer,
                       size_t size, loff_t *pos) {
  int ret = 0;
  struct stack_cdev_t *s = file->private_data;
  if (file->f_flags & O_NONBLOCK) {
    if (s->stack->top == s->stack->stack_size)
      return 0;
  } else {
    if (s->stack->top == s->stack->stack_size) {
      ret = wait_event_interruptible(s->wo,
                                     (s->stack->top < s->stack->stack_size));
      if (ret)
        return ret;
    }
  }
  ret = stack_push(s->stack, buffer, size);
  wake_up(&s->ro);
  return ret;
}
long my_stack_unlocked_ioctl(struct file *file, unsigned int cmd,
                             unsigned long arg) {
  return 0;
}
int my_stack_open(struct inode *inode, struct file *file) {
  struct stack_cdev_t *s =
      container_of(inode->i_cdev, struct stack_cdev_t, stack_dev);

  file->private_data = s;
  return 0;
}
int my_stack_release(struct inode *inode, struct file *file) { return 0; }

static __init int waitqueue_init(void) {
  int ret = 0;
  PERR("INIT\n");
  dev = kzalloc(sizeof(*dev), GFP_KERNEL);
  if (IS_ERR_OR_NULL(dev)) {
    ret = -ENOMEM;
    goto kzalloc_stack_dev_err;
  }
  spin_lock_init(&dev->lock);
  init_waitqueue_head(&dev->ro);
  init_waitqueue_head(&dev->wo);
  dev->flag = CLOSE;
  dev->stack = stack_init(100);
  if (IS_ERR_OR_NULL(dev->stack)) {
    ret = -ENOMEM;
    goto stack_init_err;
  }
  ret = alloc_chrdev_region(&dev->stack_num, 0, 1, "mystack");
  if (ret) {
    goto alloc_chrdev_region_err;
  }
  dev->stack_ops.open = my_stack_open;
  dev->stack_ops.release = my_stack_release;
  dev->stack_ops.read = my_stack_read;
  dev->stack_ops.write = my_stack_write;
  dev->stack_ops.unlocked_ioctl = my_stack_unlocked_ioctl;
  cdev_init(&dev->stack_dev, &dev->stack_ops);
  ret = cdev_add(&dev->stack_dev, dev->stack_num, 1);
  if (ret) {
    goto cdev_add_err;
  }
  dev->stack_class = class_create(THIS_MODULE, "mystack");
  if (IS_ERR_OR_NULL(dev->stack_class)) {
    ret = -ENOMEM;
    goto class_create_err;
  }
  dev->stack_device = device_create(dev->stack_class, NULL, dev->stack_num,
                                    NULL, "mystack_wait_queue");
  if (IS_ERR_OR_NULL(dev->stack_device)) {
    ret = -ENOMEM;
    goto device_create_err;
  }

  return 0;

device_create_err:
  class_destroy(dev->stack_class);
class_create_err:
  cdev_del(&dev->stack_dev);
cdev_add_err:
  unregister_chrdev_region(dev->stack_num, 1);
alloc_chrdev_region_err:
  stack_exit(dev->stack);
stack_init_err:
  kfree(dev);
kzalloc_stack_dev_err:
  return ret;
}

static __exit void waitqueue_exit(void) {
  device_destroy(dev->stack_class, dev->stack_num);
  class_destroy(dev->stack_class);
  cdev_del(&dev->stack_dev);
  unregister_chrdev_region(dev->stack_num, 1);
  stack_exit(dev->stack);
  kfree(dev);
  PERR("EXIT\n");
  return;
}

module_init(waitqueue_init);
module_exit(waitqueue_exit);
