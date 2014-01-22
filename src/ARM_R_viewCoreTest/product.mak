SharedMakFile=../TestShared/testsuite.mak
include $(SharedMakFile)

include $(SolutionDir)/specs/project.mak
include $(SolutionDir)/specs/qxt.mak
include $(SolutionDir)/specs/tbb.mak
include $(SolutionDir)/specs/ipp.mak
include $(SolutionDir)/specs/sqldrivers.mak
include $(SolutionDir)/specs/logger.mak

prebuild::
	$(call begin-build, Prebuild OK )
