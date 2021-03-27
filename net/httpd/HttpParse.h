/*
 * @Author: your name
 * @Date: 2021-03-26 21:18:11
 * @LastEditTime: 2021-03-27 09:14:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/httpd/ParseRequest.h
 */
#ifndef HTTPPARSE_H
#define HHTPPARSE_H
#include "HttpRequest.h"
class Buffer;
class HttpParse{
public:
    enum ParseState{
        kExpectRequestLine,
        kExpectHeaders,
        kExpectBody,
        kGotAll
    };
    HttpParse() : state_(kExpectRequestLine)
    {

    }

    bool isComplete() {
        return state_ == kGotAll;
    }
    void reset() {
        state_ = kExpectRequestLine;
        HttpRequest dummy;
        request_.swap(dummy);
    }

    const HttpRequest& request() const {
        return request_;
    }
    HttpRequest& request() {
        return request_;
    }
    bool parseRequest(Buffer *buf);
private:
    bool parseRequestLine(const char *start, const char *end);
    ParseState state_;
    HttpRequest request_;
};
#endif