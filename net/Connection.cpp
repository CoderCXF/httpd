/*
 * @Author: your name
 * @Date: 2021-03-20 14:34:41
 * @LastEditTime: 2021-03-21 07:57:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Connection.cpp
 */
#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
Connection::Connection(EventLoop* loop,
                const std::string& name,
                int sockfd,
                const AddrStruct& localAddr,
                const AddrStruct& peerAddr)
        :loop_(loop),
         connName(name),
         socket_(new Socket(sockfd)),
         channel_(new Channel(loop_, sockfd)),
         localAddr_(localAddr),
         peerAddr_(peerAddr)
{

}