#include "ListsDialog.h"
#include "ui_ListsDialog.h"

#include <QSpinBox>

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
	connect(ui->pbAdd, SIGNAL(clicked()), this, SLOT(slotAddClicked()));
	connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui->pbDelete, SIGNAL(clicked()), this, SLOT(slotDelete()));
    connect(ui->pbReport, SIGNAL(clicked()), this, SLOT(slotReportType()));
	connect(ui->pbClose, SIGNAL(clicked(bool)), this, SIGNAL(onClosing()));

	ui->listView->setVisible(false);
	ui->workListView->setVisible(false);
	ui->tvWhiteAreas->setVisible(false);

	QByteArray outData;
	QDataStream stream(&outData, QIODevice::ReadWrite);

	QFile f("./Tabs/whiteareacache.dat");
	f.open(QIODevice::ReadOnly);
	outData = f.readAll();
	f.close();

	QList<StationsFrequencyAndBandwith> m_stList;
	stream >> m_stList;
	setFrequencyAndBandwidthByWhiteAreas(m_stList);
}

ListsDialog::~ListsDialog()
{
	QList<StationsFrequencyAndBandwith> m_stList;
	getFrequencyAndBandwidthByWhiteAreasSave(m_stList);

	QByteArray outData;
	QDataStream stream(&outData, QIODevice::WriteOnly);
	stream << m_stList;

	QFile f("./Tabs/whiteareacache.dat");
	f.open(QIODevice::WriteOnly);
	f.write(outData);
	f.close();

	delete ui;
}

QTableView *ListsDialog::getTableView()
{
	return ui->tvLists;
}

void ListsDialog::addDetectFreq(double num, int index)
{
	ui->listView->addItem(tr("index: %1  freq %2").arg(index).arg(QString::number(num)));
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

//		QString tst1 = QString::number(ind);
//		QString tst2 =  ui->workListView->item(i)->text().left(ui->workListView->item(i)->text().indexOf("|"));

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

void ListsDialog::getFrequencyAndBandwidthByWhiteAreas(QList<StationsFrequencyAndBandwith>& list)
{
	for(int i = 0; i < ui->tvWhiteAreas->rowCount(); i++) {
		QSpinBox* sb = (QSpinBox*)(ui->tvWhiteAreas->cellWidget(i, 0));
		int start = sb->value();
		sb = (QSpinBox*)(ui->tvWhiteAreas->cellWidget(i, 1));
		int end = sb->value();
		sb = (QSpinBox*)(ui->tvWhiteAreas->cellWidget(i, 2));
		int band = sb->value();

		for(int j = 0; j<((end-start)/band); j++) {
			StationsFrequencyAndBandwith item;
			item.frequency = start+band*j;
			item.isChecked = true;
			item.bandwidth = band;
			list.append(item);
		}
	}
}

void ListsDialog::getFrequencyAndBandwidthByWhiteAreasSave(QList<StationsFrequencyAndBandwith>& list)
{
	for(int i = 0; i < ui->tvWhiteAreas->rowCount(); i++) {
		QSpinBox* sb = (QSpinBox*)(ui->tvWhiteAreas->cellWidget(i, 0));
		int start = sb->value();
		sb = (QSpinBox*)(ui->tvWhiteAreas->cellWidget(i, 1));
		int end = sb->value();
		sb = (QSpinBox*)(ui->tvWhiteAreas->cellWidget(i, 2));
		int band = sb->value();

			StationsFrequencyAndBandwith item;
			item.frequency = start;
			item.isChecked = true;
			item.bandwidth = end;
			item.stationName = QString::number(band);
			list.append(item);
	}
}


void ListsDialog::setFrequencyAndBandwidthByWhiteAreas(QList<StationsFrequencyAndBandwith>& list)
{
	foreach (StationsFrequencyAndBandwith val, list) {
		slotAddInternalWhiteAreaClicked(val.frequency, val.bandwidth, val.stationName.toInt());
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
	if(ind >= 4) {
		ui->tvLists->setVisible(false);
		ui->listView->setVisible(true);
		ui->workListView->setVisible(true);
		ui->tvWhiteAreas->setVisible(false);
	} else if(ind < 3) {
		ui->tvLists->setVisible(true);
		ui->listView->setVisible(false);
		ui->workListView->setVisible(false);
		ui->tvWhiteAreas->setVisible(false);
	} else {
		ui->tvLists->setVisible(false);
		ui->listView->setVisible(false);
		ui->workListView->setVisible(false);
		ui->tvWhiteAreas->setVisible(true);
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

void ListsDialog::slotAddClicked(int frequency1, int frequency2, int split)
{
	if(ui->cbLists->currentIndex() == 3) {
		slotAddInternalWhiteAreaClicked(frequency1, frequency2, split);
	} else {
		emit signalAddClicked();
	}
}

void ListsDialog::slotAddInternalWhiteAreaClicked(int frequency1, int frequency2, int split)
{
	int row = ui->tvWhiteAreas->rowCount();
	ui->tvWhiteAreas->insertRow( row );
	QSpinBox *sb = new QSpinBox(this);
	sb->setMinimum(30);
	sb->setMaximum(6990);
	sb->setValue(frequency1);
	QSpinBox *sb1 = new QSpinBox(this);
	sb1->setMinimum(30);
	sb1->setMaximum(6990);
	sb1->setValue(frequency2);
	QSpinBox *sb2 = new QSpinBox(this);
	sb2->setMinimum(1);
	sb2->setMaximum(6990);
	sb2->setValue(split);

	ui->tvWhiteAreas->setCellWidget(row, 0, sb);
	ui->tvWhiteAreas->setCellWidget(row, 1, sb1);
	ui->tvWhiteAreas->setCellWidget(row, 2, sb2);
}

void ListsDialog::slotRemoveWhiteArea()
{
	QObject* obj = sender();
	QPushButton* pb = qobject_cast<QPushButton*>(obj);

	if(!pb) {
		return;
	}

	ui->tvWhiteAreas->clearContents();
	ui->tvWhiteAreas->setRowCount(0);
}

void ListsDialog::slotDelete()
{
	if(ui->cbLists->currentIndex() == 3) {
		slotRemoveWhiteArea();
	} else {
		emit signalDelete();
	}
}
