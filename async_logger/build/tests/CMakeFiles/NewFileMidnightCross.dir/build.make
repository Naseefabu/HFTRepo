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
include tests/CMakeFiles/NewFileMidnightCross.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/NewFileMidnightCross.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/NewFileMidnightCross.dir/flags.make

tests/CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.o: tests/CMakeFiles/NewFileMidnightCross.dir/flags.make
tests/CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.o: ../tests/NewFileMidnightTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/atrimo-test/async_logger/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.o"
	cd /home/user/Desktop/atrimo-test/async_logger/build/tests && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.o -c /home/user/Desktop/atrimo-test/async_logger/tests/NewFileMidnightTest.cpp

tests/CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.i"
	cd /home/user/Desktop/atrimo-test/async_logger/build/tests && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/atrimo-test/async_logger/tests/NewFileMidnightTest.cpp > CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.i

tests/CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.s"
	cd /home/user/Desktop/atrimo-test/async_logger/build/tests && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/atrimo-test/async_logger/tests/NewFileMidnightTest.cpp -o CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.s

# Object files for target NewFileMidnightCross
NewFileMidnightCross_OBJECTS = \
"CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.o"

# External object files for target NewFileMidnightCross
NewFileMidnightCross_EXTERNAL_OBJECTS =

tests/NewFileMidnightCross: tests/CMakeFiles/NewFileMidnightCross.dir/NewFileMidnightTest.cpp.o
tests/NewFileMidnightCross: tests/CMakeFiles/NewFileMidnightCross.dir/build.make
tests/NewFileMidnightCross: tests/CMakeFiles/NewFileMidnightCross.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/Desktop/atrimo-test/async_logger/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable NewFileMidnightCross"
	cd /home/user/Desktop/atrimo-test/async_logger/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/NewFileMidnightCross.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/NewFileMidnightCross.dir/build: tests/NewFileMidnightCross

.PHONY : tests/CMakeFiles/NewFileMidnightCross.dir/build

tests/CMakeFiles/NewFileMidnightCross.dir/clean:
	cd /home/user/Desktop/atrimo-test/async_logger/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/NewFileMidnightCross.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/NewFileMidnightCross.dir/clean

tests/CMakeFiles/NewFileMidnightCross.dir/depend:
	cd /home/user/Desktop/atrimo-test/async_logger/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Desktop/atrimo-test/async_logger /home/user/Desktop/atrimo-test/async_logger/tests /home/user/Desktop/atrimo-test/async_logger/build /home/user/Desktop/atrimo-test/async_logger/build/tests /home/user/Desktop/atrimo-test/async_logger/build/tests/CMakeFiles/NewFileMidnightCross.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/NewFileMidnightCross.dir/depend
