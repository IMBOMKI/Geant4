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

# Utility rule file for B3a.

# Include the progress variables for this target.
include B3/B3a/CMakeFiles/B3a.dir/progress.make

B3/B3a/CMakeFiles/B3a: B3/B3a/exampleB3a


B3a: B3/B3a/CMakeFiles/B3a
B3a: B3/B3a/CMakeFiles/B3a.dir/build.make

.PHONY : B3a

# Rule to build all files generated by this target.
B3/B3a/CMakeFiles/B3a.dir/build: B3a

.PHONY : B3/B3a/CMakeFiles/B3a.dir/build

B3/B3a/CMakeFiles/B3a.dir/clean:
	cd /home/bomki/Geant4/examples/basic/B3/B3a && $(CMAKE_COMMAND) -P CMakeFiles/B3a.dir/cmake_clean.cmake
.PHONY : B3/B3a/CMakeFiles/B3a.dir/clean

B3/B3a/CMakeFiles/B3a.dir/depend:
	cd /home/bomki/Geant4/examples/basic && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bomki/Geant4/examples/basic /home/bomki/Geant4/examples/basic/B3/B3a /home/bomki/Geant4/examples/basic /home/bomki/Geant4/examples/basic/B3/B3a /home/bomki/Geant4/examples/basic/B3/B3a/CMakeFiles/B3a.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : B3/B3a/CMakeFiles/B3a.dir/depend

