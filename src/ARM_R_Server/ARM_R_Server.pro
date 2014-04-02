QT += core network sql
QT -= gui

CONFIG   += console
CONFIG   -= app_bundle

TARGET   = ARM_R_Server
TEMPLATE = app

ProjectDir=$$PWD

SOURCES = main.cpp

include(../../specs/project.pri)
include(../ARM_R_ServerCore/reference.pri)

applyReferences()
