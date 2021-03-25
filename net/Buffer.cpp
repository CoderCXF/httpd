/*
 * @Author: your name
 * @Date: 2021-03-24 16:51:44
 * @LastEditTime: 2021-03-25 07:12:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Buffer.cpp
 */
#include <errno.h>
#include <sys/uio.h>
#include "Buffer.h"
#include "Socket.h"
const char Buffer::RN[] = "\r\n";

// 结合栈上的空间，避免内存使用过大，提高内存使用率
ssize_t Buffer::readFd(int fd, int* savedErrno)
{
	// 节省一次ioctl系统调用（获取有多少可读数据）
	char extrabuf[65536];
	struct iovec vec[2];
	const size_t writable = writableBytes();
    // 高级IO函数readv和writev
	// 第一块缓冲区
	vec[0].iov_base = begin() + writerIndex_;
	vec[0].iov_len = writable;
	// 第二块缓冲区
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof extrabuf;
	const ssize_t n = ::readv(fd, vec, 2);
	if (n < 0)
	{
		*savedErrno = errno;
	}
	else if (static_cast<size_t>(n) <= writable)	//第一块缓冲区足够容纳
	{
		writerIndex_ += n;
	}
	else		
	{
		writerIndex_ = buffer_.size();
		append(extrabuf, n - writable);
	}
	return n;
}