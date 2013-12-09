prebuild::
ifeq "$(OS)" "Windows_NT"
	-$(cp) -ud $(SolutionDir)/redist/ipp/lib/*.dll $(DESTDIR)
endif
