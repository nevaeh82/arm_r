
#
# Sample of common rules for all projects
#

#BUILD_ALL = $$(BUILD_ALL)


VERSION_FILE = $$PWD/../version.ini


include (../scripts/common.pri)

LIBS += -L$$SolutionDir/build/bin/$$SPEC

#
# Common variables
#

DEFINES +=
