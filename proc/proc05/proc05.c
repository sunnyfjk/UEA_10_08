/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: proc05.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-16T10:39:11+08:00
 * @License: GPL
 */

#include "proc05.h"
MODULE_LICENSE("GPL");

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
int dequeue(struct array_queue_t *au, char *data, size_t size) {
  int i = 0;
  if (IS_ERR_OR_NULL(au) || IS_ERR_OR_NULL(data) || !size)
    return 0;

  if (au->count == 0 || au->head == au->tail)
    return 0;
  if (size > au->count)
    size = au->count;
  for (i = 0; i < size; i++) {
    data[i] = au->queue[(au->tail + i) % QUEUE_SIZE];
  }
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

void *seq_start(struct seq_file *m, loff_t *pos) {
  int ret = 0;
  if (m->private == NULL)
    m->private = kzalloc(1, GFP_KERNEL);
  ret = dequeue(au, m->private, 1);
  if (0 == ret) {
    PERR("no data\n");
    return NULL;
  }
  PERR("\n");
  return m->private;
}
void seq_stop(struct seq_file *m, void *v) {
  if (m->private != NULL)
    kfree(m->private);
  m->private = NULL;
  PERR("\n");
}
void *seq_next(struct seq_file *m, void *v, loff_t *pos) {
  int ret = 0;
  ret = dequeue(au, m->private, 1);
  if (0 == ret) {
    PERR("no data\n");
    return NULL;
  }
  PERR("\n");
  return m->private;
}
int seq_show(struct seq_file *m, void *v) {
  PERR("\n");
  return seq_write(m, m->private, 1);
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
  int ret = enqueue(au, buffer, count);
  if (ret == 0) {
    PERR("full data\n");
  }
  return ret;
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

static __init int proc05_init(void) {
  PERR("INIT\n");

  au = array_queue_alloc();
  if (au == NULL)
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
  array_queue_free(au);
  return -ENOMEM;
}

static __exit void proc05_exit(void) {
  /*删除 porc文件*/
  remove_proc_entry("myfile", NULL);

  array_queue_free(au);
  PERR("EXIT\n");
  return;
}

module_init(proc05_init);
module_exit(proc05_exit);
