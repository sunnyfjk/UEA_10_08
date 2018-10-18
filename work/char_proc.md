# 将char设备和proc文件系统结合
## 功能

1. 在`kernel` 中创建`空递增` `栈`

   * 数据成员

     ```c
     struct stack_t{
         char *stack;
         //表示栈顶
         int top;
         // 表示栈的大小
         int size;
         //表示栈中的数据
         int count;
     }
     ```

   * 需要对 栈 的内存进行初始化，并对size进行初始化

2. 通过字符设备可以写入数据至内核的中创建的`栈`中（进栈）

   - 如果 count==0 说明栈空，可以入栈，不可以出栈 

3. 通过字符设备可以读取内核中的`栈`的数据（出栈）

   - 如果 size==count 说明栈满，可以出栈，不可以入栈

4. 通过ioctl控制`栈` 大小、清空`栈` 的数据、读取`栈` 现在存在的数据的多少、读取`栈`的大小

   - 定义 ioctl 的命令

     - 控制栈的大小

       ```c
       #define SET_STACK_SIZE _IOW('S',1,int)
       ```

     - 清空栈的数据

       ```c
       #define STACK_CLEAR _IO('S',2)
       ```

     - 读取栈中数据的数量

       ```c
       #define GET_DATA_SIZE _IOR('S',3,int *)
       ```

     - 读取栈的大小

       ```c
       #define GET_STACK_SIZE _IOR('S',4,int *)
       ```

   - 重新设置栈的大小：重新申请一个栈空间，并将原来的栈中的数据，复制到新栈中，修改size 的大小，如果原来栈中的数据比新的栈小，count 和 top 直接是远来的值，如果原来栈中的数据比新的栈大，需要修改 count 和 top 大小

   - 清空栈数据：将top和count的值修改为0

   - 出栈：(判断栈是否为空)top-=1;count-=1;拷贝top现在指向的栈中的数据

   - 入栈：(判断栈是否已满)拷贝数据至top指向的空间，top+=1;count+=1;

   - 读取栈中数据的多少：返回count的值

   - 读取栈的大小：返回 size 的值

5. 可以通过`proc` 文件系统读取`栈` 中现在存在的数据的多少、`栈` 的大小

   - 读取栈中数据的数量，读取栈的大小

     - 创建一个 全局数组

     - 将现在的状态转化为 字符串，放到数组当中

       ```c
       snprintf()
       ```

     - start->show->next->show->stop 的过程 相当读取数组当中的数据吧

6. 可以通过`proc` 文件系统设置`栈` 的大小、清空`栈`中的数据

   - 控制栈的大小

        ```shell
        echo "set stack size:100" > /proc/stack
        ```

        ```c
        /*可以在 proc 的write函数中分析字符串*/
        char buf[256];
        char *value[2];
        char *tmp;
        copy_from_user(buf,buffer,count>256?256:count);
        tmp=strtok(buf,":");
        for(i=0;i<2;i++){
            if(tmp==NULL){
                break;
            }
            value[i]=tmp;
            tmp=strtok(NULL,":");
        }
        if(strcmp("set stack size",value[0]))
        {
           /*
           value[1] 转化为数字
           */
        }
        ```

      - 清空栈的数据

        ```c
        echo "set stack clear" > /proc/stack
        ```

        ```c
        char buf[256];
        char *value[2];
        char *tmp;
        copy_from_user(buf,buffer,count>256?256:count);
        tmp=strtok(buf,":");
        for(i=0;i<2;i++){
            if(tmp==NULL){
                break;
            }
            value[i]=tmp;
            tmp=strtok(NULL,":");
        }
        if(strcmp("set stack clear",value[0]))
        {
          if(valie[1]==NULL)
              /*执行清空栈的操作*/
        }
        ```


   
