<!--
 * @Author: your name
 * @Date: 2021-03-29 10:10:00
 * @LastEditTime: 2021-03-31 16:10:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \httpd文档\readme.md
-->
# httpd
| Part Ⅰ | Part Ⅱ | Part Ⅲ | Part Ⅳ | Part Ⅴ | Part Ⅵ | Part VII |
| :--------: | :---------: | :---------: | :---------: | :---------: | :---------: | :---------: |
| [Inroduce](#1.介绍) | [Build](#2.编译) | [Point](#3.技术要点) | [Log](#4.异步日志) |[Buffer](#5.应用层buffer) |[Test](#6.压力测试)| [FAQ](#7.FAQ) |
# 1.介绍
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
![](https://gitee.com/codercxf/Blog_image_hexo/raw/master/2021/20210330092713.png)
## 3.1 接受新连接

![](https://gitee.com/codercxf/Blog_image_hexo/raw/master/2021/20210331150030.png)
1. 当一个新的连接请求到来的时候，首先会触发产生一个可读事件，epoll产生一个活跃的通道：`Acceptor::acceptChannel_`，调用 `Channel::handleEvent` 函数；
2. 因为是读事件，所以又会调用通道注册的读事件回调函数：`Acceptor::handleRead()`；`在handleRead`中调用`accept`函数接收这个新连接，并且会调用一个`Acceptor::newConnection`回调函数；
3. 而这个函数又是在`TcpServer`类中注册并实现的，所以就会回调 `TcpServer::newConnectionCallback`；
   >那么这个函数是什么时候注册的呢？
   其实是我们在编写程序的时候首先构建一个 `TcpServer` 对象，这时，我们在该类的构造函数中进行了注册：`acceptor_->setNewConnectionCallback`
4. `在TcpServer::newConnectionCallback`函数中建立一个新的连接`conn`，并且分配给IO线程监听事件。并且回调了用户注册`connCallback`函数。
5. 至此，新的连接建立完成。

## 3.2 事件到来
![](https://gitee.com/codercxf/Blog_image_hexo/raw/master/2021/20210331154942.png)
1. 在接收新连接的时候，我们已经在IO线程中注册了新连接的读写异常事件的回调函数。所以当通道有读事件发生，就会调用`Channel::handleEvent`函数，因为是读事件，又会调用`Channel`注册的`readCallback_` 函数进行处理:
2. 而这个函数又是什么时候注册的呢，其实是在建立一个新的连接对象到来的时候就已经注册了：
![](https://gitee.com/codercxf/Blog_image_hexo/raw/master/2021/20210331152422.png)
3. 这个时候就会调用 `Connection::handleRead` 函数，然后读数据至缓冲区，并且回调用户注册的消息到来函数`messageCallback` ：
![](https://gitee.com/codercxf/Blog_image_hexo/raw/master/2021/20210331153434.png)

## 3.3 连接断开
![](https://gitee.com/codercxf/Blog_image_hexo/raw/master/2021/20210331155654.png)

1. 连接断开属于可读事件，用户同样回调上面的`Connection::handleRead` 所示的 `Connection::handleRead函数` 读取数据，但是读取结果为0，表示对端关闭。于是调用 `handleClose` 函数；
2. 在这个函数中，首先回调了用户注册的 `connectionCallback`函数（可以看到，这个函数在新建立连接以及断开连接的时候都会调用），然后调用了一个叫做 `closeCallback_`，这个函数不是用户注册的，而是等于(绑定) `TcpServer::removeConnection` 这个函数。
![](https://gitee.com/codercxf/Blog_image_hexo/raw/master/2021/20210331154325.png)

3. 在这个函数中又调用了`Connection::connectDestroy`函数，在这个函数中将此`channel`(连接)从监听树上移除。

# 4. Timer设计

很多场景都会用到定时器，例如：

1. 使用 TCP 长连接时，客户端需要定时向服务端发送心跳请求；并且可以踢掉空闲的连接；
2. 双 11 的 0 点，定时开启秒杀开关

### 定时器数据结构的选择要求：

要求能够快速根据当前时间找到已到期的定时器，并且也要高效的添加和删除 `Timer`。实现的方式主要有 **双向升序链表、小根堆和时间轮** 等方法。

### 自己的选择：

这里使用的是红黑树，在C++中就是 `map` 和 `set` 这两个集合。键就是定时器的过期时间，而值就是对应的该定时器对象。因为同样的过期时间的定时器可能所做的工作不一样（也就是回调函数不一样），而map是只保存不重复的key，所以不能简单的使用map，而可以使用`multi_map`，但是`multi_map` 集合不熟悉，所以还是使用 `map` 和 `set` 结合进行使用。

在本项目中使用的是如下结构保存所有的timerlist:

```c++
typedef std::pair<TimeStamp, Timer*> Entry;
typedef std::set<Entry> TimerList;
```
>选择红黑树(map/set)的原因：
查找，插入和删除的时间复杂度都是O(n);



# 4.异步日志
在服务端编程中，日志是必不可少的。在实现中使用的是双缓冲的异步日志系统。
一条典型的日志记录如下：
```shell
20210328 20:37:32.191497 10487 TRACE nothing happend-EPoll.cpp:49
```

## 4.1 实现思路
前端和后端各提供两个`buffer`，前端两个缓冲区一个为当前缓冲区`currentbuffer_`，另一个为预备缓冲区`nextbuffer_`，当前缓冲区写满的时候，就把它送入一个vector集合`buffers_`中，等待写入磁盘，并把预备缓冲`nextbuffer_`移为当前缓冲，然后开始追加日志到`nextbuffer_`并通知后端可以开始写出日志到磁盘。

后端，同样准备好两块空`buffer`：`bufferA` 和 `bufferB`，当条件满足的时候就立即将当前缓冲区也添加到`buffers_`中准备写入，注意，这时候的当前缓冲区已经是预备缓冲区了（此时前端已经没有`buffer`了），这时候立即将一个空`buffer`(`currentbuffer_ = bufferA`)移为当前缓冲区，工作线程开始往这个新的`currentbuffer_`中写。并且将另一个空的`bufferB`替换为 `nextbuffer_`，这样前端始终有一个空闲的预备缓冲区。然后后端开始写`buffers_`集合内部的所有`buffer`至磁盘。

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
# 7.FAQ
**1. 为什么使用EPOLL的LT模式？不是ET模式更加高效吗？**

主要有两个原因：
1. LT模式不会发生漏掉事件的`BUG`。但是 `POLLOUT` 事件不能一开始就关注，否则会出现 `busy loop`,（因为一开始发送缓冲区不满，会一直触发），而应该再write无法写入内核的时候才关注（内核缓冲区已满），将未写入内核缓冲区的数据添加到应用层`outputbuffer` ,直到应用层 `outbuffer`写完，立即停止关注`POLLOUT`事件。
2. 对于非阻塞IO，读写的时候不必等候 `errno = EAGIN`，可以节省系统调用，降低延迟。（注：如果用`ET`模式，读的时候读到 `EAGAIN`,写的时候直到`output buffer`写完或者写到`EAGAIN`）

## 2.智能指针和RAII机制？
**2.1 智能指针是如何使用的?**
主要使用了std标准库的三类指针指针：`std::shared_ptr`、`unique_ptr` 以及 `weak_ptr`。

**2.2 关于shared_ptr<>线程安全性的问题：**

`shared_ptr` 的数据结构有两个数据成员，一个是指向所管理的对象的指针`ptr`，另一个是引用计数指针`ref_count`，`shared_ptr`的引用计数本身是线程安全原子的，但是管理的资源对象不是线程安全的。比如在拷贝一个`shared_ptr`的时候，因为有两个数据成员，数据成员需要顺序拷贝，也就是不会同步（原子）发生，有可能在只拷贝了一个的时候被其他的线程打断了，所以不是线程安全的，需要加锁。
>**shared_ptr**的线程安全级别和内建类型、标准库容器、std::string一样

**2.3 `RAII` 机制：**

**利用C++局部对象自动销毁的特性来控制对象的生命周期**。在构造函数中初始化创建对象，在析构函数中销毁对象。比如我们使用`RAII`机制封装了互斥锁（称为"守卫锁"），在构造函数中初始化锁并加锁，在析构函数中解锁并销毁锁，当守卫锁离开作用域的时候自动会调用析构函数，从而自动解锁和销毁锁对象。`MutexLockGuard`类不可复制，所以拷贝构造函数和拷贝赋值运算符都要定义为`delete`。同样的对条件变量也是这样操作。还有就是`socket`，创建`socket`后要保证`close`。
## 3.线程池实现
线程池实现比较简单，使用的是固定数目的线程池。线程的个数可以由用户自已确定。`N + 1`(CPU密集型)/`2N + 1`(IO密集型)

**实现机制：**

内部维护一个任务队列和线程队列。

内部有两个条件变量`notEmpty_`, `notFull_`，一个是当任务队列为空的时候就会阻塞，不为空的时候通知线程池中的线程去执行任务，当没有线程可用的时候，直接执行，而不用去等待线程池中有线程可用。还有一个变量是线程池任务队列满的时候，就阻塞等待任务被执行后才能添加一个任务。
## 4.简单状态机解析HTTP请求
主要分为四个状态：

* `kExpectRequestLine:`： 当前处于解析请求行的状态；

* `kExpectHeaders` : 当前处于解析请求头部字段的状态；

* `kExpectBody`：当前处于解析请求体的状态；

* `kGotAll` : 全部解析成功。

