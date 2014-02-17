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
	Tabs/TabManager.cpp \
	Tabs/TabsProperty.cpp \
	Common/CommonSpectra.cpp \
	Common/CommandMessage.cpp \
	Tabs/RPC/RPCClient.cpp \
	Common/CommonCorrelations.cpp \
	Tabs/Controls/ControlPRM.cpp \
	Tabs/RPC/RPCAtlant.cpp \
	Controls/ControlPanelWidgets.cpp \
	Controls/ButtonShowPanel.cpp \
    MainWindow.cpp \
    MainWindowController.cpp \
    ControlPanel/ControlPanelWidget.cpp \
    ControlPanel/ControlPanelController.cpp \
    Tabs/TabSpectrumWidget.cpp \
    Correlations/CorrelationGroupWidget.cpp \
    Tabs/AtlantTabWidget.cpp \
    Tabs/CommonSpectrumTabWidget.cpp \
    SpectrumWidget.cpp \
    Tabs/TabSpectrumWidgetController.cpp \
    DB/DbController.cpp \
    DB/DbManager.cpp \
    SettingsTree/TreeItem.cpp \
    SettingsTree/TreeModel.cpp \
    SettingsTree/TreeWidgetDelegate.cpp \
    SpectrumWidgetController.cpp

HEADERS += \
        Correlations/CorrelationWidget.h \
        GraphicData.h \
        ICorrelationWidget.h \
        IGraphicWidget.h \
        stdafx.h \
	Abstracts/IModuleController.h \
	Abstracts/ICommonComponents.h \
	Tabs/TabManager.h \
	Tabs/TabsProperty.h \
	Common/CommonSpectra.h \
	Common/IMessage.h \
	Common/CommandMessage.h \
	Tabs/ITabSpectrum.h \
	Tabs/RPC/IRPC.h \
	Tabs/RPC/RPCClient.h \
	Tabs/ITabManager.h \
	Common/CommonCorrelations.h \
	Tabs/Controls/ControlPRM.h \
	Tabs/Controls/IControlPRM.h \
	Tabs/RPC/RPCAtlant.h \
	Tabs/ITabAtlant.h \
	Controls/ControlPanelWidgets.h \
	Controls/ButtonShowPanel.h \
    MainWindow.h \
    MainWindowController.h \
     ControlPanel/ControlPanelWidget.h \
    ControlPanel/ControlPanelController.h \
    Tabs/TabSpectrumWidget.h \
    Correlations/CorrelationGroupWidget.h \
    Tabs/AtlantTabWidget.h \
    Tabs/CommonSpectrumTabWidget.h \
    Abstracts/ITabWidget.h \
    SpectrumWidget.h \
    Tabs/TabSpectrumWidgetController.h \
    DB/DbController.h \
    DB/DbManager.h \
    Interfaces/IDbController.h \
    Interfaces/IDbManager.h \
    SettingsTree/TreeItem.h \
    SettingsTree/TreeModel.h \
    SettingsTree/TreeWidgetDelegate.h \
    Interfaces/IDbChangedListener.h \
    Interfaces/ISettingsChangedListener.h \
    UiDefines.h \
    SpectrumWidgetController.h \
    Interfaces/ISpectrumWidget.h

FORMS += \
    ControlPanel.ui \
    MainWindow.ui \
    Correlations/CorrelationWidget.ui \
    TabSpectrumWidget.ui \
    Correlations/CorrelationGroupWidget.ui \
    AtlantTabWidget.ui \
    Tabs/CommonSpectrumTabWidget.ui \
    SpectrumWidget.ui
	
LIBS += -luser32

DEFINES += UNICODE
DEFINES += _UNICODE

OTHER_FILES +=

