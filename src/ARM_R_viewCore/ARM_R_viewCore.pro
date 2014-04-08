QT += core sql network

TARGET   = ARM_R_viewCore
TEMPLATE = lib
ProjectDir=$$PWD
CONFIG += staticlib

include(../../specs/project.pri)
include(../../specs/tbb.pri)
include(../../specs/ipp.pri)
include(../../specs/qxt.pri)
include(../../specs/CISCommonLib.pri)
include(../Graphics_ipp/reference.pri)
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
	DBStation/ListsDialogController.cpp \
	DBStation/ListsProxyModel.cpp \
	DBStation/DBStationController.cpp \
	DBStation/AddStationDataDialog.cpp \
	DBStation/AddStationDataDialogController.cpp \
	DBStation/ListsDialog.cpp \
	DBStation/ListsDialogController.cpp \
	SettingsTree/TreeItem.cpp \
	SettingsTree/TreeModel.cpp \
	SettingsTree/TreeWidgetDelegate.cpp \
	Prm300ControlWidget.cpp \
	Prm300ControlWidgetController.cpp \
    ControlPanel/ListWhiteDialog.cpp \
    ControlPanel/ListWhiteDialogController.cpp \
    ControlPanel/CheckBoxDelegate.cpp

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
	Tabs/RPC/IRPC.h \
	Tabs/RPC/RpcPrmClient.h \
	Tabs/RPC/RpcFlakonClient.h \
	Tabs/RPC/RpcAtlantClient.h \
	Tabs/ITabAtlant.h \
	Tabs/TabSpectrumWidgetController.h \
	ControlPanel/ControlPanelWidget.h \
	ControlPanel/ControlPanelController.h \
	DB/DbController.h \
	DB/DbManager.h \
	SettingsTree/TreeItem.h \
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
	DBStation/ListsDialogController.h \
	DBStation/ListsProxyModel.h \
	DBStation/DBStationController.h \
	DBStation/DBFillTables.h \
	DBStation/DBConnectionParameters.h \
	DBStation/AddStationDataDialog.h \
	DBStation/AddStationDataDialogController.h \
	DBStation/ListsDialog.h \
	DBStation/ListsDialogController.h \
	Prm300ControlWidget.h \
	Prm300ControlWidgetController.h \
    ControlPanel/ListWhiteDialog.h \
    ControlPanel/ListWhiteDialogController.h \
    ControlPanel/CheckBoxDelegate.h

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
    ControlPanel/ListWhiteDialog.ui


RESOURCES += \
	translations.qrc

TRANSLATIONS += ARM-R_en.ts\
	ARM-R_ru.ts

#LIBS += -luser32

DEFINES += UNICODE
DEFINES += _UNICODE

OTHER_FILES +=

