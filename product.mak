#  Sample of solution build makefile.
#
#

.NOTPARALLEL:

SolutionName=src/ARM_R.pro
include src/ARM_R_projects.pri

include scripts/common.mak

ifeq "$(OS)" "Windows_NT"
	main-targets = msvc-release
	mkrel-targets = mkrel
	platform=msvc
endif

help: build-help

clean::

include $(ScriptsDir)/build-solution.mak

all:
	$(MAKE) $(main-targets)

scripts/common.pri:
	make -C scripts

ignore:
	call scripts\update-ignore.cmd

cleanup:
	for %%i in ( $(Projects) ) do $(MAKE) -C %%i -f product.mak $@ || exit 1

distclean:
	$(rm) -rf build

ifeq "$(OS)" "Windows_NT"
mkrel:
	$(MAKE) -C install -f mkrel-win32.mak
else
mkrel:
	$(MAKE) BUILD_BOARD=$(BUILD_BOARD) -C install -f mkrel-unix.mak
endif


test:
	$(info so: $(SolutionDir)  --  $(fix-sln-make) -- $(SolutionDir)/scripts/make-common ) 
	$(foreach item, $(MAKEFILE_LIST), $(info $(item) )) 
	$(info $(dir $(filter %/build-msvc.mak,$(MAKEFILE_LIST))) )
