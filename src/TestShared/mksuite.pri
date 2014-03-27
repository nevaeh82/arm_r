INCLUDEPATH += \
		 . \
	 $$SolutionDir/tools \
	 $$SolutionDir/tools/cxxtest \
	 $$SolutionDir/tools/cxxmock \
	 $$ProjectDir/../TestShared

SOURCES += $$PWD/testsuite.cpp

HEADERS += ../TestShared/QApplicationRunner.h
HEADERS += ../TestShared/QSignalCounter.h

#include(../PwGisCommon/reference.pri)
#include(../RegKit/reference.pri)
#include(../Utilities/reference.pri)

applyReferences()

#generateCxxTests( testsuite.generated.cpp )
