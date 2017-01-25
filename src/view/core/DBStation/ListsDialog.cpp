#include "ListsDialog.h"
#include "ui_ListsDialog.h"

ListsDialog::ListsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ListsDialog)
{
	ui->setupUi(this);

	// Hide "?" button
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	flags = flags & (~helpFlag);

	setWindowFlags(flags);
	setWindowIcon(QIcon(":/images/icons/ListsDialog.png"));

	connect(ui->cbLists, SIGNAL(activated(int)), this, SIGNAL(signalTypeList(int)));
	connect(ui->cbLists, SIGNAL(activated(int)), this, SLOT(slotTypeList(int)));
	connect(ui->pbAdd, SIGNAL(clicked()), this, SIGNAL(signalAddClicked()));
	connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui->pbDelete, SIGNAL(clicked()), this, SIGNAL(signalDelete()));
    connect(ui->pbReport, SIGNAL(clicked()), this, SLOT(slotReportType()));
	connect(ui->pbClose, SIGNAL(clicked(bool)), this, SIGNAL(onClosing()));

	ui->listView->setVisible(false);
	ui->workListView->setVisible(false);
}

ListsDialog::~ListsDialog()
{
	delete ui;
}

QTableView *ListsDialog::getTableView()
{
	return ui->tvLists;
}

void ListsDialog::addDetectFreq(double num, int index)
{
	ui->listView->addItem(tr("index: %1  freq %2").arg(QString::number(num)).arg(index));
}

void ListsDialog::rmDetectFreq(double num)
{
	QList<QListWidgetItem*> findLst = ui->listView->findItems(QString::number((int)num), Qt::MatchStartsWith);

	foreach (QListWidgetItem* item, findLst) {
			ui->listView->takeItem(ui->listView->row(item));
	}
}

void ListsDialog::clearDetectFreq()
{
	int cnt = ui->listView->count();
	for(int i = 0; i<cnt; i++) {
		ui->listView->takeItem(0);
	}
}

void ListsDialog::setWorkList(QList<StationsFrequencyAndBandwith> list)
{
	int index = 0;
	foreach (StationsFrequencyAndBandwith item, list) {
		if(item.isChecked) {
			ui->workListView->addItem(tr("%1| Freq: %2  bandwidth: %3").arg(index).
									  arg(item.frequency).arg(item.bandwidth));
		}

		index++;
	}
}

void ListsDialog::clearWorkList()
{
	int cnt = ui->workListView->count();
	for(int i = 0; i<cnt; i++) {
		ui->workListView->takeItem(0);
	}
}

void ListsDialog::setCheckPointer(int ind)
{
	for(int i = 0; i<ui->workListView->count(); i++) {

		QString tst1 = QString::number(ind);
		QString tst2 =  ui->workListView->item(i)->text().left(ui->workListView->item(i)->text().indexOf("|"));

		if( ui->workListView->item(i)->text().left(ui->workListView->item(i)->text().indexOf("|")) == QString::number(ind) ) {
			ui->workListView->item(i)->setBackgroundColor(Qt::green);
		} else {
			ui->workListView->item(i)->setBackgroundColor(Qt::white);
		}
	}

	for(int i = 0; i<ui->listView->count(); i++) {
		ui->listView->item(i)->setBackgroundColor(Qt::white);
	}

}

void ListsDialog::setDetectPointer(int ind)
{
	if( (ind-1) >= 0) {
		ui->listView->item(ind-1)->setBackgroundColor(Qt::white);
	}
	ui->listView->item(ind)->setBackgroundColor(Qt::green);

	for(int i = 0; i<ui->workListView->count(); i++) {
		ui->workListView->item(i)->setBackgroundColor(Qt::white);
	}
}

void ListsDialog::slotShow()
{
    this->show();
}

void ListsDialog::slotReportType()
{
	emit signalReport(ui->cbLists->currentIndex());
}

void ListsDialog::slotTypeList(int ind)
{
	if(ind >= 3) {
		ui->tvLists->setVisible(false);
		ui->listView->setVisible(true);
		ui->workListView->setVisible(true);
	} else {
		ui->tvLists->setVisible(true);
		ui->listView->setVisible(false);
		ui->workListView->setVisible(false);
	}
}

void ListsDialog::on_tvLists_doubleClicked(const QModelIndex &index)
{
//    if(index.column() == 9)
//    {
//        bool state = index.model()->data(index, Qt::DisplayRole).toBool();
//        log_debug(QString::number(index.column()));
//    }
//    int row = index.row();
//    QString name = index.sibling(row, 0).data().toString();
//    QString surname = index.sibling(row, 1).data().toString();
//    int age = index.sibling(row, 2).data().toInt();
//    QString username = index.sibling(row, 3).data().toString();
}
