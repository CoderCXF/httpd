/*
 * @Author: your name
 * @Date: 2021-03-17 15:10:24
 * @LastEditTime: 2021-03-17 20:45:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/AddrStruct.h
 */
#ifndef ADDRSTRUCT_H
#define ADDRSTRUCT_H
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <stdint.h>

class AddrStruct{
public:
    // once call. only port
    AddrStruct() = default;
    AddrStruct(uint16_t port);
    AddrStruct(const std::string& ip, uint16_t port);
    AddrStruct(struct sockaddr_in addr) 
        : addr_(addr) 
    {
            
    }
    std::string getIP() const;
    uint16_t getPort() const;
    std::string getIPort() const;
    const struct sockaddr* getSocketAddr() const;
    void setSockAddrInet(const struct sockaddr_in& addr) 
    { 
        addr_ = addr;
        // for debug   char* <---> std::string 
        ///
        // printf("setSockAddrInet success\n"); 
        // char buf[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &(addr_.sin_addr.s_addr), buf, sizeof buf);
        // printf("%s\n", buf);
    }

private:
    struct sockaddr_in addr_;
};
#endif