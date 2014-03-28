
TARGET   = ARM_R_viewCoreTest
TEMPLATE = app

QT     +=  core network sql
QT     += gui
CONFIG += console
CONFIG += qtestlib

ProjectDir=$$PWD

INCLUDEPATH += ../../tools/cxxtest

HEADERS += \
    testDBStation.h \
    testGraphicsData.h

include(../../specs/project.pri)
include(../ARM_R_viewCore/reference.pri)
include(../ARM_R_Common/reference.pri)
include(../Graphics_ipp/reference.pri)
include(../../specs/tbb.pri)
include(../../specs/ipp.pri)
include(../../specs/qxt.pri)
include(../../specs/logger.pri )
include(../TestShared/mksuite.pri)

applyReferences()

SOURCES +=

SOURCES +=

SOURCES +=
