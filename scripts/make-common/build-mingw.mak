#
# Solution targets for building for MinGW
#

ifeq "$(OS)" "Windows_NT"

.PHONY:: mingw mingw-debug mingw-release 

HELP_PLATFORMS += mingw mingw-debug mingw-release 

ifeq "$(MAKE_PLATFORM)" "mingw"
  build-project = cd $(call native-path,$(1)) && $(QMAKE) -r -spec win32-g++ $(2) $(QMAKE_PROFILE) && $(MAKE) -C $(1)
endif

mingw: 
	$(MAKE) -f $(firstword $(MAKEFILE_LIST)) mingw-debug mingw-release


mingw-debug mingw-release: build-prepare 
	$(call begin-build, Building MinGw $(BUILD_PROFILE) )
	$(mkdir) $(TargetBuildDir)
	cd $(TargetBuildDir) && $(QMAKE) -r -spec win32-g++ $(SolutionPath) $(QMAKE_PROFILE) BUILD_ALL=$(BUILD_ALL)
	$(MAKE) -C $(TargetBuildDir)

endif