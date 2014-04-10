#include "CheckBoxDelegate.h"

CheckBoxDelegate::CheckBoxDelegate(QObject *parent) :
	QAbstractItemDelegate(parent)
{
}

void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (!index.isValid()) {
		return;
	}

//	QAbstractItemDelegate::paint(painter, option, index);
}

QWidget* CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QCheckBox* editor = new QCheckBox(parent);
	editor->setEnabled(true);
//	connect(editor, SIGNAL(editingFinished()), this, SLOT(commitSBEditorSlot()));
	return editor;
}

bool CheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	return true;
}

QSize CheckBoxDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QSize(10, 10);
}

void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if(index.isValid()&&index.column()==0)
		editor->setGeometry(option.rect);
}

void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if(index.isValid()&&index.column()==0) {
		bool value=index.model()->data(index,Qt::CheckStateRole).toBool();
		QCheckBox *checkBox=static_cast<QCheckBox*>(editor);
		if (value==true)
		  checkBox->setCheckState(Qt::Checked);
		else
		  checkBox->setCheckState(Qt::Unchecked);
	  } /*else {
		QItemDelegate::setEditorData(editor,index);
	  }*/
}

void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if(index.isValid()&&index.column()==0) {
		QCheckBox *checkBox=static_cast<QCheckBox*>(editor);
		bool value;
		if (checkBox->checkState()==Qt::Checked)
		  value=true;
		else
		  value=false;
		model->setData(index,value);
	  }
}
