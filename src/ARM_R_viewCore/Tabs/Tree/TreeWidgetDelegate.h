#ifndef TREEWIDGETDELEGATE_H
#define TREEWIDGETDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QPainter>

class TreeWidgetDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TreeWidgetDelegate(QObject *parent = 0);

    void setUserData();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

//    QImage defaultIcon() const;
//    void setDefaultIcon(const QImage & img);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QPixmap *_pm_round_red;
    QPixmap *_pm_round_green;
    QString _styleSheet;

    QVector<QString> _cb_items_main_op;

    int     _cur_view;
    
signals:
    
public slots:
    
};

#endif // TREEWIDGETDELEGATE_H
