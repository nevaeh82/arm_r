#
# Solution targets for building for MSVC2010
#
ifeq "$(OS)" "Windows_NT"


SlnPath       := $(call windows-path,$(TargetBuildDir)/$(notdir $(SolutionName:.pro=.sln)))
fix-sln-make  := $(dir $(filter %/build-msvc.mak,$(MAKEFILE_LIST)))/fix-generated-sln.mak




.PHONY:: msvc msvc-debug msvc-release

HELP_PLATFORMS += msvc msvc-debug msvc-release



ifeq "$(MAKE_PLATFORM)" "msvc"
  build-project = cd $(call native-path,$(1)) && $(QMAKE) -r -tp vc -spec win32-msvc2010 -o project.vcxproj $(2) $(QMAKE_PROFILE) \
	&& $(msbuild) project.vcxproj /p:Configuration=$(BUILD_PROFILE) \
			/fileLoggerParameters:LogFile="$(call windows-path,$(strip $(1)))\build.log";Verbosity=normal;Encoding=UTF-8
endif


msvc: 
	$(MAKE) -f $(firstword $(MAKEFILE_LIST)) msvc-debug msvc-release


msvc-release msvc-debug: build-prepare 
	$(call begin-build,  Building MSVC2010  $(BUILD_PROFILE) )

	$(mkdir) $(TargetBuildDir)
	cd $(TargetBuildDir) && $(QMAKE) -r -tp vc -spec win32-msvc2010 $(call windows-path, $(SolutionPath)) $(QMAKE_PROFILE) BUILD_ALL=$(BUILD_ALL)
	$(MAKE) -f $(fix-sln-make) fix BuildDir=$(TargetBuildDir) SlnFile=$(SlnPath) -C $(SolutionDir)/scripts/make-common
	$(msbuild) $(SlnPath) /target:Build  /p:Configuration=$(BUILD_PROFILE)  /fileLoggerParameters:LogFile=$(TargetBuildDir)/build.log;Verbosity=normal;Encoding=UTF-8

endif