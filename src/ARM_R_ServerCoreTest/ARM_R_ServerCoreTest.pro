TARGET   = ARM_R_ServerCoreTest
TEMPLATE = app

ProjectDir=$$PWD

QT     +=  core network
QT     -= gui

CONFIG += console
CONFIG += qtestlib

INCLUDEPATH += ../../tools/cxxtest

HEADERS += testDBIB.h \
	testSettings.h


include(../../specs/project.pri)
include(../../specs/qxt.pri)
include(../../specs/Solver.pri)
include(../../specs/protobuf.pri)
include(../TestShared/mksuite.pri)
include(../ARM_R_ServerCore/reference.pri)

applyReferences()
