/**
 * @Author: fjk
 * @Date:   2018-10-11T14:05:58+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: proc01.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-12T10:30:35+08:00
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

/*
如果定义了 CHILD_DIR 的宏 就会在 mkdir 目录创建 mydir_01 的目录
否则相反
 */
#define CHILD_DIR

struct proc_dir_entry *mydir = NULL;
#if defined(CHILD_DIR)
struct proc_dir_entry *mydir_01 = NULL;
#endif

static __init int proc01_init(void) {
  PERR("INIT\n");

  mydir = proc_mkdir("mydir", NULL);
  if (IS_ERR_OR_NULL(mydir)) {
    PERR("create mydir proc entry err\n");
    goto create_mydir_proc_entry_err;
  }
#if defined(CHILD_DIR)
  mydir_01 = proc_mkdir("mydir_01", mydir);
  if (IS_ERR_OR_NULL(mydir_01)) {
    PERR("create mydir 01 proc entry err\n");
    goto create_mydir_01_proc_entry_err;
  }
#endif

  return 0;
#if defined(CHILD_DIR)
create_mydir_01_proc_entry_err:
  remove_proc_entry("mydir", NULL);
#endif
create_mydir_proc_entry_err:
  return -ENOMEM;
}

static __exit void proc01_exit(void) {
#if defined(CHILD_DIR)
  remove_proc_entry("mydir_01", mydir);
#endif
  remove_proc_entry("mydir", NULL);
  PERR("EXIT\n");
  return;
}

module_init(proc01_init);
module_exit(proc01_exit);
