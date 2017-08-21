#include "ListsDialog.h"
#include "ui_ListsDialog.h"

#include <QSpinBox>
#include <QLabel>

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

	connect(ui->tvWhiteAreas, SIGNAL(cellClicked(int,int)), this, SLOT(slotAreasCellClicked(int ,int)));

	connect(this, SIGNAL(onClosing()), this, SLOT(saveAreasSettings()));

	ui->listWidget->setVisible(false);
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

	ui->tvWhiteAreas->setColumnWidth(3, 180);
}

ListsDialog::~ListsDialog()
{
//	QList<StationsFrequencyAndBandwith> m_stList;
//	getFrequencyAndBandwidthByWhiteAreasSave(m_stList);

//	QByteArray outData;
//	QDataStream stream(&outData, QIODevice::WriteOnly);
//	stream << m_stList;

//	QFile f("./Tabs/whiteareacache.dat");
//	f.open(QIODevice::WriteOnly);
//	f.write(outData);
//	f.close();
	delete ui;
}

QTableView *ListsDialog::getTableView()
{
	return ui->tvLists;
}

void ListsDialog::addDetectFreq(double num, int index, int id)
{
	if(!m_listViewMap.contains(id)) {
		return;
	}

	stListView listViewStruct = m_listViewMap.value(id);

	listViewStruct.listView->addItem(tr("index: %1  freq %2").arg(index).arg(QString::number(num)));
}

void ListsDialog::rmDetectFreq(double num, int id)
{
	if(!m_listViewMap.contains(id)) {
		return;
	}

	stListView listViewStruct = m_listViewMap.value(id);

	QList<QListWidgetItem*> findLst = listViewStruct.listView->findItems(QString::number((int)num), Qt::MatchStartsWith);

	foreach (QListWidgetItem* item, findLst) {
		listViewStruct.listView->takeItem(listViewStruct.listView->row(item));
	}
}

void ListsDialog::clearDetectFreq(int id)
{
	if(!m_listViewMap.contains(id)) {
		return;
	}

	stListView listViewStruct = m_listViewMap.value(id);

	int cnt = listViewStruct.listView->count();
	for(int i = 0; i<cnt; i++) {
		listViewStruct.listView->takeItem(0);
	}
}

void ListsDialog::setWorkList(QList<StationsFrequencyAndBandwith> list, int id, QString title)
{
	if(!m_listViewMap.contains(id)) {
		stListView listViewStruct;
		QListWidget* listView = new QListWidget(this);
		QListWidget* workListView = new QListWidget(this);

        connect(workListView, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this, SLOT(slotWorkListItemClick(QListWidgetItem*)));

		QLabel* lbl = new QLabel(QString("%1) ").arg(title));
		ui->listLayout->addWidget(lbl, id, 0);
		ui->listLayout->addWidget(listView, id, 1);
		ui->listLayout->addWidget(workListView, id, 2);
		listViewStruct.listView = listView;
		listViewStruct.workListView = workListView;
		listViewStruct.lbl = lbl;

		m_listViewMap.insert(id, listViewStruct);
	}

	stListView listViewStruct = m_listViewMap.value(id);

	{
		listViewStruct.workListView->setVisible(true);
		listViewStruct.listView->setVisible(true);
		listViewStruct.lbl->setVisible(true);
	}


	int index = 0;
	foreach (StationsFrequencyAndBandwith item, list) {
		if(item.isChecked) {
			listViewStruct.workListView->addItem(tr("%1| Freq: %2  bandwidth: %3").arg(index).
												 arg(item.frequency).arg(item.bandwidth));
		}

		index++;
	}
}

void ListsDialog::clearWorkList(int id)
{
	if(!m_listViewMap.contains(id)) {
		return;
	}

	stListView listViewStruct = m_listViewMap.value(id);

	int cnt = listViewStruct.workListView->count();
	for(int i = 0; i<cnt; i++) {
		listViewStruct.workListView->takeItem(0);
	}

	{
		listViewStruct.workListView->setVisible(false);
		listViewStruct.listView->setVisible(false);
		listViewStruct.lbl->setVisible(false);
	}
}

