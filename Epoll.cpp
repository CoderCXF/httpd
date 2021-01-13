/*
 * @Author: your name
 * @Date: 2021-01-07 11:06:42
 * @LastEditTime: 2021-01-08 21:31:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/epoll.cpp
 */
#include "Epoll.h"
Epoll::Epoll() {
    m_epfd = epoll_create1(EPOLL_CLOEXEC);
}
Epoll::~Epoll() {
    close(m_epfd);
}

int Epoll::EpollCtl(int fd, int op, uint32_t event) {
    struct epoll_event tepEvt;
    tepEvt.events = event;
    tepEvt.data.fd = fd;
    if (-1 == epoll_ctl(m_epfd, op, fd, &tepEvt)) {
        perror("epoll_ctl error");
        return -1;
    }
    switch (op)
    {
    case EPOLL_CTL_ADD:
        if (EEXIST == errno) {
            if (0 == epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &tepEvt)) {
                printf("epoll_ctl_add change to epoll_ctl_mod success\n");
                return 0;
            } else {
                perror("epoll_ctl_add change to epoll_ctl_mod fail\n");
                return -1;
            }
        }
        break;
    case EPOLL_CTL_DEL:
        if (ENOENT == errno || EBADF == errno || EPERM == errno) {
            printf("epoll_ctl_add change to epoll_ctl_mod success\n");
            return -1;
        }
        break;
    case EPOLL_CTL_MOD:
        if (ENOENT == errno) {
            if (0 == epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &tepEvt)) {
                printf("epoll_ctl_mod change to epoll_ctl_add success\n");
                return 0;
            } else {
                perror("epoll_ctl_mod change to epoll_ctl_add fail\n");
                return -1;
            }
        }
        break;
    default:
        return -1;
        break;
    }
    return 0;
}

int Epoll::EpollDispatch(vecActiveEventType & activeEvents, int timeout)
{
	int nRet = epoll_wait(m_epfd, &activeEvents[0], activeEvents.capacity(), timeout);
	if (-1 == nRet)
	{
		if (EINTR != errno)
		{
			perror("epoll_wait error");
			return -1;
		}
		return 0;
	}
	
	if (activeEvents.capacity() == nRet)
	{
		// since fd_max limits, it will never overflow
		size_t newSize = 2 * (activeEvents.capacity());
		activeEvents.resize(newSize);
		// DEBUGLOG("%s %s %d, activeEvents Resize to %d", __FILE__, __func__, __LINE__, newSize);
        
	}
	return nRet;
}

