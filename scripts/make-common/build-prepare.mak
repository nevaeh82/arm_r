#
# Prepare build output
#
.PHONY:: build-prepare

build-prepare:
	$(call begin-build, Bulding version: $(PRODUCT_VERSION))


clean::
	$(rm) -rf build

