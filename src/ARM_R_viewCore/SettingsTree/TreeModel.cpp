#include "TreeModel.h"

#include <QSize>

#include <TreeModel\TreeItem.h>
#include "UiDefines.h"

TreeModel::TreeModel(IDbManager *dbManager, const QStringList &headers, QObject *parent)
	: TreeModelBase(headers, parent)
{
	m_dbManager = dbManager;
}

TreeModel::~TreeModel()
{
}


void TreeModel::onSettingsNodeChanged(const SettingsNode& settingsNode)
{
	SettingsNode inSettingsNode = settingsNode;

	if (settingsNode.object.pid == 0) {
		emit layoutAboutToBeChanged();

		TreeItem* item =  new TreeItem(inSettingsNode.object, m_rootItem);

		foreach (Property property, settingsNode.properties) {

			TreeItem* childItem =  new TreeItem(property, item);
			item->appendChild(childItem);
		}

		m_rootItem->appendChild(item);

		emit layoutChanged();
	}

	emit onItemAddedSignal();
}

void TreeModel::onPropertyChanged(const Property &property)
{
	Property inProperty = property;

	if (inProperty.pid == 0) {
		return;
	}

	for (int i = 0; i< m_rootItem->childCount(); i++) {
		TreeItem* stationItem = m_rootItem->child(i);

		for (int j = 0; j< stationItem->childCount(); j++) {
			if (stationItem->child(j)->data().id == inProperty.id) {
				stationItem->child(j)->updateData(inProperty);
				break;
			}
		}
	}

	refreshModel();
}

void TreeModel::onCleanSettings()
{
	if (m_rootItem)
		delete m_rootItem;

	m_rootItem = new TreeItem(NULL);
	reset();
}

void TreeModel::setStationsList(const QStringList &stationsList)
{
	foreach (QString stationName, stationsList) {
		SettingsNode settingsNode = m_dbManager->getSettingsNode(stationName);
		onSettingsNodeChanged(settingsNode);
	}
}

QString TreeModel::getTranslateItemNameFromReal(const QString& itemName) const
{
	if (itemName == "Frequency") {
		return tr("Frequency");
	} else if (itemName == "Selected") {
		return tr("Selected");
	} else if (itemName == "Center") {
		return tr("Center");
	} else if (itemName == "Start") {
		return tr("Start");
	} else if (itemName == "Stop") {
		return tr("Stop");
	} else if (itemName == "Averaging") {
		return tr("Averaging");
	} else if (itemName == "Leading OP") {
		return tr("Leading OP");
	} else if (itemName == "Panorama start") {
		return tr("Panorama start");
	} else if (itemName == "Panorama end") {
		return tr("Panorama end");
	}

	return itemName;
}

bool TreeModel::updateModelData(TreeItem *item)
{
	if (item->data().name == DB_LEADING_OP_PROPERTY){
		return m_dbManager->updatePropertyForAllObjects(DB_LEADING_OP_PROPERTY, item->data().value);
	}

	if (item->data().name == DB_AVERAGING_PROPERTY){
		return m_dbManager->updatePropertyForAllObjects(DB_AVERAGING_PROPERTY, item->data().value);
	}

	return m_dbManager->updateProperty(Property(item->data()));
}

