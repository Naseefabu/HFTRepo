# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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

# Suppress display of executed commands.
$$(VERBOSE).SILENT:


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
CMAKE_SOURCE_DIR = /home/user/Desktop/atrimo-test/async_logger

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/Desktop/atrimo-test/async_logger/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/WriteLogTest.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/WriteLogTest.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/WriteLogTest.dir/flags.make

tests/CMakeFiles/WriteLogTest.dir/writelogtest.cpp.o: tests/CMakeFiles/WriteLogTest.dir/flags.make
tests/CMakeFiles/WriteLogTest.dir/writelogtest.cpp.o: ../tests/writelogtest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/atrimo-test/async_logger/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/WriteLogTest.dir/writelogtest.cpp.o"
	cd /home/user/Desktop/atrimo-test/async_logger/build/tests && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/WriteLogTest.dir/writelogtest.cpp.o -c /home/user/Desktop/atrimo-test/async_logger/tests/writelogtest.cpp

tests/CMakeFiles/WriteLogTest.dir/writelogtest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WriteLogTest.dir/writelogtest.cpp.i"
	cd /home/user/Desktop/atrimo-test/async_logger/build/tests && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/atrimo-test/async_logger/tests/writelogtest.cpp > CMakeFiles/WriteLogTest.dir/writelogtest.cpp.i

tests/CMakeFiles/WriteLogTest.dir/writelogtest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WriteLogTest.dir/writelogtest.cpp.s"
	cd /home/user/Desktop/atrimo-test/async_logger/build/tests && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/atrimo-test/async_logger/tests/writelogtest.cpp -o CMakeFiles/WriteLogTest.dir/writelogtest.cpp.s

# Object files for target WriteLogTest
WriteLogTest_OBJECTS = \
"CMakeFiles/WriteLogTest.dir/writelogtest.cpp.o"

# External object files for target WriteLogTest
WriteLogTest_EXTERNAL_OBJECTS =

tests/WriteLogTest: tests/CMakeFiles/WriteLogTest.dir/writelogtest.cpp.o
tests/WriteLogTest: tests/CMakeFiles/WriteLogTest.dir/build.make
tests/WriteLogTest: tests/CMakeFiles/WriteLogTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/Desktop/atrimo-test/async_logger/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable WriteLogTest"
	cd /home/user/Desktop/atrimo-test/async_logger/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/WriteLogTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/WriteLogTest.dir/build: tests/WriteLogTest

.PHONY : tests/CMakeFiles/WriteLogTest.dir/build

tests/CMakeFiles/WriteLogTest.dir/clean:
	cd /home/user/Desktop/atrimo-test/async_logger/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/WriteLogTest.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/WriteLogTest.dir/clean

tests/CMakeFiles/WriteLogTest.dir/depend:
	cd /home/user/Desktop/atrimo-test/async_logger/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Desktop/atrimo-test/async_logger /home/user/Desktop/atrimo-test/async_logger/tests /home/user/Desktop/atrimo-test/async_logger/build /home/user/Desktop/atrimo-test/async_logger/build/tests /home/user/Desktop/atrimo-test/async_logger/build/tests/CMakeFiles/WriteLogTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/WriteLogTest.dir/depend

