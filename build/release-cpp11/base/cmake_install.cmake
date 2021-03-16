# Install script for directory: /home/cxf/Desktop/WebServer/base

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/cxf/Desktop/WebServer/build/release-install-cpp11")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/cxf/Desktop/WebServer/build/release-cpp11/lib/libbase.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/base" TYPE FILE FILES
    "/home/cxf/Desktop/WebServer/base/AppendFile.h"
    "/home/cxf/Desktop/WebServer/base/AsyncLog.h"
    "/home/cxf/Desktop/WebServer/base/Base.h"
    "/home/cxf/Desktop/WebServer/base/BlockQueue.h"
    "/home/cxf/Desktop/WebServer/base/Condition.h"
    "/home/cxf/Desktop/WebServer/base/ConditionLatch.h"
    "/home/cxf/Desktop/WebServer/base/FixedBuffer.h"
    "/home/cxf/Desktop/WebServer/base/LogFile.h"
    "/home/cxf/Desktop/WebServer/base/LogStream.h"
    "/home/cxf/Desktop/WebServer/base/Logging.h"
    "/home/cxf/Desktop/WebServer/base/Mutex.h"
    "/home/cxf/Desktop/WebServer/base/Singleton.h"
    "/home/cxf/Desktop/WebServer/base/Thread.h"
    "/home/cxf/Desktop/WebServer/base/Threadpool.h"
    "/home/cxf/Desktop/WebServer/base/Timestamp.h"
    "/home/cxf/Desktop/WebServer/base/rwn.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/cxf/Desktop/WebServer/build/release-cpp11/base/test/cmake_install.cmake")

endif()

