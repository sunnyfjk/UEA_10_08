/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: proc02.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-12T15:35:02+08:00
 * @License: GPL
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#define PERR(fmt, args...)                                                     \
  do {                                                                         \
    printk(KERN_ERR "[%s:%d]" fmt, __FUNCTION__, __LINE__, ##args);            \
  } while (0)

MODULE_LICENSE("GPL");

char a[100] = "asdasdasdasdsadsadsadsadsadsadsadA";

int only_read_file(char *page, char **start, off_t off, int count, int *eof,
                   void *data) {

  /*
  判断文件是否读完
   */
  if (off > strlen(a)) {
    *eof = 1;
    return 0;
  }

  if (count > strlen(a) - off)
    count = strlen(a) - off;

#if 0
  /*
    start==NULL
    数据向 page+off的位置写
    返回值为 读取成功的个数 + off

   */
  memcpy(page + off, "123456", 7);
  return 7 + off;
#endif

#if 0
  /*
      start<page
      数据向page中写
      *start=读取的个数
      返回值为成功读取的个数
  */
  memcpy(page, a + off, count);
  *start = count;
  return count;
#endif
#if 0
  /*
    start>page
    数据写向 page+off
    *start=page+off
    返回值为成功读取的个数
   */
  *start = page + off;
  memcpy(*start, a + off, count);
  return count;
#endif
}

struct proc_dir_entry *mydir = NULL;
struct proc_dir_entry *onlyRead = NULL;

static __init int proc02_init(void) {
  PERR("INIT\n");

  mydir = proc_mkdir("mydir", NULL);
  if (IS_ERR_OR_NULL(mydir)) {
    PERR("create mydir proc entry err\n");
    goto create_mydir_proc_entry_err;
  }
  onlyRead =
      create_proc_read_entry("onlyRead", 0644, mydir, only_read_file, NULL);
  if (IS_ERR_OR_NULL(onlyRead)) {
    PERR("create_proc_only_read_entry_err\n");
    goto create_proc_only_read_entry_err;
  }
  return 0;
create_proc_only_read_entry_err:
  remove_proc_entry("mydir", NULL);
create_mydir_proc_entry_err:
  return -ENOMEM;
}

static __exit void proc02_exit(void) {
  remove_proc_entry("onlyRead", mydir);
  remove_proc_entry("mydir", NULL);
  PERR("EXIT\n");
  return;
}

module_init(proc02_init);
module_exit(proc02_exit);
