
specmak = specs/$(SPEC).mak

specs = $(foreach s, $(wildcard specs/*.mak), $(notdir $(s)))

ifneq "$(wildcard $specmak)" ""
	include $(specmak)
endif

include core/settings.mak

revision_file = $(TEMP)/$(SPEC)-revision
version_file  = $(TEMP)/$(SPEC)-version

ifneq "$(wildcard $(version_file))" ""
include $(version_file)
endif

ifneq "$(wildcard $(revision_file))" ""
include $(revision_file)
endif

ifneq "$(wildcard $(buildinfo_dst_path))" ""
include $(buildinfo_dst_path)
endif

ifeq "$(BUILD_NUMBER)" ""
BUILD_NUMBER=1
endif


restart-make = $(MAKE) --no-print-directory -f $(firstword $(MAKEFILE_LIST)) SPEC=$(SPEC) $(1)


default: get-trunk-revision
	$(info restart with revision....)
	$(call restart-make, default-revisioned)


default-revisioned: get-trunk-version buildinfo-load
	$(info default target)
	$(info restart with version....)
	$(call restart-make, default-versioned)

default-versioned: buildinfo-update-versioned
	$(info restart with updated buildinfo....)
	$(call restart-make, buildinfo-print)


include core/mkversion.mak


get-trunk-revision: checkout_ci
	$(call message, openrelease get trunk revision )
	-$(rm) $(revision_file)
	@$(echo) TRUNK_REVISION=$(shell $(call vcs, get-revision) build_dir=$(build_dir) src=$(src) trunk=$(trunk)) > $(revision_file)

get-trunk-version:
	$(call message, openrelease get trunk version )
	-$(rm) $(version_file)

	$(call vcs, export) build_dir=$(build_dir) src=$(src)  filename=$(buildinfo_src) trunk=$(trunk) rev=$(TRUNK_REVISION)  target=$(version_file)
	sed "s/set //g" < $(version_file)  >$(version_file).tmp
	mv -f $(version_file).tmp $(version_file)
	cat $(version_file)


buildinfo-load:
	$(call message, openrelease convert buildinfo to mak)
ifneq "$(wildcard $(buildinfo_dst_path))" ""
	sed "s/set //g" < $(buildinfo_dst_path) > $(buildinfo_dst_path).tmp
	mv -f $(buildinfo_dst_path).tmp  $(buildinfo_dst_path)
	-$(cp) $(buildinfo_dst_path) $(buildinfo_dst_path)$(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION))
else
	-$(cp) $(buildinfo_dst_path) $(buildinfo_dst_path)$(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION))
	-$(svn) add $(buildinfo_dst_path)$(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION))
endif

buildinfo-update-versioned:
	$(call message, openrelease update buildinfo )
	$(cat $(buildinfo_dst_path))
ifeq ($(strip $(BUILD_VERSION)),)
	$(xsltproc) --param BUILD_NUMBER $(BUILD_NUMBER) \
		--param MAJOR_NUMBER $(call major,$(PRODUCT_VERSION)) \
		--param MINOR_NUMBER $(call minor,$(PRODUCT_VERSION)) \
		--param TRUNK_REVISION $(TRUNK_REVISION) \
		--param UP "'MINOR'" \
		--output $(buildinfo_dst_path) \
		core/buildinfo.xsl core/buildinfo.xsl
else
	$(xsltproc) --param BUILD_NUMBER $(BUILD_NUMBER) \
		--param MAJOR_NUMBER $(call major,$(BUILD_VERSION)) \
		--param MINOR_NUMBER $(call minor,$(BUILD_VERSION)) \
		--param TRUNK_REVISION $(TRUNK_REVISION) \
		--param UP "'MINOR'" \
		--output $(buildinfo_dst_path) \
		core/buildinfo.xsl core/buildinfo.xsl
endif
		$(cat $(buildinfo_dst_path))

buildinfo-print: make-branch
	$(info  ---------------------------------------------)
	$(info Release for product opened.)
	cat $(buildinfo_dst_path)
	-$(cp) $(buildinfo_dst_path) $(buildinfo_dst_path)$(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION))
	-$(svn) add $(buildinfo_dst_path)$(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION))
	$(info Product name     : $(PRODUCT_NAME))
	$(info Product orig ver : $(PRODUCT_VERSION))
	$(info Product version  : $(BUILD_VERSION))
	$(info Product revision : $(BUILD_REVISION))
	$(MAKE) --no-print-directory -f openbuild.mak SPEC=$(SPEC) branch=$(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION)) BUILD_REVISION=$(shell $(call vcs, get-revision) build_dir=$(build_dir) src=$(src) trunk=branches/$(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION)))

buldinfo-commit:
	$(call message, openrelease register release)
	$(svn) ci -m "Branch for $(PRODUCT_NAME) version $(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION)) opened"  $(build_ci)

make-branch: buldinfo-commit
	$(call restart-make, verify-branch-exists)
	$(call message, openrelease make-branch)
	$(call vcs, mkbranch) build_dir=$(build_dir) src=$(src) trunk=$(trunk) rev=$(BUILD_REVISION) branch=$(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION))

verify-branch-exists:
	$(call message, openrelease verify branch)
	$(info $(strip $(shell $(call vcs, ls-branch) src=$(src) branch=$(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION)))))
	$(if $(strip $(shell $(call vcs, ls-branch) src=$(src) branch=$(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION)))),$(error Branch $(call major,$(BUILD_VERSION)).$(call minor,$(BUILD_VERSION)) already exists. Abort),$(info  Version verified))
