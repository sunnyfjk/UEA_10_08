/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: proc03.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-15T10:00:33+08:00
 * @License: GPL
 */
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

char a[100] = "asdasdasdasdsadsadsadsadsadsadsadA";

struct queue_t {
  char a[100];
  int head;
  int tail;
  /*记录已经有的数据*/
  int count;
  /**/
  int allCount;
};

struct queue_t qu;

int only_read_file(char *page, char **start, off_t off, int count, int *eof,
                   void *data) {
  int i = 0;
  if (qu.count == 0) {
    *eof = 1;
    return 0;
  }
  if (count > qu.count)
    count = qu.count;

  qu.count -= count;

  *start = count;

  for (i = 0; i < count; i++)
    page[i] = qu.a[(qu.head + i) % 100];

  qu.head = (qu.head + count) % 100;
  return count;
}

int only_write_file(struct file *file, const char __user *buffer,
                    unsigned long count, void *data) {
  if (count > sizeof(a))
    count = sizeof(a);

  count -= copy_from_user(a, buffer, count);
  return count;
}

struct proc_dir_entry *mydir = NULL;
struct proc_dir_entry *myfile = NULL;

static __init int proc03_init(void) {
  PERR("INIT\n");

  mydir = proc_mkdir("mydir", NULL);
  if (IS_ERR_OR_NULL(mydir)) {
    PERR("create mydir proc entry err\n");
    goto create_mydir_proc_entry_err;
  }
  myfile = create_proc_entry("myfile", 0644, mydir);
  if (IS_ERR_OR_NULL(myfile)) {
    PERR("create_proc_myfile_entry_err\n");
    goto create_proc_myfile_entry_err;
  }

  myfile->read_proc = only_read_file;
  myfile->write_proc = only_write_file;
  myfile->data = &qu;

  return 0;
create_proc_myfile_entry_err:
  remove_proc_entry("mydir", NULL);
create_mydir_proc_entry_err:
  return -ENOMEM;
}

static __exit void proc03_exit(void) {
  remove_proc_entry("myfile", mydir);
  remove_proc_entry("mydir", NULL);
  PERR("EXIT\n");
  return;
}

module_init(proc03_init);
module_exit(proc03_exit);
