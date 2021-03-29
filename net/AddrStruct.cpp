/*
 * @Author: your name
 * @Date: 2021-03-17 15:10:35
 * @LastEditTime: 2021-03-29 19:10:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/AddrStruct.cpp
 */
#include "AddrStruct.h"
AddrStruct::AddrStruct(uint16_t port) {
    memset(&addr_, 0, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
}

AddrStruct::AddrStruct(const std::string& ip, uint16_t port) {
    memset(&addr_, 0, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    int dst;
    inet_pton(AF_INET, ip.c_str(), (void *)&dst);
    addr_.sin_addr.s_addr = dst;
}
///
//FIXME: char* <---> std::string
// pritnf(%s, (std::string).c_str());
///
std::string AddrStruct::getIP() const {
    // malloc from heap
    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr_.sin_addr.s_addr), buf, sizeof buf);
    return buf;
}

uint16_t AddrStruct::getPort() const {
    return ntohs(addr_.sin_port);
}

///
//FIXME: char* <---> std::string
// pritnf(%s, (std::string).c_str());
///
    std::string AddrStruct::getIPort() const{
    // malloc from heap
    char buf[INET_ADDRSTRLEN];
    size_t size = INET_ADDRSTRLEN;
    inet_ntop(AF_INET, &(addr_.sin_addr.s_addr), buf, sizeof buf);
    size_t end = ::strlen(buf);
    uint16_t port = ntohs(addr_.sin_port);
    snprintf(buf+strlen(buf), size - end, ":%u", port);
    return buf;
}

const struct sockaddr* AddrStruct::getSocketAddr() const {
    return (struct sockaddr*)(&addr_);
}