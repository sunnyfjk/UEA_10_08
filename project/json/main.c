/**
 * @Author: fjk
 * @Date:   2018-10-30T09:53:03+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-30T10:35:03+08:00
 * @License: GPL
 */
#include <cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
  cJSON *root = NULL, *item = NULL, *value = NULL, *arr = NULL, *item0 = NULL;
  char *str;
  root = cJSON_CreateObject();
  if (root == NULL) {
    goto cJSON_CreateObject_err;
  }
  value = cJSON_AddStringToObject(root, "名字", "李三");
  if (value == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  value = cJSON_AddNumberToObject(root, "年龄", 15);
  if (value == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  item = cJSON_AddObjectToObject(root, "学习成绩");
  if (item == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  /*语文数组*/
  arr = cJSON_AddArrayToObject(item, "语文");
  if (arr == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  item0 = cJSON_CreateObject();
  if (item0 == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  cJSON_AddItemToArray(arr, item0);
  value = cJSON_AddStringToObject(item0, "日期", "2010-10-08");
  if (value == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  value = cJSON_AddNumberToObject(item0, "成绩", 16);
  if (value == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }

  item0 = cJSON_CreateObject();
  if (item0 == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  cJSON_AddItemToArray(arr, item0);
  value = cJSON_AddStringToObject(item0, "日期", "2010-10-09");
  if (value == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  value = cJSON_AddNumberToObject(item0, "成绩", 17);
  if (value == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  /*数学数组*/
  arr = cJSON_AddArrayToObject(item, "数学");
  if (arr == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  item0 = cJSON_CreateObject();
  if (item0 == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  cJSON_AddItemToArray(arr, item0);
  value = cJSON_AddStringToObject(item0, "日期", "2010-10-08");
  if (value == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  value = cJSON_AddNumberToObject(item0, "成绩", 16);
  if (value == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }

  item0 = cJSON_CreateObject();
  if (item0 == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  cJSON_AddItemToArray(arr, item0);
  value = cJSON_AddStringToObject(item0, "日期", "2010-10-09");
  if (value == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  value = cJSON_AddNumberToObject(item0, "成绩", 17);
  if (value == NULL) {
    goto cJSON_AddVlaueToObject_err;
  }
  str = cJSON_Print(root);
  printf("%s\n", str);
  free(str);
  str = cJSON_PrintUnformatted(root);
  printf("%s\n", str);
  free(str);
  cJSON_Delete(root);
  return 0;
cJSON_AddVlaueToObject_err:
  cJSON_Delete(root);
cJSON_CreateObject_err:
  return -1;
}