void ListsDialog::setCheckPointer(int ind, int id)
{
	if(!m_listViewMap.contains(id)) {
		return;
	}

	stListView listViewStruct = m_listViewMap.value(id);

	for(int i = 0; i<listViewStruct.workListView->count(); i++) {

		//		QString tst1 = QString::number(ind);
		//		QString tst2 =  ui->workListView->item(i)->text().left(ui->workListView->item(i)->text().indexOf("|"));

		if( listViewStruct.workListView->item(i)->text().left(listViewStruct.workListView->item(i)->text().indexOf("|")) == QString::number(ind) ) {
			listViewStruct.workListView->item(i)->setBackgroundColor(Qt::green);
		} else {
			listViewStruct.workListView->item(i)->setBackgroundColor(Qt::white);
		}
	}

	for(int i = 0; i<listViewStruct.listView->count(); i++) {
		listViewStruct.listView->item(i)->setBackgroundColor(Qt::white);
	}

}

void ListsDialog::setDetectPointer(int ind, int id)
{
	if(!m_listViewMap.contains(id)) {
		return;
	}

	stListView listViewStruct = m_listViewMap.value(id);

	if( (ind-1) >= 0) {
		listViewStruct.listView->item(ind-1)->setBackgroundColor(Qt::white);
	}
	listViewStruct.listView->item(ind)->setBackgroundColor(Qt::green);

	for(int i = 0; i<listViewStruct.workListView->count(); i++) {
		listViewStruct.workListView->item(i)->setBackgroundColor(Qt::white);
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

		if(!sb) {
			return;
		}

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
	ui->tvWhiteAreas->clearContents();
//	for(int i = 0; i<ui->tvWhiteAreas->rowCount(); i++) {
//		ui->tvWhiteAreas->removeRow(0);
//	}
	ui->tvWhiteAreas->setRowCount(0);

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
	if(ind >= 5) {
		ui->tvLists->setVisible(false);
		ui->listWidget->setVisible(true);
		ui->tvWhiteAreas->setVisible(false);
	} else if(ind < 3) {
		ui->tvLists->setVisible(true);
		ui->listWidget->setVisible(false);
		ui->tvWhiteAreas->setVisible(false);
	} else if(ind == 3){
		ui->tvLists->setVisible(false);
		ui->listWidget->setVisible(false);
		loadWhiteAreas();
		ui->tvWhiteAreas->setVisible(true);
	} else if(ind == 4){
		ui->tvLists->setVisible(false);
		ui->listWidget->setVisible(false);
		loadWhiteDoplerAreas();
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
	if(ui->cbLists->currentIndex() == 3 || ui->cbLists->currentIndex() == 4) {
		slotAddInternalWhiteAreaClicked(frequency1, frequency2, split);
		saveAreasSettings();
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

	connect(sb, SIGNAL(editingFinished()), this, SLOT(saveAreasSettings()));
	connect(sb1, SIGNAL(editingFinished()), this, SLOT(saveAreasSettings()));
	connect(sb2, SIGNAL(editingFinished()), this, SLOT(saveAreasSettings()));

	ui->tvWhiteAreas->setCellWidget(row, 0, sb);
	ui->tvWhiteAreas->setCellWidget(row, 1, sb1);
	ui->tvWhiteAreas->setCellWidget(row, 2, sb2);
	ui->tvWhiteAreas->setItem(row, 3, new QTableWidgetItem(""));
}

void ListsDialog::slotAreasCellClicked(int row, int col)
{
	int ololo = 1;
	int ololo1 = 1;

	ololo = row;
	ololo1 = col;
}

void ListsDialog::slotRemoveWhiteArea()
{
	QObject* obj = sender();
	QPushButton* pb = qobject_cast<QPushButton*>(obj);

	if(!pb) {
		return;
	}

	if(ui->tvWhiteAreas->selectedItems().size() > 0) {
		int row = ui->tvWhiteAreas->selectedItems().first()->row();
		ui->tvWhiteAreas->removeRow(row);
	}

//	ui->tvWhiteAreas->clearContents();
//	ui->tvWhiteAreas->setRowCount(0);
}

void ListsDialog::slotDelete()
{
	if(ui->cbLists->currentIndex() == 3 || ui->cbLists->currentIndex() == 4) {
		slotRemoveWhiteArea();
		saveAreasSettings();
	} else {
		emit signalDelete();
	}
}

void ListsDialog::saveAreasSettings()
{
	if(ui->cbLists->currentIndex() == 3) {
		QList<StationsFrequencyAndBandwith> m_stList;
		getFrequencyAndBandwidthByWhiteAreasSave(m_stList);

		QByteArray outData;
		QDataStream stream(&outData, QIODevice::WriteOnly);
		stream << m_stList;

		QFile f("./Tabs/whiteareacache.dat");
		f.open(QIODevice::WriteOnly);
		f.write(outData);
		f.close();
	} else if(ui->cbLists->currentIndex() == 4){
		QList<StationsFrequencyAndBandwith> m_stList;
		getFrequencyAndBandwidthByWhiteAreasSave(m_stList);

		QByteArray outData;
		QDataStream stream(&outData, QIODevice::WriteOnly);
		stream << m_stList;

		QFile f("./Tabs/whiteareaDoplerCache.dat");
		f.open(QIODevice::WriteOnly);
		f.write(outData);
		f.close();
    }
}

void ListsDialog::slotWorkListItemClick(QListWidgetItem *item)
{
    QObject* obj = sender();
    QListWidget* wgt = dynamic_cast<QListWidget*>(obj);

    if(wgt) {

        int tabId = -1;
        foreach (stListView st, m_listViewMap.values()) {
            if(st.workListView == wgt) {

                tabId = m_listViewMap.key(st);
                break;
            }
        }

        if(tabId >= 0) {
            stListView listViewStruct = m_listViewMap.value(tabId);
            int row = listViewStruct.workListView->row(item);
            emit onDetectAdd(tabId, row);
        }
    }
}

QList<StationsFrequencyAndBandwith> ListsDialog::loadWhiteAreas()
{
	QByteArray outData;
	QDataStream stream(&outData, QIODevice::ReadWrite);

	QFile f("./Tabs/whiteareacache.dat");
	f.open(QIODevice::ReadOnly);
	outData = f.readAll();
	f.close();

	QList<StationsFrequencyAndBandwith> m_stList;
	stream >> m_stList;
	setFrequencyAndBandwidthByWhiteAreas(m_stList);

	ui->tvWhiteAreas->setColumnWidth(3, 180);

	return m_stList;
}

void ListsDialog::getFrequencyAndBandwidthByWhiteAreasIni(QList<StationsFrequencyAndBandwith> &list)
{
	QList<StationsFrequencyAndBandwith> listIn = loadWhiteAreas();
	foreach (StationsFrequencyAndBandwith item, listIn) {
		int start = item.frequency;
		int end = item.bandwidth;
		int band = item.stationName.toInt();

		for(int j = 0; j<((end-start)/band); j++) {
			StationsFrequencyAndBandwith item;
			item.frequency = start+band*j;
			item.isChecked = true;
			item.bandwidth = band;
			list.append(item);
		}
	}
}

QList<StationsFrequencyAndBandwith> ListsDialog::loadWhiteDoplerAreas()
{
	QByteArray outData;
	QDataStream stream(&outData, QIODevice::ReadWrite);

	QFile f("./Tabs/whiteareaDoplercache.dat");
	f.open(QIODevice::ReadOnly);
	outData = f.readAll();
	f.close();

	QList<StationsFrequencyAndBandwith> m_stList;
	stream >> m_stList;
	setFrequencyAndBandwidthByWhiteAreas(m_stList);

	ui->tvWhiteAreas->setColumnWidth(3, 180);

	return m_stList;
}

void ListsDialog::getFrequencyAndBandwidthByWhiteAreasDoplerIni(QList<StationsFrequencyAndBandwith> &list)
{
	QList<StationsFrequencyAndBandwith> listIn = loadWhiteDoplerAreas();
	foreach (StationsFrequencyAndBandwith item, listIn) {
		int start = item.frequency;
		int end = item.bandwidth;
		int band = item.stationName.toInt();

		for(int j = 0; j<((end-start)/band); j++) {
			StationsFrequencyAndBandwith item;
			item.frequency = start+band*j;
			item.isChecked = true;
			item.bandwidth = band;
			list.append(item);
		}
	}
}
