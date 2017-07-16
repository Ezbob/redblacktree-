##
#	This file is part of the quick make makefile project and contains all the 
#	variables needed to build the project.
##

# Which compiler to use
CXX = g++

# Compiler flags added
CXXFLAGS = -O3 -std=c++14 -Wall -Wextra -Wpedantic

# Valgrind options
VALGRIND_OPTS =

# Libraries to use 
LDLIBS = 

# Linker flags
LDFLAGS =

# Modules within source directory and test directory
MODULES = redblacktree
TEST_MODULES = redblacktree_test

# Test executable name (may be empty if the project doesn't have any tests) 
TEST_EXEC = test

# Main executable name (may be empty if the project doesn't have a specific main function)
EXEC = rbt

## Directory names (remember to add the trailing "/" for this to work)

# template definition directory
TEMPLATE_PREFIX = templates/

# Name for the build directory
BUILD_PREFIX = build/

# Name for the source directory where the source files are located
SOURCE_PREFIX = src/

# Directory name for the directory where the test source code is located 
TEST_PREFIX = tests/

# Directory name for where to look for header files (included with double quotes)
INCLUDE_PREFIX = include/

# Sub directory name for the compiled source code(found in SOURCE_PREFIX) to be placed within the build directory
COMPILED_DIR = out/

# Sub directory name for the compiled test source code(found in SOURCE_PREFIX) to be placed within the build directory
T_COMPILED_DIR = test_out/
