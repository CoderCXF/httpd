/*
 * @Author: your name
 * @Date: 2021-03-26 17:27:48
 * @LastEditTime: 2021-03-27 10:21:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/httpd/Request.h
 */
#ifndef REQUEST_H
#define REQUEST_H
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
class HttpRequest{
public:
    enum Method
    {
        kInvalid, kGet, kPost, kHead, kPut, kDelete
    };
    enum Version
    {
        kUnknown, kHttp10, kHttp11
    };
    HttpRequest() : method_(kInvalid), version_(kUnknown), path_("/")
    {
        
    }
    void setMethod(const char *start, const char *end) {
        std::string m(start, end);
        if (m == "GET") {
            method_ = kGet;
        } else if (m == "POST") {
            method_ = kPost;
        } else if (m == "HEAD") {
            method_ = kHead;
        } else if (m == "PUT") {
            method_ = kPut;
        } else if (m == "DELETE") {
            method_ = kDelete;
        } else {
            m = kInvalid;
        }
    }
    void setVersion(Version v) {
        version_ = v;
    }
    void setPath(const char *start, const char *end) {
        path_.assign(start, end);        
    }
    void addHeader(const char* start, const char* colon, const char* end)
    {
        std::string field(start, colon);
        ++colon;
        while (colon < end && isspace(*colon))
        {
        ++colon;
        }
        std::string value(colon, end);
        while (!value.empty() && isspace(value[value.size()-1]))
        {
        value.resize(value.size()-1);
        }
        headers_[field] = value;
    }
    //
    /// get function
    //
    Method method() const{
        return method_;
    }
    Version version() const{
        return version_;
    }
    std::string path() const{
        return path_;
    }
    std::map<std::string, std::string> headers() const{
        return headers_;
    }
    std::string getHead(const std::string& field) const{
        std::string res;
        auto iter = headers_.find(field);
        if (iter != headers_.end()) {
            res = iter->second;
        }
        return res;
    }
    // swap is used in HttpParse class
    void swap(HttpRequest &that) {
        std::swap(method_, that.method_);
        std::swap(version_, that.version_);
        path_.swap(that.path_);
        headers_.swap(that.headers_);
    }
private:
    Method method_;
    Version version_;
    std::string path_;
    std::map<std::string, std::string> headers_;
};
#endif
