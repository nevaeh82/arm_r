#include <QHeaderView>

#include <Logger.h>

#include "ListsDialogController.h"

ListsDialogController::ListsDialogController(IDBStation* stationDb, QObject* parent)
	: QObject(parent)
	, m_stationDb( stationDb )
{
	m_model = m_stationDb->createQueryModel( this );
	m_model->setHeaderData(0,Qt::Horizontal, tr("id"));
	m_model->setHeaderData(1,Qt::Horizontal, tr("Station Name"));
	m_model->setHeaderData(2,Qt::Horizontal, tr("IP"));
	m_model->setHeaderData(3,Qt::Horizontal, tr("Port"));
	m_model->setHeaderData(4,Qt::Horizontal, tr("Type"));
	m_model->setHeaderData(5,Qt::Horizontal, tr("Frequency"));
	m_model->setHeaderData(6,Qt::Horizontal, tr("Bandwidth"));
	m_model->setHeaderData(7,Qt::Horizontal, tr("Signal Type"));
	m_model->setHeaderData(8,Qt::Horizontal, tr("Date"));

	m_proxyModel = new ListsProxyModel( this );
	m_proxyModel->setSourceModel( m_model );
}

ListsDialogController::~ListsDialogController()
{
	m_stationDb->deregisterReceiver( this );
}

void ListsDialogController::appendView(ListsDialog *widget)
{
	m_view = widget->getTableView();
	m_view->setModel(m_proxyModel);
	m_view->setSortingEnabled(true);
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_view->hideColumn( 0 );

	adjustTableSize();

	connect(widget, SIGNAL(signalTypeList(int)), this, SLOT(filter(int)));
	connect(widget, SIGNAL(signalAddClicked()), this, SLOT(showAddDialog()));
	connect(widget, SIGNAL(signalDelete()), this, SLOT(deleteSelectedRecords()));
	connect(widget, SIGNAL(finished(int)), this, SLOT(deleteLater()));
}

void ListsDialogController::adjustTableSize()
{
	m_view->resizeColumnsToContents();

	int width = (m_view->model()->columnCount() - 1) + m_view->verticalHeader()->width();

	for( int column = 0; column < m_view->model()->columnCount(); column++ ) {
		width = width + m_view->columnWidth( column );
	}

	m_view->setMinimumWidth(width);
}

void ListsDialogController::update()
{
	m_model->update();
}

void ListsDialogController::filter(int type)
{
	if( type == 0 ) {
		m_proxyModel->setFilterKeyColumn( -1 );
		m_proxyModel->setFilterFixedString( "" );
		m_view->showColumn( 4 );
	} else {
		m_proxyModel->setFilterKeyColumn( 4 );
		m_proxyModel->setFilterFixedString( type == 1 ? "White" : "Black" );
		m_view->hideColumn( 4 );
	}

	adjustTableSize();
}

void ListsDialogController::showAddDialog()
{
	AddStationDataDialog* listAdd = new AddStationDataDialog(m_view);
	AddStationDataDialogController* listAddController = new AddStationDataDialogController(m_stationDb, this);

	listAddController->appendView(listAdd);

	connect( listAdd, SIGNAL(finished(int)), listAdd, SLOT(deleteLater()) );
	connect( listAdd, SIGNAL(finished(int)), listAddController, SLOT(deleteLater()) );

	listAdd->show();
}

void ListsDialogController::deleteSelectedRecords()
{
	QModelIndexList list = m_view->selectionModel()->selectedRows();

	foreach(QModelIndex index, list) {
		// get record id
		int id = m_model->record( index.row() ).value( "id" ).toInt();

		m_stationDb->deleteStationData( id );
	}

	update();
}
