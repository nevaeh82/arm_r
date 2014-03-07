#
# Solution targets for building for Linux
#

ifeq "$(MAKE_PLATFORM)" "linux"

.PHONY:: linux linux-debug linux-release 

HELP_PLATFORMS += linux linux-debug linux-release 

ifeq "$(MAKE_PLATFORM)" "linux"
  build-project = cd $(1) && $(QMAKE) -r -spec linux-g++ $(2) $(QMAKE_PROFILE) && $(MAKE) -C $(1)
endif

linux: 
	$(MAKE) -f $(firstword $(MAKEFILE_LIST)) linux-debug linux-release


linux-debug linux-release: build-prepare 
	$(call begin-build, Building Linux $(BUILD_PROFILE) )
	$(mkdir) $(TargetBuildDir)
	cd $(TargetBuildDir) && $(QMAKE) -r -spec linux-g++ $(SolutionPath) $(QMAKE_PROFILE)
	$(MAKE) -C $(TargetBuildDir)

endif