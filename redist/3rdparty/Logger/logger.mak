#
# Copy logger all files for PwLogger
#


ifneq "$(wildcard app.log4qt)" ""
  log4qt-config = $(DESTDIR)/$(TARGET).log4qt
endif


prebuild:: $(log4qt-config)
	@echo Deploy logger libs
ifeq "$(OS)" "Windows_NT"
	-$(cp) -ud $(SolutionDir)/redist/PwLogger-$(HOST)/lib_$(MAKE_PLATFORM)/*.dll $(DESTDIR)
else
	-$(cp) -ud $(SolutionDir)/redist/PwLogger-$(HOST)/lib_$(MAKE_PLATFORM)/*.so* $(DESTDIR)
endif


$(DESTDIR)/$(TARGET).log4qt: app.log4qt
	@echo Create log environment
	@$(mkdir) $(DESTDIR)/var/logs
	@echo Deploy $@
	@$(cp) $<  $@
