QT += core sql network

TARGET   = ARM_R_viewCore
TEMPLATE = lib
ProjectDir=$$PWD
CONFIG += staticlib
CONFIG += qaxcontainer

include(../../specs/project.pri)
include(../../specs/tbb.pri)
include(../../specs/ipp.pri)
include(../../specs/qxt.pri)
include(../../specs/Solver.pri)
include(../../specs/CISCommonLib.pri)
include(../../specs/GraphicsIpp.pri)
include(../ARM_R_Common/reference.pri)

INCLUDEPATH += $$PWD

SOURCES += \
	Station.cpp \
	SpectrumWidget.cpp \
	SpectrumWidgetController.cpp \
	SpectrumWidgetDataSource.cpp \
	MainWindow.cpp \
	MainWindowController.cpp \
	Correlations/CorrelationWidget.cpp \
	Correlations/CorrelationGroupWidget.cpp \
	Correlations/CorrelationWidgetDataSource.cpp \
	Correlations/CorrelationWidgetController.cpp \
	Correlations/CorrelationControllersContainer.cpp \
	Common/BaseDataSource.cpp \
	Common/CommandMessage.cpp \
	Tabs/AtlantTabWidget.cpp \
	Tabs/RPC/RpcPrmClient.cpp \
	Tabs/RPC/RpcAtlantClient.cpp \
	Tabs/RPC/RpcFlakonClient.cpp \
	Tabs/TabManager.cpp \
	Tabs/TabSpectrumWidget.cpp \
	Tabs/TabSpectrumWidgetController.cpp \
	Tabs/CommonSpectrumTabWidget.cpp \
	ControlPanel/ControlPanelWidget.cpp \
	ControlPanel/ControlPanelController.cpp \
	DB/DbController.cpp \
	DB/DbManager.cpp \
	DBStation/ListsProxyModel.cpp \
	DBStation/DBStationController.cpp \
	DBStation/AddStationDataDialog.cpp \
	DBStation/AddStationDataDialogController.cpp \
	DBStation/ListsDialog.cpp \
	DBStation/ListsDialogController.cpp \
	SettingsTree/TreeModel.cpp \
	SettingsTree/TreeWidgetDelegate.cpp \
	Prm300ControlWidget.cpp \
	Prm300ControlWidgetController.cpp \
    Tabs/RPC/RpcConfigClient.cpp \
    ControlPanel/CheckBoxDelegate.cpp \
    ControlPanel/ListWhiteDialog.cpp \
    ControlPanel/ListWhiteDialogController.cpp \
    SignalDetectedDialog.cpp \
    Tabs/RPC/RpcFlakonClientWrapper.cpp \
    DBStation/StationHelper.cpp \
    DBStation/StationsDataTableModel.cpp \
    Tabs/SolverResultWidget.cpp \
    Tabs/SolverResultWidgetController.cpp \
    Tabs/SolverErrorsWidget.cpp \
    Tabs/SolverErrorsWidgetController.cpp

HEADERS += \
	stdafx.h \
	Station.h \
	Defines.h \
	UiDefines.h \
	SpectrumWidget.h \
	SpectrumWidgetController.h \
	SpectrumWidgetDataSource.h \
	IGraphicWidget.h \
	ICorrelationWidget.h \
	MainWindow.h \
	MainWindowController.h \
	Tabs/TabManager.h \
	Tabs/ITabSpectrum.h \
	Tabs/TabSpectrumWidget.h \
	Tabs/AtlantTabWidget.h \
	Tabs/CommonSpectrumTabWidget.h \
    Tabs/Controls/IControlPRM.h \
	Tabs/RPC/RpcPrmClient.h \
	Tabs/RPC/RpcFlakonClient.h \
	Tabs/RPC/RpcAtlantClient.h \
	Tabs/ITabAtlant.h \
	Tabs/TabSpectrumWidgetController.h \
	ControlPanel/ControlPanelWidget.h \
	ControlPanel/ControlPanelController.h \
	DB/DbController.h \
	DB/DbManager.h \
	SettingsTree/TreeModel.h \
	SettingsTree/TreeWidgetDelegate.h \
	Common/IMessage.h \
	Common/CommandMessage.h \
	Common/BaseDataSource.h \
	Correlations/CorrelationGroupWidget.h \
	Correlations/CorrelationWidget.h \
	Correlations/CorrelationWidgetDataSource.h \
	Correlations/CorrelationWidgetController.h \
	Correlations/CorrelationControllersContainer.h \
	Interfaces/ICorrelationControllersContainer.h \
	Interfaces/ITabManager.h \
	Interfaces/ITabWidget.h \
	Interfaces/IDbController.h \
	Interfaces/IDbManager.h \
	Interfaces/IDbChangedListener.h \
	Interfaces/ISettingsChangedListener.h \
	Interfaces/ISpectrumWidget.h \
	Interfaces/IDataSourceListener.h \
	Interfaces/IDataSource.h \
	Interfaces/IControlPanelListener.h \
	Interfaces/IControlPanelController.h \
	Interfaces/ICorrelationWidget.h \
	Interfaces/IStation.h \
	DBStation/IDBStation.h \
	DBStation/ListsProxyModel.h \
	DBStation/DBStationController.h \
    DBStation/DBFillTables.h \
	DBStation/AddStationDataDialog.h \
	DBStation/AddStationDataDialogController.h \
	DBStation/ListsDialog.h \
	DBStation/ListsDialogController.h \
	Prm300ControlWidget.h \
	Prm300ControlWidgetController.h \
    Tabs/RPC/RpcConfigClient.h \
    Interfaces/IRpcConfigClient.h \
    ControlPanel/CheckBoxDelegate.h \
    ControlPanel/ListWhiteDialog.h \
    ControlPanel/ListWhiteDialogController.h \
    SignalDetectedDialog.h \
    Tabs/RPC/RpcFlakonClientWrapper.h \
    DBStation/StationHelper.h \
    DBStation/IDBStationListener.h \
    DBStation/StationsDataTableModel.h \
    Tabs/SolverResultWidget.h \
    Tabs/SolverResultWidgetController.h \
    Tabs/SolverErrorsWidget.h \
    Tabs/SolverErrorsWidgetController.h

FORMS += \
	ControlPanel.ui \
	MainWindow.ui \
	Correlations/CorrelationWidget.ui \
	TabSpectrumWidget.ui \
	Correlations/CorrelationGroupWidget.ui \
	AtlantTabWidget.ui \
	Tabs/CommonSpectrumTabWidget.ui \
	SpectrumWidget.ui \
	DBStation/AddStationDataDialog.ui \
	DBStation/ListsDialog.ui \
	Prm300ControlWidget.ui \
    ControlPanel/ListWhiteDialog.ui \
    SignalDetectedDialog.ui \
    Tabs/SolverResultWidget.ui \
    Tabs/SolverErrorsWidget.ui


RESOURCES += \
        translations.qrc

TRANSLATIONS += ARM-R_en.ts\
	ARM-R_ru.ts

LIBS += -lqaxcontainer
#LIBS += -luser32

DEFINES += UNICODE
DEFINES += _UNICODE

OTHER_FILES +=

