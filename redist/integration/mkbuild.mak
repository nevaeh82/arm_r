
specmak = specs/$(SPEC).mak

specs = $(foreach s, $(wildcard specs/*.mak), $(notdir $(s)))

ifneq "$(wildcard $specmak)" ""
	include $(specmak)
endif

export build_mode
include core/settings.mak

ifeq  "$(build_mode)" "release"
	checkout_source = checkout_source_tag
	include $(current_version_file)
endif
ifeq  "$(build_mode)" "light"
	checkout_source = checkout_source_trunk
	include $(trunk_version_file)

	#make current version as build version.
	BUILD_VERSION := $(PRODUCT_VERSION)
endif

$(info  buildmode: '$(build_mode)'  $(MAKEFILES) --- $(MAKECMDGOALS) -- $(MAKEFLAGS) )

ifneq "$(wildcard $(imagedef_src))" ""
	image = image
endif

PRODUCT_VERSION_REL := $(BUILD_VERSION)
PRODUCT_VERSION     := $(BUILD_VERSION)

ifeq "$(build_mode)" "light"
	PRODUCT_VERSION_REL := $(PRODUCT_VERSION).0
endif


tagsrc=$(dst)/trunk
tagdst=$(dst)/tags/$(PRODUCT_VERSION_REL)


export PRODUCT_VERSION
export BUILD_REVISION
export BUILD_VERSION
export PRODUCT_NAME
export PRODUCT_VERSION
export PRODUCT_VERSION_REL



.PHONY:: light release stop-no-spec pre-target build $(current_version_file)

default: light

light: init $(pretarget)
	$(info ------------------------------ start as light_build   ---------------------- )
	$(MAKE) -f mkbuild.mak SPEC=$(SPEC) "$(specmak)"
	$(MAKE) -f mkbuild.mak SPEC=$(SPEC) mkversion_trunk
	$(MAKE) -f mkbuild.mak SPEC=$(SPEC) build_mode=light light_build

light_build: init mergeinfo
	$(echo) Light build done.

release: init $(pretarget)
	$(info ------------------------------ start as release_build  ---------------------- )
	$(MAKE) -f mkbuild.mak SPEC=$(SPEC) "$(specmak)"
	$(MAKE) -f mkbuild.mak SPEC=$(SPEC) mkversion_ci
	$(MAKE) -f mkbuild.mak SPEC=$(SPEC) build_mode=release release_build

release_build: settags
	$(echo) Release build done.


include core/mkversion.mak


mkversion_light: mkversion_trunk
	$(info )
	$(info Product Version:$(PRODUCT_VERSION), Build=$(BUILD_NUMBER), Release=$(PRODUCT_VERSION_REL))
	$(info )


mkversion_release_info:
	$(info )
	$(info Next product Version:$(PRODUCT_VERSION), Build=$(BUILD_NUMBER), Release=$(PRODUCT_VERSION_REL))
	$(info )

verify_duplicated_build: mkversion_release_info
	$(info ------------------------------  verify tag  $(BUILD_VERSION) in release ---------------------------)
	$(if $(shell $(svn) ls  $(dst)/tags/$(PRODUCT_VERSION_REL) 2>$(nul)), $(error Version '$(PRODUCT_VERSION_REL)' already exists. Build rejected.))

mkversion_release: verify_duplicated_build


# -------------------------------------------------------------------------------------------------------------------
# Build. Verify build results.
# -------------------------------------------------------------------------------------------------------------------

checkout_source_trunk:
	$(info ------------------------------ checkout trunk version ---------------------- )
	$(call vcs, checkout-branch) src=$(src) trunk=$(trunk) target=$(build_dir)

checkout_source_tag:
	$(info ------------------------------  checkout tag $(BUILD_VERSION) ---------------------------)
ifneq "$(wildcard $(build_dir))" ""
	$(call vcs, switch-tag) src=$(src) tag=$(BUILD_VERSION) target=$(build_dir) >$(nul)
else
	$(info found $(build_dir))
	$(call vcs, checkout-tag) src=$(src) tag=$(PRODUCT_VERSION_REL) target=$(build_dir) trunk=$(trunk)
endif

build: mergeinfo


build_execute: $(checkout_source)
	$(info ------------------------------  Build ---------------------------)
	cd $(build_dir) && $(buildscript)
	$(call echo,Build done.)

verify_tmprel_exists: build_execute
	$(if $(wildcard $(src_build)),$(info tmprel ok),\
			$(error Result directory "$(src_build)" does not exists.))
checkinvalid: verify_tmprel_exists
	$(info ------------------------------  check invalid files  ---------------------------)
	$(if $(shell $(find) $(src_build) -name .svn), $(error Error: Tmprel MUST not contain .svn))

mergeinfo: checkinvalid
	$(info ----------------------------- Collect mergeinfo  -----------------)

	$(call vcs, merged) src=$(src) trunk=$(trunk) > $(call unix-path,$(src_build)\mergeinfo.txt)

# -------------------------------------------------------------------------------------------------------------------
# Update repository of binaries.
# -------------------------------------------------------------------------------------------------------------------
.PHONY:: checkout_rel update commit settags

checkout_rel:
	$(info ------------------------------ Checkout release  -----------------)
	-rm -rf $(build_rel)
	$(svn) co $(dst)/trunk  $(build_rel)

update: build  checkout_rel
	$(info ------------------------------  Update repository  ---------------)
	-rm -rf $(build_rel)-svn
	mv $(build_rel)/.svn $(build_rel)-svn
	rm -rf $(build_rel)/*
	$(call copy_dir,$(build_dir)/$(tmprel)/*,$(build_rel))
	mv $(build_rel)-svn $(build_rel)/.svn

addremove: update
	$(info ------------------------------  Add/Remove files ---------------)
	svn status $(build_rel) | grep "? " | cut -b 2- >addfiles
	svn status $(build_rel) | grep "! " | cut -b 2- >rmfiles
	-svn rm --targets rmfiles
	-svn add --no-ignore  --targets addfiles

commit: addremove
	$(info  ------------------------------  Commit ---------------------------)
	$(svn) ci -m "Build created $(PRODUCT_VERSION_REL)" $(build_rel)


settags: commit
	$(info ------------------------------  Set tags  -------------------------)

	$(svn) cp -m "Created tag $(tagdst)"  $(tagsrc) $(tagdst)
	$(call echo, Created tag $(tagdst))

include core/mkimage.mak
