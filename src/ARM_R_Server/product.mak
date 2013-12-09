include ../../scripts/common.mak
include $(SolutionDir)/scripts/project.mak
include ../../specs/project.mak
include ../../specs/qxt.mak
include ../../specs/Solver.mak
include ../../specs/protobuf.mak
include ../../specs/sqldrivers.mak

prebuild::
	-$(cp) -ud qt.conf $(DESTDIR)
	$(call begin-build, Prebuild OK )
