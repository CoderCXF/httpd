/*
 * @Author: your name
 * @Date: 2021-03-17 10:17:58
 * @LastEditTime: 2021-03-27 21:28:44
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
    inline int fd() { return sockfd_; }
    ///
    //NON_BLOCK
    ///
    int     setSocketNonBlocking();
    ///
    // SO_REUSEADDR
    ///
    void    setReuseAddr(bool on = true);
    ///
    // TCP_NODELAY(nagle)
    ///
    void    setTcpNoDelay(bool on = true);
    ///
    // SO_KEEPALIVE
    ///
    void	setKeepAlive(bool on = true);
    ///
private:
    const int sockfd_;
};

