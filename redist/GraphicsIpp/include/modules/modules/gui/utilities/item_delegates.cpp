#include "StdAfx.h"
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPointer>
#include "item_delegates.h"

QWidget *SpinBoxDelegate::createEditor(QWidget *parent,	const QStyleOptionViewItem &/* option */,const QModelIndex &/* index */) const
{
	QSpinBox *editor = new QSpinBox(parent);
	editor->setMinimum(min_);
	editor->setMaximum(max_);
	editor->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
	editor->setAlignment(Qt::AlignCenter);
	editor->setFrame(false);
	if (settings_func_) settings_func_(editor);
	return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
	int value = index.model()->data(index, Qt::EditRole).toInt();

	QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
	spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
	QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
	spinBox->interpretText();
	int value = spinBox->value();
	model->setData(index, value, Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}




//////////////////////////////////////////////////////////////////////////
//DoubleSpinBoxDelegate

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */,const QModelIndex & index ) const
{
	if (ignore_columns_ == index.column()) return nullptr;
	QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
	editor->setMinimum(min_);
	editor->setMaximum(max_);
	editor->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
	editor->setAlignment(Qt::AlignCenter);
	editor->setDecimals(3);
	editor->setFrame(false);
	editor->setGeometry(parent->geometry());
	if (settings_func_) settings_func_(editor);
	return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
	double value = index.model()->data(index, Qt::EditRole).toDouble();
	QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
	spinBox->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
	QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
	spinBox->interpretText();
	double val = spinBox->value();
	model->setData(index, val, Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}

QString DoubleSpinBoxDelegate::displayText( const QVariant & value, const QLocale & locale ) const
{
	if (value.type() == QVariant::Double)
	{
		QString temp = QString("%1").arg(value.toDouble(), 0, 'f', ddecimalc_);
		return temp.replace('.',',');
	}
	return value.toString();
}

void DoubleSpinBoxDelegate::SetDisplayDecimals( int ddecimalc )
{
	ddecimalc_ = ddecimalc;
}



//////////////////////////////////////////////////////////////////////////
//QComboBox

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,	const QStyleOptionViewItem &/* option */,const QModelIndex & index) const
{
	if(index.column() == ignore_columns_) return nullptr;
	QComboBox *editor = new QComboBox(parent);
	editor->setGeometry(parent->geometry());
	editor->addItems(items_list_);
	if (settings_func_) settings_func_(editor);
	return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
	QString val = index.model()->data(index, Qt::EditRole).toString();
	QComboBox *cb = static_cast<QComboBox*>(editor);
	int ind = cb->findText(val);
	if (ind >= 0) cb->setCurrentIndex(ind);
	else
	{
		cb->addItem(val);
		cb->setCurrentIndex(cb->count()-1);
	}
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
	QComboBox *cB = static_cast<QComboBox*>(editor);
	
	QString val = cB->currentText();
	model->setData(index, val, Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}



//////////////////////////////////////////////////////////////////////////
//ComboBox with button

ComboBoxButtonDelegate::ComboBoxButtonDelegate( QObject *parent, QStringList* values )
{
	items_list_ = values;
	//connect(this,SIGNAL(commitData(QWidget*)),this,SLOT(EditingFinished_slot(QWidget*)));
}

QWidget* ComboBoxButtonDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index ) const
{
	QWidget* wid = new QWidget(parent);
	wid->setToolTip("Widget");
	wid->setLayout(new QHBoxLayout());
	wid->layout()->setContentsMargins(0,0,0,0);
	wid->layout()->setSpacing(0);

	QComboBox *combo_editor = new QComboBox(wid);
	wid->layout()->addWidget(combo_editor);
	combo_editor->setToolTip("ComboBox");
	combo_editor->addItems(*items_list_);
	combo_editor->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	combo_editor->setProperty("row",index.row());
	connect(combo_editor,SIGNAL( activated (QString) ),this,SLOT(ComboBoxItemCh_slot(QString) ));
	QPushButton* PB = new QPushButton("...",wid);
	PB->setToolTip("Button");
	PB->setMaximumWidth(25);
	wid->layout()->addWidget(PB);
	PB->setProperty("row",index.row());
	connect(PB,SIGNAL(clicked(bool)),this,SLOT(ButtonClick_slot()));
		
	return wid;
}

void ComboBoxButtonDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
	QString val = index.model()->data(index, Qt::EditRole).toString();
	
	QComboBox *cb = static_cast<QComboBox*>(editor->layout()->itemAt(0)->widget());
	if (cb == NULL) return;
	int ind = cb->findText(val);
	if (ind >= 0) cb->setCurrentIndex(ind);
	else
	{
		cb->addItem(val);
		cb->setCurrentIndex(cb->count()-1);
	}
	cb->showPopup();
}

