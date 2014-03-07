#
# Each Qt project we can build individually.
# Usually this case ued for DEBUG build.



$(SUBDIRS): build-prepare
ifeq "$(MAKE_PLATFORM)"	 ""
	$(info Error: MAKE_PLATFORM does not defined)
	$(info )
	$(info Using: $(notdir $(basename $(MAKE))) $@ MAKE_PLATFORM=mingw BUILD_PROFILE=debug)
	$(info Or:    $(notdir $(basename $(MAKE))) $@ MAKE_PLATFORM=msvc  BUILD_PROFILE=release)
	$(info )
	$(error MAKE_PLATFORM does not defined)
endif
	$(call begin-build, Buiding project $(dir $(SolutionPath))$@ as $(MAKE_PLATFORM)/$(BUILD_PROFILE)  )
	$(if $(wildcard $(call native-path,$(TargetBuildDir)/$@) ), , $(mkdir) $(call native-path,$(TargetBuildDir)/$@))
	$(call build-project, $(TargetBuildDir)/$@, $(dir $(SolutionPath))$@/$@.pro)

