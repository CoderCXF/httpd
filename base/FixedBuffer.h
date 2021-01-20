/*
 * @Author: your name
 * @Date: 2021-01-13 10:20:19
 * @LastEditTime: 2021-01-20 10:00:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Fixedbuf_fer.h
 */
#ifndef _Fixedbuf_fer_H_
#define _Fixedbuf_fer_H_

#include "Base.h"
#include "string.h"
// #include <types.h>
#include <string.h>
#include <assert.h>

template <int SIZE>
class FixedBuffer{
public:
    FixedBuffer() : cur_(buf_) {}
    ~FixedBuffer() = default;
    // FixedBuffer(const Fixedbuf_fer& ) = delete;
    // FixedBuffer& operator=(const Fixedbuf_fer& ) = delete;
    void append(const char* buf, size_t len) {
        // FIXME: error?
        if (static_cast<size_t>(avail()) >= len) {
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

    inline int size() { return static_cast<int>(cur_ - buf_); }

    inline void reset() { cur_ = buf_; }

    void bzero() { 
        // ::bzero(buf_, sizeof(buf_)); 
        memset(buf_, 0, sizeof buf_);
    }

    inline char* data() { return buf_; }

    inline size_t avail() { return static_cast<size_t>(end() - cur_); }

    // for Debug
    char& operator[](int i) {
        assert(i >= 0 && i < sizeof(buf_));
        return buf_[i];
    }

    std::string toString() const { return std::string(buf_); }

    
private:
    const char* end() const {
        return buf_ + sizeof(buf_);
    }
    char buf_[SIZE];
    char *cur_;
};

#endif