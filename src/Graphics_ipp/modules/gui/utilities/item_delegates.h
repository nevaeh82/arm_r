#ifndef ITEM_DELEGATES_H
#define ITEM_DELEGATES_H

#include <QObject>
#include <QStyledItemDelegate>
#include <functional>


class ReadOnlyDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ReadOnlyDelegate(QObject *parent): QStyledItemDelegate(parent) {}
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
	{
		return nullptr;
	}
};


class SpinBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	template<class fT>
	SpinBoxDelegate(QObject *parent, fT set_func = nullptr, int minimum = 0, int maximum = 2000000000): QStyledItemDelegate(parent)
	{
		min_ = minimum;
		max_ = maximum;
		settings_func_ = set_func;
	}
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	int min_;
	int max_;
	std::function<void (QWidget* editor)> settings_func_;
};


//////////////////////////////////////////////////////////////////////////

class DoubleSpinBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	template<class fT>
	DoubleSpinBoxDelegate(QObject *parent, fT set_func = nullptr, double minimum = 0, double maximum = 2000000000.0, int ignore_columns = -1): QStyledItemDelegate(parent)
	{
		min_ = minimum;
		max_ = maximum;
		settings_func_ = set_func;
		ddecimalc_ = 3;
		ignore_columns_ = ignore_columns;
	}
	
	virtual QString displayText ( const QVariant & value, const QLocale & locale ) const;
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void SetDisplayDecimals(int ddecimalc);

private:
	double	min_;
	double	max_;
	int		ddecimalc_;
	int		ignore_columns_;
	std::function<void (QWidget* editor)> settings_func_;
};

//////////////////////////////////////////////////////////////////////////

class ComboBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	template<class fT>
	ComboBoxDelegate(QObject *parent, fT set_func, QStringList values, int ignore_columns = -1) : QStyledItemDelegate(parent)//ignore_columns игнорирует указанную колонку (делая ее read_only, в случае когда расположение делигатов в строку)
	{
		items_list_ = values;
		settings_func_ = set_func;
		ignore_columns_ = ignore_columns;
	}
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	QStringList items_list_;
	int			ignore_columns_;
	std::function<void (QWidget* editor)> settings_func_;
};


//////////////////////////////////////////////////////////////////////////

class ComboBoxButtonDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ComboBoxButtonDelegate (QObject *parent, QStringList* values );
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	QStringList* items_list_;

private slots:
	void ButtonClick_slot();
	void EditingFinished_slot(QWidget* editor);//неиспользуется
	void ComboBoxItemCh_slot(QString str);
signals:
	void ButtonClick(int row);
	void DelegateFinished(int row, QString value);
};

//////////////////////////////////////////////////////////////////////////


class LineEditDelegate: public QStyledItemDelegate
{
	Q_OBJECT
public:
	template<class fT>
	LineEditDelegate (QObject *parent,fT set_func = nullptr, int ignore_columns = -1, int max_len = 256)
	{
		max_len_ = max_len;
		settings_func_ = set_func;
		ignore_columns_ = ignore_columns;
	}

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	int max_len_;
	int ignore_columns_;
	std::function<void (QWidget* editor)> settings_func_;
};


//////////////////////////////////////////////////////////////////////////

class HexEditDelegate: public QStyledItemDelegate
{
	Q_OBJECT
public:
	template<class fT>
	HexEditDelegate (QObject *parent,fT set_func = nullptr, int BytesCount = 1024*2, int ignore_columns = -1)//-1 не проверяется
	{
		read_only_check_role_ = -1;
		bytes_count_ = BytesCount;
		settings_func_ = set_func;
		ignore_columns_ = ignore_columns;
		bytes_count_check_role_ = -1;
	}

	QWidget *createEditor			(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
	void setEditorData				(QWidget *editor, const QModelIndex &index) const;
	void setModelData				(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;
	void updateEditorGeometry		(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void SetBytesCountCheckRoleNum	(int role);//при установке игнорируется параметр BytesCount поданный в конструктор и считывается длинна из указанной  роли, каждый раз при создании
	void SetReadOnlyCheckRoleNum	(int role);//SetReadOnlyCheckRoleNum - делать только если у элемента не выставить флаг ItemEditable

private:
	int read_only_check_role_;
	int bytes_count_check_role_;
	int bytes_count_;
	int ignore_columns_;
	std::function<void (QWidget* editor)> settings_func_;
};

class HexEditDelegateValidator: public QValidator
{
	Q_OBJECT
public:
	HexEditDelegateValidator (QObject *parent = nullptr): QValidator(parent) { };
	virtual QValidator::State	validate ( QString & input, int & pos ) const;
};


#endif // ITEM_DELEGATES_H
