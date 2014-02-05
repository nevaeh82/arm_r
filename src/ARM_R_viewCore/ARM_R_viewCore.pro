QT += core sql network

TARGET   = ARM_R_viewCore
TEMPLATE = lib
ProjectDir=$$PWD
CONFIG += staticlib

include(../../specs/project.pri)
include(../Graphics_ipp/reference.pri)
include(../ARM_R_Common/reference.pri)
include(../../specs/tbb.pri)
include(../../specs/ipp.pri)
include(../../specs/qxt.pri)
include(../../specs/logger.pri)

INCLUDEPATH += $$PWD

SOURCES += \
        Correlations/CorrelationWidget.cpp \
        GraphicData.cpp \
        GraphicWidget.cpp \
	Tabs/TabManager.cpp \
	Tabs/TabsProperty.cpp \
	Common/CommonSpectra.cpp \
	Tabs/Tree/TreeItem.cpp \
	Tabs/Tree/TreeModel.cpp \
	Tabs/Tree/DBManager.cpp \
	Tabs/Tree/DBController.cpp \
	Tabs/Tree/TreeView.cpp \
	Tabs/Tree/TreeWidgetDelegate.cpp \
	Common/CommandMessage.cpp \
	Tabs/RPC/RPCClient.cpp \
	Tabs/Tree/DBCache.cpp \
	Common/CommonCorrelations.cpp \
	Tabs/Controls/ControlPRM.cpp \
	Tabs/RPC/RPCAtlant.cpp \
	Controls/ControlPanelWidgets.cpp \
	Controls/ButtonShowPanel.cpp \
    Common/ServiceHandler/ServiceHandler.cpp \
    Common/ServiceHandler/CommonCodes.cpp \
    MainWindow.cpp \
    MainWindowController.cpp \
    ControlPanel/ControlPanelWidget.cpp \
    ControlPanel/ControlPanelController.cpp \
    Tabs/TabSpectrumWidget.cpp \
    Correlations/CorrelationGroupWidget.cpp \
    Tabs/AtlantTabWidget.cpp

HEADERS += \
        Correlations/CorrelationWidget.h \
        GraphicData.h \
        GraphicWidget.h \
        ICorrelationWidget.h \
        IGraphicWidget.h \
        stdafx.h \
	Abstracts/IModuleController.h \
	Abstracts/ISubModule.h \
	Abstracts/ICommonComponents.h \
	Tabs/TabManager.h \
	Tabs/TabsProperty.h \
	Common/CommonSpectra.h \
	Tabs/Tree/TreeItem.h \
	Tabs/Tree/TreeModel.h \
	Tabs/Tree/IDBManager.h \
	Tabs/Tree/DBManager.h \
	Tabs/Tree/IDBController.h \
	Tabs/Tree/DBController.h \
	Tabs/Tree/TreeView.h \
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
	Tabs/RPC/RPCAtlant.h \
	Tabs/ITabAtlant.h \
	Controls/ControlPanelWidgets.h \
	Controls/ButtonShowPanel.h \
    Common/ServiceHandler/ServiceTerminateCauseEnumsToString.h \
    Common/ServiceHandler/ServiceTerminateCause.h \
    Common/ServiceHandler/ServiceHandler.h \
    Common/ServiceHandler/ProcessStateEnumsToString.h \
    Common/ServiceHandler/ProcessState.h \
    Common/ServiceHandler/IServiceHandler.h \
    Common/ServiceHandler/CommonCodesEnumsToString.h \
    Common/ServiceHandler/CommonCodes.h \
    MainWindow.h \
    MainWindowController.h \
     ControlPanel/ControlPanelWidget.h \
    ControlPanel/ControlPanelController.h \
    Tabs/TabSpectrumWidget.h \
    Correlations/CorrelationGroupWidget.h \
    Tabs/AtlantTabWidget.h

FORMS += \
    ControlPanel.ui \
    MainWindow.ui \
    Correlations/CorrelationWidget.ui \
    GraphicsWidget.ui \
    TabSpectrumWidget.ui \
    Correlations/CorrelationGroupWidget.ui \
    AtlantTabWidget.ui
	
LIBS += -luser32

DEFINES += UNICODE
DEFINES += _UNICODE

