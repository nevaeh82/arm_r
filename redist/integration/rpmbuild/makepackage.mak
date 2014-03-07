imagemak  = $(imagedef).mak

ifneq "$(wildcard $(imagemak))"  ""
	include $(imagemak)
endif

ifeq "$(PWD)"  ""
	PWD = .
endif

buildroot  = $(PWD)/mybuild

tarname = $(pkgname).tar
tarpath = $(buildroot)/SOURCES/$(tarname)
tarsource =$(projectroot)/$(pkgroot) 

rpmrc   = ~/.rpmmacros
rpmspec  = $(buildroot)/SPECS/$(pkgname).spec
rpmname  = $(pkgname)-0.i386.rpm
rpmpath  = $(buildroot)/RPMS/i386/$(rpmname)
srcpath  = $(buildroot)/SOURCES/$(pkgspec)

imagedir = $(projectroot)/$(dir $(pkgimage))
image    = $(imagedir)/$(rpmname)

ifeq "$(pkgroot)" ""
	pkgfilesroot = $(dir $(pkgfiles))
	build_opt = build_opt
	tarsource := $(buildroot)/$(productname)/fakeroot
endif

include ../core/environment.mak

ifeq "$(OS)" "Windows_NT"
    mkdir = c:\bin\unix\usr\local\wbin\mkdir.exe -p
else
    mkdir = mkdir -p
endif

image-target = $(image)

ifeq "$(pkgname)" ""
    ifeq "$(imagedef)" ""
	image-target = stop
    else
	image-target = restart
    endif
endif

ifeq "$(imagedef)" ""
	all-prebuild = stop
endif
ifeq "$(projectroot)" ""
	all-prebuild = stop
endif


.PHONY: $(rpmrc) $(tarpath)  verifyargs image all clean $(all-prebuild) build_opt


all: $(all-prebuild) $(imagemak)
	$(MAKE) -f makepackage.mak image imagedef=$(imagedef)
	$(MAKE) -f makepackage.mak clean


image: $(image-target)

stop:
	@echo
	@echo Error: Package does not defined. 
	@echo Run: $(MAKE) -f makepackage.mak all imagedef=path/to/imagedef.xml projectroot=path/to/project/root
	@echo 
	@exit /B 1

restart:
	@echo Restarting make...
	$(MAKE) -f makepackage.mak all imagedef=$(imagedef)


$(imagemak): $(imagedef)
	xsltproc -o $@ templates/make-imagedef-mak.xsl $<


$(rpmpath) : $(rpmspec) $(tarpath) $(rpmrc)
	rpmbuild --quiet -ba $(rpmspec) 

$(rpmspec): $(buildroot) $(imagedef) 
	xsltproc -o $@ templates/make-rpm-spec.xsl $(imagedef) 

$(rpmrc):
	echo "%_topdir  $(buildroot) "> $@

$(tarpath):  $(buildroot) $(build_opt)
	tar cf $@ -C $(tarsource) .

$(image): $(rpmpath)
	mv -f $< $@

$(buildroot):
	$(mkdir)  $(buildroot)/BUILD\
		$(buildroot)/RPMS\
		$(buildroot)/RPMS/i386\
		$(buildroot)/SOURCES\
		$(buildroot)/SPECS\
		$(buildroot)/SRPMS
		
build_opt:
	-$(rm)   -rf $(tarsource)
	-$(mkdir) -p $(tarsource)/opt/$(productname)
	$(cp) -r  $(projectroot)/$(pkgfilesroot)/* $(tarsource)/opt/$(productname)
	
	

clean:
	rm -rf $(buildroot) $(imagemak)
