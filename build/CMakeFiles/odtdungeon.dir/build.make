# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_SOURCE_DIR = /home/rt/odtdungeon

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rt/odtdungeon/build

# Include any dependencies generated for this target.
include CMakeFiles/odtdungeon.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/odtdungeon.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/odtdungeon.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/odtdungeon.dir/flags.make

CMakeFiles/odtdungeon.dir/src/main.cpp.o: CMakeFiles/odtdungeon.dir/flags.make
CMakeFiles/odtdungeon.dir/src/main.cpp.o: /home/rt/odtdungeon/src/main.cpp
CMakeFiles/odtdungeon.dir/src/main.cpp.o: CMakeFiles/odtdungeon.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rt/odtdungeon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/odtdungeon.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/odtdungeon.dir/src/main.cpp.o -MF CMakeFiles/odtdungeon.dir/src/main.cpp.o.d -o CMakeFiles/odtdungeon.dir/src/main.cpp.o -c /home/rt/odtdungeon/src/main.cpp

CMakeFiles/odtdungeon.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odtdungeon.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rt/odtdungeon/src/main.cpp > CMakeFiles/odtdungeon.dir/src/main.cpp.i

CMakeFiles/odtdungeon.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odtdungeon.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rt/odtdungeon/src/main.cpp -o CMakeFiles/odtdungeon.dir/src/main.cpp.s

CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.o: CMakeFiles/odtdungeon.dir/flags.make
CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.o: /home/rt/odtdungeon/src/utils/Debugging.cpp
CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.o: CMakeFiles/odtdungeon.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rt/odtdungeon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.o -MF CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.o.d -o CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.o -c /home/rt/odtdungeon/src/utils/Debugging.cpp

CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rt/odtdungeon/src/utils/Debugging.cpp > CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.i

CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rt/odtdungeon/src/utils/Debugging.cpp -o CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.s

CMakeFiles/odtdungeon.dir/src/utils/List.cpp.o: CMakeFiles/odtdungeon.dir/flags.make
CMakeFiles/odtdungeon.dir/src/utils/List.cpp.o: /home/rt/odtdungeon/src/utils/List.cpp
CMakeFiles/odtdungeon.dir/src/utils/List.cpp.o: CMakeFiles/odtdungeon.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rt/odtdungeon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/odtdungeon.dir/src/utils/List.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/odtdungeon.dir/src/utils/List.cpp.o -MF CMakeFiles/odtdungeon.dir/src/utils/List.cpp.o.d -o CMakeFiles/odtdungeon.dir/src/utils/List.cpp.o -c /home/rt/odtdungeon/src/utils/List.cpp

CMakeFiles/odtdungeon.dir/src/utils/List.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odtdungeon.dir/src/utils/List.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rt/odtdungeon/src/utils/List.cpp > CMakeFiles/odtdungeon.dir/src/utils/List.cpp.i

CMakeFiles/odtdungeon.dir/src/utils/List.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odtdungeon.dir/src/utils/List.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rt/odtdungeon/src/utils/List.cpp -o CMakeFiles/odtdungeon.dir/src/utils/List.cpp.s

CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.o: CMakeFiles/odtdungeon.dir/flags.make
CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.o: /home/rt/odtdungeon/src/utils/Logger.cpp
CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.o: CMakeFiles/odtdungeon.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rt/odtdungeon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.o -MF CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.o.d -o CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.o -c /home/rt/odtdungeon/src/utils/Logger.cpp

CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rt/odtdungeon/src/utils/Logger.cpp > CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.i

CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rt/odtdungeon/src/utils/Logger.cpp -o CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.s

CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.o: CMakeFiles/odtdungeon.dir/flags.make
CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.o: /home/rt/odtdungeon/src/utils/Threading.cpp
CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.o: CMakeFiles/odtdungeon.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rt/odtdungeon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.o -MF CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.o.d -o CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.o -c /home/rt/odtdungeon/src/utils/Threading.cpp

CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rt/odtdungeon/src/utils/Threading.cpp > CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.i

CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rt/odtdungeon/src/utils/Threading.cpp -o CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.s

# Object files for target odtdungeon
odtdungeon_OBJECTS = \
"CMakeFiles/odtdungeon.dir/src/main.cpp.o" \
"CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.o" \
"CMakeFiles/odtdungeon.dir/src/utils/List.cpp.o" \
"CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.o" \
"CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.o"

# External object files for target odtdungeon
odtdungeon_EXTERNAL_OBJECTS =

odtdungeon: CMakeFiles/odtdungeon.dir/src/main.cpp.o
odtdungeon: CMakeFiles/odtdungeon.dir/src/utils/Debugging.cpp.o
odtdungeon: CMakeFiles/odtdungeon.dir/src/utils/List.cpp.o
odtdungeon: CMakeFiles/odtdungeon.dir/src/utils/Logger.cpp.o
odtdungeon: CMakeFiles/odtdungeon.dir/src/utils/Threading.cpp.o
odtdungeon: CMakeFiles/odtdungeon.dir/build.make
odtdungeon: CMakeFiles/odtdungeon.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rt/odtdungeon/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable odtdungeon"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/odtdungeon.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/odtdungeon.dir/build: odtdungeon
.PHONY : CMakeFiles/odtdungeon.dir/build

CMakeFiles/odtdungeon.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/odtdungeon.dir/cmake_clean.cmake
.PHONY : CMakeFiles/odtdungeon.dir/clean

CMakeFiles/odtdungeon.dir/depend:
	cd /home/rt/odtdungeon/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rt/odtdungeon /home/rt/odtdungeon /home/rt/odtdungeon/build /home/rt/odtdungeon/build /home/rt/odtdungeon/build/CMakeFiles/odtdungeon.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/odtdungeon.dir/depend

