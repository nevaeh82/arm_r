#ifndef TreeModel_H
#define TreeModel_H

#include <TreeModel/TreeModelBase.h>

#include "Interfaces/IDbChangedListener.h"

#include "Interfaces/IDbCache.h"
#include "Interfaces/IDbManager.h"

class TreeModel : public TreeModelBase, public IDbChangedListener
{
	Q_OBJECT

private:
	IDbManager* m_dbManager;

public:
	TreeModel(IDbManager* dbManager, const QStringList& headers, QObject *parent = 0);

	virtual ~TreeModel();

	void onSettingsNodeChanged(const SettingsNode & settingsNode);

	void onPropertyChanged(const Property& property);

	void onCleanSettings();

	void setStationsList(const QStringList& stationsList);

protected:
	QString getTranslateItemNameFromReal(const QString& itemName) const;
	bool updateModelData(TreeItem *item);

signals:
	void onItemAddedSignal();

};

#endif // TreeModel_H
