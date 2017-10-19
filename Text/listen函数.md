###网络编程--listen函数

####1.分析

1. 问题：

    我们知道在网络编程中，listen接口是用来建立socket监听的，其参数只有两个。它的第二个参数是什么？三次握手中的半连接、完全连接在listen监听中充当着什么角色？

2. 函数原型:`int listen(int sockfd, int backlog);`

    1. 当socket函数创建一个套接字时，它被假设为一个主动套接字，也就是说，它是一个将调用`connect`发起连接的客户套接字。`listen`函数把一个未连接的套接字转换成一个被动套接字，指示内核应接受指向该套接字的连接请求。根据TCP状态转换图，调用`listen`导致套接字从CLOSED状态转换成LISTEN状态。

3. 参数

    1. sockfd：成功创建的TCP套接字

    2. backlog：定义内核监听队列的最大长度。APUE中指出，`backlog`只是一个提示，具体的数值实际上由系统决定。在内核版本2.2之前的Linux中，`backlog`参数是指所有处于半连接状态(SYN\_RCVD)和完全连接状态(ESTABLISHED)的socket的上限。但自内核版本2.2之后，它只表示处于完全连接状态的socket的上限，处于半连接状态的socket的上限则由`/proc/sys/net/ipv4/tcp_max_syn_backlog`内核参数定义。`backlog`参数的典型值是5(4.2BSD支持的最大值)。

