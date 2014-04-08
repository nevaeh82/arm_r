#include "ListWhiteDialogController.h"
#include <Logger.h>

ListWhiteDialogController::ListWhiteDialogController(const QSqlDatabase& db, QObject* parent):
	QObject(parent)
{
	m_db = db;
	m_model = new QSqlQueryModel(this);

//	m_proxyModel = new ListsProxyModel(m_model, this);

	m_model->setQuery(getAllFromWhiteList());

	m_model->insertColumn(0);
	int i = 0;
	while(i < m_model->rowCount())
	{
		m_model->setData(m_model->index(i, 0), true, Qt::CheckStateRole);
		i++;
	}

	m_model->setHeaderData(0,Qt::Horizontal, tr(""));
	m_model->setHeaderData(1,Qt::Horizontal, tr("Station Name"));
	m_model->setHeaderData(2,Qt::Horizontal, tr("Frequency"));
	m_model->setHeaderData(3,Qt::Horizontal, tr("Bandwidth"));
}

ListWhiteDialogController::~ListWhiteDialogController()
{

}

void ListWhiteDialogController::appendView(ListWhiteDialog *widget)
{
	m_view = widget->getTableView();
	m_view->setModel(m_model);
	m_view->setSortingEnabled(true);
//	m_view->hideColumn(0);

	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	adjustTableSize();
//	connect(widget, SIGNAL(signalTypeList(int)), this, SLOT(m_slotChooseTypeList(int)));
//	connect(widget, SIGNAL(signalAddClicked()), this, SLOT(m_slotAdd()));

	connect(widget, SIGNAL(signalAccept()), this, SLOT(slotAccept()));

	m_view->setItemDelegateForColumn(0, new CheckBoxDelegate());
//	m_model->set


//	connect(widget, SIGNAL(SignalCancel()), this, SLOT(slotCancel()));
}

QSqlQuery ListWhiteDialogController::getAllFromWhiteList()
{
	QSqlQuery query(m_db);
	bool succeeded = query.prepare("SELECT s.name, st.frequency, st.bandwidth " \
								   "FROM stationData AS st " \
								   "INNER JOIN stationDevices as sdi on st.deviceID=sdi.id " \
								   "INNER JOIN station as s on s.id=sdi.stationID " \
								   "INNER JOIN category AS cat on st.categoryID=cat.id " \
								   "WHERE cat.name='White'");

	if (!succeeded) {
		log_error(QString("SQL is wrong! Error = %1").arg(query.lastError().text()));
		return QSqlQuery();
	}

	succeeded = query.exec();

	if (!succeeded) {
		log_error(QString("SQL is wrong! Error = %1").arg(query.lastError().text()));
		return QSqlQuery();
	}

	return query;
}

void ListWhiteDialogController::adjustTableSize()
{
	m_view->resizeColumnsToContents();

//	int width = (m_view->model()->columnCount() - 1) + m_view->verticalHeader()->width();
//	for(int column = 0; column < m_view->model()->columnCount(); column++)
//		width = width + m_view->columnWidth(column);
	//	m_view->setMinimumWidth(width);
}

void ListWhiteDialogController::slotAccept()
{

}

void ListWhiteDialogController::slotCancel()
{
//	m_view->close();
}
