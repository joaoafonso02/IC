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
CMAKE_SOURCE_DIR = /home/rafael/Desktop/IC/projetos/project02

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rafael/Desktop/IC/projetos/project02

# Include any dependencies generated for this target.
include CMakeFiles/test_Golomb.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_Golomb.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_Golomb.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_Golomb.dir/flags.make

CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.o: CMakeFiles/test_Golomb.dir/flags.make
CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.o: src/test_Golomb.cpp
CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.o: CMakeFiles/test_Golomb.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rafael/Desktop/IC/projetos/project02/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.o -MF CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.o.d -o CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.o -c /home/rafael/Desktop/IC/projetos/project02/src/test_Golomb.cpp

CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rafael/Desktop/IC/projetos/project02/src/test_Golomb.cpp > CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.i

CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rafael/Desktop/IC/projetos/project02/src/test_Golomb.cpp -o CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.s

CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.o: CMakeFiles/test_Golomb.dir/flags.make
CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.o: src/include/Golomb.cpp
CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.o: CMakeFiles/test_Golomb.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rafael/Desktop/IC/projetos/project02/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.o -MF CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.o.d -o CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.o -c /home/rafael/Desktop/IC/projetos/project02/src/include/Golomb.cpp

CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rafael/Desktop/IC/projetos/project02/src/include/Golomb.cpp > CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.i

CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rafael/Desktop/IC/projetos/project02/src/include/Golomb.cpp -o CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.s

CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.o: CMakeFiles/test_Golomb.dir/flags.make
CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.o: src/include/BitStream.cpp
CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.o: CMakeFiles/test_Golomb.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rafael/Desktop/IC/projetos/project02/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.o -MF CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.o.d -o CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.o -c /home/rafael/Desktop/IC/projetos/project02/src/include/BitStream.cpp

CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rafael/Desktop/IC/projetos/project02/src/include/BitStream.cpp > CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.i

CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rafael/Desktop/IC/projetos/project02/src/include/BitStream.cpp -o CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.s

# Object files for target test_Golomb
test_Golomb_OBJECTS = \
"CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.o" \
"CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.o" \
"CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.o"

# External object files for target test_Golomb
test_Golomb_EXTERNAL_OBJECTS =

bin/test_Golomb: CMakeFiles/test_Golomb.dir/src/test_Golomb.cpp.o
bin/test_Golomb: CMakeFiles/test_Golomb.dir/src/include/Golomb.cpp.o
bin/test_Golomb: CMakeFiles/test_Golomb.dir/src/include/BitStream.cpp.o
bin/test_Golomb: CMakeFiles/test_Golomb.dir/build.make
bin/test_Golomb: CMakeFiles/test_Golomb.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rafael/Desktop/IC/projetos/project02/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable bin/test_Golomb"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_Golomb.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_Golomb.dir/build: bin/test_Golomb
.PHONY : CMakeFiles/test_Golomb.dir/build

CMakeFiles/test_Golomb.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_Golomb.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_Golomb.dir/clean

CMakeFiles/test_Golomb.dir/depend:
	cd /home/rafael/Desktop/IC/projetos/project02 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rafael/Desktop/IC/projetos/project02 /home/rafael/Desktop/IC/projetos/project02 /home/rafael/Desktop/IC/projetos/project02 /home/rafael/Desktop/IC/projetos/project02 /home/rafael/Desktop/IC/projetos/project02/CMakeFiles/test_Golomb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_Golomb.dir/depend

