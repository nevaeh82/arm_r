#include "StdAfx.h"
#include "radial_progress_bar.h"
#include <QRadialGradient>
#include <QPainter>
#include <QIcon>


namespace gui
{


RadialProgressBar::RadialProgressBar(QWidget *parent): QWidget(parent)
{
	geom_param_1_ = 0.030;
	geom_param_2_ = 0.070;
	geom_param_3_ = 0.065;
	geom_param_4_ = 0.140;
	
	fill_center_	= QColor(0,80,80);
	fill_edge_		= QColor(0,255,255);
	back_center_	= QColor(180,180,180);
	back_edge_		= QColor(220,220,220);
	border_color_	= Qt::black;
	value_			= 0;
	maximum_		= 100;
	draw_border_	= false;
}

RadialProgressBar::~RadialProgressBar()
{

}

void RadialProgressBar::SetValue( unsigned int value )
{
	if (value > maximum_) value = maximum_;
	value_ = value;
	if (value_ == maximum_) emit MaximumReached();
	update();
}

void RadialProgressBar::SetMaximum( unsigned int maximum )
{
	if (maximum < value_) value_ = maximum;
	maximum_ = maximum;
	if (value_ == maximum_) emit MaximumReached();
	update();
}

void RadialProgressBar::paintEvent( QPaintEvent * event )
{
	QPainter painter(this);
	QPixmap pxm = Render(size());
	painter.drawPixmap(0,0,pxm);
}

void RadialProgressBar::SetGeometryParams( qreal param_1,qreal param_2,qreal param_3,qreal param_4 )
{
	geom_param_1_ = param_1;
	geom_param_2_ = param_2;
	geom_param_3_ = param_3;
	geom_param_4_ = param_4;
	update();
}

void RadialProgressBar::SetBorderColor( QColor border_color )
{
	border_color_ = border_color;
	update();
}

void RadialProgressBar::SetFillColors( QColor center,QColor edge )
{
	fill_center_ = center;
	fill_edge_ = edge;
	update();
}

void RadialProgressBar::SetBackgroundColors( QColor center,QColor edge )
{
	back_center_ = center;
	back_edge_ = edge;
	update();
}

void RadialProgressBar::GetFillColors( QColor& center, QColor& edge ) const
{
	center = fill_center_;
	edge = fill_edge_;
}

void RadialProgressBar::GetBackgroundColors( QColor& center, QColor& edge ) const
{
	center = back_center_;
	edge = back_edge_;
}

QColor RadialProgressBar::GetBorderColor() const
{
	return border_color_;
}

void RadialProgressBar::SetDrawBorder( bool draw_border )
{
	draw_border_ = draw_border;
	update();
}

QPixmap RadialProgressBar::Render( QSize sz)
{
	double side = qMin(sz.width(),sz.height());
	QRectF main_rect(0,0,side,side);

	if (sz.width() > sz.height())
	{
		main_rect.moveLeft( (double)(sz.width() - side)/2.0);
	}
	else
	{
		main_rect.moveTop( (double)(sz.height() - side)/2.0);
	}

	QPixmap circles(sz);
	circles.fill(QColor(0,0,0,0));

	QPainter painter(&circles);
	painter.setRenderHint(QPainter::Antialiasing);
	
	int div_line_w = side* geom_param_1_;
	int first_circle_w = side* geom_param_2_;
	QRectF second_rect = main_rect.adjusted(first_circle_w,first_circle_w,-first_circle_w,-first_circle_w);
	int second_circle_w = side* geom_param_3_;
	QRectF third_rect = second_rect.adjusted(second_circle_w,second_circle_w,-second_circle_w,-second_circle_w);
	int third_circle_w = side* geom_param_4_;
	QRectF last_rect = third_rect.adjusted(third_circle_w,third_circle_w,-third_circle_w,-third_circle_w);


	QLineF line1(QPointF(main_rect.x()+div_line_w+0.7,main_rect.top()),QPointF(main_rect.right(),main_rect.bottom()-div_line_w-0.7)); /* \ */
	QLineF line2(QPointF(main_rect.x(),main_rect.top()+div_line_w+0.7),QPointF(main_rect.right()-div_line_w-0.7,main_rect.bottom())); 

	QLineF line3(QPointF(main_rect.right()-div_line_w-0.7,main_rect.top()),QPointF(main_rect.x(),main_rect.bottom()-div_line_w-0.7)); /* / */
	QLineF line4(QPointF(main_rect.right(),main_rect.top()+div_line_w+0.7),QPointF(main_rect.x()+div_line_w+0.7,main_rect.bottom()));

	QLineF line5(QPointF(main_rect.x()-5,main_rect.top()+(side/2.0)-div_line_w),QPointF(main_rect.right()+5,main_rect.top()+(side/2.0)-div_line_w)); /* - */
	QLineF line6(QPointF(main_rect.x()-5,main_rect.top()+(side/2.0)+div_line_w),QPointF(main_rect.right()+5,main_rect.top()+(side/2.0)+div_line_w));

	QLineF line7(QPointF(main_rect.x()+(side/2.0)-div_line_w,main_rect.top()-5),QPointF(main_rect.x()+(side/2.0)-div_line_w,main_rect.bottom()+5)); /* | */
	QLineF line8(QPointF(main_rect.x()+(side/2.0)+div_line_w,main_rect.top()-5),QPointF(main_rect.x()+(side/2.0)+div_line_w,main_rect.bottom()+5));


	QPainterPath out_clip;
	out_clip.addEllipse(main_rect.adjusted(-0.5,-0.5,0.5,0.5));
	out_clip.addEllipse(second_rect.adjusted(0.5,0.5,-0.5,-0.5));
	if ((second_circle_w > 0) && (third_circle_w > 0))
	{
		out_clip.addEllipse(third_rect.adjusted(-0.5,-0.5,0.5,0.5));
		out_clip.addEllipse(last_rect.adjusted(0.5,0.5,-0.5,-0.5));
	}
	painter.setClipPath(out_clip,Qt::ReplaceClip);


	QRadialGradient fill_grad(main_rect.center(),side/2,main_rect.center());
	fill_grad.setColorAt(0,fill_center_);
	fill_grad.setColorAt(0.2,fill_center_);
	fill_grad.setColorAt(1,fill_edge_);

	QRadialGradient empty_grad(main_rect.center(),side/2,main_rect.center());
	empty_grad.setColorAt(0,back_center_);
	empty_grad.setColorAt(0.2,back_center_);
	empty_grad.setColorAt(1,back_edge_);


	int coner = 360.0*(double)value_/maximum_;

	painter.setPen(Qt::NoPen);
	painter.setBrush(empty_grad);
	painter.drawRect(main_rect);

	painter.setBrush(fill_grad);
	painter.drawPie(main_rect,90 * 16,-coner * 16);
	painter.setBrush(Qt::NoBrush);

	if (draw_border_) painter.setPen(border_color_);
	else painter.setPen(Qt::NoPen);

	if (div_line_w > 0)
	{
		painter.drawLine(line1);
		painter.drawLine(line2);

		painter.drawLine(line3);
		painter.drawLine(line4);

		painter.drawLine(line5);
		painter.drawLine(line6);

		painter.drawLine(line7);
		painter.drawLine(line8);
	}

	painter.drawEllipse(main_rect);
	painter.drawEllipse(second_rect);
	if ((second_circle_w > 0) && (third_circle_w > 0))
	{
		painter.drawEllipse(third_rect);
		painter.drawEllipse(last_rect);
	}

	painter.end();

	QPixmap retVal(sz);
	retVal.fill(QColor(0,0,0,0));
	painter.begin(&retVal);

	if (div_line_w > 0)
	{
		QPainterPath lines_clip;
		lines_clip.setFillRule(Qt::WindingFill);
		auto MakeLinesClip = [&](const QLineF& line_1, const QLineF& line_2)
		{
			QPolygonF myPolygon;
			myPolygon << line_1.p1() << line_1.p2() << line_2.p2() << line_2.p1() << line_1.p1();
			lines_clip.addPolygon(myPolygon);
		};
		MakeLinesClip(line1.translated(-0.5,0),line2.translated(0,-0.5));
		MakeLinesClip(line3.translated(0.5,0),line4.translated(0,-1.5));
		MakeLinesClip(line5.translated(0,0.5),line6.translated(0,-0.5));
		MakeLinesClip(line7.translated(0.5,0),line8.translated(-0.5,0));
		
		QPainterPath aaa;
		aaa.addRect(QRect(main_rect.x()-2,main_rect.y()-2,main_rect.width()+4,main_rect.height()+4));
		painter.setClipPath(aaa.subtracted(lines_clip));
	}

	painter.drawPixmap(0,0,circles);
	painter.end();
	return retVal;
}

QIcon RadialProgressBar::RenderToIcon( QSize icon_size )
{
	return QIcon( Render(icon_size) );
}

QPixmap RadialProgressBar::RenderToPixmap( QSize pixmap_size )
{
	return Render(pixmap_size);
}

unsigned int RadialProgressBar::value() const
{
	return value_;
}

unsigned int RadialProgressBar::maximum() const
{
	return maximum_;
}

void RadialProgressBar::IncrementProgressBy( unsigned int inc_value )
{
	SetValue(value()+inc_value);
}



}//namespace gui