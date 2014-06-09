include makeimage-win32-common.mak

.NOTPARALLEL:

DEPLOY = *.exe *.dll *.conf *.ini *.bat
DEPLOY_EXCLUDE = *Test.exe *debug.dll

TmpRel = $(SolutionDir)/build/tmprel
BinDir = $(SolutionDir)/build/bin/msvc

Qt = Gui Core Network Sql

all: makeimage

makeimage: create-dirs copy-qt-msvc copy-bin
	$(call mknsisimage, imagedef.xml)


create-dirs:
	$(mkdir) $(TmpRel)
	$(mkdir) $(TmpRel)/bin
	$(mkdir) $(TmpRel)/image
		
copy-qt-msvc: get-msvc
	$(cp) -u $(foreach name,$(Qt),$(SolutionDir)/build/qt-msvc/Qt$(name)4.dll) $(TmpRel)/bin
	
copy-bin:
	$(cp) -rudf $(foreach name,$(DEPLOY), $(BinDir)/$(name)) $(TmpRel)/bin
	$(rm) $(foreach name,$(DEPLOY_EXCLUDE), $(TmpRel)/bin/$(name))
	$(cp) -rudf $(BinDir)/libiomp5md.dll $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/plugins $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/DB $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/Tabs $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/RDS $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/TCP $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/DBMySqlSources $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/RDP $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/Rpc $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/icons $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/audio $(TmpRel)/bin
	$(cp) -rudf $(BinDir)/phonon-backend $(TmpRel)/bin


	$(echo) VERSION = $(if $(PRODUCT_VERSION_REL),$(PRODUCT_VERSION_REL),$(PRODUCT_VERSION).0) > $(TmpRel)/bin/version.ini

move-components:

include ../specs/get-runtime.mak
