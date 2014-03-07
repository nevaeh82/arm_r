#
# Add Product platform into PATH.
#

export MAKE_PLATFORM
MACHTYPE=$(strip $(shell echo $$MACHTYPE))

ifneq "$(OS)" "Windows_NT"
    ifeq "$(MACHTYPE)" "i686-vniins-linux-gnu"
	MAKE_PLATFORM=msvs
    else ifeq "$(MACHTYPE)" "x86_64-pc-linux-gnu"
	MAKE_PLATFORM=astra
    else
        MAKE_PLATFORM=linux
    endif
else

ifeq "$(PRODUCT_PLATFORM)" ""
$(MAKECMDGOALS) error:
	$(error PRODUCT_PLATFORM env. variable does not defined.)
endif
  
   export PATH

   PATH := $(PRODUCT_PLATFORM)\unxutils\usr\local\wbin;$(PATH)

ifneq "$(filter mingw%, $(MAKECMDGOALS) $(MAKE_PLATFORM) )" ""
   MAKE_PLATFORM=mingw
   QTDIR=$(PRODUCT_PLATFORM)\qt\mingw

   export QTDIR
endif

ifneq "$(filter msvc%, $(MAKECMDGOALS) $(MAKE_PLATFORM) )" ""
   PATH:=$(PRODUCT_PLATFORM)\qt\msvc\bin;$(PATH);"$(VSRoot)";"$(WinSDK)\bin"
   QTDIR=$(PRODUCT_PLATFORM)\qt\msvc
   MAKE_PLATFORM=msvc
   
   export QTDIR
endif


endif  # OS != windows_NT


