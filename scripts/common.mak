#
# Common makefile definitions.
#

.SUFFIXES:     

SolutionDir    := $(realpath $(dir $(filter %/common.mak,$(MAKEFILE_LIST)))/..)
ScriptsDir     := $(SolutionDir)/scripts/make-common


include $(ScriptsDir)/common-functions.mak 
include $(ScriptsDir)/common-environment.mak 
include $(ScriptsDir)/common-version.mak 
include $(ScriptsDir)/common-platform.mak 
include $(ScriptsDir)/common-buildprofile.mak 
include $(ScriptsDir)/common-qmake.mak 
include $(ScriptsDir)/common-target.mak 


BuildDir       := $(SolutionDir)/build


ifneq "$(wildcard $(SolutionDir)/custom.mak)" ""
   include $(SolutionDir)/custom.mak
endif
