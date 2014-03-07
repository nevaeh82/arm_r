
specmak = specs/$(SPEC).mak

specs = $(foreach s, $(wildcard specs/*.mak), $(notdir $(s)))

ifneq "$(wildcard $specmak)" ""
	include $(specmak)
endif

include core/settings.mak

ifdef branch
trunk=branches/$(branch)
endif

revision_file = $(TEMP)/$(SPEC)-revision
version_file  = $(TEMP)/$(SPEC)-version

ifneq "$(wildcard $(version_file))" ""
include $(version_file)
endif

ifneq "$(wildcard $(revision_file))" ""
include $(revision_file)
endif

ifneq "$(wildcard $(buildinfo_dst_path)$(branch))" ""
include $(buildinfo_dst_path)$(branch)
endif

ifeq "$(BUILD_NUMBER)" ""
BUILD_NUMBER=1
endif


restart-make = $(MAKE) --no-print-directory -f $(firstword $(MAKEFILE_LIST)) SPEC=$(SPEC) $(1)

default: get-trunk-revision
	$(info restart with revision....)
	$(call restart-make, default-revisioned)


default-revisioned: get-trunk-version
	$(info default target)
	$(info restart with version....)
	$(call restart-make, default-versioned)

default-versioned: buildinfo-update-versioned
	$(info restart with updated buildinfo....)
	$(call restart-make, buildinfo-print)

include core/mkversion.mak

get-trunk-revision: checkout_ci
	$(call message, openbuild get trunk revision )
	-$(rm) $(revision_file)
	@$(echo) TRUNK_REVISION=$(shell $(call vcs, get-revision) build_dir=$(build_dir) src=$(src) trunk=$(trunk)) > $(revision_file)

get-trunk-version: buildinfo-load
	$(call message, openbuild get trunk version )
	-$(rm) $(version_file)
	$(call vcs, export) build_dir=$(build_dir) src=$(src)  filename=$(buildinfo_src) trunk=trunk rev=$(TRUNK_REVISION)  target=$(version_file)
	sed "s/set //g" < $(version_file)  >$(version_file).tmp
	mv -f $(version_file).tmp $(version_file)
	cat $(version_file)


buildinfo-load:
	$(call message, openbuild convert buildinfo to mak )
ifneq "$(wildcard $(buildinfo_dst_path)$(branch))" ""
	sed "s/set //g" < $(buildinfo_dst_path)$(branch) >$(buildinfo_dst_path)$(branch).tmp
	mv -f $(buildinfo_dst_path)$(branch).tmp $(buildinfo_dst_path)$(branch)
else
	-$(svn) add $(buildinfo_dst_path)$(branch)
endif

buildinfo-update-versioned:
	$(call message, openbuild update buildinfo )

	$(cat $(buildinfo_dst_path)$(branch) )
ifeq ($(strip $(BUILD_VERSION)),)
	$(xsltproc) --param BUILD_NUMBER $(BUILD_NUMBER)  \
		--param MAJOR_NUMBER $(call major,$(PRODUCT_VERSION)) \
		--param MINOR_NUMBER $(call minor,$(PRODUCT_VERSION)) \
		--param TRUNK_REVISION $(TRUNK_REVISION) \
		--param UP "'BUILD'" \
		--output $(buildinfo_dst_path)$(branch) \
		core/buildinfo.xsl core/buildinfo.xsl
else
	$(xsltproc) --param BUILD_NUMBER $(BUILD_NUMBER)  \
		--param MAJOR_NUMBER $(call major,$(BUILD_VERSION)) \
		--param MINOR_NUMBER $(call minor,$(BUILD_VERSION)) \
		--param TRUNK_REVISION $(TRUNK_REVISION) \
		--param UP "'BUILD'" \
		--output $(buildinfo_dst_path)$(branch) \
		core/buildinfo.xsl core/buildinfo.xsl
endif
	-$(cp) $(buildinfo_dst_path)$(branch) $(buildinfo_dst_path)

buildinfo-print: make-tag
	$(info  ---------------------------------------------)
	$(info Build for product opened.)
	$(info Product name     : $(PRODUCT_NAME))
	$(info Product version  : $(BUILD_VERSION))
	$(info Product revision : $(BUILD_REVISION))

buldinfo-commit:
	$(call message, openbuild register build)
	$(svn) ci -m "Build for $(PRODUCT_NAME) version $(BUILD_VERSION) opened"  $(build_ci)

make-tag: buldinfo-commit
	$(call restart-make, verify-tag-exists)
	$(call message, openbuild make-tag)
	$(call vcs, mktag) build_dir=$(build_dir) src=$(src) trunk=$(trunk) rev=$(shell $(call vcs, get-revision) src=$(src) trunk=branches/$(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION))) tag=$(BUILD_VERSION)

verify-tag-exists:
	$(call message, openbuild verify tag)
	$(info $(strip $(shell $(call vcs, ls-tag) src=$(src) tag=$(BUILD_VERSION))))
	$(if $(strip $(shell $(call vcs, ls-tag) src=$(src) tag=$(BUILD_VERSION))), $(error Version $(BUILD_VERSION) already exists. Abort), $(info  Version verified))
