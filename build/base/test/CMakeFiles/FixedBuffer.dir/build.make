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
include base/test/CMakeFiles/FixedBuffer.dir/depend.make

# Include the progress variables for this target.
include base/test/CMakeFiles/FixedBuffer.dir/progress.make

# Include the compile flags for this target's objects.
include base/test/CMakeFiles/FixedBuffer.dir/flags.make

base/test/CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.o: base/test/CMakeFiles/FixedBuffer.dir/flags.make
base/test/CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.o: ../base/test/FixedBuffer_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxf/Desktop/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object base/test/CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.o"
	cd /home/cxf/Desktop/WebServer/build/base/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.o -c /home/cxf/Desktop/WebServer/base/test/FixedBuffer_test.cpp

base/test/CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.i"
	cd /home/cxf/Desktop/WebServer/build/base/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cxf/Desktop/WebServer/base/test/FixedBuffer_test.cpp > CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.i

base/test/CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.s"
	cd /home/cxf/Desktop/WebServer/build/base/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cxf/Desktop/WebServer/base/test/FixedBuffer_test.cpp -o CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.s

# Object files for target FixedBuffer
FixedBuffer_OBJECTS = \
"CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.o"

# External object files for target FixedBuffer
FixedBuffer_EXTERNAL_OBJECTS =

bin/FixedBuffer: base/test/CMakeFiles/FixedBuffer.dir/FixedBuffer_test.cpp.o
bin/FixedBuffer: base/test/CMakeFiles/FixedBuffer.dir/build.make
bin/FixedBuffer: lib/libbase.a
bin/FixedBuffer: base/test/CMakeFiles/FixedBuffer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cxf/Desktop/WebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/FixedBuffer"
	cd /home/cxf/Desktop/WebServer/build/base/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FixedBuffer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
base/test/CMakeFiles/FixedBuffer.dir/build: bin/FixedBuffer

.PHONY : base/test/CMakeFiles/FixedBuffer.dir/build

base/test/CMakeFiles/FixedBuffer.dir/clean:
	cd /home/cxf/Desktop/WebServer/build/base/test && $(CMAKE_COMMAND) -P CMakeFiles/FixedBuffer.dir/cmake_clean.cmake
.PHONY : base/test/CMakeFiles/FixedBuffer.dir/clean

base/test/CMakeFiles/FixedBuffer.dir/depend:
	cd /home/cxf/Desktop/WebServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cxf/Desktop/WebServer /home/cxf/Desktop/WebServer/base/test /home/cxf/Desktop/WebServer/build /home/cxf/Desktop/WebServer/build/base/test /home/cxf/Desktop/WebServer/build/base/test/CMakeFiles/FixedBuffer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : base/test/CMakeFiles/FixedBuffer.dir/depend

