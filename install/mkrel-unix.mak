include ../scripts/common.mak

.NOTPARALLEL:

# include only for root Makefile

mkimagever  = $(sed) 's/__VERSION__/$(PRODUCT_VERSION_BUILD)/g' < $(SolutionDir)/install/$(strip $(1)) >$(SolutionDir)/build/$(strip $(1)).ver

TmpRel 		= $(SolutionDir)/build/tmprel
BinDir 		= $(SolutionDir)/build/bin/$(MAKE_PLATFORM)
AppDir      = $(BinDir)/opt/SkyHobbit


export MACHTYPE

astra: makeimage

makeimage: create-dirs copy-files mkrel-unix-image

create-dirs:
	$(mkdir) $(TmpRel)
	$(mkdir) $(TmpRel)/bin
	$(mkdir) $(TmpRel)/image
	$(mkdir) $(TmpRel)/opt/SkyHobbit
	$(mkdir) $(TmpRel)/usr/share/icons/
	$(mkdir) $(TmpRel)/usr/share/applications/
	
copy-files:
	$(cp) -udfr  $(BinDir)/* $(TmpRel)/bin
	$(rm) -rf  $(TmpRel)/bin/*.a $(TmpRel)/bin/*d.so* $(TmpRel)/bin/*test* $(TmpRel)/bin/*Test* 
	
mkrel-unix-image:
ifeq "$(MAKE_PLATFORM)" "astra"

	$(mv) $(TmpRel)/bin/* $(TmpRel)/opt/SkyHobbit
	chmod 0666 $(TmpRel)/opt/SkyHobbit/*.ini
	$(cp) -udf $(SolutionDir)/src/SkyHobbitAOMS/resources/skyhobbitaoms.png $(TmpRel)/usr/share/icons/
	$(cp) -udf $(SolutionDir)/src/SkyHobbitAOMS/resources/SkyHobbitAOMS.desktop $(TmpRel)/usr/share/applications/
	$(mv) $(TmpRel)/usr $(TmpRel)/opt $(TmpRel)/bin
	
	$(cp) -dur DEBIAN $(TmpRel)/bin
	find $(TmpRel) -iname .svn | xargs rm -rf
	$(sed) 's/__VERSION__/2.0/g' < DEBIAN/control >$(TmpRel)/bin/DEBIAN/control
	fakeroot dpkg-deb --build $(TmpRel)/bin
	mv $(TmpRel)/bin.deb $(TmpRel)/image/SkyHobbit_2.0-0_amd64.deb
	$(rm) -fr $(TmpRel)/bin/DEBIAN
endif
