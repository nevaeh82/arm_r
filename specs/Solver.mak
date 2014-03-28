prebuild::
ifeq "$(OS)" "Windows_NT"
	-$(cp) -ud $(SolutionDir)/redist/Solver/lib/*.dll $(DESTDIR)
	-$(cp) -ud $(SolutionDir)/redist/Solver/lib/detectors.ini $(DESTDIR)
endif