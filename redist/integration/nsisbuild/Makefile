#
# 
#

include ../core/environment.mak

ifeq "$(imagedef_src)"  ""
  imagedef_src       = $(build_dir)/imagedef.xml
endif

ifeq "$(build_dir)" ""
verify::
	$(error build_dir does not defined)
endif


ifeq "$(PRODUCT_VERSION)" ""
verify::
	$(error PRODUCT_VERSION does not defined)
endif


imagedef_versioned = $(build_dir)/imagedefVersioned.xml
image_nsi          = $(build_dir)/image.nsi


export PRODUCT_VERSION

all:
	$(info Usage:  make image imagedef_src=path/to/imagedef.xml [PRODCUCT_VERSION])

$(imagedef_versioned): $(imagedef_src)
	$(sed) s/\(product.*version=\"\)\([0-9\.]\+\)\(\"\)/\1$(PRODUCT_VERSION)\3/i < $(imagedef_src) > $(imagedef_versioned)

$(image_nsi): $(imagedef_versioned)
	$(xsltproc) templates/mkimage.xsl $<  > $@

image: verify $(image_nsi)
ifeq "$(OS)" "Windows_NT"
	"..\nsis\makensis.exe" /V1 /X"SetCompressor zlib" $(image_nsi)
endif
	$(info ----------------------------- Make image done -----------------------)

verify::
	$(info ----------------------------- Make image -----------------------)
	$(info src      : $(imagedef_src) )
	$(info versioned: $(imagedef_versioned) )
	$(info nsi: $(image_nsi) )
