/*
 * @Author: your name
 * @Date: 2021-01-16 11:51:59
 * @LastEditTime: 2021-01-16 12:09:23
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
#include <string>
#include <memory>

const static int kFixedBufferSize = 4096;

class AsyncLog{
    typedef FixedBuffer<kFixedBufferSize> Buffer;
    typedef std::unique_ptr<Buffer> BufferPtr;

public:
    AsyncLog(const string& basename);
    ~AsyncLog();
    // Front end(app)
    void append(const string& msg, size_t len);

    
private:
    // Back end
    void writeLogFileThread();
    
    std::string basename_;
    Mutex mutex_;
    Condition cond_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    std::vector<BufferPtr> buffers_
    std::unique_ptr<LogFile> g_logfile_

};

#endif