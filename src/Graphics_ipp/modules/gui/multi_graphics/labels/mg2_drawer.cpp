#include "../../../../StdAfx.h"
#include "mg2_drawer.h"
#include <QLinearGradient>
#undef max
#undef min
#include <algorithm>

namespace gui
{
namespace mg2
{
namespace drawers
{


Frame::Frame()
{
	coner_pixmap_LT_	= QPixmap(":/Drawer/DrawerResources/conLT.png");
	coner_pixmap_RT_	= QPixmap(":/Drawer/DrawerResources/conRT.png");
	coner_pixmap_LB_	= QPixmap(":/Drawer/DrawerResources/conLB.png");
	coner_pixmap_RB_	= QPixmap(":/Drawer/DrawerResources/conRB.png");
	begin_color_		= QColor(0,100,100,150);
	mid_color_			= QColor(0,0,0,200);
	border_color_		= QColor(0,150,150);
}

Frame::~Frame()
{}

void Frame::Draw( ImagePtr img )
{
	QPainter painter(img.get());
	PainterDraw(&painter,QRect(0,0,img->width(),img->height()));
	painter.end();
}

void Frame::PainterDraw( QPainter* painter, QRect frame_rect )
{
	painter->save();

	painter->setRenderHint(QPainter::Antialiasing,false);
	painter->setRenderHint(QPainter::TextAntialiasing,false);
	painter->setRenderHint(QPainter::NonCosmeticDefaultPen,true);

	QLinearGradient BackGrad(0.0, 2.0, 0.0, frame_rect.height()-3);
	BackGrad.setColorAt(0.0, begin_color_);
	if (frame_rect.height() < 30)
	{
		BackGrad.setColorAt(0.5, mid_color_);
	}
	else
	{
		BackGrad.setColorAt(0.3, mid_color_);
		BackGrad.setColorAt(0.7, mid_color_);
	}
	BackGrad.setColorAt(1.0, begin_color_);

	painter->setPen(border_color_);
	painter->setBrush(BackGrad);
	painter->drawRect(QRectF(frame_rect.left()+2,frame_rect.top()+2,frame_rect.right()-4,frame_rect.bottom()-4));

	painter->drawPixmap(frame_rect.left(),frame_rect.top(),coner_pixmap_LT_);
	painter->drawPixmap(frame_rect.right()-4,frame_rect.top(),coner_pixmap_RT_);
	painter->drawPixmap(frame_rect.left(),frame_rect.bottom()-4,coner_pixmap_LB_);
	painter->drawPixmap(frame_rect.right()-4,frame_rect.bottom()-4,coner_pixmap_RB_);

	painter->restore();
}

void Frame::setBeginColor( QColor color)
{
	begin_color_ = color;
}

void Frame::setBorderColor( QColor color)
{
	border_color_ = color;
}

void drawers::Frame::setMidColor( QColor color)
{
	mid_color_ = color;
}

QColor Frame::getBeginColor() const
{
	return begin_color_;
}

QColor Frame::getMidColor() const
{
	return mid_color_;
}

QColor Frame::getBorderColor() const
{
	return border_color_;
}


//////////////////////////////////////////////////////////////////////////

Text::Text()
{
	text_color_ = QColor(250,250,250);
	squre_border_color_ = text_color_;
	squre_background_color_ = QColor(0,0,0,180);
	use_cur_pen_brush_for_next_paint_draw_ = false;
}

Text::~Text()
{

}

void Text::PainterDraw( QPainter* pnt, QRect rect, QString text, QFont font, Qt::AlignmentFlag aligment )
{
	pnt->save();
	if (!use_cur_pen_brush_for_next_paint_draw_)
	{		
		pnt->setPen(text_color_);
	}
	pnt->setFont(font);
	QTextOption tOpt(aligment);
	pnt->drawText(rect,text,tOpt);
	pnt->restore();
	use_cur_pen_brush_for_next_paint_draw_ = false;
}

void Text::PainterDrawSqure( QPainter* pnt, QRect rect, QString text, QFont font, Qt::AlignmentFlag aligment /*= Qt::AlignCenter*/ )
{
	pnt->save();
	if (!use_cur_pen_brush_for_next_paint_draw_)
	{
		pnt->setPen(squre_border_color_);
		pnt->setBrush(squre_background_color_);
	}
	pnt->drawRect(QRectF(rect));
	PainterDraw( pnt, rect, text, font, aligment );
	pnt->restore();
}


void Text::setTextColor( QColor color )
{
	text_color_ = color;
}

QSize Text::calcTextSize( QString text, QFont font )
{
	if (text.isEmpty()) return QSize(0,0);
	QFontMetrics fnt(font);
	QRect fntRect = fnt.boundingRect(QRect (0,0,0,0), Qt::AlignJustify , text );
	QRectF retVal(0,0,fntRect.width()+qAbs(fntRect.x()),fntRect.height()+qAbs(fntRect.y()));
	return QSize(retVal.width(), retVal.height());
}

void Text::Draw( ImagePtr img, QString text, QFont font )
{
	QPainter painter(img.get());
	PainterDraw(&painter,QRect(0,0,img->width(),img->height()),text,font);
	painter.end();
}

void Text::setSqureBorderColor( QColor color )
{
	squre_border_color_ = color;
}

void Text::setSqureBackgroundColor( QColor color )
{
	squre_background_color_ = color;
}

void Text::useCurPenBrushForNextPaintDraw_singleshot()
{
	use_cur_pen_brush_for_next_paint_draw_ = true;
}

//////////////////////////////////////////////////////////////////////////

ArrowText::ArrowText()
{
	setArrowColor( QColor(0,0,200) );
}

ArrowText::~ArrowText()
{

}

void ArrowText::setTextColor( QColor color )
{
	text_drawer_.setTextColor(color);
}

void ArrowText::setArrowColor( QColor color )
{
	text_drawer_.setSqureBorderColor(color);
}

void ArrowText::setTextBackgroundColor( QColor color )
{
	text_drawer_.setSqureBackgroundColor(color);
}

void ArrowText::PainterDraw( QPainter* pnt, QRect dstRect, QRect screenRect, QString text, QFont font,Qt::Orientation orient, bool top_or_right)
{
	QRect  screen_intersect_rect_ = dstRect.intersect(screenRect);
	if (screen_intersect_rect_.isNull()) return;
	
	QSize text_size = text_drawer_.calcTextSize(text,font);
	
	if (orient == Qt::Horizontal)
	{
		int start_x	= std::min(dstRect.x(),dstRect.right());
		int end_x	= std::max(dstRect.x(),dstRect.right());

		if (screen_intersect_rect_.width() < 20) return;

		int TextXpos = ((screen_intersect_rect_.width()/2)+screen_intersect_rect_.left()) - (text_size.width()/2);
		int TextYpos;
		if (!top_or_right)	//bottom
		{
			TextYpos = screen_intersect_rect_.bottom()-text_size.height()-5;
			if (TextYpos < 3) TextYpos = 3;
		}
		else	//top
		{
			TextYpos = screen_intersect_rect_.top()+4;
			if (TextYpos+text_size.height() > screenRect.bottom()-5) TextYpos = screenRect.bottom() - text_size.height()-5;
		}
		

		QRect text_rect = QRect(TextXpos-3,TextYpos,text_size.width()+6,text_size.height()+2);
		QPair<int,int> Arr1(start_x,text_rect.x());
		QPair<int,int> Arr2(text_rect.right()+1,end_x);
		
		if (text_rect.width()+15 > screen_intersect_rect_.width())
		{
			Arr1 = QPair<int,int>(start_x,screen_intersect_rect_.right());
			Arr2 = QPair<int,int>(screen_intersect_rect_.right(),end_x);
			text = "";
		}

		if (!text.isEmpty())
		{
			text_drawer_.PainterDrawSqure(pnt,text_rect,text,font);
		}
		
		pnt->setPen(text_drawer_.getSqureBorderColor());
		int arrow_XY = TextYpos+(text_size.height()/2);
		
		//рисование направляющих если значение выскачило за пределы квадрата
		if (!screen_intersect_rect_.contains(screen_intersect_rect_.x(),arrow_XY))
		{
			pnt->save();
			QPen pen( pnt->pen() );
			pen.setStyle(Qt::DashLine);
			pnt->setPen(pen);
			pnt->drawLine(QLineF(screen_intersect_rect_.x(),screen_intersect_rect_.y(),screen_intersect_rect_.x(),arrow_XY));
			pnt->drawLine(QLineF(screen_intersect_rect_.right()+1,screen_intersect_rect_.y(),screen_intersect_rect_.right()+1,arrow_XY));
			pnt->restore();
		}
		
		DrawArrow(pnt,Arr1,Arr2,arrow_XY,orient);
	}
	else
	{
		int start_y	= std::min(dstRect.y(),dstRect.bottom())+1;
		int end_y	= std::max(dstRect.y(),dstRect.bottom())+1;

		if (screen_intersect_rect_.height() < 20) return;

		int TextYpos = ((screen_intersect_rect_.height()/2)+screen_intersect_rect_.top()) - (text_size.width()/2);
		int TextXpos;
		if (!top_or_right)	//left
		{
			TextXpos = screen_intersect_rect_.left() + (text_size.height()/2)-3;
			if (TextXpos+text_size.height() > screenRect.right()-3) TextXpos = screenRect.right() - text_size.height()-3;
		}
		else	//right
		{
			TextXpos = screen_intersect_rect_.right()- text_size.height()-3;
			if (TextXpos < 3) TextXpos = 3;
		}
				
		
		QRect text_rect = QRect(TextXpos,TextYpos-3,text_size.height(),text_size.width()+6);
		QPair<int,int> Arr1(start_y,text_rect.y());
		QPair<int,int> Arr2(text_rect.bottom()+2,end_y);

		if (text_rect.height()+15 > screen_intersect_rect_.height())
		{
			Arr1 = QPair<int,int>(start_y,screen_intersect_rect_.bottom());
			Arr2 = QPair<int,int>(screen_intersect_rect_.bottom(),end_y);
			text = "";
		}

		pnt->setPen(text_drawer_.getSqureBorderColor());
		if (!text.isEmpty())
		{
			pnt->setBrush(text_drawer_.getSqureBackgroundColor() );
			pnt->drawRect(text_rect);

			QImage img(text_rect.height(),text_rect.width(),QImage::Format_ARGB32_Premultiplied);
			img.fill(0);
			{
				QPainter imgPnt(&img);
				text_drawer_.PainterDraw(&imgPnt,QRect(0,0,text_rect.height(),text_rect.width()),text,font);
				imgPnt.end();
				img = img.transformed(QTransform(0,-1,0,1,0,0,1,1,1), Qt::FastTransformation);
			}
			pnt->drawImage(text_rect.x(),text_rect.y(),img);
		}

		int arrow_XY = TextXpos+(text_size.height()/2);

		//рисование направляющих если значение выскачило за пределы квадрата
		if (!screen_intersect_rect_.contains(arrow_XY,screen_intersect_rect_.y()))
		{
			pnt->save();
			QPen pen( pnt->pen() );
			pen.setStyle(Qt::DashLine);
			pnt->setPen(pen);
			pnt->drawLine(QLineF(screen_intersect_rect_.x(),screen_intersect_rect_.y(),arrow_XY,screen_intersect_rect_.y()));
			pnt->drawLine(QLineF(screen_intersect_rect_.x(),screen_intersect_rect_.bottom()+1,arrow_XY,screen_intersect_rect_.bottom()+1));
			pnt->restore();
		}

		DrawArrow(pnt,Arr1,Arr2,arrow_XY,orient);
	}
}

void ArrowText::DrawArrow(QPainter* painter, QPair<int,int>  arr1,QPair<int,int>  arr2, int lineXY_pos, Qt::Orientation orient )
{
	if (orient == Qt::Horizontal)
	{
		painter->drawLine(QLine(arr1.first,lineXY_pos,arr1.second,lineXY_pos));
		painter->drawLine(QLine(arr2.first,lineXY_pos,arr2.second,lineXY_pos));
		painter->drawLine(QLine(arr1.first,lineXY_pos,arr1.first+4,lineXY_pos-2));
		painter->drawLine(QLine(arr1.first,lineXY_pos,arr1.first+4,lineXY_pos+2));
		painter->drawLine(QLine(arr2.second,lineXY_pos,arr2.second-4,lineXY_pos-2));
		painter->drawLine(QLine(arr2.second,lineXY_pos,arr2.second-4,lineXY_pos+2));
	}
	else
	{
		painter->drawLine(QLine(lineXY_pos,arr1.first,lineXY_pos,arr1.second));
		painter->drawLine(QLine(lineXY_pos,arr2.first,lineXY_pos,arr2.second));
		painter->drawLine(QLine(lineXY_pos,arr1.first, lineXY_pos+2,arr1.first+4));
		painter->drawLine(QLine(lineXY_pos,arr1.first, lineXY_pos-2,arr1.first+4));
		painter->drawLine(QLine(lineXY_pos,arr2.second, lineXY_pos+2,arr2.second-4));
		painter->drawLine(QLine(lineXY_pos,arr2.second, lineXY_pos-2,arr2.second-4));
	}
}





}//namespace drawers
}//namespace mg2
}//namespace gui
