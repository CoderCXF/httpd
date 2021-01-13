/*
 * @Author: your name
 * @Date: 2021-01-06 16:32:31
 * @LastEditTime: 2021-01-11 16:57:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/test/netutil_test.cpp
 */
#include "../Netutil.h"
#include <ctype.h>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999
int main() {
    
    int lfd, cfd;
    int nbyte;
    int i;
    char buf[BUFSIZ];
    lfd = NetUtil::Listen(SERVER_IP, SERVER_PORT);
    if (-1 == lfd) {
        printf("NetUtil::Listen error\n");
        return -1;
    }
    struct sockaddr_in client_addr;
    socklen_t sock_len = sizeof(client_addr);
    cfd = accept(lfd, reinterpret_cast<struct sockaddr *>(&client_addr), &sock_len);
    if (-1 == cfd) {
        perror("accept error");
        close(lfd);
        return -1;
    }
    while (1) {
        nbyte = read(cfd, buf, sizeof(buf));
        if (nbyte < 0 && errno != EINTR) { 
            perror("read error");
            close(lfd);
            close(cfd);
            return -1;
        } else if (nbyte == 0) {  //client close
            break;
        } else {
            for (i = 0; i < nbyte; ++i) {
                buf[i] = toupper(buf[i]);
            }
            write(cfd, buf, nbyte);
            write(STDOUT_FILENO, buf, nbyte);
        }
    }
    close(lfd);
    close(cfd);
    return 0;
}