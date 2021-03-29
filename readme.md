<!--
 * @Author: your name
 * @Date: 2021-03-29 10:10:00
 * @LastEditTime: 2021-03-29 21:30:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \httpd文档\readme.md
-->
# httpd
| Part Ⅰ | Part Ⅱ | Part Ⅲ | Part Ⅳ | Part Ⅴ | Part Ⅵ |
| :--------: | :---------: | :---------: | :---------: | :---------: | :---------: |
| [Inroduce](#介绍) | [Build](#编译) | [Point](#技术要点) | [Log](#异步日志) |[Buffer](#应用层buffer) |[Test](#压力测试)

<a name="介绍"></a>
# 1. 介绍
基于`C++11` + `STL` 实现的高性能 `HTTP` 服务器。在`Ubuntu16.04(gcc 5.4.0)` 以及 `Centos7(gcc 4.8.5)` 平台编译通过，目前不支持`Windows`系统。
# 2.编译
```bash
# git clone https://github.com/CoderCXF/httpd.git
# cd httpd
# sudo ./build.sh
```

# 3.技术要点
* 基于`Multiple reactors + threadpool`模型实现主框架，使用`epoll`默认的水平触发+非阻塞`IO`；
* `Main Reactor` 所在线程只负责`accept`新连接请求，并以`round-robin/hash`方式分配给其他`IO`线程；
*  实现线程池以充分利用多核`CPU`，避免频繁的创建和销毁线程的开销；
* 设计应用层`buffer`以支持非阻塞`IO`模型；
* 参考实现`muduo`的异步双缓冲日志设计；
* 使用状态机解析`HTTP`请求；
* 使用智能指针和`RAII`机制管理系统资源；

## 实现框架
![](https://user-gold-cdn.xitu.io/2020/3/25/17110c95048a2009?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)

# 4.异步日志
在服务端编程中，日志是必不可少的。在实现中使用的是双缓冲的异步日志系统。
一条典型的日志记录如下：
```shell
20210328 20:37:32.191497 10487 TRACE nothing happend-EPoll.cpp:49
```
## 4.1 实现思路
前端和后端各提供两个`buffer`，前端两个缓冲区一个为当前缓冲区`currentbuffer_`，另一个为预备缓冲区`nextbuffer_`，当前缓冲区写满的时候，就把它送入一个vector集合`buffers_`中，等待写入磁盘，并把预备缓冲`nextbuffer_`移为当前缓冲，然后开始追加日志到`nextbuffer_`并通知后端可以开始写出日志到磁盘。

而在后端，同样准备好两块空`buffer`：`bufferA` 和 `bufferB`，当条件满足的时候就立即将当前缓冲区也添加到`buffers_`中准备写入，注意，这时候的当前缓冲区已经是预备缓冲区了（此时前端已经没有`buffer`了），然后立即将一个空`buffer`(`currentbuffer_ = bufferA`)移为当前缓冲区，工作线程开始往这个新的`currentbuffer_`中写。并且将另一个空的`bufferB`替换为 `nextbuffer_`，这样前端始终有一个空闲的预备缓冲区。然后后端开始写`buffers_`集合内部的所有buffer。

## 4.2 实现图解
![](https://gitee.com/codercxf/Blog_image_hexo/raw/master/2021/20210329143345.png)

# 5.应用层buffer
`httpd` 使用非阻塞IO模型，保证IO线程不会阻塞在 `read` 、`write` 等IO操作。每一个连接都会有一个发送接收缓冲区(`inputbuffer_`)和发送缓冲区(`outputbuffer_`)。
![](https://gitee.com/codercxf/Blog_image_hexo/raw/master/2021/20210329151922.png)


* 为了保证数据能够发送完成，需要有应用层发送缓冲区支持。如果用户所发送的数据对应的套接字的内核缓冲区装不下，则会保存在自己的应用层缓冲区，并且开始监听写事件；当写事件触发（内核缓冲区有空间容纳数据）的时候，继续将内核缓冲区中的数据写入内核缓冲区，若还没有写完，则继续监听写事件。但是一旦应用层缓冲区没有数据，则立即停止监听`EPOLL_OUT`事件。
* 在读数据的时候，最好保证一次性的读完，当然，由于采用的是LT模式，也可以多次触发，多次读完。所以既然一次不是一个完整的包，则需要一个缓冲区保存半包数据，这就是接收缓存区。
  从 `socket` 内核接收到数据，存至 `inputbuffer_`，通知上层应用程序，应用程序判定是否是一个完整的包，如果不是一个完整的包，则什么也不做；如果是一个完整的包，将取走这个消息，并进行相应的处理。
# 6.压力测试
## 6.1 测试环境：
* `Ubuntu 16.04`
* 内存：`8G`
* `CPU`：`Core i7`
  
## 6.2 测试方法：
* 使用工具`Webbench`，开启`1000`客户端进程，时间为`60s`；
* 分别测试短连接和长连接；
* 关闭日志打印功能；且为避免磁盘IO对性能的影响，测试响应为`hello httpd`，并添加上相应的响应报文头部；
* 线程池开启四线程；
* 禁用 `Nagle` 算法；
## 6.3 测试结果：
**短连接：**
![](https://gitee.com/codercxf/Blog_image_hexo/raw/master/2021/20210329212140.png)


**长连接：**
![](https://gitee.com/codercxf/Blog_image_hexo/raw/master/2021/20210329212202.png)





