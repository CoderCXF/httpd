/*
 * @Author: your name
 * @Date: 2021-01-16 11:52:08
 * @LastEditTime: 2021-01-17 12:46:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/AsyncLog.cpp
 */
#include "AsyncLog.h"

AsyncLog::AsyncLog(const std::string& basename)
    : basename_(basename),
      running_(false),
      thread_(std::bind(&AsyncLog::writeLogFileThread, this)),
      mutex_(),
      cond_(mutex_),
      currentBuffer_(new Buffer),
      nextBuffer_(new Buffer()),
      buffers_(),
      g_logfile_(new LogFile(basename))
{
}
void AsyncLog::start() {
    running_ = true;
    // Thread thread_([&](){
    //     this->writeLogFileThread();
    // });
    thread_.Start();
}

void AsyncLog::stop() {
    running_ = false;
    thread_.Join();
}

void AsyncLog::append(const char* logline, size_t len) {
    std::cout << "enter append" << std::endl;
    MutexGuard lock(mutex_);
    if (currentBuffer_->avail() > len) {
        currentBuffer_->append(logline, len);
    } else {
        buffers_.push_back(std::move(currentBuffer_)); //currentBuffer == NULL
        if (nextBuffer_) {
            currentBuffer_ = std::move(nextBuffer_);
        } else {
            currentBuffer_.reset(new Buffer());
        }
        currentBuffer_->append(logline, len);
        cond_.Notify();
        std::cout << "cond_.Notify()" << std::endl;
    }
}

void AsyncLog::writeLogFileThread() {
    std::cout << "into threadFunc:writeLogFileThread" << "    Thread is running?" << running_ << std::endl;
    BufferPtr newBuffer1;
    BufferPtr newBuffer2;
    // FIXME: bzero() cause Segmentation fault
    // newBuffer1->bzero();
    // newBuffer1->bzero();
    std::cout << "***" << std::endl;

    BufferVec bufToWrite;
    while (running_) {
        if (buffers_.empty()) {
            cond_.WaitForSeconds(kFlushInterval);
        }
        {
            MutexGuard lock(mutex_);
            buffers_.push_back(std::move(currentBuffer_));
            currentBuffer_ = std::move(newBuffer1);
            bufToWrite.swap(buffers_);
            // As nextBuffer_
            if (!nextBuffer_) {
                nextBuffer_ = std::move(newBuffer2);
            }
            // Start logfile
            for (const auto& buffer : bufToWrite)
            {
            g_logfile_->append(buffer->data(), buffer->size());
            }
        } // ~MutexGuard
        // FIXME: reset() cause Segmentation fault
        std::cout << "->reset before" << std::endl;
        if (!newBuffer1) {
            newBuffer1 = std::move(bufToWrite.back());
            bufToWrite.pop_back();
            newBuffer1->reset();
        }
        if (!newBuffer2) {
            newBuffer2 = std::move(bufToWrite.back());
            bufToWrite.pop_back();
            newBuffer2->reset();
        }
        std::cout << "->reset before" << std::endl;
        bufToWrite.clear();
        // BufferVec temp;
        // bufToWrite.swap(temp);
        g_logfile_->flush();
    }
    
}
