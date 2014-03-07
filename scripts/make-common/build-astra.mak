#
# Solution targets for building for Astra
#

ifneq "$(OS)" "Windows_NT"

.PHONY:: astra astra-debug astra-release 

HELP_PLATFORMS += astra astra-debug astra-release 

ifeq "$(MAKE_PLATFORM)" "astra"
  build-project = cd $(1) && $(QMAKE) -r -spec linux-g++ $(2) $(QMAKE_PROFILE) && $(MAKE) -C $(1)
endif

astra: 
	$(MAKE) -f $(firstword $(MAKEFILE_LIST)) astra-debug astra-release


astra-debug astra-release: build-prepare 
	$(call begin-build, Building Astra $(BUILD_PROFILE) )
	$(mkdir) $(TargetBuildDir)
	cd $(TargetBuildDir) && $(QMAKE) -r -spec linux-g++ $(SolutionPath) $(QMAKE_PROFILE) BUILD_ALL=$(BUILD_ALL)
	$(MAKE) -C $(TargetBuildDir)

endif