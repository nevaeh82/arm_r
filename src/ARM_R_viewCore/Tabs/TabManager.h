#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <QWidget>
#include <QTabWidget>
#include <QString>
#include <QSettings>
#include <QMap>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QMutex>

#include "Common/CommonCorrelations.h"

#include "TabsProperty.h"

#include "TabSpectrumWidget.h"

#include "Interfaces/IDBManager.h"
//#include "SettingsTree/TreeModel.h"

#include "Interfaces/ITabManager.h"

#include "TabSpectrumWidgetController.h"

#include "Interfaces/IDbChangedListener.h"
#include "Interfaces/IControlPanelListener.h"

/// ATLANT
#include "AtlantTabWidget.h"

class TabManager: public QObject, public ITabManager, public IDbChangedListener, public IControlPanelListener
{
	Q_OBJECT

private:
	QTabWidget* m_tabWidget;

	QMap<int, TabsProperty *>   m_tabsPropertyMap;
	QMap<QString, ITabWidget* >    m_tabWidgetsMap;
	ICommonComponents*          _common_correlations;

	IDbManager* m_dbManager;
	ITabWidget* m_currentTabWidget;

public:
	TabManager(QTabWidget* tabWidget, QObject *parent = 0);
	virtual ~TabManager();

	void start();
	int createSubModules(const QString& settingsFile);
	void setDbManager(IDbManager* dbManager);

	virtual QString getStationName(const int id);
	virtual void sendCommand(const QString& stationName, TypeCommand type, IMessage* msg);
	virtual void setActiveTab(const int id);

	virtual void onSettingsNodeChanged(const SettingsNode &);
	virtual void onPropertyChanged(const Property &);
	virtual void onCleanSettings();

	virtual void onGlobalAutoSearchEnabled(const bool isEnabled);
	virtual void onGlobalPanoramaEnabled(const bool isEnabled);

private:
	int readSettings(const QString &settingsFile);
	void checkStatus();

private slots:
	void changeTabSlot(int index);
};

#endif // TABMANAGER_H
