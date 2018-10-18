/**
 * @Author: fjk
 * @Date:   2018-10-15T10:45:43+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: proc04.h
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-16T09:59:52+08:00
 * @License: GPL
 */
#ifndef __PROC_05_H__
#define __PROC_05_H__

#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

#define QUEUE_SIZE 100

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
int dequeue(struct array_queue_t *au, char *data, size_t size);
/*入队*/
int enqueue(struct array_queue_t *au, const char *data, size_t size);

#endif
