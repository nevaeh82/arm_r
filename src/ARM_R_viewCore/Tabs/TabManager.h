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

#include "../Abstracts/IModuleController.h"
#include "../Common/CommonSpectra.h"
#include "../Common/CommonCorrelations.h"

#include "TabsProperty.h"

#include "TabSpectrumWidget.h"

//#include "Tree/Controller.h"

#include "Interfaces/IDBManager.h"
#include "SettingsTree/TreeModel.h"

#include "ITabManager.h"

#include "TabSpectrumWidgetController.h"

#include "Interfaces/IDbChangedListener.h"

/// ATLANT
#include "AtlantTabWidget.h"

class TabManager: public QObject, /*public IModuleController, */public ITabManager, public IDbChangedListener
{
	Q_OBJECT

private:
	QTabWidget* m_tabWidget;

	unsigned int    _id;
	QString         _name;
	QMap<int, TabsProperty *>   m_tabsPropertyMap;
	QMap<QString, ITabWidget* >    m_tabWidgetsMap;
	ICommonComponents*          _common_spectra;
	ICommonComponents*          _common_correlations;

	IDbManager*                 m_dbManager;

	ITabWidget*                m_currentTabWidget;

	QMutex                      m_mutex;

public:
	TabManager(QTabWidget* tabWidget, QObject *parent = 0);
	virtual ~TabManager();

	virtual int start();

	virtual int createSubModules(const QString& settingsFile);

    virtual QString getStationName(int id);
	virtual void send_data(int pid, TypeCommand type, IMessage* msg);
    virtual void set_tab(int id);

	virtual void onSettingsNodeChanged(const SettingsNode &);
	virtual void onPropertyChanged(const Property &);
	virtual void onCleanSettings();

private:
	int readSettings(const QString &settingsFile);
	void checkStatus();

private slots:
	void changeTabSlot(int index);

signals:
	void changeTabSignal(int index);

};

#endif // TABMANAGER_H
