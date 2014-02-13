#include "DbManager.h"

#include <QDir>
#include <QDebug>

#define DB_PATH		"DB"
#define DB_NAME		"Zaviruha.sqlite"

DbManager::DbManager(QObject *parent) :
	QObject(parent)
{
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(DbManager));

	QString dbPath = QString(DB_PATH) + QDir::separator() + QString(DB_NAME);
	m_dbController = new DbController(dbPath, this);
}

DbManager::~DbManager()
{
}

//void DbManager::registerDbChangedReceiver(ISettingsChangedListener *receiver)
//{
//	m_receiversList.append(receiver);
//}

//void DbManager::deregisterDbChangedReceiver(ISettingsChangedListener *receiver)
//{
//	int index = m_receiversList.indexOf(receiver);

//	if (index < 0){
//		return;
//	}

//	m_receiversList.removeAt(index);
//}

SettingsNode DbManager::getSettingsNode(const QString &objectName)
{
	SettingsNode settingsNode;

	Object obj = m_dbController->getObject(objectName);

	if (obj.id == INVALID_INDEX) {
		obj = createSettingsNode(objectName);
	}

	if (obj.id == INVALID_INDEX) {
		return settingsNode;
	}

	PropertiesList propersties = m_dbController->getProperties(obj.id);

	settingsNode.object = obj;
	settingsNode.properties = propersties;

	return settingsNode;
}

bool DbManager::updateProperty(const Property &property)
{
	bool result = m_dbController->setPropertyValue(property.id, property.value);

	if (!result) {
		return false;
	}

	notifyDbChanged(property);

//	Object obj =  m_dbController->getObject(property.pid);

//	if (INVALID_INDEX == obj.id) {
//		return false;
//	}

//	SettingsNode settingsNode = getSettingsNode(obj.name);

//	notifyDbChanged(settingsNode);

	return true;
}

QString DbManager::getObjectName(const uint id) const
{
	// Create object KTP settings
	Object obj= m_dbController->getObject(id);

	if (obj.id == INVALID_INDEX) {
		return QString();
	}

	return obj.name;
}

Object DbManager::createSettingsNode(const QString &objectName)
{
	Object obj = createStation(objectName);

	return obj;
}

Object DbManager::createObject(const QString &objectName)
{
	Object obj;

	obj.name = objectName;
	obj.pid = INVALID_INDEX;
	obj.state = 0;
	obj.isEditable = false;

	// Create object KTP settings
	uint recordId = m_dbController->addObject(obj);

	if (recordId == INVALID_INDEX) {
		return obj;
	}

	return m_dbController->getObject(recordId);

}

Object DbManager::createStation(const QString &stationName)
{
	Object obj = createObject(stationName);

	if (obj.id == INVALID_INDEX) {
		return obj;
	}

	// Create roating antenna object properties

	Property prop;
	prop.pid = obj.id;
	prop.name = DB_FREQUENCY_PROPERTY;
	prop.value = "1830";
	prop.state = 0;
	prop.isEditable = true;

	uint recordId = m_dbController->addProperty(prop);

	if (recordId == INVALID_INDEX) {
		m_logger->debug("Failed to create freq property");
	}

	prop.name = DB_SELECTED_PROPERTY;
	prop.value = "0";
	prop.state = 0;
	prop.isEditable = true;

	recordId = m_dbController->addProperty(prop);

	if (recordId == INVALID_INDEX) {
		m_logger->debug("Failed to create selected property");
	}

	prop.name = DB_CENTER_PROPERTY;
	prop.value = "0";
	prop.state = 0;
	prop.isEditable = true;

	recordId = m_dbController->addProperty(prop);

	if (recordId == INVALID_INDEX) {
		m_logger->debug("Failed to create center property");
	}

	prop.name = DB_START_PROPERTY;
	prop.value = "0";
	prop.state = 0;
	prop.isEditable = true;

	recordId = m_dbController->addProperty(prop);

	if (recordId == INVALID_INDEX) {
		m_logger->debug("Failed to create start property");
	}

	prop.name = DB_STOP_PROPERTY;
	prop.value = "0";
	prop.state = 0;
	prop.isEditable = true;

	recordId = m_dbController->addProperty(prop);

	if (recordId == INVALID_INDEX) {
		m_logger->debug("Failed to create end property");
	}

	prop.name = DB_LEADING_OP_PROPERTY;
	prop.value = "0";
	prop.state = 0;
	prop.isEditable = true;

	recordId = m_dbController->addProperty(prop);

	if (recordId == INVALID_INDEX) {
		m_logger->debug("Failed to create leading OP property");
	}

	prop.name = DB_AVERAGING_PROPERTY;
	prop.value = "0";
	prop.state = 0;
	prop.isEditable = true;

	recordId = m_dbController->addProperty(prop);

	if (recordId == INVALID_INDEX) {
		m_logger->debug("Failed to create averaging property");
	}


	prop.name = DB_PANORAMA_START_PROPERTY;
	prop.value = "0";
	prop.state = 0;
	prop.isEditable = true;

	recordId = m_dbController->addProperty(prop);

	if (recordId == INVALID_INDEX) {
		m_logger->debug("Failed to create panorama start property");
	}


	prop.name = DB_PANORAMA_END_PROPERTY;
	prop.value = "0";
	prop.state = 0;
	prop.isEditable = true;

	recordId = m_dbController->addProperty(prop);

	if (recordId == INVALID_INDEX) {
		m_logger->debug("Failed to create panorama end property");
	}


	return obj;

}

void DbManager::notifyDbChanged(const SettingsNode& category)
{
	foreach(IDbChangedListener* receiver, m_receiversList) {
		receiver->onSettingsNodeChanged(category);
	}
}

void DbManager::notifyDbChanged(const Property &property)
{
	foreach(IDbChangedListener* receiver, m_receiversList) {
		receiver->onPropertyChanged(property);
	}
}


