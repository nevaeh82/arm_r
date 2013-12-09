include ../../scripts/common.mak
# include $(SolutionDir)/specs/project.mak


Suites=$(HEADERS:.h=)


.PHONY:: prebuild postbuild mkdefines copyout testsuite.cpp  pretest


all::
	@$(echo) Avaiable targets: mksuite copyout postbuild clean
	@echo Suites: $(Suites)

prebuild:: testsuite.generated.cpp

testsuite.generated.cpp: $(HEADERS)
#	$(python)  $(SolutionDir)/tools/cxxtest/cxxtestgen.py --runner=VerbosePrinter --have-eh  --have-std --abort-on-fail --template ../TestShared/runner.tpl -o testsuite.cpp $(HEADERS)
	$(python)  $(SolutionDir)/tools/cxxtest/cxxtestgen.py --have-eh  --have-std --abort-on-fail --template ../TestShared/runner.tpl -o testsuite.generated.cpp $(HEADERS)

postbuild:: pretest copyout
ifneq "$(RunInPostbuild)" "no"
	@echo $(DESTDIR_TARGET)
	@echo Starting unit-tests $(patsubst /,\,$(DESTDIR_TARGET))...
ifeq "$(OS)" "Windows_NT"
	cd $(DESTDIR) && $(if $(NORUNSUITE) neq "y", $(DESTDIR_TARGET:/=\\))
else
	cd $(DESTDIR) && $(if $(NORUNSUITE) neq "y", LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(DESTDIR) $(DESTDIR_TARGET))
endif
endif


clean::
	$(rm) testsuite.genrated.cpp ProjectInfo.h



