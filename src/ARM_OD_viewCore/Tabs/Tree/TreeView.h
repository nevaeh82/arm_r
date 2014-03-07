#ifndef TREEVIEW_H
#define TREEVIEW_H

//#include <QAction>
#include <QWidget>
#include <QMap>
#include <QTreeView>
#include <QHBoxLayout>

//#include "TreeWidgetDelegate.h"
//#include "MainTreeModelManager.h"

#include "TreeWidgetDelegate.h"

class TreeView : public QWidget
{
    Q_OBJECT
public:
    TreeView(QWidget *parent = 0);
    ~TreeView() {}

private:
    QTreeView               *_tree_view;
    TreeWidgetDelegate       *_delegate;

//    MainTreeModelManager    *_model;

public:
    QTreeView *get_tree();

//    void setModel(MainTreeModelManager *model);

    void resizeEvent(QResizeEvent *event);

signals:
    void signalResize(int width, int height);
};

#endif // TREEVIEW_H
