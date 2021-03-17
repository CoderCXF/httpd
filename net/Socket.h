/*
 * @Author: your name
 * @Date: 2021-03-17 10:17:58
 * @LastEditTime: 2021-03-17 17:44:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Socket.h
 */
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include "AddrStruct.h"

class Socket{
public:
    explicit Socket(int sockfd) :
                sockfd_(sockfd)
    {
        
    }
    ~Socket();
    void Bind(const AddrStruct &addr);
    void Listen();
    int Accept(AddrStruct &perraddr);

private:
    const int sockfd_;
};

