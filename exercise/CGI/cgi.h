//
// Created by roux on 18-2-24.
//

#ifndef EXERCISE_CGI_H
#define EXERCISE_CGI_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <sys/epoll.h>
#include <csignal>
#include <sys/wait.h>
#include <sys/stat.h>

#include "../include/processPool.h"

/*用于处理客户CGI请求的类，可以作为processpool类的模板参数*/
class cgi_conn
{
public:
	cgi_conn(){}
	~cgi_conn(){}

	void init (int epollfd, int sockfd, const sockaddr_in &client_addr);
	void process();

private:
	/*读缓冲区的大小*/
	static const int BUFFER_SIZE = 1024;
	static int m_epollfd;
	int m_sockfd;
	sockaddr_in m_address;
	char m_buf[BUFFER_SIZE];
	/*标记读缓冲区已经读入的客户数据的最后一个字节的下一个位置*/
	int m_read_idx;
};

int cgi_conn::m_epollfd = -1;

/*初始化客户连接，清空读缓冲区*/
void cgi_conn::init(int epollfd, int sockfd, const sockaddr_in &client_addr)
{
	m_epollfd = epollfd;
	m_sockfd = sockfd;
	m_address = client_addr;
	memset(m_buf, '\0', BUFFER_SIZE);
	m_read_idx = 0;
}

void cgi_conn::process()
{
	int idx = 0;
	int ret = -1;

	/*循环读取和分析客户数据*/
	while (true) {
		idx = m_read_idx;
		ret = recv(m_sockfd, m_buf + idx, BUFFER_SIZE - 1 - idx, 0);

		/*如果读失败，则关闭连接。但若是暂无数据可读，则退出循环*/
		if (ret < 0) {
			if (errno != EAGAIN) {
				removefd(m_epollfd, m_sockfd);
			}
			break;
		} else if (ret == 0) {
			/*如果对方关闭，则服务器也关闭*/
			removefd(m_epollfd, m_sockfd);
			break;
		} else {
			m_read_idx += ret;
			printf("user content is %s\n", m_buf);

			/*如果遇到"\r\n"，则开始处理客户请求*/
			for (; idx < m_read_idx; ++idx) {
				if ((idx >= 1) && (m_buf[idx-1] == '\r') && (m_buf[idx] == '\n')) {
					break;
				}
			}

			/*如果没有遇到"\r\n"，则需要读取更多的客户数据*/
			if (idx == m_read_idx) {
				continue;
			}
			m_buf[idx-1] = '\0';

			char* file_name = m_buf;

			/*判断客户要运行的CGI程序是否存在*/
			if (access(file_name, F_OK) == -1) {
				removefd(m_epollfd, m_sockfd);
				break;
			}

			/*创建子进程来执行CGI程序*/
			ret = fork();
			if (ret == -1) {
				removefd(m_epollfd, m_sockfd);
				break;
			} else if (ret > 0) {
				/*父进程关闭连接*/
				removefd(m_epollfd, m_sockfd);
				break;
			} else {
				/*子进程将标准输出定向到m_sockfd，并执行CGI程序*/
				close(STDOUT_FILENO);
				dup(m_sockfd);
				execl(m_buf, m_buf, NULL);
				exit(0);
			}
		}

	}
}


#endif //EXERCISE_CGI_H
