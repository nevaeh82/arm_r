
prebuild::
ifeq "$(OS)" "Windows_NT"
	-$(cp) -ud $(SolutionDir)/redist/QXT/lib_$(MAKE_PLATFORM)/*.dll $(DESTDIR)
else
	-$(cp) -ud $(SolutionDir)/redist/QXT/lib_$(MAKE_PLATFORM)/*.so* $(DESTDIR)
endif
