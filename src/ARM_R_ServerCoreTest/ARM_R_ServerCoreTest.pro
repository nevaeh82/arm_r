
TARGET   = ARM_R_ServerCoreTest
TEMPLATE = app

QT     +=  core network
QT     -= gui
CONFIG += console
CONFIG += qtestlib

INCLUDEPATH += ../../tools/cxxtest


ProjectDir=$$PWD

HEADERS += testDBIB.h

include(../../specs/project.pri)
include(../ARM_R_ServerCore/reference.pri)
include(../../specs/qxt.pri)
include(../../specs/Solver.pri)
include(../../specs/protobuf.pri)
include(../TestShared/mksuite.pri)

applyReferences()
