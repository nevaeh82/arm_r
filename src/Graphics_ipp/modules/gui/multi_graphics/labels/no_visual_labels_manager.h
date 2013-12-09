#ifndef NO_VISUAL_LABELS_MANAGER_H
#define NO_VISUAL_LABELS_MANAGER_H

#include <QObject>
#include <QTimer>
#include "../../../../modules/gui/multi_graphics/labels/mg1_label_instrument_item.h"

class Q_MG_MouseCursor;


class NoVisualLabelsManager : public QObject
{
	Q_OBJECT

public:
	NoVisualLabelsManager(Q_MG_MouseCursor *parent);
	~NoVisualLabelsManager();
	//ОСНОВНЫЕ
	void addLabel(double pos, QString text, QString name = QString(), bool immediately_update = false, QColor label_text_color = QColor(100,250,250));
	void changeLabelText(QString name, QString new_text, bool immediately_update = false);
	void changeLabelPosition(QString name, double new_pos, bool immediately_update = false);
	void removeLabel(QString name, bool immediately_update = false);
	void clearLabels();
	void redraw();
	void setUnionsText(QString union_text);
	void setUnionsShowHints(bool show_hints);
private:
	bool fastItemsIntersect(QGraphicsItem* first_item,QGraphicsItem* second_item);
	Q_MG_MouseCursor *graphics_parent_;

	void					ReCalcUnions();
	gui::mg1::LabelInstrumentUnion*	findInUnionsList(gui::mg1::LabelInstrumentItem* itm);
	void					clearUnions();
	void					updateUnionPosition(gui::mg1::LabelInstrumentUnion* un);
	void					UpdateLabelsGeomentry(bool update_unions);
	gui::mg1::LabelInstrumentUnion*	CreateNewUnion();
	bool					StartTimerForUpdate();//возвращает: true - стартовал ; false - сразу обновил, нестартовал
	gui::mg1::LabelInstrumentItem* findItemByName(QString name);

	QList<gui::mg1::LabelInstrumentItem*>		labels_list_;
	QList<gui::mg1::LabelInstrumentUnion*>	labels_unions_list_;
	gui::mg1::drawers::Text					text_drawer_;
	QTimer									timer_recalc_unions_;

	double	prev_kf_x_;
	double prev_trans_x_;
	QString union_text_;
	bool show_hints_;
private slots:
	void					Timer_recalc_unions_timeout_slot();

};




#endif // NO_VISUAL_LABELS_MANAGER_H
