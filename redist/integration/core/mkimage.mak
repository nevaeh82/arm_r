$(imagedef_versioned): $(imagedef_src)
	$(sed) s/\(product.*version=\"\)\([0-9\.]\+\)\(\"\)/\1$(PRODUCT_VERSION)\3/i < $(imagedef_src) > $(imagedef_versioned)

$(image_nsi): $(imagedef_versioned)
	$(xsltproc) mkimage.xsl $<  > $@

image: build mkversion $(imagedef_versioned)
	$(info ----------------------------- Make image -----------------------)
ifeq "$(OS)" "Windows_NT"
	"nsis\makensis.exe" /V1 /X"SetCompressor zlib" $(image_nsi)
else
	$(MAKE) -C rpmbuild -f makepackage.mak imagedef=$(imagedef_versioned) projectroot=$(build_dir)
endif
