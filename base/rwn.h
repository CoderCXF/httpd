/*
 * @Author: your name
 * @Date: 2021-01-06 17:53:28
 * @LastEditTime: 2021-01-06 20:20:00
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/test/base/readn.h
 */
#ifndef _RWN_H_
#define _RWN_H_
#include <iostream>
#include <unistd.h>
#include <errno.h>

ssize_t Readn(int fd, void *ptr, size_t nbytes);
void Writen(int fd, void *ptr, ssize_t nbytes);

#endif