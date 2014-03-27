#
# Default project make.
#

SpecsDir     := $(SolutionDir)/specs
RootMakefile := $(SolutionDir)/Makefile
PLATFORM := $(MAKE_PLATFORM)

all::
	$(info project.mak root)

include $(SolutionDir)/scripts/project.mak

clean::
	$(rm) object_script.* *.user