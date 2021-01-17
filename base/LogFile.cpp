/*
 * @Author: your name
 * @Date: 2021-01-14 11:47:23
 * @LastEditTime: 2021-01-16 15:46:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/LogFile.cpp
 */
#include "LogFile.h"
#include "Thread.h"
std::string Helper::hostname() {
    char name[256];
    if (0 == gethostname(name, sizeof(name))) {
        return std::string(name);
    } else {
        return "unknown host";
    }
}

LogFile::LogFile(const std::string& basename) 
    : basename_(basename),
      count_(0),
      writedSize_(0),
      mutex_(new Mutex()),
      startOfPeriod_(0),
      lastRoll_(0),
      lastFlush_(0) 
{ 
    rollFile();
}

LogFile::~LogFile() {
    
}

void LogFile::append(const std::string& logline, size_t len) {
    // shared_ptr and unique_ptr has */-> operator
    MutexGuard lock(*mutex_);
    file_->append(logline, len);
    if (file_->writtenBytes() > kRollSize_) {
        rollFile();
    } else {
        count_++;
        if (count_ >= kCheckSteps) {
            time_t now;
            now = time(NULL);
            time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;
            if (start != startOfPeriod_) {
                rollFile();
            } else if ((now - lastFlush_) >= kFlushInterval_) {
                file_->flush();
                flush();
            }
        }
    }
    
}
    
std::string LogFile::getLogFileName(const std::string& basename, time_t &now) {
    std::string fileName(basename);
    // time_t now;
    struct tm *local_tm;
    time(&now);
    local_tm = localtime(&now);
    char buf[18];
    strftime(buf, sizeof(buf), ".%Y%m%d-%H%M%S.", local_tm);
    fileName += buf;
    fileName += Helper::hostname();
    char pidbuf[32];
    snprintf(pidbuf, sizeof(pidbuf), ".%d.", getpid());
    // char tidbuf[64];
    // sprintf(tidbuf, "%d.", CurrentThread::tid());
    // fileName += tidbuf;
    fileName += pidbuf;
    fileName += "log";

    return fileName;
}

// FIXME: problem exist or error mybe 
bool LogFile::rollFile() {
    time_t now = 0;
    std::string fileName = getLogFileName(basename_, now);
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;
    // if (now > lastRoll_) { 
        startOfPeriod_ = start;
        lastRoll_ = now;
        lastFlush_ = now;
        writedSize_ = 0;
        count_ = 0;
        // create new log file
        file_.reset(new AppendFile(fileName));
        return true;
    // }
    return false;
}

void LogFile::flush() {
    file_->flush();
}

