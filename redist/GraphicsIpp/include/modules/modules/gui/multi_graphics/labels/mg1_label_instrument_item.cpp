#include "../../../../StdAfx.h"
#include "mg1_label_instrument_item.h"
#undef max
#undef min
#include <algorithm>

namespace gui
{
namespace mg1
{



LabelInstrumentItem::LabelInstrumentItem( drawers::Text* text_drawer, DoubleScene_MG1 scene_pos, QString text, QString name,QFont font,  QColor	lable_text_color )
{
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setAcceptedMouseButtons(Qt::LeftButton);
	
	QFontMetrics fnt_metr(font);
	minimum_mode_el_size_ = QSize(16,fnt_metr.height()+4);
	text_drawer_  = text_drawer;
	value_scene_pos_ = scene_pos;
	text_ = text;
	font_ = font;
	name_ = name;
	lable_text_color_ = lable_text_color;
	prefered_el_size_ = text_drawer_->calcTextSize(text_,font_);
	prefered_el_size_.setHeight(prefered_el_size_.height()+4);
	prefered_el_size_.setWidth(prefered_el_size_.width()+7);
	real_el_size_ = prefered_el_size_;
	setToolTip(text_);
	i_pixmap_ = QPixmap(":/freelabels/i.png");
}

QRectF LabelInstrumentItem::boundingRect() const
{
	return QRectF(QPointF(0,0),real_el_size_);
}

void LabelInstrumentItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
	if (real_el_size_.width() < prefered_el_size_.width())
	{
		painter->setPen(text_drawer_->getSqureBorderColor());
		painter->setBrush(text_drawer_->getSqureBackgroundColor());
		painter->drawRect(QRectF(0,0,real_el_size_.width(),real_el_size_.height()));
		painter->drawPixmap(0,0,i_pixmap_);
	}
	else
	{
		text_drawer_->setTextColor(lable_text_color_);
		text_drawer_->PainterDrawSqure(painter,QRect(0,0,real_el_size_.width(),real_el_size_.height()),text_,font_);
	}
}

DoubleScene_MG1 LabelInstrumentItem::getValueScenePos() const
{
	return value_scene_pos_;
}

QSize LabelInstrumentItem::getPreferedSize() const
{
	return prefered_el_size_;
}

void LabelInstrumentItem::setMinimalMode()
{
	if (minimum_mode_el_size_ == real_el_size_) return;
	int width_diff = prefered_el_size_.width()-minimum_mode_el_size_.width();
	setPos ( pos().x()+ width_diff/2 , pos().y() );
	real_el_size_ = minimum_mode_el_size_;
}

bool LabelInstrumentItem::isInMinimalMode() const
{
	return (minimum_mode_el_size_ == real_el_size_);
}

void LabelInstrumentItem::setNormalMode()
{
	real_el_size_ = prefered_el_size_;
}

QString LabelInstrumentItem::getText() const
{
	return text_;
}

void LabelInstrumentItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	force_emit_click();
}

void LabelInstrumentItem::force_emit_click()
{
	emit ItemClicked(text_,value_scene_pos_);
}

QString LabelInstrumentItem::getName() const
{
	return name_;
}

void LabelInstrumentItem::changeText( QString new_text )
{
	text_ = new_text;
	prefered_el_size_ = text_drawer_->calcTextSize(text_,font_);
	prefered_el_size_.setHeight(prefered_el_size_.height()+4);
	prefered_el_size_.setWidth(prefered_el_size_.width()+7);
	setToolTip(text_);
	update();
}

void LabelInstrumentItem::changePos( DoubleScene_MG1 new_pos )
{
	value_scene_pos_ = new_pos;
}



//////////////////////////////////////////////////////////////////////////
//		LabelInstrumentUnion
//////////////////////////////////////////////////////////////////////////

LabelInstrumentUnion::LabelInstrumentUnion( drawers::Text* text_drawer, QString union_text,QFont font,bool show_hints )
{
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	setAcceptedMouseButtons(Qt::LeftButton);
	font_ = font;
	text_drawer_ = text_drawer;
	union_text_ = union_text;
	show_hints_ = show_hints;
}

void LabelInstrumentUnion::addToUnion( LabelInstrumentItem* item )
{
	if (isItemInUnion(item)) return;
	if (isEmpty())//не инициализирован min_max
	{
		min_val_ = item->getValueScenePos();
		max_val_ = min_val_;
	}
	else
	{
		min_val_ = std::min( min_val_,item->getValueScenePos());
		max_val_ = std::max( max_val_,item->getValueScenePos());
	}
	
	childs_.append(item);
	
	out_text_ = union_text_+QString::number(childs_.size());
	QSize text_size = text_drawer_->calcTextSize(out_text_,font_);
	real_el_size_.setHeight(text_size.height()+4);
	real_el_size_.setWidth(text_size.width()+7);
	central_scene_x_pos_ = DoubleScene_MG1( ( (max_val_ - min_val_) / 2.0 )+min_val_ );
}

void LabelInstrumentUnion::addToUnion( LabelInstrumentUnion* item )
{
	for (int i = 0; i < item->childs_.count();i++)
	{
		addToUnion(item->childs_.at(i));
	}
}

QRectF LabelInstrumentUnion::boundingRect() const
{
	return QRectF(QPointF(0,0),real_el_size_);
}

void LabelInstrumentUnion::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
	text_drawer_->PainterDrawSqure(painter,QRect(0,0,real_el_size_.width(),real_el_size_.height()),out_text_,font_);
}

DoubleScene_MG1 LabelInstrumentUnion::getCentralScenePos() const
{
	return central_scene_x_pos_;
}

bool LabelInstrumentUnion::isEmpty() const
{
	return (childs_.size() == 0);
}

LabelInstrumentItem* LabelInstrumentUnion::getLastItem() const
{
	return childs_.last();
}

QList<LabelInstrumentItem*> LabelInstrumentUnion::getChilds() const
{
	return childs_;
}


bool LabelInstrumentUnion::isItemInUnion( LabelInstrumentItem* item ) const
{
	int index = childs_.indexOf(item);
	if (index < 0) return false;
	return true;
}

QString LabelInstrumentUnion::GenerateHint()
{
	if (!show_hints_) return QString();
	QString header = "<html><head/><body>";
	QString total_str = tr("Total:") + " !TOTAL_RECORDS! <br/>";
	QString end = "</body></html>";
	QString html_return = "<br/>";
	
	int total_records = childs_.size();

	QString table = header;
	if (total_records > 5)
	{
		table+=total_str;
		table.replace("!TOTAL_RECORDS!",QString::number(total_records));
	}
	
	if (total_records > 30) 
	{
		table+= "...";
		table+=end;
		return table;
	}

	for (int i = 0; i < childs_.size();i++)
	{
		if (i > 0) table+= html_return;
		table+= QString::number(i+1)+". "+childs_.at(i)->getText();
	}
	
	table+=end;
	return table;
}

void LabelInstrumentUnion::setHint()
{
	setToolTip(GenerateHint());
}

void LabelInstrumentUnion::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	for (int i = 0; i < childs_.count();i++)
	{
		childs_.at(i)->force_emit_click();
	}
}

void LabelInstrumentUnion::setShowHints( bool show_hint )
{
	if (show_hints_ == show_hint) return;
	show_hints_ = show_hint;
	setHint();
}


}//namespace mg2
}//namespace gui
