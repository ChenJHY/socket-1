---
title: TCP三次握手
date: 2017/10/10
categories:
	- 网络编程
tags:
	- 网络编程
	- TCP
---

### TCP三次握手

#### TCP特性

1. 说明：

	>1. TCP提供一种面向连接的、可靠的字节流服务
	>2. 在一个TCP连接中，仅有两方进行彼此通信。广播和多播不能用于TCP
	>3. TCP使用校验和，确认和重传机制来保证可靠传输
	>4. TCP给数据分节(给每一个传送的数据字节都编号)进行排序，并使用累积确认保证数据的顺序不变和非重复
	>5. TCP使用滑动窗口机制来实现流量控制，通过动态改变窗口的大小进行拥塞控制

2. 注意：

	> TCP 并不能保证数据一定会被对方接收到，因为这是不可能的。TCP 能够做到的是，如果有可能，就把数据递送到接收方，否则就（通过放弃重传并且中断连接）通知用户。因此准确说 TCP 也不是 100% 可靠的协议，它所能提供的是数据的可靠递送或故障的可靠通知。

#### TCP首部

1. 说明：

	>1. TCP（Transmission Control Protocol，传输控制协议）是一种面向连接的、可靠的、基于字节流的通信协议，数据在传输前要建立连接，传输完毕后还要断开连接。
	>2. 客户端在收发数据前要使用 connect() 函数和服务器建立连接。建立连接的目的是保证IP地址、端口、物理链路等正确无误，为数据的传输开辟通道。
	>3. TCP建立连接时要传输三个数据包，俗称三次握手（Three-way Handshaking）。

	![TCP/IP](https://raw.githubusercontent.com/McXing/Pictures/master/%E6%8F%A1%E6%89%8B/%E6%8A%A5%E6%96%87%E9%A6%96%E9%83%A8.png "TCP/IP")

	- 解释：

		1. 序号：seq（Sequence）序号，占32位，用来标识从TCP源端向目的端发送的字节流，发起方发送数据时对此进行标记。
	  	2. 确认序号：ack（Acknowledge）序号，占32位，只有ACK标志位为1时，确认序号字段才有效，ack=seq+1。
	  	3. 标志位：每个标志位占用1Bit，共6个，即URG、ACK、PSH、RST、SYN、FIN等，具体含义如下：
	  		1. URG：紧急指针（urgent pointer）有效。
	  		2. ACK(Acknowledge)：确认序号有效。
	  		3. PSH(Push)：接收方应该尽快将这个报文交给应用层。
	  		4. RST(Reset)：重置连接。
	  		5. SYN(Synchronous)：发起一个新连接。
	  		6. FIN(Finish)：释放一个连接。

	 	4. 需要注意的是：
	  		1. 不要将确认序号 ack(表示确认信息)与标志位中的 ACK(为1表示确认有效)搞混了。
	  		2. 确认方 ack等于发起方 seq+1，两端配对。

#### 三次握手

1. 说明：

	>1. 所谓三次握手(Three-way Handshake)，是指建立一个 TCP 连接时，需要客户端和服务器总共发送3个包。
	>2. 三次握手的目的是连接服务器指定端口，建立 TCP连接，并同步连接双方的序列号和确认号，交换 TCP窗口大小信息。在 socket 编程中，客户端执行 connect() 时。将触发三次握手。

	![三次握手](https://raw.githubusercontent.com/McXing/Pictures/master/%E6%8F%A1%E6%89%8B/%E6%8F%A1%E6%89%8B%E4%B8%89%E6%AC%A1.png "三次握手")

2. 解释：
	1. 第一次握手(SYN=1, seq=x):

		>1. 客户端发送一个 TCP的 SYN 标志位置1的包，指明客户端打算连接的服务器的端口(请求同步)，并选择序号 seq=x，表明传送数据时的第一个数据字节的序号是 x。(seq是个随机值)
		>2. 发送完毕后，客户端进入 SYN_SEND 状态。

	2. 第二次握手(SYN=1, ACK=1, seq=y, ack=x+1):

		>1. 服务器的TCP收到连接请求报文段后，如同意，则发回确认包(ACK)应答。即 SYN 标志位和 ACK 标志位均为1。服务器端选择自己 ISN 序列号(随机值y)，放到seq 域里，同时将确认序号ack设置为客户的 ISN 加1，即 x+1(即回复对方确认收到了序列号为x开始的包，且希望下次的数据从x+1的位置开始)。
		>2. 发送完毕后，服务器端进入 SYN_RCVD 状态。

	3. 第三次握手(ACK=1，ack=y+1，seq=x+1)

		>1. 客户端收到此报文段后再次发送确认包(ACK)，SYN 标志位为0，ACK 标志位为1，并且把服务器发来 ACK 的序号字段+1，放在确定字段ack中发送给对方，并且告诉服务器自己的seq=x+1
		>2. 发送完毕后，客户端进入 ESTABLISHED 状态，当服务器端接收到这个包时，也进入 ESTABLISHED 状态，TCP握手结束。

3. 问题解决：

	1. 为什么初始seq要取随机值呢?

		>1. 首先这个随机值并不是随机的，而是可以预测的。
		>2. 其次,它一般基于时钟产生,在rfc793中讲到**"The generator is bound to a (possibly fictitious) 32 bit clock whose low order bit is incremented roughly every 4 microseconds."**大概就是每 4ms加1,这样`ISN`循环一次需要4.55小时,而一个连接中传送的 `segment`在网络中存在的最大时间小于4.55小时。如我们需要 `segment`的`seq`为1,则至少4.55小时前的`segment`其`seq`才可能为1,而`segment`不可能在网络中存在4.55小时,所以如果我们接收到`seq`为 1 的`segment`则必然是我们需要的`segment`(恶意攻击除外^_^)。
		>3. 这样可以防止上一次连接产生的`segment`被本次连接错误接收，同时也可以从某种程度上防止其它用户恶意攻击。

	2. 为什么要三次握手，而不是两次握手或者四次握手

		>1. 为了防止已失效的连接请求报文段突然又传送到了服务端，因而产生错误或者说是解决网络中存在延迟的重复分组的问题。
		>>1. `已失效的连接请求报文段`的产生在这样一种情况下：`client`发出的第一个连接请求报文段并没有丢失，而是在某个网络结点长时间的滞留了，以致延误到连接释放以后的某个时间才到达`server`。
		>>2. 本来这是一个早已失效的报文段。但`server`收到此失效的连接请求报文段后，就误认为是`client`再次发出的一个新的连接请求。于是就向`client`发出确认报文段，同意建立连接。
		>>3. 假设不采用“三次握手”，那么只要`server`发出确认，新的连接就建立了。由于现在`client`并没有发出建立连接的请求，因此不会理睬`server`的确认，也不会向`server`发送数据。但`server`却以为新的运输连接已经建立，并一直等待`client`发来数据。这样，`server`的很多资源就白白浪费掉了。

		>2. 因为在上述的三次握手中已经确认并建立了连接，那么就没有必要去进行第四次握手，这样可以节约资源。

4. 补充：

	1. 半链接队列
	>1. 半连接队列：
	>>1. 在三次握手协议中，服务器维护一个半连接队列，该队列为每个客户端的SYN包开设一个条目（服务器端在接收到SYN包时，就已经创建了`request_sock`结构，存储在半连接队列中），该条目表明服务器已收到SYN 包，并向客户发出确认，正在等待客户的确认包。这些条目所识别的连接在服务器处于`SYN_RCVD`状态，当服务器收到客户端的确认包时，删除该条目，服务器进入`ESTABLISHED`状态。
	>>2. 未连接队列的大小为`max（64， /proc/sys/net/ipv4/tcp_max_syn_backlog）`，也就是可以在说未连接队列的大小可以在`/proc/sys/net/ipv4/tcp_max_syn_backlog`中修改配置，如果服务器经常出现过载，可尝试增加这个数字`（tcp_max_syn_backlog）`。

		>2. 半连接（half-open connect）存活时间：
		>>1. 是指半连接队列的条目存活的最大时间，也即服务器端从收到`SYN`包确认这个报文无效的最长时间，该时间值是所有重传请求包的最长等待时间总和，有时我们也称半连接存活时间为`Timeout`时间、`SYN_RCVD`存活时间。


	2. 完全连接队列

		>1. 完全连接队列：
		>>1. 在第三次握手时，当`Server`接收到`ACK`报之后，会进入一个新的叫ACCEPT的队列，该队列的长度为`min（backlog， /proc/sys/net/core/somaxconn）`，默认情况下，`somaxconn`的值为128，表示最多有128个ESTABLISHED的连接等待accept()，而`backlog`值则是由`int listen（int sockfd， int backlog）`中的第二个参数指定（指定的`backlog`与半连接状态的`backlog`无关系），`listen`里面的`backlog`可以由我们的程序去指定。
		>>2. 当服务器绑定、监听某个端口后，这个端口的SYN队列和ACCEPT队列就建立好了。


		>2. 半连接队列未满但是全连接队列已满：
		>>1. 客户端发出`SYN`分节，服务器端收下`SYN`分节并向客户端发送`SYN+ACK`，客户端收到服务器端`SYN+ACK`后，成为`ESTABLISHED`状态，并向服务器端发送第三次握手`ACK`，服务器端收到`ACK`后发现全连接队列已满，默认情况下服务器端什么也不做，状态依然是`SYN_RCVD`，此时`ListenOverflows+1`，
		>>2. 同时服务器端通过对目录：`/proc/sys/net/ipv4/tcp_abort_on_overflow`进行修改来决定如何返回，`0`表示直接丢弃，`1`表示发送`RST`通知客户端（`ListenOverflows默认为0`，当全连接队列超过上限时，`ListenOverflow+1`），客户端会重传`SYN`（客户端第一次握手发起的请求）和`ACK`（客户端第三次握手期间发出的确认），并且内核会限制`SYN`队列的处理速度，如果在`SYN`队列中收到太多的`SYN`，服务器端将会丢弃一些，这样丢弃的`SYN`对应的客户端需要重发`SUN`包，当达到一定的阈（yù）值（可以理解为连接被动打开方的确认连接的应答最大重试数，即对于一个新建连接，内核需要发送多少`SYN`连接请求才决定放弃，阈值可以`/proc/sys/net/ipv4/tcp_synack_retries`中修改），客户端与服务器断开连接，服务器删除客户端在半连接队列中的`SYN`分节。


		>3. 不论全连接满没满，若半连接队列已满：
		>>1. 不开启`tcp_syncookies`的时候，服务器端会丢弃新来的`SYN`包，而客户端多次重发`SYN包`得不到响应而返回超时错误（`connection time out`）。但是当服务器端开启了`tcp_syncookies = 1`，那么`SYN`半连接队列就没有逻辑上的最大值了，并且`/proc/sys/net/ipv4/tcp_max_syn_backlog`设置的值也会被忽略。

5. SYN攻击

	1. 在三次握手过程中，Server发送SYN-ACK之后，收到Client的ACK之前的TCP连接称为半连接（half-open connect），此时Server处于`SYN_RCVD`状态，当收到`ACK`后，Server转入`ESTABLISHED`状态。
	>1. `SYN`攻击就是Client在短时间内伪造大量不存在的IP地址，并向Server不断地发送`SYN`包，Server回复确认包，并等待Client的确认，由于源地址是不存在的，因此，Server需要不断重发直至超时，这些伪造的`SYN`包将长时间占用未连接队列，导致正常的`SYN`请求因为队列满而被丢弃，从而引起网络堵塞甚至系统瘫痪。
	>2. `SYN`攻击时一种典型的`DDOS`攻击，检测SYN攻击的方式非常简单，即当Server上有大量半连接状态且源IP地址是随机的，则可以断定遭到`SYN`攻击了，使用如下命令可以让之现行：`#netstat -nap | grep SYN_RECV`

	2. SYN攻击处理
	>1. 减少SYN-ACK数据包的重发次数:`sysctl -w net.ipv4.tcp_synack_retries=3`,`sysctl -w net.ipv4.tcp_syn_retries=1`
	>2. 使用SYN cookie技术:`sysctl -w net.ipv4.tcp_syncookies=1`
	>3. 增加半连接队列（默认为1024）:`sysctl -w net.ipv4.tcp_max_syn_backlog=2048`
	>4. 限制SYN并发数：`iptables -A INPUT -p tcp --syn -m limit --limit 1/s -j ACCEPT --limit 1/s`
