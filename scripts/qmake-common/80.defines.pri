#
# Common variables
#

win32-g++|linux-g++  {
	DEFINES += VERSION=\"\\\"$$VERSION\\\"\" 
	DEFINES += SolutionDir=\"\\\"$$SolutionDir\\\"\"
	DEFINES += ProjectName=\"\\\"$$TARGET\\\"\"
}

win32-msvc2010 {
	DEFINES += VERSION=\"$$VERSION\"
	DEFINES += SolutionDir=\"$$SolutionDir\"
	DEFINES += ProjectName=\"$$TARGET\"
}

