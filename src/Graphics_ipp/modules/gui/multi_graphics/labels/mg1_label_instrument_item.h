#ifndef MG1_LABEL_INSTRUMENT_ITEM_H
#define MG1_LABEL_INSTRUMENT_ITEM_H

#define DoubleScene_MG1 double 

#include "mg1_drawer.h"

#include <QGraphicsObject>
#include <QFont>

namespace gui
{
namespace mg1
{


class LabelInstrumentItem : public QGraphicsObject
{
	Q_OBJECT
public:
	LabelInstrumentItem( drawers::Text* text_drawer, DoubleScene_MG1 scene_pos, QString text, QString name, QFont font, QColor	lable_text_color );
	virtual ~LabelInstrumentItem() {};

	void			changeText(QString new_text);
	void			changePos(DoubleScene_MG1 new_pos);
	DoubleScene_MG1		getValueScenePos() const;
	QString			getText() const;
	QString			getName() const;
	QSize			getPreferedSize() const;
	QRectF			boundingRect () const;
	void			setMinimalMode();
	void			setNormalMode();
	void			force_emit_click();
	bool			isInMinimalMode() const;

private:

	void	paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	void	mousePressEvent ( QGraphicsSceneMouseEvent * event );

	QPixmap			i_pixmap_;
	drawers::Text*	text_drawer_;
	DoubleScene_MG1		value_scene_pos_;
	QString			text_;
	QString			name_;
	QFont			font_;
	QSize			real_el_size_;
	QSize			prefered_el_size_;
	QSize			minimum_mode_el_size_;
	QColor			lable_text_color_;
	
signals:
	void ItemClicked(QString item_text,DoubleScene_MG1 scene_value);
};




class LabelInstrumentUnion : public QGraphicsObject
{
	Q_OBJECT
public:
	LabelInstrumentUnion( drawers::Text* text_drawer,QString union_text, QFont font, bool show_hints );
	virtual ~LabelInstrumentUnion() {};

	void							addToUnion(LabelInstrumentItem* item);
	void							addToUnion(LabelInstrumentUnion* item);
	QRectF							boundingRect () const;
	DoubleScene_MG1						getCentralScenePos() const;
	bool							isEmpty() const;
	LabelInstrumentItem*			getLastItem() const;
	QList<LabelInstrumentItem*>		getChilds() const;
	bool							isItemInUnion(LabelInstrumentItem* item) const;
	void							setHint();
	void							setShowHints(bool show_hint);
private:

	void		paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	void		mousePressEvent ( QGraphicsSceneMouseEvent * event );
	QString		GenerateHint();

	DoubleScene_MG1					min_val_;
	DoubleScene_MG1					max_val_;
	QList<LabelInstrumentItem*> childs_;

	DoubleScene_MG1					central_scene_x_pos_;
	QString						out_text_;

	drawers::Text*	text_drawer_;
	QFont			font_;
	QSize			real_el_size_;
	QString			union_text_;
	bool			show_hints_;
};


}//namespace mg1
}//namespace gui

#endif // MG2_LABEL_INSTRUMENT_ITEM_H
