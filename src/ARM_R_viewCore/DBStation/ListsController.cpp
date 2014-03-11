#include "ListsController.h"
#include <QDebug>


ListsController::ListsController(const QSqlDatabase& db, QObject* parent):
	QObject(parent)
{
	m_db = db;
	m_model = new QSqlQueryModel(this);

	m_proxyModel = new ListsProxyModel(m_model, this);

	m_model->setQuery(getAllStationsInfo());


//	m_model->setTable("StationData");
//	m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
////	m_model->setRelation(1, QSqlRelation("stationDevices", "id", "Port"));

////	m_model->setQuery("SELECT id FROM Station");
//	bool res = m_model->select();
//	QString error = m_model->lastError().text();
//	qDebug() << error;
//	qDebug() <<  "res = " << res;


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

ListsController::~ListsController()
{

}


void ListsController::appendView(ListsForm *widget)
{
	m_view = widget->getTableView();
	m_view->setModel(m_proxyModel);
	m_view->setSortingEnabled(true);
//	m_view->hideColumn(0);

	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	adjustTableSize();
	connect(widget, SIGNAL(signalTypeList(int)), this, SLOT(m_slotChooseTypeList(int)));
}

QSqlQuery ListsController::getAllStationsInfo()
{
	QSqlQuery query;
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
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return QSqlQuery();
	}
	query.exec();
	return query;
}

void ListsController::adjustTableSize()
{
	m_view->resizeColumnsToContents();

	int width = (m_view->model()->columnCount() - 1) + m_view->verticalHeader()->width();
	for(int column = 0; column < m_view->model()->columnCount(); column++)
		width = width + m_view->columnWidth(column);
	m_view->setMinimumWidth(width);

}

void ListsController::m_slotChooseTypeList(int type)
{
	switch(type)
	{
		case 0:
			break;
		default:
			break;

	}

	m_view->show();
}
