include core/environment.mak

product=$(SPEC)
specmak = specs/$(product).mak

ifeq "$(SPEC)" ""
error-not-defined:
	$(info Specification does not defined.)
	$(info )
	$(info Error: SPEC does not defined.)
ifeq ($(firstword $(MAKEFILE_LIST)),mkbuild.mak)
	$(info Run: $(MAKE) -f $(firstword $(MAKEFILE_LIST)) [light/release] SPEC=specname)
else
	$(info Run: $(MAKE) -f $(firstword $(MAKEFILE_LIST)) SPEC=specname)
endif
	$(info )
	$(info Available products: )
	$(foreach s,$(specs), $(info *  $(basename $s)))
	$(error Stop)
endif

ifeq "$(wildcard $(specmak))"  ""
error-spec-not-found:
	$(info Specification for $(SPEC) does not defined).
	$(info Please verify specification repository).
	$(error Stop)
else
	include $(specmak)
endif


buildroot = $(CURDIR)/build

ifeq "$(build_mode)" "release"
	build_dir = $(buildroot)/$(product)-rel
else
	build_dir = $(buildroot)/$(product)-light
endif

build_rel = $(buildroot)/$(product)-bin
build_ci  = $(buildroot)/$(product)-ci

imagedef_src       = $(build_dir)/imagedef.xml
imagedef_versioned = $(build_dir)/imagedefVersioned.xml
image_nsi          = $(build_dir)/image.nsi

svnupdatecmd       = $(CURDIR)/var/$(product)-svnupdate.cmd
src_build          = $(build_dir)/$(tmprel)

ifeq "$(buildinfo_dst)" ""
	buildinfo_dst = $(product).buildinfo.cmd
endif

buildinfo_rel      = $(src_build)/$(buildinfo_dst)
buildinfo_dst_path = $(build_ci)/$(buildinfo_dst)
buildinfo_src_path = $(build_dir)/$(buildinfo_src)
buldinfo_dst_uri   = $(ci)/$(buildinfo_dst)

current_version_file  = $(CURDIR)/build/version-$(product)-exists.mak
trunk_version_file    = $(CURDIR)/build/version-$(product)-trunk.mak


ifeq "$(driver)" "hg"
	vcs = $(MAKE) --no-print-directory -f driver-hg.mak vcs-$(strip $(1)) build_dir=$(build_dir)
else
	vcs = $(MAKE) --no-print-directory --silent -f driver-svn.mak vcs-$(strip $(1))
endif

ifeq "$(trunk)" ""
	trunk=trunk
endif

ifeq "$(wildcard build)" ""
	rc := $(shell mkdir build)
endif

.PHONY:: all init default \
		create-ci-repository \
		create-bin-repository \
		verify-bin-repository \
		verify-ci-repository

all: init default


#$(specmak) : $(specfile)
#	$(info Generating mak spec...)
#	sed  "s/set //g" < $< > $@
#disabled auto restart
#	$(MAKE) -f mkbuild.mak $(MAKECMDGOALS) SPEC=$(SPEC)

verify-bin-repository:
	$(svn) ls $(dst) 2>$(nul) >$(nul) || $(MAKE) -f mkbuild.mak SPEC=$(SPEC) create-bin-repository

create-bin-repository:
	$(info Create release repository...)
	$(svn) mkdir -m "Release repository created for $(product)" $(dst)
	$(svn) mkdir -m "Release repository created for $(product)" $(dst)/trunk
	$(svn) mkdir -m "Release repository created for $(product)" $(dst)/tags
	$(svn) mkdir -m "Release repository created for $(product)" $(dst)/branches

verify-ci-repository:
	$(svn) ls $(ci) 2>$(nul)  >$(nul) || $(MAKE) -f mkbuild.mak SPEC=$(SPEC) create-ci-repository

create-ci-repository:
	$(info Create integration  repository...)
	$(svn) mkdir -m "Integration repository created for $(product)" $(ci)

init:
	$(MAKE) -f core/settings.mak $(specmak)
	$(MAKE) -f core/settings.mak vcs-init

vcs-init: verify-ci-repository verify-bin-repository
	$(info VCS starting...)
	$(call vcs, init) src=$(src) branch=$(branch) trunk=$(trunk) build_dir=$(build_dir)
