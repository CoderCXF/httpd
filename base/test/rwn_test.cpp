/*
 * @Author: your name
 * @Date: 2021-01-06 20:03:05
 * @LastEditTime: 2021-01-07 10:54:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/rwn_test.cpp
 */
#include "../rwn.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    int fd;
    int nbyte;
    fd = open("../source/myfile.txt", O_RDWR);
    if (-1 == fd) {
        perror("open file error");
        return -1;
    }
    char buf[BUFSIZ];
    nbyte = Readn(fd, buf, sizeof(buf));
    Writen(STDOUT_FILENO, buf, nbyte);
    return 0;
}
