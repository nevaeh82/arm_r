
TARGET   = ARM_R_viewCoreTest
TEMPLATE = app

QT     +=  core network sql
QT     += gui
CONFIG += console
CONFIG += qtestlib

ProjectDir=$$PWD

INCLUDEPATH += ../../tools/cxxtest

HEADERS += \
	DBStationControllerTest.h \
    SpectrumWidgetDataSourceTest.h

include(../../specs/project.pri)
include(../../specs/tbb.pri)
include(../../specs/ipp.pri)
include(../../specs/qxt.pri)
include(../TestShared/mksuite.pri)
include(../ARM_R_viewCore/reference.pri)

applyReferences()

SOURCES +=
