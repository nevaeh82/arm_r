prebuild::
	#$(mkdir) $(DESTDIR)/DB
	#$(cp) -ud settings/*.* $(DESTDIR)/
	#$(mkdir) $(DESTDIR)/Tabs
	#$(cp) -ud Tabs/RPC.ini $(DESTDIR)/Tabs
	#$(cp) -ud Tabs/Tabs.ini $(DESTDIR)/Tabs
	#$(mkdir) $(DESTDIR)/RDS
	#$(cp) -ud RDS/DRCorrection.ini $(DESTDIR)/RDS
	#$(cp) -ud RDS/RDSpoints.ini $(DESTDIR)/RDS
	#$(mkdir) $(DESTDIR)/TCP
	#$(cp) -ud TCP/coders.ini $(DESTDIR)/TCP
		
	$(call begin-build, Prebuild OK )
