/**
 * @Author: fjk
 * @Date:   2018-10-30T16:19:39+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-30T17:42:36+08:00
 * @License: GPL
 */
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define INSERT_SAMPLE "INSERT INTO mqtt_user(username,password) VALUES(?,?);"

int mysql_insert_value(MYSQL *mysql, const char *name, const char *pass) {
  MYSQL_STMT *stmt;
  MYSQL_BIND bind[2];
  int param_count;

  stmt = mysql_stmt_init(mysql);
  if (stmt == NULL) {
    fprintf(stderr, " mysql_stmt_init(), out of memory\n");
    goto mysql_stmt_init_err;
  }
  if (mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE))) {
    fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    goto mysql_stmt_prepare_err;
  }
  param_count = mysql_stmt_param_count(stmt);
  if (param_count != 2) {
    fprintf(stdout, " total parameters in INSERT: %d\n", param_count);
    goto mysql_stmt_prepare_err;
  }
  memset(bind, 0, sizeof(bind));
  bind[0].buffer_type = MYSQL_TYPE_STRING;
  bind[0].buffer = (char *)name;
  bind[0].buffer_length = strlen(name);

  bind[1].buffer_type = MYSQL_TYPE_STRING;
  bind[1].buffer = (char *)pass;
  bind[1].buffer_length = strlen(pass);

  if (mysql_stmt_bind_param(stmt, bind)) {
    fprintf(stderr, " mysql_stmt_bind_param() failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    goto mysql_stmt_prepare_err;
  }
  if (mysql_stmt_execute(stmt)) {
    fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    goto mysql_stmt_prepare_err;
  }
  mysql_stmt_close(stmt);
  return 0;
mysql_stmt_prepare_err:
  mysql_stmt_close(stmt);
mysql_stmt_init_err:
  return -1;
}
#define SELCT_SAMPLE "SELECT password FROM mqtt_user where username=?;"
int mysql_select_value(MYSQL *mysql, const char *name, char *pass) {
  MYSQL_STMT *stmt;
  MYSQL_BIND bind[2];
  int param_count, column_count;
  MYSQL_RES *prepare_meta_result;
  my_bool is_null;
  unsigned long length;

  stmt = mysql_stmt_init(mysql);
  if (stmt == NULL) {
    fprintf(stderr, " mysql_stmt_init(), out of memory\n");
    goto mysql_stmt_init_err;
  }
  if (mysql_stmt_prepare(stmt, SELCT_SAMPLE, strlen(SELCT_SAMPLE))) {
    fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    goto mysql_stmt_prepare_err;
  }
  param_count = mysql_stmt_param_count(stmt);
  if (param_count != 1) {
    fprintf(stdout, " total parameters in INSERT: %d\n", param_count);
    goto mysql_stmt_prepare_err;
  }
  memset(bind, 0, sizeof(bind));
  bind[0].buffer_type = MYSQL_TYPE_STRING;
  bind[0].buffer = (char *)name;
  bind[0].buffer_length = strlen(name);
  if (mysql_stmt_bind_param(stmt, &(bind[0]))) {
    fprintf(stderr, " mysql_stmt_bind_param() failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    goto mysql_stmt_prepare_err;
  }
  prepare_meta_result = mysql_stmt_result_metadata(stmt);
  if (!prepare_meta_result) {
    fprintf(stderr,
            " mysql_stmt_result_metadata(), returned no meta information\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    goto mysql_stmt_prepare_err;
  }
  column_count = mysql_num_fields(prepare_meta_result);
  if (column_count != 1) {
    fprintf(stderr, " invalid column count returned by MySQL\n");
    goto mysql_stmt_result_metadata_err;
  }
  if (mysql_stmt_execute(stmt)) {
    fprintf(stderr, " mysql_stmt_execute(), failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    goto mysql_stmt_result_metadata_err;
  }

  bind[1].buffer_type = MYSQL_TYPE_STRING;
  bind[1].buffer = (char *)pass;
  bind[1].buffer_length = 45;
  bind[1].is_null = &is_null;
  bind[1].length = &length;

  if (mysql_stmt_bind_result(stmt, &(bind[1]))) {
    fprintf(stderr, " mysql_stmt_bind_result() failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    goto mysql_stmt_result_metadata_err;
  }
  if (mysql_stmt_store_result(stmt)) {
    fprintf(stderr, " mysql_stmt_store_result() failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    goto mysql_stmt_result_metadata_err;
  }

  if (mysql_stmt_fetch(stmt)) {
    fprintf(stderr, " mysql_stmt_fetch() failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    goto mysql_stmt_result_metadata_err;
  }
  mysql_free_result(prepare_meta_result);
  mysql_stmt_close(stmt);
  return 0;
mysql_stmt_result_metadata_err:
  mysql_free_result(prepare_meta_result);
mysql_stmt_prepare_err:
  mysql_stmt_close(stmt);
mysql_stmt_init_err:
  return -1;
}
int main(void) {
  MYSQL mysql;
  char value = 1;
  char buf[45] = {0};
  mysql_init(&mysql);
  mysql_options(&mysql, MYSQL_OPT_RECONNECT, (void *)&value);
  if (!mysql_real_connect(&mysql, "127.0.0.1", "root", "fanjingkai", "mqtt",
                          3306, NULL, 0)) {
    fprintf(stderr, "Failed to connect to database: Error: %s\n",
            mysql_error(&mysql));
  }
  fprintf(stderr, "mysql connect success\n");
  fprintf(stderr, "mysql_insert_value=%d\n",
          mysql_insert_value(&mysql, "123", "456"));
  fprintf(stderr, "mysql_select_value=%d\n",
          mysql_select_value(&mysql, "123", buf));
  fprintf(stderr, "password=\"%s\"\n", buf);
  sleep(10);
  mysql_close(&mysql);

  return 0;
}
