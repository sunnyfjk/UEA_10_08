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



# 将LED、Buzzer 、KEY中断设备结合

1. 可以自动生成 `/dev/mydev` 设备文件

   - 创建描述设备的结构体
     - dev_t 类型的设备号
     - struct cdev 类型的结构体
     - struct file_operations 类型的结构体
     - void __iomem * led 的虚拟地址
     - void __iomem * buzzer 的虚拟地址
     - int irq[4] KEY的中断号
     - char irq_name[4]\[32] 注册的中断的名字
     - struct class 结构体
     - struct device 结构体
     - 设置4.3中描述的状态
     - struct proc_dir_entry 结构体
   - 编写初始化函数
     - 创建 描述设备的结构体
     - 初始化结构体中的内容
   - 编写注销函数
     - 注销初始化的数据

2. 可以通过设备文件读取到 `led` `Buzzer` 的状态（read）

   - struct file_operations 中的read函数
   - 读取led灯状态的函数
   - 定义和用户层传输的协议

3. 可以通过设备文件控制 `led` `Buzzer` 的状态（write）

   - struct file_operations 中的write函数
   - 定义和用户层通信的协议
   - 定义设置led灯状态的函数
   - 定义设置`Buzzer`状态的函数
   - 判断 4.3 的功能是否打开 检测led灯的状态，判断是否需要关闭或打开buzeer

4. 可以通过`ioct` 控制设备文件
   1. 全部打开 `led` 设备 
      - struct file_operations 中的unlocked_ioctl函数
      - 定义 ioctl 命令
      - 定义全部打开led灯的函数
   2. 全部关闭`led` 设备
      - struct file_operations 中的unlocked_ioctl函数
      - 定义 ioctl 命令
      - 定义全部关闭led灯的函数
   3. 可以设置只要打开`led` 任何一个设备，`Buzzer` 都会响，`led` 设备全部关闭时，`Buzzer` 停止
      - struct file_operations 中的unlocked_ioctl函数
      - 定义 ioctl 命令
      - 设置该功能的状态 为打开
      - 读取led灯的状态 如果有灯亮 打开蜂鸣器否则不操作蜂鸣器

   1. 设置可以关闭 3 的功能
      - struct file_operations 中的unlocked_ioctl函数
      - 定义 ioctl 命令
      - 设置该功能的状态 关闭
      - 关闭蜂鸣器

5. 通过`proc`可以获取`led` `Buzzer` 的状态

   - 实现proc的读函数

   - 定义一块缓存
   - 将描述led buzzer 的状态的字符放入缓存
   - 读存缓存中的内容

6. 空过按键 可以 控制`led` 灯的状态
   1. 按key_1第一次打开 `led 0`,再按关闭`led 0`
      - 编写中断处理函数吧
      - 判断 irq 的值和哪个引脚的中断线一样 就是哪个按键
      - 判断 4.3 的功能是否打开 检测led灯的状态，判断是否需要关闭或打开led灯
   2. 按key_2第一次打开 `led 1`,再按关闭`led 1`
      - 和6.1的功能一样
   3. 按key_3第一次打开 `led 2`,再按关闭`led 2`
      - 和6.1的功能一样
   4. 按key_4第一次打开 `led 3`,再按关闭`led 3`
      - 和6.1的功能一样

