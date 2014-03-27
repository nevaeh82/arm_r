SharedMakFile=../TestShared/testsuite.mak
include $(SharedMakFile)

include $(SolutionDir)/specs/logger.mak
include $(SolutionDir)/specs/qxt.mak
include $(SolutionDir)/specs/Solver.mak
include $(SolutionDir)/specs/protobuf.mak

prebuild::
	$(cp) -ud ../ARM_R_Server/ARM*.log4qt $(DESTDIR)
	$(cp) -ud ../ARM_R_Server/qt.conf $(DESTDIR)
	$(mkdir) $(DESTDIR)/DB
	$(cp) -ud ../ARM_R_Server/DB/db_model.ini $(DESTDIR)/DB
	$(mkdir) $(DESTDIR)/Tabs
	$(cp) -ud ../ARM_R_Server/Tabs/RPC.ini $(DESTDIR)/Tabs
	$(cp) -ud ../ARM_R_Server/Tabs/Tabs.ini $(DESTDIR)/Tabs
	$(mkdir) $(DESTDIR)/RDS
	$(cp) -ud ../ARM_R_Server/RDS/DRCorrection.ini $(DESTDIR)/RDS
	$(cp) -ud ../ARM_R_Server/RDS/RDSpoints.ini $(DESTDIR)/RDS
	$(mkdir) $(DESTDIR)/TCP
	$(cp) -ud ../ARM_R_Server/TCP/coders.ini $(DESTDIR)/TCP
	$(call begin-build, Prebuild OK )
