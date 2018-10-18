/**
 * @Author: fjk
 * @Date:   2018-10-16T17:24:26+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: char01.h
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-16T17:32:59+08:00
 * @License: GPL
 */
#ifndef __CHAR_01_H__
#define __CHAR_01_H__

#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

#define DEVICE_MAJOR 243
#define QUEUE_SIZE 100

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

struct array_queue_t {
  char queue[QUEUE_SIZE];
  size_t head;
  size_t tail;
  /*现在已有的数据*/
  int count;
};

struct array_queue_t *array_queue_alloc(void);
void array_queue_free(struct array_queue_t *au);

/*出队*/
int dequeue(struct array_queue_t *au, char __user *data, size_t size);
/*入队*/
int enqueue(struct array_queue_t *au, const char __user *data, size_t size);

#endif
