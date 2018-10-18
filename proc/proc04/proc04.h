/**
 * @Author: fjk
 * @Date:   2018-10-15T10:45:43+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: proc04.h
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-15T11:28:09+08:00
 * @License: GPL
 */
#ifndef __PROC_04_H__
#define __PROC_04_H__

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

struct queue_node_t {
  struct list_head queue_node;
  int data_size;
  char data[0];
};

struct queue_head_t {
  struct list_head queue_head;
  int queue_node_num;
  int data_size;
};
/**
 * [queue_alloc 初始化队列]
 * @return  [成共返回 队列头指针，失败返回 NULL]
 */
struct queue_head_t *queue_alloc(void);
/**
 * [queue_add description]
 * @param  head   [description]
 * @param  buffer [description]
 * @param  count  [description]
 * @return        [description]
 */
struct queue_node_t *queue_add(struct queue_head_t *head,
                               const char __user *buffer, size_t count);
/**
 * [queue_free description]
 * @param head [description]
 */
void queue_free(struct queue_head_t *head);

#endif
