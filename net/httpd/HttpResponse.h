/*
 * @Author: your name
 * @Date: 2021-03-26 20:29:25
 * @LastEditTime: 2021-03-29 20:48:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/httpd/HttpResponse.h
 */
#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include <string>
#include <string.h>
#include <map>
#include "../Buffer.h"
// class Buffer;
class HttpResponse{
public:
    enum StatusCode{
        kUnKnownCode,
        k200OK = 200,
        k301MovedPermanently = 301,
        k400BadRequest = 400,
        k404NotFound = 404
    };
    enum HttpVersion{
        kUnKnown, kHttp10, kHttp11
    };
    HttpResponse(bool keep_alive) : version_(HttpVersion::kUnKnown), statusCode_(StatusCode::kUnKnownCode), keep_alive_(keep_alive)
    {
        
    }

    HttpVersion version() {
        return version_;
    }
    void setVersion(HttpVersion v) {
        version_ = v;
    }
    void setStatusCode(StatusCode statusCode) {
        statusCode_ = statusCode;
    }
    void setStatusMessage(const std::string& msg) {
        statusMessage_ = msg;
    }
    void addHeader(const std::string &field, const std::string &value) {
        headers_[field] = value;
    }

    void setContentType(const std::string &contentType) {
        headers_["ContentType"] = contentType;
    }

    void setBody(const std::string &body) {
        responseBody_ = body;
    }

    std::string header(const std::string &field) const{
        std::string res;
        auto iter = headers_.find(field);
        if (iter != headers_.end()) {
            res = iter->second;
        }
        return res;
    }

    void appendBuf(Buffer *output) {
        char buf[32];
        if (version_ == HttpVersion::kHttp10) {
            snprintf(buf, sizeof buf, "HTTP/1.0 ");
        } else if (version_ == HttpVersion::kHttp11) {
            snprintf(buf, sizeof buf, "HTTP/1.1 ");
        }
        output->append(buf);
        snprintf(buf, sizeof buf, "%d ", statusCode_);
        output->append(buf, strlen(buf));
        output->append(statusMessage_);
        output->append("\r\n");

        for (const auto& head : headers_) {
            output->append(head.first);
            output->append(": ");
            output->append(head.second);
            output->append("\r\n");
        }
        if (keep_alive_) {
            snprintf(buf, sizeof buf, "Content-Length: %ld\r\n", responseBody_.size());
            output->append(buf);
            output->append("Connection: Keep-Alive\r\n");
        } else {
            output->append("Connection: close\r\n");
        }
        
        output->append("\r\n");
        output->append(responseBody_);
    }
private:
    HttpVersion version_;
    StatusCode statusCode_;
    std::string statusMessage_;
    std::map<std::string, std::string> headers_;
    std::string responseBody_;
    bool keep_alive_;
};

#endif