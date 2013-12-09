include ../scripts/common.mak


mkimagever = $(sed) s/__VERSION__/$(PRODUCT_VERSION_BUILD)/g < $(SolutionDir)/install/$(strip $(1)) >$(SolutionDir)/build/$(strip $(1)).ver
nsisbuild   = $(MAKE) -C ..\redist\integration\nsisbuild image build_dir=$(SolutionDir) PRODUCT_VERSION=$(if $(PRODUCT_VERSION_REL),$(PRODUCT_VERSION_REL),$(PRODUCT_VERSION).0)  imagedef_src=$(SolutionDir)\build\$(strip $(1)).ver
mknsisimage = $(mkdir) $(SolutionDir)/build/tmprel/image && $(call mkimagever,$(1) ) && $(call nsisbuild,$(1))


