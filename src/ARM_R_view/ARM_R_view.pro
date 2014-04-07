QT += core gui network sql
CONFIG   += gui
#CONFIG   += console

TARGET   = ARM_R_view
TEMPLATE = app
ProjectDir=$$PWD

SOURCES = main.cpp

include(../../specs/project.pri)
include(../ARM_R_viewCore/reference.pri)

RESOURCES += \
		rc.qrc

DEFINES += UNICODE
DEFINES += _UNICODE

applyReferences()
