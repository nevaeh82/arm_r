#-------------------------------------------------
#
# Project created by QtCreator 2013-09-06T11:37:23
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = ARM_R_Common
TEMPLATE = lib
CONFIG += staticlib

include( ../../specs/project.pri )
include( ../../specs/qxt.pri )
include( ../../specs/CISCommonLib.pri )

INCLUDEPATH += $$PWD

SOURCES += \
        CRCs.cpp \

HEADERS += \
	Interfaces/IRpcControllerBase.h \
	Interfaces/IRpcListener.h \
	Rpc/RpcDefines.h \
	TcpDevicesDefines.h \
	CRCs.h \
	CommonDefines.h \
	Info/BaseSettings.h \
	Info/FlakonSettings.h \
	Info/AtlantSettings.h \
	Info/Prm300Settings.h \
	Rpc/RpcMessageStruct.h \
    Info/StationConfiguration.h \
    Info/AtlantConfiguraton.h \
    Info/DBConnectionParameters.h
