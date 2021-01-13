// /*
//  * @Author: your name
//  * @Date: 2021-01-07 15:45:03
//  * @LastEditTime: 2021-01-07 19:30:29
//  * @LastEditors: Please set LastEditors
//  * @Description: In User Settings Edit
//  * @FilePath: /WebServer/test/epoll_test.cpp
//  */
// #include "../epoll.h"
// #include <arpa/inet.h>
// #define PORT 9999
// int main() {
//     typedef std::vector<struct epoll_event> vecActiveEventType;
//     vecActiveEventType vecActive;
//     int sockfd, confd;
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (-1 == sockfd) {
//         perror("socket error");
//         return -1;
//     }
//     struct sockaddr_in sock_addr;
//     sock_addr.sin_family = AF_INET;
//     sock_addr.sin_port = htons(PORT);
//     if (1 != inet_pton(AF_INET, "127.0.0.1", &sock_addr.sin_addr.s_addr)) {
//         perror("inet_pton error");
//         return -1;
//     }
//     if (-1 == bind(sockfd, (struct sockaddr *)&sock_addr, static_cast<socklen_t>(sizeof(sock_addr)))) {
//         perror("bind error");
//         return -1;
//     }
//     if (-1 == listen(sockfd, 128)) {
//         perror("listen error");
//         return -1;
//     }
//     Epoll epfd;
//     if (-1 == epfd.EpollCtl(sockfd, EPOLL_CTL_ADD, EPOLLIN)) {
//         printf("epfd.EpollCtl error\n");
//         return -1;
//     }
//     while (1) {
//         if (-1 == epfd.EpollDispatch(vecActive, -1)) {
//             printf("epfd.EpollDispatch error");
//             return -1;
//         }
//     }
//     return 0;
// }
#include <iostream>
#include <vector>
#include <unistd.h>

#include "../Epoll.h"
#include "../base/log/thread.h"

using namespace std;

typedef Epoll::vecActiveEventType vecActiveEventType;

int nFdpairs = 10;
Epoll epoller;
vector<vector<int>> fdpairs(nFdpairs, vector<int>(2));

void WritePipe();

int main()
{
	vecActiveEventType vecActive;
	vecActive.resize(1);
	
	for (int i = 0; i < nFdpairs; ++i)
	{
		pipe(static_cast<int *>(&fdpairs[i][0]));
		// fd[0] for read, fd[1] for write
		// Fix me, set fd to non-blocking, Epoll work in ET model should togther with non-blocking fd, otherwise, read/write fd will be blocked.
		epoller.EpollCtl(fdpairs[i][0], EPOLL_CTL_ADD, EPOLLIN);
		epoller.EpollCtl(fdpairs[i][0], EPOLL_CTL_ADD, EPOLLIN | EPOLLOUT);
		//epoller.EpollCtl(fdpairs[i][0], EPOLL_CTL_DEL, EPOLLOUT);
	}
	
	Thread thread(WritePipe);
	thread.Start();
	
	while(true)
	{
		int nActive = epoller.EpollDispatch(vecActive, 3000);
		cout << nActive << " active events" << endl;
		for (int i = 0; i < nActive; ++i)
		{
			cout << __func__ << " " << __LINE__ << "active fd = " << vecActive[i].data.fd << endl;
			char buff[1024];
			read(vecActive[i].data.fd, buff, 4);
		}
	}
	
	for (int i = 0; i < nFdpairs; ++i)
	{
		close(fdpairs[i][0]);
		close(fdpairs[i][1]);
	}
	thread.Join();
}

void WritePipe()
{
	int deleteNum = 0;
	while(true)
	{
		sleep(5);
		for (int i = 0; i < 10; ++i)
		{
			int n = write(fdpairs[i][1], "xiao", 4);
			if (0 > n)
			{
				perror("write error");
			}
		}
		
		if (deleteNum < nFdpairs)
		{
			epoller.EpollCtl(fdpairs[deleteNum][0], EPOLL_CTL_DEL, EPOLLIN | EPOLLOUT);
		}
		++deleteNum;
	}
}