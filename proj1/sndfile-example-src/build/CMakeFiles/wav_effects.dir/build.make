# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.25.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.25.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src/build

# Include any dependencies generated for this target.
include CMakeFiles/wav_effects.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/wav_effects.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/wav_effects.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/wav_effects.dir/flags.make

CMakeFiles/wav_effects.dir/wav_effects.cpp.o: CMakeFiles/wav_effects.dir/flags.make
CMakeFiles/wav_effects.dir/wav_effects.cpp.o: /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src/wav_effects.cpp
CMakeFiles/wav_effects.dir/wav_effects.cpp.o: CMakeFiles/wav_effects.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/wav_effects.dir/wav_effects.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/wav_effects.dir/wav_effects.cpp.o -MF CMakeFiles/wav_effects.dir/wav_effects.cpp.o.d -o CMakeFiles/wav_effects.dir/wav_effects.cpp.o -c /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src/wav_effects.cpp

CMakeFiles/wav_effects.dir/wav_effects.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wav_effects.dir/wav_effects.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src/wav_effects.cpp > CMakeFiles/wav_effects.dir/wav_effects.cpp.i

CMakeFiles/wav_effects.dir/wav_effects.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wav_effects.dir/wav_effects.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src/wav_effects.cpp -o CMakeFiles/wav_effects.dir/wav_effects.cpp.s

# Object files for target wav_effects
wav_effects_OBJECTS = \
"CMakeFiles/wav_effects.dir/wav_effects.cpp.o"

# External object files for target wav_effects
wav_effects_EXTERNAL_OBJECTS =

/Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-bin/wav_effects: CMakeFiles/wav_effects.dir/wav_effects.cpp.o
/Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-bin/wav_effects: CMakeFiles/wav_effects.dir/build.make
/Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-bin/wav_effects: CMakeFiles/wav_effects.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-bin/wav_effects"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wav_effects.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/wav_effects.dir/build: /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-bin/wav_effects
.PHONY : CMakeFiles/wav_effects.dir/build

CMakeFiles/wav_effects.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wav_effects.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wav_effects.dir/clean

CMakeFiles/wav_effects.dir/depend:
	cd /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src/build /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src/build /Users/joaoafonso/Desktop/ICProjs/IC/sndfile-example-src/build/CMakeFiles/wav_effects.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/wav_effects.dir/depend

