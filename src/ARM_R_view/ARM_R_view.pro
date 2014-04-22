QT += core gui network sql
CONFIG   += gui
#CONFIG   += console

TARGET   = ARM_R_view
TEMPLATE = app
ProjectDir=$$PWD

CONFIG += qaxcontainer

LIBS+=user32.lib
LIBS += -lqaxcontainer

SOURCES = main.cpp

include(../../specs/project.pri)
include(../ARM_R_viewCore/reference.pri)

RESOURCES += \
		rc.qrc

DEFINES += UNICODE
DEFINES += _UNICODE

applyReferences()

win32{
    HEADERS = version.h
    RC_FILE = details_res.rc
}

