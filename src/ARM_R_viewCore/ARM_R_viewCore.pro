QT += core sql network

TARGET   = ARM_R_viewCore
TEMPLATE = lib
ProjectDir=$$PWD
CONFIG += staticlib

include(../../specs/project.pri)
include(../Graphics_ipp/reference.pri)
include(../../specs/tbb.pri)
include(../../specs/ipp.pri)
include(../../specs/qxt.pri)

SOURCES += \
        CorrelationWidget.cpp \
        GraphicData.cpp \
        GraphicWidget.cpp \
	core.cpp\
	arm_r_view.cpp \
	Tabs/TabManager.cpp \
	Tabs/TabsProperty.cpp \
	Tabs/TabSpectrum.cpp \
	Common/CommonSpectra.cpp \
	Tabs/Tree/TreeItem.cpp \
	Tabs/Tree/TreeModel.cpp \
	Tabs/Tree/DBManager.cpp \
	Tabs/Tree/DBController.cpp \
	Tabs/Tree/TreeView.cpp \
	Tabs/Tree/TreeWidgetDelegate.cpp \
	Tabs/Tree/TreeWidgetDelegate.cpp \
	Common/CommandMessage.cpp \
	Tabs/RPC/RPCClient.cpp \
	Tabs/Tree/DBCache.cpp \
	Common/CommonCorrelations.cpp \
	Tabs/Controls/ControlPRM.cpp \
	Tabs/TabAtlant.cpp \
	Tabs/RPC/RPCAtlant.cpp \
	Controls/ControlPanelWidgets.cpp \
	Controls/ButtonShowPanel.cpp \
	ControlPanel.cpp \
	ControlPanel.cpp

HEADERS += \
        CorrelationWidget.h \
        GraphicData.h \
        GraphicWidget.h \
        ICorrelationWidget.h \
        IGraphicWidget.h \
        stdafx.h \
	arm_r_view.h \
	Abstracts/IModuleController.h \
	Abstracts/ISubModule.h \
	Abstracts/ICommonComponents.h \
	Tabs/TabManager.h \
	Tabs/TabsProperty.h \
	Tabs/TabSpectrum.h \
	Common/CommonSpectra.h \
	Tabs/Tree/TreeItem.h \
	Tabs/Tree/TreeModel.h \
	Tabs/Tree/IDBManager.h \
	Tabs/Tree/DBManager.h \
	Tabs/Tree/IDBController.h \
	Tabs/Tree/DBController.h \
	Tabs/Tree/TreeView.h \
	Tabs/Tree/TreeWidgetDelegate.h \
	Tabs/Tree/TreeWidgetDelegate.h \
	Common/IMessage.h \
	Common/CommandMessage.h \
	Tabs/ITabSpectrum.h \
	Tabs/RPC/IRPC.h \
	Tabs/RPC/RPCClient.h \
	Tabs/Tree/DBCache.h \
	Tabs/Tree/IDBCache.h \
	Tabs/ITabManager.h \
	Common/CommonCorrelations.h \
	Tabs/Controls/ControlPRM.h \
	Tabs/Controls/IControlPRM.h \
	Tabs/TabAtlant.h \
	Tabs/RPC/RPCAtlant.h \
	Tabs/ITabAtlant.h \
	Controls/ControlPanelWidgets.h \
	Controls/ButtonShowPanel.h \
	ControlPanel.h \
	ControlPanel.h

FORMS += \
	arm_r_view.ui
	
LIBS += -luser32

DEFINES += UNICODE
DEFINES += _UNICODE

