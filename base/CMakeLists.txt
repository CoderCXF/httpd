cmake_minimum_required(VERSION 3.19.0)
project(Server C CXX)

set (INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
                    /usr/local/include
					)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib
            /usr/local/lib
	    /usr/lib/x86_64-linux-gnu)

add_compile_options(-std=c++11 -g -Wall)

include_directories(${INCLUDE_DIR})
link_directories(${LIB_DIR})

# EXECUTABLE_OUTPUT_PATH
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# LIBRARY_OUTPUT_PATH
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#set (BASE_SRC
#     Mutex.cpp
#     Condition.cpp
#     ConditionLatch.cpp
#     Thread.cpp
#     Threadpool.cpp)


#add_library(server_base ${BASE_SRC})
#target_link_libraries(server_base pThread)

# Mutex
add_executable(Mutex Mutex.cpp test/Mutex_test.cpp)
target_link_libraries(Mutex -lpthread)

# Condition
add_executable(Condition Mutex.cpp Thread.cpp Condition.cpp test/Condition_test.cpp)
target_link_libraries(Condition -lpthread)

# Thread
add_executable(Thread Mutex.cpp Thread.cpp test/Thread_test.cpp)
target_link_libraries(Thread -lpthread)

# Threadpool
add_executable(Threadpool Mutex.cpp Thread.cpp Condition.cpp ConditionLatch.cpp Threadpool.cpp test/Threadpool_test.cpp)
target_link_libraries(Threadpool -lpthread)

# Singleton
add_executable(Singleton test/Singleton_test.cpp)
target_link_libraries(Singleton -lpthread)

# AppendFile
add_executable(AppendFile AppendFile.cpp test/AppendFile_test.cpp)
target_link_libraries(AppendFile -lpthread)

# LogFile
add_executable(LogFile LogFile.cpp Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp test/LogFile_test.cpp)
target_link_libraries(LogFile -lpthread)

# FixedBuffer
add_executable(FixedBuffer Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp LogFile.cpp test/FixedBuffer_test.cpp)
target_link_libraries(FixedBuffer -lpthread)

# AsyncLog
add_executable(AsyncLog AsyncLog.cpp LogFile.cpp Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp test/AsyncLog_test.cpp)
target_link_libraries(AsyncLog -lpthread)

# LogStream
add_executable(LogStream Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp LogFile.cpp LogStream.cpp test/LogStream_test.cpp)
target_link_libraries(LogStream -lpthread)