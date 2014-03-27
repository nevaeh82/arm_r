#include "ListsDialogController.h"

ListsDialogController::ListsDialogController(const QSqlDatabase& db, QObject* parent):
	QObject(parent)
{
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(ListsDialogController));

	m_db = db;
	m_model = new QSqlQueryModel(this);

	m_proxyModel = new ListsProxyModel(m_model, this);

	m_model->setQuery(getAllStationsInfo());

	m_model->setHeaderData(0,Qt::Horizontal, tr("id"));
	m_model->setHeaderData(1,Qt::Horizontal, tr("Station Name"));
	m_model->setHeaderData(2,Qt::Horizontal, tr("IP"));
	m_model->setHeaderData(3,Qt::Horizontal, tr("Port"));
	m_model->setHeaderData(4,Qt::Horizontal, tr("Type"));
	m_model->setHeaderData(5,Qt::Horizontal, tr("Frequency"));
	m_model->setHeaderData(6,Qt::Horizontal, tr("Bandwidth"));
	m_model->setHeaderData(7,Qt::Horizontal, tr("Signal Type"));
	m_model->setHeaderData(8,Qt::Horizontal, tr("Date"));
}

ListsDialogController::~ListsDialogController()
{
}

void ListsDialogController::appendView(ListsDialog *widget)
{
	m_view = widget->getTableView();
	m_view->setModel(m_proxyModel);
	m_view->setSortingEnabled(true);
	m_view->hideColumn(0);

	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	adjustTableSize();
	connect(widget, SIGNAL(signalTypeList(int)), this, SLOT(m_slotChooseTypeList(int)));
	connect(widget, SIGNAL(signalAddClicked()), this, SLOT(m_slotAdd()));
}

QSqlQuery ListsDialogController::getAllStationsInfo()
{
	QSqlQuery query(m_db);
	bool succeeded = query.prepare("SELECT sdi.id, st.name AS stationName, " \
					"st.ip AS stationIP, sd.port, cat.name AS CategoryName, " \
					"sdi.frequency, sdi.bandwidth, sigType.name AS signalType, " \
					"sdi.datetime FROM stationData AS sdi " \
					"INNER JOIN stationDevices as sd ON sdi.deviceID=sd.id " \
					"INNER JOIN station AS st ON st.id=sd.stationID " \
					"INNER JOIN category AS cat ON sdi.categoryID=cat.id " \
					"INNER JOIN signalType AS sigType ON sdi.signalTypeID=sigType.id " \
					"ORDER BY stationName");

	if (!succeeded) {
		m_logger->error(QString("SQL is wrong! Error = %1").arg(query.lastError().text()));
		return QSqlQuery();
	}

	succeeded = query.exec();

	if (!succeeded) {
		m_logger->error(QString("SQL is wrong! Error = %1").arg(query.lastError().text()));
		return QSqlQuery();
	}

	return query;
}

void ListsDialogController::adjustTableSize()
{
	m_view->resizeColumnsToContents();

	int width = (m_view->model()->columnCount() - 1) + m_view->verticalHeader()->width();
	for(int column = 0; column < m_view->model()->columnCount(); column++)
		width = width + m_view->columnWidth(column);
	m_view->setMinimumWidth(width);
}

void ListsDialogController::m_slotChooseTypeList(int type)
{
	if(type == 0)
	{
		m_model->setQuery(getAllStationsInfo());
		m_view->showColumn(4);
	}
	else
	{
		m_model->setQuery(getStationsInfoByCategory(type));
		m_view->hideColumn(4);
	}

	adjustTableSize();

	m_view->hideColumn(0);
}

void ListsDialogController::m_slotAdd()
{
	AddStationDataDialog* listAdd = new AddStationDataDialog(m_view);
	AddStationDataDialogController* listAddController = new AddStationDataDialogController(m_db, this);
	bool isOpen = m_db.isOpen();
	if(!isOpen)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("DataBase is not opened!"));
		msgBox.exec();
		return;
	}
	listAddController->appendView(listAdd);
	connect(listAdd, SIGNAL(signalUpdateList()), this, SLOT(m_slotAddClose()));
	listAdd->show();
}

void ListsDialogController::m_slotAddClose()
{
	m_model->query().exec();
	m_model->setQuery(m_model->query());
}

QSqlQuery ListsDialogController::getStationsInfoByCategory(int type)
{
	QSqlQuery query(m_db);
	bool succeeded = query.prepare("SELECT sdi.id, st.name AS stationName, " \
					"st.ip AS stationIP, sd.port, cat.name AS CategoryName," \
					"sdi.frequency, sdi.bandwidth, sigType.name AS signalType, " \
					"sdi.datetime FROM stationData AS sdi " \
					"INNER JOIN stationDevices as sd ON sdi.deviceID=sd.id " \
					"INNER JOIN station AS st ON st.id=sd.stationID " \
					"INNER JOIN category AS cat ON sdi.categoryID=cat.id " \
					"INNER JOIN signalType AS sigType ON sdi.signalTypeID=sigType.id " \
					"WHERE cat.name=:objectCategory ORDER BY stationName");

	if (!succeeded) {
		m_logger->error(QString("SQL is wrong! Error = %1").arg(query.lastError().text()));
		return QSqlQuery();
	}

	switch(type)
	{
		case 1:
			query.bindValue(":objectCategory", "White");
			break;
		case 2:
			query.bindValue(":objectCategory", "Black");
			break;
		default:
			break;
	}

	succeeded = query.exec();

	if (!succeeded) {
		m_logger->error(QString("SQL is wrong! Error = %1").arg(query.lastError().text()));
		return QSqlQuery();
	}

	return query;
}

