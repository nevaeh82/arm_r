SharedMakFile=../TestShared/testsuite.mak
include $(SharedMakFile)

include $(SolutionDir)/specs/logger.mak
include $(SolutionDir)/specs/qxt.mak
include $(SolutionDir)/specs/Solver.mak
include $(SolutionDir)/specs/protobuf.mak

prebuild::
	$(call begin-build, Prebuild OK )
