include ../../scripts/common.mak
include $(SolutionDir)/scripts/project.mak
include ../../specs/project.mak
include ../../specs/qxt.mak
include ../../specs/tbb.mak
include ../../specs/ipp.mak
include ../../specs/sqldrivers.mak

prebuild::
	$(cp) -ud *.log4qt $(DESTDIR)
	-$(cp) -ud qt.conf $(DESTDIR)
	-$(cp) -ud ../../version.ini $(DESTDIR)
	$(call begin-build, Prebuild OK )

postbuild::
	-$(mkdir) $(SolutionDir)/build/bin/$(MAKE_PLATFORM)/DB
	-$(cp) -ud *.sqlite $(DESTDIR)/DB
	$(cp) -ud *.bat $(DESTDIR)
	$(mkdir) $(SolutionDir)/build/bin/$(MAKE_PLATFORM)/icons
	$(cp) -ud images/shortcut.ico $(DESTDIR)/icons
	$(cp) -ud ARM_R.ini $(DESTDIR)
