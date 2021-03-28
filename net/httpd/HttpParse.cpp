/*
 * @Author: your name
 * @Date: 2021-03-27 08:25:06
 * @LastEditTime: 2021-03-28 08:08:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/httpd/HttpParse.cpp
 */
#include "HttpParse.h"
#include "../Buffer.h"
#include "../../base/Logging.h"
#include <algorithm>
bool HttpParse::parseRequest(Buffer *buf) {
    bool hasMore = true;
    bool ok = true;
    while (hasMore) {
        if (state_ == ParseState::kExpectRequestLine) {
            const char* crlf = buf->findCRLF();
            // request line
            if (crlf) {
                ok = parseRequestLine(buf->peek(), crlf);
                if (ok) {
                    buf->retrieveUntil(crlf + 2);
                    state_ = ParseState::kExpectHeaders;
                } else {
                    hasMore = false;
                }
            } else {
                hasMore = false;
            }
            // request headser;
        }
        else if (state_ == ParseState::kExpectHeaders) {
            const char *crlf = buf->findCRLF();
            if (crlf) {
                // 从第一个请求头部中查找：号
                const char *colon = std::find(buf->peek(), crlf, ':');
                if (colon != crlf) {
                    request_.addHeader(buf->peek(), colon, crlf);
                } else {
                    // 当在buf中找到的冒号等于crlf的时候，说明根本就没有冒号，也就是到了\r\n这一行
                    state_ = HttpParse::kGotAll;
                    // 没有解析body，而是直接hasMore = false(退出状态循环)
                    hasMore = false;
                }
                buf->retrieveUntil(crlf + 2);
            } else {
                hasMore = false;
            }
        }
        else if (state_ == ParseState::kExpectBody) {
            hasMore = false;
        }
    }
    return ok;
}

bool HttpParse::parseRequestLine(const char *start, const char *end) {
    bool success = true;
    const char* space = std::find(start, end, ' ');
    if (space != end) {
        request_.setMethod(start, space);
        start = space + 1;
        space = std::find(start, end, ' ');
        if (space != end) {
            request_.setPath(start, space);
            start = space + 1;
        } 
        else {
            success = false;
            return success;
        }
        success = end - start == 8 && std::equal(start, end - 1, "HTTP/1.");
        if (success) {
            if (*(end - 1) == '1') {
                request_.setVersion(HttpRequest::kHttp11);
            } 
            else if (*(end - 1) == '0') {
                request_.setVersion(HttpRequest::kHttp10);

            }
            else {
                success = false;
            }
        } 
    }
    return success;
}
