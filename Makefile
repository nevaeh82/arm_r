#  Sample of solution build makefile.
#
#


#include scripts/common.mak

# Set solution filename
#
#SolutionName = src/project.pro


# Include project list
#
#include src/projects.pri



# define default targts
#
#ifeq "$(OS)" "Windows_NT"
#	DEFAULT_TARGETS = mingw msvc
#else
#	DEFAULT_TARGETS = linux
#endif


#.PHONY:: all help 

#help: build-help

#all: $(DEFAULT_TARGETS)


#
# Add solution build rules.
# 

#include $(ScriptsDir)/build-solution.mak

include product.mak