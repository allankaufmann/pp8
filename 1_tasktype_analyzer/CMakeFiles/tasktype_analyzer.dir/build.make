# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/allan/git/pp8/1_tasktype_analyzer/build

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/allan/git/pp8/1_tasktype_analyzer

# Include any dependencies generated for this target.
include CMakeFiles/tasktype_analyzer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tasktype_analyzer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tasktype_analyzer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tasktype_analyzer.dir/flags.make

CMakeFiles/tasktype_analyzer.dir/main.cpp.o: CMakeFiles/tasktype_analyzer.dir/flags.make
CMakeFiles/tasktype_analyzer.dir/main.cpp.o: main.cpp
CMakeFiles/tasktype_analyzer.dir/main.cpp.o: CMakeFiles/tasktype_analyzer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/allan/git/pp8/1_tasktype_analyzer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tasktype_analyzer.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tasktype_analyzer.dir/main.cpp.o -MF CMakeFiles/tasktype_analyzer.dir/main.cpp.o.d -o CMakeFiles/tasktype_analyzer.dir/main.cpp.o -c /home/allan/git/pp8/1_tasktype_analyzer/main.cpp

CMakeFiles/tasktype_analyzer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tasktype_analyzer.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/allan/git/pp8/1_tasktype_analyzer/main.cpp > CMakeFiles/tasktype_analyzer.dir/main.cpp.i

CMakeFiles/tasktype_analyzer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tasktype_analyzer.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/allan/git/pp8/1_tasktype_analyzer/main.cpp -o CMakeFiles/tasktype_analyzer.dir/main.cpp.s

# Object files for target tasktype_analyzer
tasktype_analyzer_OBJECTS = \
"CMakeFiles/tasktype_analyzer.dir/main.cpp.o"

# External object files for target tasktype_analyzer
tasktype_analyzer_EXTERNAL_OBJECTS =

tasktype_analyzer: CMakeFiles/tasktype_analyzer.dir/main.cpp.o
tasktype_analyzer: CMakeFiles/tasktype_analyzer.dir/build.make
tasktype_analyzer: CMakeFiles/tasktype_analyzer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/allan/git/pp8/1_tasktype_analyzer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tasktype_analyzer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tasktype_analyzer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tasktype_analyzer.dir/build: tasktype_analyzer
.PHONY : CMakeFiles/tasktype_analyzer.dir/build

CMakeFiles/tasktype_analyzer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tasktype_analyzer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tasktype_analyzer.dir/clean

CMakeFiles/tasktype_analyzer.dir/depend:
	cd /home/allan/git/pp8/1_tasktype_analyzer && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/allan/git/pp8/1_tasktype_analyzer/build /home/allan/git/pp8/1_tasktype_analyzer/build /home/allan/git/pp8/1_tasktype_analyzer /home/allan/git/pp8/1_tasktype_analyzer /home/allan/git/pp8/1_tasktype_analyzer/CMakeFiles/tasktype_analyzer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tasktype_analyzer.dir/depend
