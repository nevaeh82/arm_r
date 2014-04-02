SharedMakFile=../TestShared/testsuite.mak
include $(SharedMakFile)

include $(SolutionDir)/specs/project.mak
include $(SolutionDir)/specs/qxt.mak
include $(SolutionDir)/specs/tbb.mak
include $(SolutionDir)/specs/ipp.mak
include $(SolutionDir)/specs/sqldrivers.mak

prebuild::
	-$(cp) -ud ../ARM_R_view/qt.conf $(DESTDIR)
	$(call begin-build, Prebuild OK )
