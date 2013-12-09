INCLUDEPATH += $$SolutionDir/redist/QXT/include
INCLUDEPATH += $$SolutionDir/redist/QXT/include/QxtCore
INCLUDEPATH += $$SolutionDir/redist/QXT/include/QxtNetwork
LIBS        += -L$$SolutionDir/redist/QXT/lib_$$SPEC
LIBS        += $$library( QxtCore )
LIBS        += $$library( QxtNetwork )
