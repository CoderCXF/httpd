# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cxf/Desktop/WebServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cxf/Desktop/WebServer/build/release-cpp11

# Include any dependencies generated for this target.
include net/CMakeFiles/net.dir/depend.make

# Include the progress variables for this target.
include net/CMakeFiles/net.dir/progress.make

# Include the compile flags for this target's objects.
include net/CMakeFiles/net.dir/flags.make

net/CMakeFiles/net.dir/EventLoop.cpp.o: net/CMakeFiles/net.dir/flags.make
net/CMakeFiles/net.dir/EventLoop.cpp.o: ../../net/EventLoop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxf/Desktop/WebServer/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object net/CMakeFiles/net.dir/EventLoop.cpp.o"
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/net.dir/EventLoop.cpp.o -c /home/cxf/Desktop/WebServer/net/EventLoop.cpp

net/CMakeFiles/net.dir/EventLoop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/net.dir/EventLoop.cpp.i"
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cxf/Desktop/WebServer/net/EventLoop.cpp > CMakeFiles/net.dir/EventLoop.cpp.i

net/CMakeFiles/net.dir/EventLoop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/net.dir/EventLoop.cpp.s"
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cxf/Desktop/WebServer/net/EventLoop.cpp -o CMakeFiles/net.dir/EventLoop.cpp.s

net/CMakeFiles/net.dir/Channel.cpp.o: net/CMakeFiles/net.dir/flags.make
net/CMakeFiles/net.dir/Channel.cpp.o: ../../net/Channel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxf/Desktop/WebServer/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object net/CMakeFiles/net.dir/Channel.cpp.o"
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/net.dir/Channel.cpp.o -c /home/cxf/Desktop/WebServer/net/Channel.cpp

net/CMakeFiles/net.dir/Channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/net.dir/Channel.cpp.i"
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cxf/Desktop/WebServer/net/Channel.cpp > CMakeFiles/net.dir/Channel.cpp.i

net/CMakeFiles/net.dir/Channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/net.dir/Channel.cpp.s"
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cxf/Desktop/WebServer/net/Channel.cpp -o CMakeFiles/net.dir/Channel.cpp.s

net/CMakeFiles/net.dir/EPoll.cpp.o: net/CMakeFiles/net.dir/flags.make
net/CMakeFiles/net.dir/EPoll.cpp.o: ../../net/EPoll.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxf/Desktop/WebServer/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object net/CMakeFiles/net.dir/EPoll.cpp.o"
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/net.dir/EPoll.cpp.o -c /home/cxf/Desktop/WebServer/net/EPoll.cpp

net/CMakeFiles/net.dir/EPoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/net.dir/EPoll.cpp.i"
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cxf/Desktop/WebServer/net/EPoll.cpp > CMakeFiles/net.dir/EPoll.cpp.i

net/CMakeFiles/net.dir/EPoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/net.dir/EPoll.cpp.s"
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cxf/Desktop/WebServer/net/EPoll.cpp -o CMakeFiles/net.dir/EPoll.cpp.s

# Object files for target net
net_OBJECTS = \
"CMakeFiles/net.dir/EventLoop.cpp.o" \
"CMakeFiles/net.dir/Channel.cpp.o" \
"CMakeFiles/net.dir/EPoll.cpp.o"

# External object files for target net
net_EXTERNAL_OBJECTS =

lib/libnet.a: net/CMakeFiles/net.dir/EventLoop.cpp.o
lib/libnet.a: net/CMakeFiles/net.dir/Channel.cpp.o
lib/libnet.a: net/CMakeFiles/net.dir/EPoll.cpp.o
lib/libnet.a: net/CMakeFiles/net.dir/build.make
lib/libnet.a: net/CMakeFiles/net.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cxf/Desktop/WebServer/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library ../lib/libnet.a"
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && $(CMAKE_COMMAND) -P CMakeFiles/net.dir/cmake_clean_target.cmake
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/net.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
net/CMakeFiles/net.dir/build: lib/libnet.a

.PHONY : net/CMakeFiles/net.dir/build

net/CMakeFiles/net.dir/clean:
	cd /home/cxf/Desktop/WebServer/build/release-cpp11/net && $(CMAKE_COMMAND) -P CMakeFiles/net.dir/cmake_clean.cmake
.PHONY : net/CMakeFiles/net.dir/clean

net/CMakeFiles/net.dir/depend:
	cd /home/cxf/Desktop/WebServer/build/release-cpp11 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cxf/Desktop/WebServer /home/cxf/Desktop/WebServer/net /home/cxf/Desktop/WebServer/build/release-cpp11 /home/cxf/Desktop/WebServer/build/release-cpp11/net /home/cxf/Desktop/WebServer/build/release-cpp11/net/CMakeFiles/net.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : net/CMakeFiles/net.dir/depend

