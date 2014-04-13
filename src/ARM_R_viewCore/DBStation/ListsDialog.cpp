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
	connect(ui->pbAdd, SIGNAL(clicked()), this, SIGNAL(signalAddClicked()));
	connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui->pbDelete, SIGNAL(clicked()), this, SIGNAL(signalDelete()));
}

ListsDialog::~ListsDialog()
{
	delete ui;
}

QTableView *ListsDialog::getTableView()
{
	return ui->tvLists;
}

void ListsDialog::slotShow()
{
	this->show();
}
