#!/bin/sh
###
 # @Author: your name
 # @Date: 2021-03-16 09:26:42
 # @LastEditTime: 2021-03-26 15:08:01
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /WebServer/build.sh
### 

set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-./build}
BUILD_TYPE=${BUILD_TYPE:-debug}
INSTALL_DIR=${INSTALL_DIR:-../}
CXX=${CXX:-g++}


mkdir $BUILD_DIR/
cd $BUILD_DIR/
cmake \
      -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
      -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      $SOURCE_DIR \
  && make $*

# Use the following command to run all the unit tests
# at the dir $BUILD_DIR/$BUILD_TYPE :
# CTEST_OUTPUT_ON_FAILURE=TRUE make test