void ComboBoxButtonDelegate::setModelData( QWidget *editor, QAbstractItemModel *model,const QModelIndex &index ) const
{
	QComboBox *cB = static_cast<QComboBox*>(editor->layout()->itemAt(0)->widget());
	if (cB == NULL) return;
	QString val = cB->currentText();
	model->setData(index, val, Qt::EditRole);
}

void ComboBoxButtonDelegate::updateEditorGeometry( QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	editor->setGeometry(option.rect);
}

void ComboBoxButtonDelegate::ButtonClick_slot()
{
	emit ButtonClick(sender()->property("row").toInt());
}

void ComboBoxButtonDelegate::EditingFinished_slot( QWidget* editor )
{
	QComboBox *cB = static_cast<QComboBox*>(editor->layout()->itemAt(0)->widget());
	if (cB == NULL) return;
	QString val = cB->currentText();
	int row = cB->property("row").toInt();
	emit DelegateFinished(row,val);
}

void ComboBoxButtonDelegate::ComboBoxItemCh_slot( QString str )
{
	int row = sender()->property("row").toInt();
	emit DelegateFinished(row,str);
}



//////////////////////////////////////////////////////////////////////////
//HexEditDelegate with button

QWidget * HexEditDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index ) const
{
	if(index.column() == ignore_columns_) return nullptr;
	if (read_only_check_role_ > 0)
	{
		if (index.data(read_only_check_role_).toBool())
			return nullptr;
	}
	
	QLineEdit* wid = new QLineEdit(parent);
	wid->setFrame(false);
	wid->setValidator(new HexEditDelegateValidator(wid));
	wid->setAlignment(Qt::AlignCenter);
	
	if (bytes_count_check_role_ >= 0)
	{
		int tt = 0;
		tt = index.data(bytes_count_check_role_).toInt();
		if (tt <= 0) wid->setMaxLength(bytes_count_*2);
		else wid->setMaxLength(tt*2);
	}
	else wid->setMaxLength(bytes_count_*2);
	
	wid->setProperty("row",index.row());
	wid->setGeometry(parent->geometry());
	if (settings_func_) settings_func_(wid);
	return wid;
}

void HexEditDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
	QString value = index.model()->data(index, Qt::EditRole).toString();
	QLineEdit *editor_le = static_cast<QLineEdit *>(editor);
	editor_le->setText(value);
}

void HexEditDelegate::setModelData( QWidget *editor, QAbstractItemModel *model,const QModelIndex &index ) const
{
	QLineEdit *le = static_cast<QLineEdit*>(editor);
	QString val = le->text();
	model->setData(index, val, Qt::EditRole);
}

void HexEditDelegate::updateEditorGeometry( QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	editor->setGeometry(option.rect);
}

void HexEditDelegate::SetBytesCountCheckRoleNum( int role )
{
	bytes_count_check_role_ = role;
}

void HexEditDelegate::SetReadOnlyCheckRoleNum( int role )
{
	read_only_check_role_ = role;
}


QValidator::State HexEditDelegateValidator::validate( QString & input, int & pos ) const
{
	if (input.size() == 0) return QValidator::Invalid;
	input = input.toUpper();
	for (int i =0;i < input.size();i++)
		if (!(((input[i].toAscii() >= 0x30) && (input[i].toAscii() <= 0x39)) || ((input[i].toAscii() >= 0x41) && (input[i].toAscii() <= 0x46)))) return QValidator::Invalid;
	
	return QValidator::Acceptable;
}


//////////////////////////////////////////////////////////////////////////
//LineEditDelegate

QWidget * LineEditDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index ) const
{
	if (ignore_columns_ == index.column()) return nullptr;

	QLineEdit* wid = new QLineEdit(parent);
	wid->setFrame(false);
	wid->setMaxLength(max_len_);
	wid->setProperty("row",index.row());
	wid->setGeometry(parent->geometry());
	if (settings_func_) settings_func_(wid);
	return wid;
}

void LineEditDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
	QString value = index.model()->data(index, Qt::EditRole).toString();
	QLineEdit *editor_le = static_cast<QLineEdit *>(editor);
	editor_le->setText(value);
}

void LineEditDelegate::setModelData( QWidget *editor, QAbstractItemModel *model,const QModelIndex &index ) const
{
	QLineEdit *le = static_cast<QLineEdit*>(editor);
	QString val = le->text();
	model->setData(index, val, Qt::EditRole);
}

void LineEditDelegate::updateEditorGeometry( QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	editor->setGeometry(option.rect);
}
