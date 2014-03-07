#
# Solution targets for building for MSVS
#

ifneq "$(OS)" "Windows_NT"

.PHONY:: msvs msvs-debug msvs-release 

HELP_PLATFORMS += msvs msvs-debug msvs-release 


ifeq "$(MAKE_PLATFORM)" "msvs"
  build-project = mkdir -p $(1) && cd $(1) && $(QMAKE) -r -spec linux-g++ $(2) $(QMAKE_PROFILE) && $(MAKE) -C $(1)
endif

msvs: 
	$(MAKE) -f $(firstword $(MAKEFILE_LIST)) msvs-debug msvs-release


msvs-debug msvs-release: build-prepare 
	$(call begin-build, Building MSVS $(BUILD_PROFILE) )
	$(mkdir) $(TargetBuildDir)
	cd $(TargetBuildDir) && $(QMAKE) -r -spec linux-g++ $(SolutionPath) $(QMAKE_PROFILE) BUILD_ALL=$(BUILD_ALL)
	$(MAKE) -C $(TargetBuildDir)

endif