#include "TreeView.h"
#include "TreeItem.h"

TreeView::TreeView(QWidget *parent)
{
    QStringList headers;
    headers << tr("") << tr("");
//    _model = new MainTreeModelManager(headers);

    _tree_view = new QTreeView();
    _tree_view->setSelectionMode(QAbstractItemView::SingleSelection);
    _tree_view->setSelectionBehavior(QAbstractItemView::SelectItems);
//    _tree_view->setModel(_model);
    _tree_view->expandAll();

    _tree_view->setWindowTitle(QObject::tr("MainModel"));
    _tree_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _delegate = new TreeWidgetDelegate();
    _tree_view->setItemDelegate(_delegate);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);
    hbox->addWidget(_tree_view, Qt::AlignJustify);
    this->setLayout(hbox);
}

QTreeView *TreeView::get_tree()
{
    return _tree_view;
}

void TreeView::resizeEvent(QResizeEvent *event)
{
    emit signalResize(this->size().width(), this->size().height());
}

//void TreeView::setModel(MainTreeModelManager *model)
//{
//    _model = model;
//    _tree_view->setModel(_model);
//}
