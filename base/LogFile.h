/*
 * @Author: your name
 * @Date: 2021-01-14 11:47:11
 * @LastEditTime: 2021-01-19 15:21:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/LogFile.h
 */
#include "Base.h"
#include "pthread.h"
#include <time.h>
#include "Mutex.h"
#include <string>
#include <memory>
#include "AppendFile.h"

class Helper{
public:
    static std::string hostname();
    // static 
};

class LogFile{
public:      
    /*explicit*/ LogFile(const std::string& basename);
    ~LogFile();

    void append(const std::string& basename, size_t len);
    void flush();
    
    const std::string getLogFileName() {
        time_t now;
        now = time(NULL);
        return getLogFileName(basename_, now);
    }
    /*public for test conveniently*/
private:
    static std::string getLogFileName(const std::string& basename, time_t &now);
    bool rollFile();
    
    std::string basename_;
    
    int count_;
    size_t writedSize_;

    std::unique_ptr<Mutex> mutex_;
    time_t startOfPeriod_;
    time_t lastRoll_;
    time_t lastFlush_;
    std::unique_ptr<AppendFile> file_; // for log file

    const static int kRollSize_ = 64 * 1024 ;
    const static int kFlushInterval_ = 3;
    const static int kCheckSteps = 1024;
    const static int kRollPerSeconds_ = 60 * 60 * 24;
};