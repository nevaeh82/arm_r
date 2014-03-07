#
# Default implementation of product.mak
#
.SUFFIXES:     

unique = $(if $(1),$(call unique,$(wordlist 2,$(words $(1)),$(1)),$(if $(filter $(firstword $(1)),$(2)),$(2),$(firstword $(1)) $(2))),$(2))

all::

clean::
	$(RM) *.user *.vcxproj.* *.vcproj *.pdb ui_*.h *.gch
	$(RM) -rf debug release ipch 
	$(RM) Makefile Makefile.* 

distclean:: clean
	$(RM) *.user

prebuild:: $(call unique,$(REFERENCES))

postbuild::


include $(SolutionDir)/scripts/make-common/build-reference.mak
