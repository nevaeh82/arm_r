include ../../scripts/common.mak
include $(SolutionDir)/scripts/project.mak
include ../../specs/project.mak
include ../../specs/qxt.mak
include ../../specs/tbb.mak
include ../../specs/ipp.mak
include ../../specs/sqldrivers.mak
include ../../specs/logger.mak

prebuild::
	-$(cp) -ud qt.conf $(DESTDIR)
	$(call begin-build, Prebuild OK )
	
postbuild::
	-$(mkdir) $(SolutionDir)/build/bin/$(MAKE_PLATFORM)/DB
	-$(cp) -ud *.sqlite $(DESTDIR)/DB
	
	
