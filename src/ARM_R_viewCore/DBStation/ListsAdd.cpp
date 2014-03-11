#include "ListsAdd.h"
#include "ui_ListsAdd.h"

ListsAdd::ListsAdd(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ListsAdd)
{
	ui->setupUi(this);
	connect(ui->pbAccept, SIGNAL(clicked()), this, SLOT(slotAccept()));
	connect(ui->pbClear, SIGNAL(clicked()), this, SLOT(slotClear()));
	connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(slotClose()));
}

ListsAdd::~ListsAdd()
{
	delete ui;
}

void ListsAdd::fillStation(const QStringList &list)
{
	ui->cbStation->addItems(list);
}

void ListsAdd::fillCategory(const QStringList &list)
{
	ui->cbCategory->addItems(list);
}

void ListsAdd::fillSignalType(const QStringList &list)
{
	ui->cbSignalType->addItems(list);
}

void ListsAdd::slotAccept()
{
	QStringList list;
	list.append(ui->cbStation->currentText());
	list.append(QString::number(ui->sbPort->value()));
	list.append(ui->cbCategory->currentText());
	list.append(ui->dsbFrequency->text());
	list.append(ui->dsbBandwidth->text());
	list.append(ui->cbSignalType->currentText());
	emit signalAccept(list);
}

void ListsAdd::slotClear()
{

}

void ListsAdd::slotClose()
{
	this->close();
}
