include ../../scripts/common.mak
include $(SolutionDir)/scripts/project.mak
include ../../specs/project.mak
include ../../specs/qxt.mak
include ../../specs/Solver.mak
include ../../specs/protobuf.mak

prebuild::
	$(cp) -ud qt.conf $(DESTDIR)
	$(mkdir) $(DESTDIR)/DB
	$(cp) -ud DB/db_model.ini $(DESTDIR)/DB
	$(cp) -ud DB/db_tree.sqlite $(DESTDIR)/DB
	$(mkdir) $(DESTDIR)/Tabs
	$(cp) -ud Tabs/RPC.ini $(DESTDIR)/Tabs
	$(cp) -ud Tabs/Tabs.ini $(DESTDIR)/Tabs

	$(call begin-build, Prebuild OK )
