/*
 * @Author: cxf
 * @Date: 2021-01-06 11:12:06
 * @LastEditTime: 2021-01-08 21:28:02
 * @LastEditors: Please set LastEditors
 * @Description: socket function
 * @FilePath: /WebServer/netutil.c
 */
#include "Netutil.h"
int NetUtil::Listen(const char *serverIP, uint16_t port) {
    int serv_fd;
    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == serv_fd) {
        perror("create socket error");
        return -1;
    }
    SetReuseAddr(serv_fd, 1);
    // SetNonblock(serv_fd);
    struct sockaddr_in serverAddr;
    SetAddr(serverIP, port, serverAddr);
    if (-1 == bind(serv_fd, reinterpret_cast<struct sockaddr *>(&serverAddr), static_cast<socklen_t>(sizeof(serverAddr)))) {
        perror("bind error");
        close(serv_fd);
        return -1;
    }
    if (-1 == listen(serv_fd, 128)) {
        perror("listen error");
        close(serv_fd);
        return -1;
    }
    return serv_fd;
}
int NetUtil::Connect(const char *serverIP, uint16_t port) {
    int client_fd;
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == client_fd) {
        perror("socket errror");
        return -1;
    }
    struct sockaddr_in serverAddr;
    SetAddr(serverIP, port, serverAddr);
    if (-1 == connect(client_fd, reinterpret_cast<struct sockaddr *>(&serverAddr), static_cast<socklen_t>(sizeof(serverAddr)))) {
        perror("connect error");
        close(client_fd);
        return -1;
    }
    return client_fd;
}
void NetUtil::SetReuseAddr(int fd, int opt) {
    opt = 1;
    if (0 != setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, static_cast<socklen_t>(sizeof(opt))))
	{
		perror("set reuse address error");
        return;
	}
}
void NetUtil::SetNonblock(int fd)
{
	int ret = fcntl(fd, F_GETFL);
	ret = ret | O_NONBLOCK;
	if (-1 == fcntl(fd, F_SETFL, ret))
	{
		perror("set non block error");
        return;
	}
}
void NetUtil::SetAddr(const char * serverIp, uint16_t port, struct sockaddr_in & sockAddr)
{
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(port);
	if (1 != inet_pton(AF_INET, serverIp, &sockAddr.sin_addr))
	{
		perror("inet_pton error");
        return;
	}
}