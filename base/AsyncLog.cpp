/*
 * @Author: your name
 * @Date: 2021-01-16 11:52:08
 * @LastEditTime: 2021-01-25 14:04:44
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
    thread_.start();
}

void AsyncLog::stop() {
    running_ = false;
    thread_.join();
}

void AsyncLog::append(const char* logline, size_t len) {
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
    }
}

void AsyncLog::writeLogFileThread() {
    BufferPtr newBuffer1(new Buffer());
    BufferPtr newBuffer2(new Buffer());
    newBuffer1->bzero();
    newBuffer1->bzero();

    BufferVec bufsToWrite;
    while (running_) {
        if (buffers_.empty()) {
            cond_.WaitForSeconds(kFlushInterval);
            // cond_.Wait();
        }
        
        {
            // TODO: dead lock?
            MutexGuard lock(mutex_);
            buffers_.push_back(std::move(currentBuffer_));
            currentBuffer_ = std::move(newBuffer1);
            bufsToWrite.swap(buffers_);
            // As nextBuffer_
            if (!nextBuffer_) {
                nextBuffer_ = std::move(newBuffer2);
            }
            
            // Start logfile
            for (auto &buffer : bufsToWrite)
            {
                g_logfile_->append(buffer->data(), buffer->size());

            }
        } // ~MutexGuard
        if (!newBuffer1) {
            newBuffer1 = std::move(bufsToWrite.back());
            bufsToWrite.pop_back();
            newBuffer1->reset();
        }
        if (!newBuffer2) {
            newBuffer2 = std::move(bufsToWrite.back());
            bufsToWrite.pop_back();
            newBuffer2->reset();
        }
        // bufsToWrite.clear();
        BufferVec temp;
        bufsToWrite.swap(temp);
        g_logfile_->flush();
    }
    
}
