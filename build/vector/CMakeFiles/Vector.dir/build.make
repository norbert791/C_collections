# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/norbert/Documents/c_projects/c_collections

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/norbert/Documents/c_projects/c_collections/build

# Include any dependencies generated for this target.
include vector/CMakeFiles/Vector.dir/depend.make

# Include the progress variables for this target.
include vector/CMakeFiles/Vector.dir/progress.make

# Include the compile flags for this target's objects.
include vector/CMakeFiles/Vector.dir/flags.make

vector/CMakeFiles/Vector.dir/src/vector.c.o: vector/CMakeFiles/Vector.dir/flags.make
vector/CMakeFiles/Vector.dir/src/vector.c.o: ../vector/src/vector.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/norbert/Documents/c_projects/c_collections/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object vector/CMakeFiles/Vector.dir/src/vector.c.o"
	cd /home/norbert/Documents/c_projects/c_collections/build/vector && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Vector.dir/src/vector.c.o   -c /home/norbert/Documents/c_projects/c_collections/vector/src/vector.c

vector/CMakeFiles/Vector.dir/src/vector.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Vector.dir/src/vector.c.i"
	cd /home/norbert/Documents/c_projects/c_collections/build/vector && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/norbert/Documents/c_projects/c_collections/vector/src/vector.c > CMakeFiles/Vector.dir/src/vector.c.i

vector/CMakeFiles/Vector.dir/src/vector.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Vector.dir/src/vector.c.s"
	cd /home/norbert/Documents/c_projects/c_collections/build/vector && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/norbert/Documents/c_projects/c_collections/vector/src/vector.c -o CMakeFiles/Vector.dir/src/vector.c.s

# Object files for target Vector
Vector_OBJECTS = \
"CMakeFiles/Vector.dir/src/vector.c.o"

# External object files for target Vector
Vector_EXTERNAL_OBJECTS =

vector/libVector.a: vector/CMakeFiles/Vector.dir/src/vector.c.o
vector/libVector.a: vector/CMakeFiles/Vector.dir/build.make
vector/libVector.a: vector/CMakeFiles/Vector.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/norbert/Documents/c_projects/c_collections/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libVector.a"
	cd /home/norbert/Documents/c_projects/c_collections/build/vector && $(CMAKE_COMMAND) -P CMakeFiles/Vector.dir/cmake_clean_target.cmake
	cd /home/norbert/Documents/c_projects/c_collections/build/vector && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Vector.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
vector/CMakeFiles/Vector.dir/build: vector/libVector.a

.PHONY : vector/CMakeFiles/Vector.dir/build

vector/CMakeFiles/Vector.dir/clean:
	cd /home/norbert/Documents/c_projects/c_collections/build/vector && $(CMAKE_COMMAND) -P CMakeFiles/Vector.dir/cmake_clean.cmake
.PHONY : vector/CMakeFiles/Vector.dir/clean

vector/CMakeFiles/Vector.dir/depend:
	cd /home/norbert/Documents/c_projects/c_collections/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/norbert/Documents/c_projects/c_collections /home/norbert/Documents/c_projects/c_collections/vector /home/norbert/Documents/c_projects/c_collections/build /home/norbert/Documents/c_projects/c_collections/build/vector /home/norbert/Documents/c_projects/c_collections/build/vector/CMakeFiles/Vector.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : vector/CMakeFiles/Vector.dir/depend

