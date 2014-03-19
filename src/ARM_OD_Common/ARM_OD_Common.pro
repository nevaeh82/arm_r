#-------------------------------------------------
#
# Project created by QtCreator 2013-09-06T11:37:23
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = ARM_OD_Common
TEMPLATE = lib
CONFIG += staticlib

include(  ../../specs/project.pri )
include(  ../../specs/qxt.pri )
include(  ../../specs/logger.pri )

INCLUDEPATH += $$PWD

SOURCES += \
    Rpc/RpcDefines.cpp

HEADERS += \
    Rpc/RpcDefines.h \
