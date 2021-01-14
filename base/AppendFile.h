/*
 * @Author: your name
 * @Date: 2021-01-14 15:13:51
 * @LastEditTime: 2021-01-14 16:57:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/WriteUtil.h
 */
#include "Base.h"
#include <string.h>

// no thread safety
class AppendFile{
public:
    explicit AppendFile(const std::string& fileName);
    explicit AppendFile(const char *fileName);
    ~AppendFile();
    
    void append(const char* data, size_t len);
    void append(const std::string& data, size_t len);
    void flush();
    long writtenBytes() { return writtenBytes_; }
    
private:

    size_t Write(const char *data, size_t len);
    FILE* fp_;
    long writtenBytes_;
    char buf_[64 * 1024];
};