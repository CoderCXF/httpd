/*
 * @Author: your name
 * @Date: 2021-01-14 15:13:40
 * @LastEditTime: 2021-01-15 15:22:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/WriteUtil.cpp
 */
#include "AppendFile.h"

AppendFile::AppendFile(const std::string& fileName) 
    : fp_(fopen(fileName.c_str(), "a+t")),
      writtenBytes_(0) {
    // set buffer(64k Bytes)
    ::setbuffer(fp_, buf_, sizeof buf_);
}

AppendFile::AppendFile(const char* fileName)
    : fp_(fopen(fileName, "a+t")),
      writtenBytes_(0) {
    ::setbuffer(fp_, buf_, sizeof buf_);
}

AppendFile::~AppendFile() {
    fclose(fp_);
}

void AppendFile::append(const char *data, size_t len) {
    size_t n = Write(data, len);
    size_t remain = len - n;
    while (remain > 0)
    {
        size_t x =Write(data + n, remain);
        if (x == 0)
        {
        int err = ferror(fp_);
        if (err)
        {
            fprintf(stderr, "AppendFile::append() failed %s\n", strerror(err));
        }
        break;
        }
        n += x;
        remain = len - n; // remain -= x
    }
  writtenBytes_ += len;
}

void AppendFile::append(const std::string& data, size_t len) {
    std::string &s = const_cast<std::string&>(data);
    append(s.c_str(), len);
}


void AppendFile::flush() {
    fflush(fp_);
}

size_t AppendFile::Write(const char *data, size_t len) {
    return fwrite(data, 1, len, fp_);
}