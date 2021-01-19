/*
 * @Author: your name
 * @Date: 2021-01-16 11:51:59
 * @LastEditTime: 2021-01-19 15:25:34
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/AsyncLog.h
 */
#ifndef _ASYNCLOG_H_
#define _ASYNCLOG_H_

#include "Base.h"
#include "Condition.h"
#include "FixedBuffer.h"
#include "LogFile.h"
#include "Thread.h"
#include <string>
#include <vector>
#include <memory>

const static int kFixedBufferSize = 4096 * 1024; // 4M Bytes

class AsyncLog{
    typedef FixedBuffer<kFixedBufferSize> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVec;
    typedef BufferVec::value_type BufferPtr;

public:
    AsyncLog(const std::string& basename);
    ~AsyncLog() = default;
    // Mang threads would append : Front end(app)
    void append(const char* msg, size_t len);
    void start();
    void stop();
    
private:
    // Only one thread to waite file: Back end
    void writeLogFileThread();
    
    std::string basename_;
    bool running_;
    Thread thread_;
    Mutex mutex_;
    Condition cond_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    std::vector<BufferPtr> buffers_;
    std::unique_ptr<LogFile> g_logfile_;
    const static int kFlushInterval = 3;
};

#endif