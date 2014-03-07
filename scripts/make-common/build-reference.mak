#
# this makefile provides service for building references
# 

$(REFERENCES):
	$(MAKE) -C $(SolutionDir) HOST=$(HOST) MAKE_PLATFORM=$(MAKE_PLATFORM) BUILD_PROFILE=$(BUILD_PROFILE) -f $(RootMakefile) $@
