# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bomki/Geant4/examples/basic

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bomki/Geant4/examples/basic

# Utility rule file for B5.

# Include the progress variables for this target.
include B5/CMakeFiles/B5.dir/progress.make

B5/CMakeFiles/B5: B5/exampleB5


B5: B5/CMakeFiles/B5
B5: B5/CMakeFiles/B5.dir/build.make

.PHONY : B5

# Rule to build all files generated by this target.
B5/CMakeFiles/B5.dir/build: B5

.PHONY : B5/CMakeFiles/B5.dir/build

B5/CMakeFiles/B5.dir/clean:
	cd /home/bomki/Geant4/examples/basic/B5 && $(CMAKE_COMMAND) -P CMakeFiles/B5.dir/cmake_clean.cmake
.PHONY : B5/CMakeFiles/B5.dir/clean

B5/CMakeFiles/B5.dir/depend:
	cd /home/bomki/Geant4/examples/basic && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bomki/Geant4/examples/basic /home/bomki/Geant4/examples/basic/B5 /home/bomki/Geant4/examples/basic /home/bomki/Geant4/examples/basic/B5 /home/bomki/Geant4/examples/basic/B5/CMakeFiles/B5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : B5/CMakeFiles/B5.dir/depend
