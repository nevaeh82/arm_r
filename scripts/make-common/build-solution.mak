#
# Remember self dir
#

BuildScriptsDir     := $(dir $(filter %/build-solution.mak,$(MAKEFILE_LIST)))

#
# Calculate solution path.
#
SolutionPath = $(SolutionDir)/$(SolutionName)


#
# Add all targets for building solution.
#

include $(BuildScriptsDir)/build-astra.mak
include $(BuildScriptsDir)/build-linux.mak
include $(BuildScriptsDir)/build-msvs.mak
include $(BuildScriptsDir)/build-mingw.mak
include $(BuildScriptsDir)/build-msvc.mak
include $(BuildScriptsDir)/build-project.mak
include $(BuildScriptsDir)/build-prepare.mak
include $(BuildScriptsDir)/build-help.mak



