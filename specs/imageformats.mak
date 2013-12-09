prebuild:: create-dirs
ifeq "$(OS)" "Windows_NT"
	-$(cp) -ud $(SolutionDir)/redist/plugins/imageformats/*.dll $(DESTDIR)/plugins/imageformats
else
	-$(cp) -ud $(SolutionDir)/redist/plugins/imageformats/*.so* $(DESTDIR)
endif

create-dirs:
	$(mkdir) $(DESTDIR)/plugins
	$(mkdir) $(DESTDIR)/plugins/imageformats