4. 监听队列

    - 内核为任何一个给定的监听套接字维护两个队列(详见:[TCP三次握手](http://blog.csdn.net/xingerr/article/details/72834303))：

    > 1. 未完成连接队列，每个这样的SYN分节对应其中一项：已由某个客户发出并到达服务器，而服务器正在等待完成相应的TCP三次握手过程。这些套接字处于SYN_RCVD状态。

    > 2. 已完成连接队列，每个已完成TCP三次握手过程的客户对应其中一项。这些套接字处于ESTABLISHED状态。

    > 3. 解释

![explain](https://raw.githubusercontent.com/McXing/Pictures/master/listen/%E6%8F%A1%E6%89%8B1.png "explain")

####2.示例

- 自己手动测试过，与原文有些出入
- 服务端代码如下：

```c
    #include<sys/socket.h>
    #include<netinet/in.h>
    #include<arpa/inet.h>
    #include<signal.h>
    #include<unistd.h>
    #include<stdlib.h>
    #include<assert.h>
    #include<stdio.h>
    #include<string.h>

    static bool stop = false;
    /*SIGTERM 信号的处理函数，触发时结束主进程中的循环*/
    static void handle_term(int sig)
    {
        stop = true;
    }

    int main(int argc, char* argv[])
    {
        signal(SIGTERM, handle_term);

        if(argc <= 3)
        {
            printf("usage: %s ip_address port_number backlog\n", basename(argv[0]));
            return 1;
        }
        const char* ip = argv[1];
        int port = atoi(argv[2]);
        int backlog = atoi(argv[3]);

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        assert(sock >= 0);

        struct sockaddr_in address;
        bzero(&address, sizeof(address));
        address.sin_family = AF_INET;
        inet_pton(AF_INET, ip, &address.sin_addr);
        address.sin_port = htons(port);

        int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
        assert(ret != -1);

        ret = listen(sock, backlog);
        assert( ret != -1);

        /*循环等待连接，直到有SIGTERM信号将它中断*/
        while(!stop)
        {
            sleep(1);
        }
        /*关闭socket*/
        close(sock);
        return 0;
    }
```

- 客户端端代码如下：

```c
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <signal.h>
  #include <unistd.h>
  #include <stdlib.h>
  #include <assert.h>
  #include <stdio.h>
  #include <string.h>

  static bool stop = false;
  /*SIGTERM 信号的处理函数，触发时结束主进程中的循环*/
  static void handle_term(int sig)
  {
      stop = true;
  }

  int main(int argc, char* argv[])
  {
      signal(SIGTERM, handle_term);

      if(argc <= 3)
      {
          printf("usage: %s ip_address port_number count\n", basename(argv[0]));
          return 1;
      }
      const char* ip = argv[1];
      int port = atoi(argv[2]);
      int count= atoi(argv[3]);

      struct sockaddr_in server_address;
      bzero(&server_address, sizeof(server_address));
      server_address.sin_family = AF_INET;
      inet_pton(AF_INET, ip, &server_address.sin_addr);
      server_address.sin_port = htons(port);

      int sockfd[count];
      for(int i = 0; i < count; i++)
      {
          sockfd[i] = socket(AF_INET, SOCK_STREAM,0);
          assert(sockfd[i] >= 0);
          if(connect(sockfd[i],(struct sockaddr*)&server_address,sizeof(server_address)) < 0)
          {
              printf("connection failed\n");
          }
      }

      /*循环等待连接，直到有SIGTERM信号将它中断*/
      while(!stop)
      {
          sleep(1);
      }
      for(int j = 0; j < count; j++)
      {
          close(sockfd[j]);
      }

      return 0;
  }
```

---

1.服务端程序(名为listen)接收3个参数：IP地址、端口号和`backlog`值。我们在Centos服务器上(2.6.32内核)运行：

`$./listen 127.0.0.1 6666 5`

2.客户端程序(名为connect)接收3个参数：IP地址、端口和count连接数。我们在Linux另外一个终端上运行（建立9个连接）：

`$./connect localhost 6666 9`

3.通过netstat命令查看listen监听队列内容

`$netstat -nt|grep 6666`

```c
tcp        0      0 127.0.0.1:6666              127.0.0.1:57636    SYN_RECV
tcp        0      0 127.0.0.1:6666              127.0.0.1:57634    SYN_RECV
tcp        0      0 127.0.0.1:6666              127.0.0.1:57632    SYN_RECV
tcp        0      0 127.0.0.1:6666              127.0.0.1:57620    ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:57630    ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:57626    ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:57628    ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:57622    ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:57624    ESTABLISHED
```

- 其中处于LISTEN状态的为建立监听的服务端程序，在监听队里中，处于ESTABLISHED状态的连接有6个`（backlog+1）`，其他的连接都处于SYN_RCVD状态。我们改变服务端与客户端程序的第3个参数并重新运行之，能发现同样的规律，即完成连接最多有`(backlog+1)`个。在不同的系统上，运行结果会有些差别，不过监听队列中已完成连接的上限通常比`backlog`值略大。

4.半连接状态socket上限

> 1. 我们先来了解下syncookies功能，它控制着系统内核ipv4参数修改是否生效。如果syncookies是启动的，那么ipv4内核参数修改无效。
>> 1. syncookies是在内核编译的时候设置的，我们通过cat查看是否启动：`$ cat /proc/sys/net/ipv4/tcp_syncookies`
>>2. 通过echo可禁用syncookies：`$echo 0 > /proc/sys/net/ipv4/tcp_syncookies`
>>3. 接下来，我们通过cat查看内核半连接状态的socket上限是多少：`$cat /proc/sys/net/ipv4/tcp_max_syn_backlog`
>>4. 通过echo修改该内核半连接socket上限为2`$echo 2 > /proc/sys/net/ipv4/tcp_max_syn_backlog`
>>5. 此时我们运行服务端程序`$./listen 127.0.0.1 6666 5`
>>6. 打开另外一个终端运行客户端程序`$./connect localhost 6666 10`
>>7. 通过netstat命令查看listen监听队列内容`$netstat -nt|grep 6666`

```c
tcp        0      0 127.0.0.1:6666              127.0.0.1:57738    SYN_RECV
tcp        0      0 127.0.0.1:6666              127.0.0.1:57740    SYN_RECV
tcp        0      0 127.0.0.1:6666              127.0.0.1:57742    SYN_RECV
tcp        0      0 127.0.0.1:6666              127.0.0.1:57734    ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:57732    ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:57728    ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:57726    ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:57736    ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:57730    ESTABLISHED
```

- 此时处于半连接(SYN\_RECV)的套接字共有3个`(tcp_max_syn_backlog + 1)`，处于已完成连接(ESTABLISHED)的套接字共6(一个处于SYN_SENT)个`(backlog + 1)`。

5.完全连接状态socket上限

>1. 通过cat查看内核连接状态的socket上限:`$cat /proc/sys/net/core/somaxconn`
>2. 通过echo修改该内核完全连接socket上限为2:`$echo 2 > /proc/sys/net/core/somaxconn`
>3. 运行服务器程序:`$./listen 127.0.0.1 6666 5`
>4. 打开另外一个终端运行客户端程序:`$./connect localhost 6666 10`  #建立10个连接
>5. 通过netstat命令查看listen监听队列内容:`$netstat -nt|grep 6666`

```c
tcp        0      0 127.0.0.1:6666              127.0.0.1:38618     SYN_RECV
tcp        0      0 127.0.0.1:6666              127.0.0.1:38614     SYN_RECV
tcp        0      0 127.0.0.1:6666              127.0.0.1:38616     SYN_RECV
tcp        0      0 127.0.0.1:6666              127.0.0.1:38610     ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:38608     ESTABLISHED
tcp        0      0 127.0.0.1:6666              127.0.0.1:38612     ESTABLISHED
```

- 这个是我第二次修改的时候测的(不冲突)。
- 我们不难发现处于半连接状态的连接共3个`(tcp_max_syn_backlog + 1)`，处于已完成状态的连接也共3个`(tcp_max_syn_backlog + 1 )`，此时有些迷惑,backlog与系统内核参数somaxconn到底有什么关系？完全连接状态socket上限究竟由谁控制？

####3.listen源码
1.listen的源码入口位于`socket.c`，具体代码如下：

![listen](https://raw.githubusercontent.com/McXing/Pictures/master/listen/listen1.jpg "scoket")

2.AF_INEF协议族(af_inet.c)的listen实现函数为`inet_listen`，代码如下：

![listen](https://raw.githubusercontent.com/McXing/Pictures/master/listen/listen2.png "scoket")

3.接下来进入`inet_csk_listen_start`，代码如下：

![listen](https://raw.githubusercontent.com/McXing/Pictures/master/listen/listen3.png "scoket")

- 通过源码剖析，我们可看出在listen第二个参数backlog不超过系统限制的最大值somaxconn时，内核直接使用其作为已完成连接队列的最大长度。如果超过了，那么系统将采用somaxconn作为已完成连接队列的最大长度。

4.总结

1. listen建立监听后，会创建两个队列：3次握手完成连接队列(ENSTABLISHED)，3次握手半连接队列(SYN_RCVD)。

2. 当syn队列满的情况(tcp_abort_on_overflow)

    >1. 对于`SYN`半连接队列的大小是由（`/proc/sys/net/ipv4/tcp_max_syn_backlog`）这个内核参数控制的，有些内核似乎也受 listen 的`backlog`参数影响，取得是两个值的最小值。当这个队列满了，不开启`syncookies`的时候，Server 会丢弃新来的`SYN`包，而Client端在多次重发SYN包得不到响应而返回（connection time out）错误。但是，当 Server 端开启了`syncookies=1`，那么`SYN`半连接队列就没有逻辑上的最大值了，并且`/proc/sys/net/ipv4/tcp_max_syn_backlog`设置的值也会被忽略。
    >2. Client端在多次重发`SYN`包得不到响应而返回connection time out错误
    >3. `netstat -s | grep LISTEN`    
     144 SYNs to LISTEN sockets ignored

3. 当accept队列满的情况

    >1. 当accept队列满了之后，即使client继续向server发送ACK的包，也会不被响应，此时ListenOverflows+1，同时server通过/proc/sys/net/ipv4/tcp_abort_on_overflow来决定如何返回，0表示直接丢弃该ACK，1表示发送RST通知client；相应的，client则会分别返回read timeout 或者 connection reset by peer。
    >2. `[root@Paul opt]# netstat -s | grep listen`   
      144 times the listen queue of a socket overflowed
    >3. accept队列满了，对 syn队列也有影响，在代码 net/ipv4/tcp_ipv4.c ：

    ```
    int tcp_v4_conn_request(struct sock *sk, struct sk_buff *skb)
  {
    /*tcp_syncookies为2 进行syn cookie
      tcp_syncookies为1 且request队列满了 进行syn cookie处理
      tcp_syncookies为0 且request队列满了 将该syn报文drop掉
    */
    if ((sysctl_tcp_syncookies == 2 ||
         inet_csk_reqsk_queue_is_full(sk)) && !isn) {
        want_cookie = tcp_syn_flood_action(sk, skb, "TCP");
        if (!want_cookie)
            goto drop;
    }

    /* Accept backlog is full. If we have already queued enough
     * of warm entries in syn queue, drop request. 
     */
    if (sk_acceptq_is_full(sk) && inet_csk_reqsk_queue_young(sk) > 1) {
        NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENOVERFLOWS);
        goto drop;
  }
    ```
    >4. accept 队列大多数情况下会比较小，所以会出现`SYN` 队列没有满，而 ACCEPT 队列满了的情况，此时会按照`tcp_aborton_overflow`来决定直接丢弃，还是返回拒绝`RST`。 而如果启用了`syncookies`，那么`syncookies`会开启，限制`SYN`包进入的速度。
    >>1. 当系统丢弃最后的 `ACK`，而系统中还有一个 `net.ipv4.tcp_synack_retries` 设置时，Linux 会重新发送 `SYN ACK `包。而客户端收到多个`SYN ACK`包，则会认为之前的 `ACK `丢包了。于是促使客户端再次发送 `ACK` ，在 accept 队列有空闲的时候最终完成连接。若 accept 队列始终满员，则最终客户端收到 `RST` 包。

4. 半连接队列的上限由`/proc/sys/net/ipv4/tcp_max_syn_backlog`控制。

5. 在listen第二个参数backlog小于`/proc/sys/net/core/somaxconn`时，完成连接队列的最大值由backlog控制，否则由`/proc/sys/net/core/somaxconn`控制。

6. 我们可以通过修改`/proc/sys/net/ipv4/tcp_syncookies`参数决定对内核参数修改是否生效。

- 一个看似简单的listen接口，想要真正掌握也不简单。需要我们编写代码进行试验、分析TCP连接状态图、调优内核参数、剖析Linux内核源码...

####4.参考

- 原文链接：
<http://www.charmingzhou.com/listen.html>   
<https://segmentfault.com/a/1190000008224853>