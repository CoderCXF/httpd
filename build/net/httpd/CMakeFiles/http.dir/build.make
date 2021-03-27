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
CMAKE_BINARY_DIR = /home/cxf/Desktop/WebServer/build

# Include any dependencies generated for this target.
include net/httpd/CMakeFiles/http.dir/depend.make

# Include the progress variables for this target.
include net/httpd/CMakeFiles/http.dir/progress.make

# Include the compile flags for this target's objects.
include net/httpd/CMakeFiles/http.dir/flags.make

net/httpd/CMakeFiles/http.dir/HttpParse.cpp.o: net/httpd/CMakeFiles/http.dir/flags.make
net/httpd/CMakeFiles/http.dir/HttpParse.cpp.o: ../net/httpd/HttpParse.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxf/Desktop/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object net/httpd/CMakeFiles/http.dir/HttpParse.cpp.o"
	cd /home/cxf/Desktop/WebServer/build/net/httpd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/http.dir/HttpParse.cpp.o -c /home/cxf/Desktop/WebServer/net/httpd/HttpParse.cpp

net/httpd/CMakeFiles/http.dir/HttpParse.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http.dir/HttpParse.cpp.i"
	cd /home/cxf/Desktop/WebServer/build/net/httpd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cxf/Desktop/WebServer/net/httpd/HttpParse.cpp > CMakeFiles/http.dir/HttpParse.cpp.i

net/httpd/CMakeFiles/http.dir/HttpParse.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http.dir/HttpParse.cpp.s"
	cd /home/cxf/Desktop/WebServer/build/net/httpd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cxf/Desktop/WebServer/net/httpd/HttpParse.cpp -o CMakeFiles/http.dir/HttpParse.cpp.s

net/httpd/CMakeFiles/http.dir/HttpServer.cpp.o: net/httpd/CMakeFiles/http.dir/flags.make
net/httpd/CMakeFiles/http.dir/HttpServer.cpp.o: ../net/httpd/HttpServer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxf/Desktop/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object net/httpd/CMakeFiles/http.dir/HttpServer.cpp.o"
	cd /home/cxf/Desktop/WebServer/build/net/httpd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/http.dir/HttpServer.cpp.o -c /home/cxf/Desktop/WebServer/net/httpd/HttpServer.cpp

net/httpd/CMakeFiles/http.dir/HttpServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http.dir/HttpServer.cpp.i"
	cd /home/cxf/Desktop/WebServer/build/net/httpd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cxf/Desktop/WebServer/net/httpd/HttpServer.cpp > CMakeFiles/http.dir/HttpServer.cpp.i

net/httpd/CMakeFiles/http.dir/HttpServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http.dir/HttpServer.cpp.s"
	cd /home/cxf/Desktop/WebServer/build/net/httpd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cxf/Desktop/WebServer/net/httpd/HttpServer.cpp -o CMakeFiles/http.dir/HttpServer.cpp.s

# Object files for target http
http_OBJECTS = \
"CMakeFiles/http.dir/HttpParse.cpp.o" \
"CMakeFiles/http.dir/HttpServer.cpp.o"

# External object files for target http
http_EXTERNAL_OBJECTS =

lib/libhttp.a: net/httpd/CMakeFiles/http.dir/HttpParse.cpp.o
lib/libhttp.a: net/httpd/CMakeFiles/http.dir/HttpServer.cpp.o
lib/libhttp.a: net/httpd/CMakeFiles/http.dir/build.make
lib/libhttp.a: net/httpd/CMakeFiles/http.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cxf/Desktop/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../../lib/libhttp.a"
	cd /home/cxf/Desktop/WebServer/build/net/httpd && $(CMAKE_COMMAND) -P CMakeFiles/http.dir/cmake_clean_target.cmake
	cd /home/cxf/Desktop/WebServer/build/net/httpd && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/http.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
net/httpd/CMakeFiles/http.dir/build: lib/libhttp.a

.PHONY : net/httpd/CMakeFiles/http.dir/build

net/httpd/CMakeFiles/http.dir/clean:
	cd /home/cxf/Desktop/WebServer/build/net/httpd && $(CMAKE_COMMAND) -P CMakeFiles/http.dir/cmake_clean.cmake
.PHONY : net/httpd/CMakeFiles/http.dir/clean

net/httpd/CMakeFiles/http.dir/depend:
	cd /home/cxf/Desktop/WebServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cxf/Desktop/WebServer /home/cxf/Desktop/WebServer/net/httpd /home/cxf/Desktop/WebServer/build /home/cxf/Desktop/WebServer/build/net/httpd /home/cxf/Desktop/WebServer/build/net/httpd/CMakeFiles/http.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : net/httpd/CMakeFiles/http.dir/depend

