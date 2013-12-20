QT += core gui network sql
CONFIG   += gui

TARGET   = ARM_R_view
TEMPLATE = app
ProjectDir=$$PWD

SOURCES = main.cpp

include(../../specs/project.pri)
include(../ARM_R_viewCore/reference.pri)
include(../Graphics_ipp/reference.pri)
include(../../specs/tbb.pri)
include(../../specs/ipp.pri)
include(../../specs/qxt.pri)

RESOURCES += \
        rc.qrc

DEFINES += UNICODE
DEFINES += _UNICODE

applyReferences()
