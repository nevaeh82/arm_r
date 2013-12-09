include ../../scripts/common.mak
include $(SolutionDir)/scripts/project.mak
include ../../specs/project.mak
include ../../specs/qxt.mak
include ../../specs/tbb.mak
include ../../specs/ipp.mak
include ../../specs/sqldrivers.mak

prebuild::
	-$(cp) -ud qt.conf $(DESTDIR)
	$(call begin-build, Prebuild OK )
