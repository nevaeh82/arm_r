ifneq "$(filter %-debug, $(MAKECMDGOALS))" ""
   BUILD_PROFILE = debug
   BUILD_ALL = true
   export BUILD_ALL 
endif
ifneq "$(filter %-release, $(MAKECMDGOALS))" ""
   BUILD_PROFILE = release
   BUILD_ALL = true
   export BUILD_ALL 
endif

ifeq "$(BUILD_PROFILE)" ""
   BUILD_PROFILE = debug
endif

ifeq "$(BUILD_PROFILE)" "release"
   QMAKE_PROFILE = CONFIG+=release CONFIG-=debug
endif

ifeq "$(BUILD_PROFILE)" "debug"
   QMAKE_PROFILE = CONFIG-=release CONFIG+=debug
endif

TargetBuildDir = $(call native-path,$(BuildDir)/$(MAKE_PLATFORM)-$(BUILD_PROFILE))


