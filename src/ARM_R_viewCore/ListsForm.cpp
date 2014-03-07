#include "ListsForm.h"
#include "ui_listsForm.h"

ListsForm::ListsForm(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ListsForm)
{
	ui->setupUi(this);
	connect(ui->cbLists, SIGNAL(activated(int)), this, SIGNAL(signalTypeList(int)));
}

ListsForm::~ListsForm()
{
	delete ui;
}

QTableView *ListsForm::getTableView()
{
	return ui->tvLists;
}

void ListsForm::slotShow()
{
	this->show();
}
