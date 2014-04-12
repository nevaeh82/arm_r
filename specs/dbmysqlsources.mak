prebuild:: create-dirs
	-$(cp) -ud $(SolutionDir)/redist/MySql/*.* $(DESTDIR)/DBMySqlSources

create-dirs:
	$(mkdir) $(DESTDIR)/DBMySqlSources   
