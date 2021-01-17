/*
 * @Author: your name
 * @Date: 2021-01-13 10:20:19
 * @LastEditTime: 2021-01-17 12:41:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/FixedBuffer.h
 */
#ifndef _FixedBuffer_H_
#define _FixedBuffer_H_

#include "Base.h"
#include "string.h"
// #include <types.h>
#include <string.h>

template <int SIZE>
class FixedBuffer{
public:
    FixedBuffer() : cur_(buf) {}
    FixedBuffer(const FixedBuffer& ) = delete;
    FixedBuffer& operator=(const FixedBuffer& ) = delete;
    void append(const char* buf, size_t len) {
        if (avail() >= len) {
            memcpy(cur_, buf, len);
            cur_ += len;
        }
    }

    void append(const std::string& str) {
        size_t len = str.size();
        append(str.c_str(), len);
    }

    inline void add(size_t len) { cur_ += len; }

    inline char* curr() { return cur_; }

    inline int size() { return static_cast<int>(cur_ - buf); }

    inline void reset() { cur_ = buf; }

    void bzero() { 
        std::cout << "bzero" << std::endl;
        std::cout << sizeof(buf) << std::endl;
        // ::bzero(buf, sizeof(buf)); 
        memset(buf, 0, sizeof buf);
        std::cout << "bzero end" << std::endl;

    }

    inline char* data() { return buf; }

    inline size_t avail() { return static_cast<size_t>(end() - buf); }
    
private:
    const char* end() const {
        return buf + sizeof(buf);
    }
    char buf[SIZE];
    char *cur_;
};

#endif