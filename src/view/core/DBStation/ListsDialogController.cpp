#include <QHeaderView>

#include <Logger/Logger.h>

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
	connect(widget, SIGNAL(signalReport(int)), this, SLOT(slotReport(int)));
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

void ListsDialogController::throwWordNfError()
{
	QMessageBox::warning(m_view, tr("Error")
						 , tr("Ms.Office package is not installed")
						 , QMessageBox::Ok);
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

	foreach( QModelIndex index, list ) {
		index = m_proxyModel->mapToSource( index );

		// get record id
		int id = m_model->record( index.row() ).value( "id" ).toInt();

		m_stationDb->deleteStationData( id );
	}

	update();
}

void ListsDialogController::slotReport(int type)
{
	QString category = "";
	switch(type)
	{
	case 0:
		category = "";
		break;
	case 1:
		category = "White";
		break;
	case 2:
		category = "Black";
		break;
	default:
		break;
	}

	QList<AllStationsReport> list;
	bool res = m_stationDb->getReportCategory(category, list);

	QAxObject* WordApplication=new QAxObject("Word.Application"); // Создаю интерфейс к MSWord
	//Show Error MessageBox if could not wrap a COM object to MS.Word
	if(WordApplication == NULL) {
		throwWordNfError();
		return;
	}
	QAxObject* WordDocuments = WordApplication->querySubObject( "Documents()" ); // Получаю интерфейсы к его подобъекту "коллекция открытых документов":
	//Show Error MessageBox if could not wrap a COM object to MS.Word Document
	if(WordDocuments == NULL) {
		throwWordNfError();
		return;
	}
	QAxObject* NewDocument = WordDocuments->querySubObject( "Add()" ); // Создаю новый документ
	WordApplication->setProperty("Visible", true); // Делаем Word видимым

	QAxObject* ActiveDocument = WordApplication->querySubObject("ActiveDocument()");
	QAxObject* Range = ActiveDocument->querySubObject("Range()");

	QAxObject *selection = WordApplication->querySubObject("Selection()");
	selection->dynamicCall("TypeParagraph()");

	selection->dynamicCall("TypeText(const QString&)","ОТЧЕТ\n");
	selection->dynamicCall("TypeText(const QString&)",QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));
	selection->dynamicCall("TypeParagraph()");
	QAxObject *range = selection->querySubObject("Range()");
	QAxObject *tables = NewDocument->querySubObject("Tables()");
	QAxObject *table = tables->querySubObject("Add(Range,NumRows,NumColumns)",range->asVariant(),list.count() + 1,6);



	QAxObject* cell = table->querySubObject("Cell(Row, Column)" , 1,1);
	QAxObject* CellRange = cell->querySubObject("Range()");
	CellRange->dynamicCall("InsertAfter(Text)", "№");

	cell = table->querySubObject("Cell(Row, Column)" , 1,2);
	CellRange = cell->querySubObject("Range()");
	CellRange->dynamicCall("InsertAfter(Text)", "Время");

	cell = table->querySubObject("Cell(Row, Column)" , 1,3);
	CellRange = cell->querySubObject("Range()");
	CellRange->dynamicCall("InsertAfter(Text)", "Пост");

	cell = table->querySubObject("Cell(Row, Column)" , 1,4);
	CellRange = cell->querySubObject("Range()");
	CellRange->dynamicCall("InsertAfter(Text)", "Частота");

	cell = table->querySubObject("Cell(Row, Column)" , 1,5);
	CellRange = cell->querySubObject("Range()");
	CellRange->dynamicCall("InsertAfter(Text)", "Полоса");

	cell = table->querySubObject("Cell(Row, Column)" , 1,6);
	CellRange = cell->querySubObject("Range()");
	CellRange->dynamicCall("InsertAfter(Text)", "Категория");

	int j = 1;
	for(int i = 0; i < list.count(); ++i)
	{
		j += 1;
		cell = table->querySubObject("Cell(Row, Column)" , j,1);
		CellRange = cell->querySubObject("Range()");
		CellRange->dynamicCall("InsertAfter(Text)", QString::number(i + 1));

		cell = table->querySubObject("Cell(Row, Column)" , j,2);
		CellRange = cell->querySubObject("Range()");
		CellRange->dynamicCall("InsertAfter(Text)", list.at(i).date.toString("dd-MM-yyyy hh:mm:ss"));

		cell = table->querySubObject("Cell(Row, Column)" , j,3);
		CellRange = cell->querySubObject("Range()");
		CellRange->dynamicCall("InsertAfter(Text)", list.at(i).sfab.stationName);

		cell = table->querySubObject("Cell(Row, Column)" , j,4);
		CellRange = cell->querySubObject("Range()");
		CellRange->dynamicCall("InsertAfter(Text)", QString::number(list.at(i).sfab.frequency));

		cell = table->querySubObject("Cell(Row, Column)" , j,5);
		CellRange = cell->querySubObject("Range()");
		CellRange->dynamicCall("InsertAfter(Text)", QString::number(list.at(i).sfab.bandwidth));

		cell = table->querySubObject("Cell(Row, Column)" , j,6);
		CellRange = cell->querySubObject("Range()");
		CellRange->dynamicCall("InsertAfter(Text)", list.at(i).category);

	}

	// освобождение памяти

	delete CellRange;

	delete cell;


	delete range;
	delete table;
	delete tables;

	delete selection;
	delete Range;
	delete ActiveDocument;
	delete NewDocument;
	delete WordDocuments;
	delete WordApplication;
}
