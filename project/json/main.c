/**
 * @Author: fjk
 * @Date:   2018-10-30T09:53:03+08:00
 * @Email:  sunnyfjk@gmai.com
 * @Filename: main.c
 * @Last modified by:   fjk
 * @Last modified time: 2018-10-30T12:03:18+08:00
 * @License: GPL
 */
#include <cJSON.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *readJsonFile(const char *file) {
  int fd = 0, ret = 0, pos = 0;
  struct stat fstat = {0};
  char *buf = NULL;
  if (file == NULL)
    return NULL;
  ret = stat(file, &fstat);
  if (ret < 0) {
    return NULL;
  }
  buf = malloc(fstat.st_size + 1);
  if (buf == NULL)
    return NULL;
  memset(buf, 0, fstat.st_size + 1);
  fd = open(file, O_RDONLY);
  if (fd < 0) {
    goto file_open_err;
  }

  while (fstat.st_size - pos > 0) {
    ret = read(fd, buf + pos, fstat.st_size - pos);
    if (ret < 0)
      continue;
    pos += ret;
  }
  close(fd);
  return buf;
file_open_err:
  free(buf);
  buf = NULL;
  return NULL;
}

int AnalysisJSON(const char *str) {
  cJSON *root, *value, *obj, *array, *item;
  int i = 0, size = 0;
  root = cJSON_Parse(str);
  if (root == NULL)
    return -1;
  value = cJSON_GetObjectItem(root, "名字");
  if (value == NULL && !(value->type & cJSON_String))
    goto cjson_get_value_err;
  printf("名字=%s\n", value->valuestring);

  value = cJSON_GetObjectItem(root, "年龄");
  if (value == NULL && !(value->type & cJSON_Number))
    goto cjson_get_value_err;
  printf("年龄=%d\n", value->valueint);
  obj = cJSON_GetObjectItem(root, "学习成绩");
  if (value == NULL && !(value->type & cJSON_Object))
    goto cjson_get_value_err;
  array = cJSON_GetObjectItem(obj, "语文");
  if (value == NULL && !(value->type & cJSON_Array))
    goto cjson_get_value_err;
  size = cJSON_GetArraySize(array);
  for (i = 0; i < size; i++) {
    item = cJSON_GetArrayItem(array, i);
    if (item == NULL && !(item->type & cJSON_Object))
      goto cjson_get_value_err;
    value = cJSON_GetObjectItem(item, "日期");
    if (value == NULL && !(value->type & cJSON_String))
      goto cjson_get_value_err;
    printf("日期=%s\n", value->valuestring);
    value = cJSON_GetObjectItem(item, "成绩");
    if (value == NULL && !(value->type & cJSON_Number))
      goto cjson_get_value_err;
    printf("成绩=%d\n", value->valueint);
  }

  array = cJSON_GetObjectItem(obj, "数学");
  if (value == NULL && !(value->type & cJSON_Array))
    goto cjson_get_value_err;
  size = cJSON_GetArraySize(array);
  for (i = 0; i < size; i++) {
    item = cJSON_GetArrayItem(array, i);
    if (item == NULL && !(item->type & cJSON_Object))
      goto cjson_get_value_err;
    value = cJSON_GetObjectItem(item, "日期");
    if (value == NULL && !(value->type & cJSON_String))
      goto cjson_get_value_err;
    printf("日期=%s\n", value->valuestring);
    value = cJSON_GetObjectItem(item, "成绩");
    if (value == NULL && !(value->type & cJSON_Number))
      goto cjson_get_value_err;
    printf("成绩=%d\n", value->valueint);
  }

  return 0;
cjson_get_value_err:
  cJSON_Delete(root);
  return -1;
}

int main(void) {
#if 0
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
#endif

  char *buf = readJsonFile("/home/fjk/UEA_10_08/project/json/data.json");
  printf("buf=%s\n", buf);
  AnalysisJSON(buf);
  free(buf);
  return 0;
}
