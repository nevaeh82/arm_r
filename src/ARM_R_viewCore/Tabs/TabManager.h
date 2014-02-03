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

#include "TabSpectrum.h"

//#include "Tree/Controller.h"

#include "Tree/DBManager.h"
#include "Tree/TreeModel.h"

#include "ITabManager.h"

/// ATLANT
#include "TabAtlant.h"

class TabManager: public QObject, /*public IModuleController, */public ITabManager
{
	Q_OBJECT
public:
	TabManager(QTabWidget* tabWidget, QObject *parent = 0);
	virtual ~TabManager();

private:
	QTabWidget* m_tabWidget;

	unsigned int    _id;
	QString         _name;
	QMap<int, TabsProperty *>   _map_settings;
	QMap<int, TabSpectrum* >    _map_tabs;
	ICommonComponents*          _common_spectra;
	ICommonComponents*          _common_correlations;

	DBManager*                 _db_manager_spectrum;

	TreeModel*                  _model_spectrum;

	TabSpectrum*                _current_tab_widget;

	QMutex                      m_mutex;

public:
	//virtual int start();

	virtual int createSubModules(const QString& settingsFile);

public:
	virtual QString getStationName(int id);
	virtual void send_data(int pid, IMessage* msg);
	virtual void set_tab(int id);


private:
	int readSettings(const QString &settingsFile);
	void checkStatus();

private slots:
	void changeTabSlot(int index);

signals:
	void changeTabSignal(int index);
};

#endif // TABMANAGER_H
