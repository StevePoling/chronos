# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.24.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.24.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/steve/Projects/chronos

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/steve/Projects/chronos

# Include any dependencies generated for this target.
include CMakeFiles/chronos.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/chronos.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/chronos.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/chronos.dir/flags.make

CMakeFiles/chronos.dir/src/chronos.cpp.o: CMakeFiles/chronos.dir/flags.make
CMakeFiles/chronos.dir/src/chronos.cpp.o: src/chronos.cpp
CMakeFiles/chronos.dir/src/chronos.cpp.o: CMakeFiles/chronos.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/steve/Projects/chronos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/chronos.dir/src/chronos.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/chronos.dir/src/chronos.cpp.o -MF CMakeFiles/chronos.dir/src/chronos.cpp.o.d -o CMakeFiles/chronos.dir/src/chronos.cpp.o -c /Users/steve/Projects/chronos/src/chronos.cpp

CMakeFiles/chronos.dir/src/chronos.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chronos.dir/src/chronos.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/steve/Projects/chronos/src/chronos.cpp > CMakeFiles/chronos.dir/src/chronos.cpp.i

CMakeFiles/chronos.dir/src/chronos.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chronos.dir/src/chronos.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/steve/Projects/chronos/src/chronos.cpp -o CMakeFiles/chronos.dir/src/chronos.cpp.s

# Object files for target chronos
chronos_OBJECTS = \
"CMakeFiles/chronos.dir/src/chronos.cpp.o"

# External object files for target chronos
chronos_EXTERNAL_OBJECTS =

chronos: CMakeFiles/chronos.dir/src/chronos.cpp.o
chronos: CMakeFiles/chronos.dir/build.make
chronos: CMakeFiles/chronos.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/steve/Projects/chronos/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable chronos"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chronos.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/chronos.dir/build: chronos
.PHONY : CMakeFiles/chronos.dir/build

CMakeFiles/chronos.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chronos.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chronos.dir/clean

CMakeFiles/chronos.dir/depend:
	cd /Users/steve/Projects/chronos && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/steve/Projects/chronos /Users/steve/Projects/chronos /Users/steve/Projects/chronos /Users/steve/Projects/chronos /Users/steve/Projects/chronos/CMakeFiles/chronos.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/chronos.dir/depend

