#
# Copy pwgis all files for PwGis
#

prebuild:: $(pwgis-config)
	@echo Deploy pwgis libs
ifeq "$(OS)" "Windows_NT"
	-$(cp) -ud $(SolutionDir)/redist/3rdparty/PwGis/lib_$(MAKE_PLATFORM)/*.dll $(DESTDIR)
else
	-$(cp) -ud $(SolutionDir)/redist/3rdparty/PwGis/lib_$(MAKE_PLATFORM)/*.so* $(DESTDIR)
endif

#$(DESTDIR)/$(TARGET).gis: app.pwgis
#	@echo Create log environment
#	@$(mkdir) $(DESTDIR)/logs
#	@echo Deploy $@
#	@$(cp) $<  $@
