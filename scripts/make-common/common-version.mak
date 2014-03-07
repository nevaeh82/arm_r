ifeq "$(PRODUCT_VERSION)" ""
	ifneq "$(wildcard $(SolutionDir)/version.ini.build)" ""
		include $(SolutionDir)/version.ini.build
	else
		include $(SolutionDir)/version.ini
	endif
	
	PRODUCT_VERSION=$(VERSION)
	export PRODUCT_VERSION
endif

#
#
PRODUCT_VERSION_BUILD := $(if $(PRODUCT_VERSION_REL),$(PRODUCT_VERSION_REL),$(PRODUCT_VERSION).0)

