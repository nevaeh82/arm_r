#include "ListWhiteDialog.h"
#include "ui_ListWhiteDialog.h"

ListWhiteDialog::ListWhiteDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ListWhiteDialog)
{
	ui->setupUi(this);

	// Hide "?" button
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	flags = flags & (~helpFlag);

	setWindowFlags(flags);
	setWindowIcon(QIcon(":/images/icons/ListWhiteDialog.png"));

	connect(ui->pbAccept, SIGNAL(clicked()), this, SIGNAL(signalAccept()));
	connect(ui->pbCancel, SIGNAL(clicked()), this, SLOT(close()));

}

ListWhiteDialog::~ListWhiteDialog()
{
	delete ui;
}

QTableView *ListWhiteDialog::getTableView()
{
	return ui->tvWhiteList;
}
