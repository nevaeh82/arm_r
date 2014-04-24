include ../../scripts/common.mak
include $(SolutionDir)/scripts/project.mak
include ../../specs/project.mak
include ../../specs/qxt.mak
include ../../specs/Solver.mak
include ../../specs/protobuf.mak
include ../../specs/get-runtime.mak
include ../../specs/dbmysqlsources.mak
prebuild::
	$(cp) -ud *.log4qt $(DESTDIR)
	$(cp) -ud qt.conf $(DESTDIR)
	$(mkdir) $(DESTDIR)/DB
	$(cp) -ud DB/db_model.ini $(DESTDIR)/DB
	$(mkdir) $(DESTDIR)/Tabs
	$(cp) -ud Tabs/RPC.ini $(DESTDIR)/Tabs
	$(cp) -ud Tabs/Tabs.ini $(DESTDIR)/Tabs
	$(cp) -ud Tabs/Db.ini $(DESTDIR)/Tabs
	$(mkdir) $(DESTDIR)/RDS
	$(cp) -ud RDS/DRCorrection.ini $(DESTDIR)/RDS
	$(cp) -ud RDS/RDSpoints.ini $(DESTDIR)/RDS
	$(mkdir) $(DESTDIR)/TCP
	$(cp) -ud TCP/coders.ini $(DESTDIR)/TCP
	$(mkdir) $(DESTDIR)/RDP
	$(cp) -ud RDP/cell.net.rdp $(DESTDIR)/RDP
	$(mkdir) $(DESTDIR)/Rpc
	$(cp) -ud Rpc/*.ini $(DESTDIR)/Rpc
              	
	$(call begin-build, Prebuild OK )
