.PHONY:: build-help-projects build-help-header build-help

build-help-header:
	$(info )
	$(info Build solution for platform:)
	$(info )
	$(info Using: make <platform>)
	$(info )
	$(info Available platforms:)
	$(foreach target,$(HELP_PLATFORMS),$(info * $(target)))
	$(info * all		- build for all available plaforms)
	$(info )
	$(info Using: make <project> [MAKE_PLATFORM=mingw/msvc/linux])
	$(info )
	$(info Available projects:)

build-help: build-help-header build-help-projects
	$(info )

build-help-projects:
	$(foreach proj,$(sort $(SUBDIRS)),$(info * $(proj)		- build project for $$(MAKE_PLATFORM)/debug))
