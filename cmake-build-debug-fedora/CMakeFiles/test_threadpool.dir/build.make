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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.zGWSYtdWMz

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.zGWSYtdWMz/cmake-build-debug-fedora

# Include any dependencies generated for this target.
include CMakeFiles/test_threadpool.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_threadpool.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_threadpool.dir/flags.make

CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.o: CMakeFiles/test_threadpool.dir/flags.make
CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.o: ../tests/test_threadpool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zGWSYtdWMz/cmake-build-debug-fedora/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.o -c /tmp/tmp.zGWSYtdWMz/tests/test_threadpool.cpp

CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zGWSYtdWMz/tests/test_threadpool.cpp > CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.i

CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zGWSYtdWMz/tests/test_threadpool.cpp -o CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.s

# Object files for target test_threadpool
test_threadpool_OBJECTS = \
"CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.o"

# External object files for target test_threadpool
test_threadpool_EXTERNAL_OBJECTS =

test_threadpool: CMakeFiles/test_threadpool.dir/tests/test_threadpool.cpp.o
test_threadpool: CMakeFiles/test_threadpool.dir/build.make
test_threadpool: libkish.so
test_threadpool: libthird_pt.so
test_threadpool: CMakeFiles/test_threadpool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.zGWSYtdWMz/cmake-build-debug-fedora/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_threadpool"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_threadpool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_threadpool.dir/build: test_threadpool

.PHONY : CMakeFiles/test_threadpool.dir/build

CMakeFiles/test_threadpool.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_threadpool.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_threadpool.dir/clean

CMakeFiles/test_threadpool.dir/depend:
	cd /tmp/tmp.zGWSYtdWMz/cmake-build-debug-fedora && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.zGWSYtdWMz /tmp/tmp.zGWSYtdWMz /tmp/tmp.zGWSYtdWMz/cmake-build-debug-fedora /tmp/tmp.zGWSYtdWMz/cmake-build-debug-fedora /tmp/tmp.zGWSYtdWMz/cmake-build-debug-fedora/CMakeFiles/test_threadpool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_threadpool.dir/depend

