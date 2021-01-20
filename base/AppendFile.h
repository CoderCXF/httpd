/*
 * @Author: your name
 * @Date: 2021-01-14 15:13:51
 * @LastEditTime: 2021-01-19 15:55:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/WriteUtil.h
 */
#include "Base.h"
#include <string.h>

// no thread safety
const static int kBufferSize = 64 * 1024;

class AppendFile{
public:
    explicit AppendFile(const std::string& fileName);
    explicit AppendFile(const char *fileName);
    ~AppendFile();
    
    void append(const char* data, size_t len);
    void append(const std::string& data, size_t len);
    void WriteUnbuffered(const char * data, size_t writeSize);
    void flush();
    long writtenBytes() { return writtenBytes_; }
    
private:

    size_t Write(const char *data, size_t len);
    FILE* fp_;
    long writtenBytes_;
    size_t mpos_;
    char buf_[kBufferSize];
};