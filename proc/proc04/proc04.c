/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: proc04.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-15T17:17:10+08:00
 * @License: GPL
 */

#include "proc04.h"
MODULE_LICENSE("GPL");

struct queue_head_t *head = NULL;

struct queue_head_t *queue_alloc(void) {
  struct queue_head_t *head = NULL;
  head = kzalloc(sizeof(*head), GFP_KERNEL);
  if (IS_ERR_OR_NULL(head)) {
    PERR("kzalloc_queue_head_err\n");
    goto kzalloc_queue_head_err;
  }
  INIT_LIST_HEAD(&head->queue_head);
  return head;
kzalloc_queue_head_err:
  return NULL;
}

struct queue_node_t *queue_add(struct queue_head_t *head,
                               const char __user *buffer, size_t count) {
  struct queue_node_t *node;
  if (IS_ERR_OR_NULL(head) || IS_ERR_OR_NULL(buffer) || !count)
    return NULL;
  node = kzalloc(sizeof(*node) + count, GFP_KERNEL);
  if (IS_ERR_OR_NULL(node))
    return NULL;
  node->data_size = count;
  if (copy_from_user(node->data, buffer, count) != 0)
    goto copy_from_user_err;

  INIT_LIST_HEAD(&node->queue_node);

  list_add_tail(&node->queue_node, &head->queue_head);
  head->queue_node_num++;
  head->data_size += count;
  return NULL;
copy_from_user_err:
  kfree(node);
  return NULL;
}

void queue_free(struct queue_head_t *head) {

  struct queue_node_t *pos = NULL, *n = NULL;

  if (IS_ERR_OR_NULL(head))
    return;
  if (!list_empty_careful(&head->queue_head)) {
    list_for_each_entry_safe(pos, n, &head->queue_head, queue_node) {
      list_del_init(&pos->queue_node);
      kfree(pos);
    }
  }
  kfree(head);
  return;
}

/*
pos 默认值为0
head->queue_node_num = 5;
start pos=1
show  pos=1
next  pos=2
show  pos=2
next  pos=3
show  pos=3
next  pos=4
show  pos=4
next  pos=5
show  pos=5
next  pos=6
stop  pos=6
 */

void *seq_start(struct seq_file *m, loff_t *pos) {
  if ((*pos) > head->queue_node_num || !head->queue_node_num)
    return NULL;
  (*pos)++;
  return list_first_entry(&head->queue_head, struct queue_node_t, queue_node);
}
void seq_stop(struct seq_file *m, void *v) {}
void *seq_next(struct seq_file *m, void *v, loff_t *pos) {
  struct queue_node_t *node = v;
  if ((*pos) > head->queue_node_num || !head->queue_node_num)
    return NULL;
  (*pos)++;
  return list_first_entry(&node->queue_node, struct queue_node_t, queue_node);
}
int seq_show(struct seq_file *m, void *v) {
  struct queue_node_t *node = v;
  return seq_write(m, node->data, node->data_size);
}
/*
  seq 文件读取操作的函数操作集合
  seq 文件读取的流程
  start->show->next->show->next
                              ==NULL->stop
                              !=NULL->show

 */
struct seq_operations seq_ops = {
    /*proc文件read操作的第一次的执行*/
    .start = seq_start,
    /*读取文件结束时执行*/
    .stop = seq_stop,
    /*读取结束时调用*/
    .next = seq_next,
    /*将内容写入到seq文件中*/
    .show = seq_show,
};
/**
 * [myproc_open 实现proc的文件的打开操作，具体时使用seq_open]
 * @param  inode []
 * @param  file  []
 * @return       []
 */
int myproc_open(struct inode *inode, struct file *file) {
  return seq_open(file, &seq_ops);
}
ssize_t myproc_write(struct file *file, const char __user *buffer, size_t count,
                     loff_t *off) {
  queue_add(head, buffer, count);
  return count;
}

struct proc_dir_entry *myfile;

/*
  定义proc文件函数操作集
  应用层用到的函数在内核中的实现
 */
struct file_operations proc_fops = {
    .open = myproc_open,
    .release = seq_release,
    .read = seq_read,
    .write = myproc_write,
};

/*
创建文件
  ->设置函数操作即 struct file_operations->实现
                                          ->open    使用open系统调用时执行
                                          ->release 使用close系统调用时执行
                                          ->read    使用read系统调用时执行
                                          ->write   使用write系统调用时执行
struct file_operations中的open 函数 时使用 seq_open 实现的.
seq_open,seq_release,seq_read 属于 seq_file 文件

 */

static __init int proc04_init(void) {
  PERR("INIT\n");
  /*
  创建队列
   */
  head = queue_alloc();
  if (IS_ERR_OR_NULL(head))
    return -ENOMEM;
  /*
    创建proc文件
   */
  myfile = proc_create_data("myfile", 0644, NULL, &proc_fops, NULL);
  if (IS_ERR_OR_NULL(myfile)) {
    PERR("proc_create_myfile_data_err\n");
    goto proc_create_myfile_data_err;
  }
  return 0;
proc_create_myfile_data_err:
  queue_free(head);
  return -ENOMEM;
}

static __exit void proc04_exit(void) {
  /*删除 porc文件*/
  remove_proc_entry("myfile", NULL);
  /*释放队列*/
  queue_free(head);
  PERR("EXIT\n");
  return;
}

module_init(proc04_init);
module_exit(proc04_exit);
