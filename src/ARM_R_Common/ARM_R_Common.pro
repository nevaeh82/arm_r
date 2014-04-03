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
	ServiceHandler/ServiceHandler.cpp \
	ServiceHandler/CommonCodes.cpp \
	CRCs.cpp \
    Rpc/RpcSettingsManager.cpp

HEADERS += \
	ServiceHandler/ServiceTerminateCause.h \
	ServiceHandler/IServiceHandler.h \
	ServiceHandler/ServiceHandler.h \
	ServiceHandler/ProcessState.h \
	ServiceHandler/CommonCodes.h \
	ServiceHandler/ServiceTerminateCauseEnumsToString.h \
	ServiceHandler/CommonCodesEnumsToString.h \
	ServiceHandler/ProcessStateEnumsToString.h \
	Interfaces/IController.h \
	Interfaces/ITerminateRpcController.h \
	Settings/BaseItem.h \
	Settings/Object.h \
	Settings/Property.h \
	Settings/SettingsNode.h \
	BaseSubject.h \
	Interfaces/IRpcControllerBase.h \
	Interfaces/IRpcListener.h \
	Rpc/RpcDefines.h \
	TcpDevicesDefines.h \
	Message.h \
	Interfaces/IMessage.h \
	CRCs.h \
	SingletonTemplate.h \
	SingletonTemplate.hpp \
	CommonDefines.h \
	Info/BaseSettings.h \
	Info/FlakonSettings.h \
	Info/AtlantSettings.h \
	Info/Prm300Settings.h \
	Rpc/RpcMessageStruct.h \
    Info/StationConfiguration.h \
    Rpc/RpcSettingsManager.h \
    Interfaces/IRpcSettingsManager.h \
    Info/AtlantConfiguraton.h
