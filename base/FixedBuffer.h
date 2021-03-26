/*
 * @Author: your name
 * @Date: 2021-01-13 10:20:19
 * @LastEditTime: 2021-03-26 14:39:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Fixedbuf_fer.h
 */
#ifndef _Fixedbuf_fer_H_
#define _Fixedbuf_fer_H_

#include "Base.h"
// #include <types.h>
#include <string.h>
#include <string>
#include <assert.h>

template <int SIZE>
class FixedBuffer{
public:
    FixedBuffer() : cur_(buf_) {}
    ~FixedBuffer() = default;
    // FixedBuffer(const Fixedbuf_fer& ) = delete;
    // FixedBuffer& operator=(const Fixedbuf_fer& ) = delete;
    void append(const char* buf, size_t len) {
        if (static_cast<size_t>(avail()) >= len) {
            memcpy(cur_, buf, len);
            cur_ += len;
        }
    }
    // FIXME:
    // void append(const std::string& str) {
    //     size_t len = str.size();
    //     append(str.c_str(), len);
    // }

    inline void add(size_t len) { cur_ += len; }

    inline char* curr() { return cur_; }

    inline int size() const { return static_cast<int>(cur_ - buf_); }

    inline void reset() { cur_ = buf_; }

    inline const char* data() const { return buf_; }

    inline size_t avail() const { return static_cast<size_t>(end() - cur_); }

    void bzero() { memset(buf_, 0, sizeof buf_); }
    
    // for Debug
    char& operator[](size_t i) {
        assert(i >= 0 && i < sizeof(buf_));
        return buf_[i];
    }

    // FIXME: copy is slow! other method?
    std::string toString() const { 
        std::string str;
        for (int i = 0; i < this->size(); ++i) {
            str += buf_[i];
        }
        return str;
    }
    
private:
    const char* end() const {
        return buf_ + sizeof(buf_);
    }
    char buf_[SIZE];
    char *cur_;
};

#endif