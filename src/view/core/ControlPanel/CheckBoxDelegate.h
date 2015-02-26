#ifndef CHECKBOXDELEGATE_H
#define CHECKBOXDELEGATE_H

#include <QObject>
#include <QAbstractItemDelegate>
#include <QCheckBox>

class CheckBoxDelegate : public QAbstractItemDelegate
{
	Q_OBJECT
public:
	CheckBoxDelegate(QObject *parent = 0);

public:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
	void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;


};

#endif // CHECKBOXDELEGATE_H
