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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jiangaoran/WorkSapce/C++/WebKish

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/third_pt.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/third_pt.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/third_pt.dir/flags.make

CMakeFiles/third_pt.dir/thirdparty/api.c.o: CMakeFiles/third_pt.dir/flags.make
CMakeFiles/third_pt.dir/thirdparty/api.c.o: ../thirdparty/api.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/third_pt.dir/thirdparty/api.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/third_pt.dir/thirdparty/api.c.o -c /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/api.c

CMakeFiles/third_pt.dir/thirdparty/api.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/third_pt.dir/thirdparty/api.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/api.c > CMakeFiles/third_pt.dir/thirdparty/api.c.i

CMakeFiles/third_pt.dir/thirdparty/api.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/third_pt.dir/thirdparty/api.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/api.c -o CMakeFiles/third_pt.dir/thirdparty/api.c.s

CMakeFiles/third_pt.dir/thirdparty/http.c.o: CMakeFiles/third_pt.dir/flags.make
CMakeFiles/third_pt.dir/thirdparty/http.c.o: ../thirdparty/http.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/third_pt.dir/thirdparty/http.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/third_pt.dir/thirdparty/http.c.o -c /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/http.c

CMakeFiles/third_pt.dir/thirdparty/http.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/third_pt.dir/thirdparty/http.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/http.c > CMakeFiles/third_pt.dir/thirdparty/http.c.i

CMakeFiles/third_pt.dir/thirdparty/http.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/third_pt.dir/thirdparty/http.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/http.c -o CMakeFiles/third_pt.dir/thirdparty/http.c.s

CMakeFiles/third_pt.dir/thirdparty/llhttp.c.o: CMakeFiles/third_pt.dir/flags.make
CMakeFiles/third_pt.dir/thirdparty/llhttp.c.o: ../thirdparty/llhttp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/third_pt.dir/thirdparty/llhttp.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/third_pt.dir/thirdparty/llhttp.c.o -c /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/llhttp.c

CMakeFiles/third_pt.dir/thirdparty/llhttp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/third_pt.dir/thirdparty/llhttp.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/llhttp.c > CMakeFiles/third_pt.dir/thirdparty/llhttp.c.i

CMakeFiles/third_pt.dir/thirdparty/llhttp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/third_pt.dir/thirdparty/llhttp.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/llhttp.c -o CMakeFiles/third_pt.dir/thirdparty/llhttp.c.s

CMakeFiles/third_pt.dir/thirdparty/cJSON.c.o: CMakeFiles/third_pt.dir/flags.make
CMakeFiles/third_pt.dir/thirdparty/cJSON.c.o: ../thirdparty/cJSON.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/third_pt.dir/thirdparty/cJSON.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/third_pt.dir/thirdparty/cJSON.c.o -c /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/cJSON.c

CMakeFiles/third_pt.dir/thirdparty/cJSON.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/third_pt.dir/thirdparty/cJSON.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/cJSON.c > CMakeFiles/third_pt.dir/thirdparty/cJSON.c.i

CMakeFiles/third_pt.dir/thirdparty/cJSON.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/third_pt.dir/thirdparty/cJSON.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/cJSON.c -o CMakeFiles/third_pt.dir/thirdparty/cJSON.c.s

CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.o: CMakeFiles/third_pt.dir/flags.make
CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.o: ../thirdparty/CJsonObject.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.o -c /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/CJsonObject.cpp

CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/CJsonObject.cpp > CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.i

CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/CJsonObject.cpp -o CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.s

CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.o: CMakeFiles/third_pt.dir/flags.make
CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.o: ../thirdparty/multipart_parser.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.o -c /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/multipart_parser.c

CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/multipart_parser.c > CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.i

CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/jiangaoran/WorkSapce/C++/WebKish/thirdparty/multipart_parser.c -o CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.s

# Object files for target third_pt
third_pt_OBJECTS = \
"CMakeFiles/third_pt.dir/thirdparty/api.c.o" \
"CMakeFiles/third_pt.dir/thirdparty/http.c.o" \
"CMakeFiles/third_pt.dir/thirdparty/llhttp.c.o" \
"CMakeFiles/third_pt.dir/thirdparty/cJSON.c.o" \
"CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.o" \
"CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.o"

# External object files for target third_pt
third_pt_EXTERNAL_OBJECTS =

libthird_pt.dylib: CMakeFiles/third_pt.dir/thirdparty/api.c.o
libthird_pt.dylib: CMakeFiles/third_pt.dir/thirdparty/http.c.o
libthird_pt.dylib: CMakeFiles/third_pt.dir/thirdparty/llhttp.c.o
libthird_pt.dylib: CMakeFiles/third_pt.dir/thirdparty/cJSON.c.o
libthird_pt.dylib: CMakeFiles/third_pt.dir/thirdparty/CJsonObject.cpp.o
libthird_pt.dylib: CMakeFiles/third_pt.dir/thirdparty/multipart_parser.c.o
libthird_pt.dylib: CMakeFiles/third_pt.dir/build.make
libthird_pt.dylib: CMakeFiles/third_pt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX shared library libthird_pt.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/third_pt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/third_pt.dir/build: libthird_pt.dylib

.PHONY : CMakeFiles/third_pt.dir/build

CMakeFiles/third_pt.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/third_pt.dir/cmake_clean.cmake
.PHONY : CMakeFiles/third_pt.dir/clean

CMakeFiles/third_pt.dir/depend:
	cd /Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jiangaoran/WorkSapce/C++/WebKish /Users/jiangaoran/WorkSapce/C++/WebKish /Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug /Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug /Users/jiangaoran/WorkSapce/C++/WebKish/cmake-build-debug/CMakeFiles/third_pt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/third_pt.dir/depend

