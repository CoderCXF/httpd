# Install script for directory: /home/cxf/Desktop/WebServer/net

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/cxf/Desktop/WebServer")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "debug")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/cxf/Desktop/WebServer/build/lib/libnet.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/net" TYPE FILE FILES
    "/home/cxf/Desktop/WebServer/net/EventLoop.h"
    "/home/cxf/Desktop/WebServer/net/EventLoopThread.h"
    "/home/cxf/Desktop/WebServer/net/EventLoopThreadPool.h"
    "/home/cxf/Desktop/WebServer/net/Channel.h"
    "/home/cxf/Desktop/WebServer/net/EPoll.h"
    "/home/cxf/Desktop/WebServer/net/SocketOps.h"
    "/home/cxf/Desktop/WebServer/net/Socket.h"
    "/home/cxf/Desktop/WebServer/net/AddrStruct.h"
    "/home/cxf/Desktop/WebServer/net/Acceptor.h"
    "/home/cxf/Desktop/WebServer/net/TcpServer.h"
    "/home/cxf/Desktop/WebServer/net/Buffer.h"
    "/home/cxf/Desktop/WebServer/net/Connection.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/cxf/Desktop/WebServer/build/net/httpd/cmake_install.cmake")
  include("/home/cxf/Desktop/WebServer/build/net/test/cmake_install.cmake")

endif()

