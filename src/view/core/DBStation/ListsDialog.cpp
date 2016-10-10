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
    connect(ui->pbReport, SIGNAL(clicked()), this, SLOT(slotReportType()));
	connect(ui->pbClose, SIGNAL(clicked(bool)), this, SIGNAL(onClosing()));
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

void ListsDialog::slotReportType()
{
    emit signalReport(ui->cbLists->currentIndex());
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
