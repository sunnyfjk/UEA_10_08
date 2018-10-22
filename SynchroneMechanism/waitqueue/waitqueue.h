/**
 * @Author: fjk
 * @Date:   2018-10-22T15:19:22+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: waitqueue.h
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-22T16:46:53+08:00
 * @License: GPL
 */
#ifndef __WAIT_QUEUE_H__
#define __WAIT_QUEUE_H__

#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/wait.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

struct MyStack_t {
  uint8_t *stack;
  size_t top;
  size_t stack_size;
};

struct stack_cdev_t {
  struct cdev stack_dev;
  dev_t stack_num;
  struct MyStack_t *stack;
  struct file_operations stack_ops;
  struct class *stack_class;
  struct device *stack_device;
  wait_queue_head_t ro, wo;
  spinlock_t lock;
#define OPEN 1
#define CLOSE 0
  uint8_t flag;
};

struct MyStack_t *stack_init(size_t size);
void stack_exit(struct MyStack_t *s);

size_t stack_pop(struct MyStack_t *s, char __user *buffer, size_t count);
size_t stack_push(struct MyStack_t *s, const char __user *buffer, size_t count);

#endif
