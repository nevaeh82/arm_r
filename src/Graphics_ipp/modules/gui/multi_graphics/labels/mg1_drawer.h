#ifndef MG1_DRAWER_H
#define MG1_DRAWER_H

#include <QImage>
#include <QPixmap>
#include <QPair>
#include <memory>

namespace gui
{
namespace mg1
{
namespace drawers
{

typedef std::shared_ptr<QImage> ImagePtr;


class Frame
{
public:
	Frame();
	virtual ~Frame();

	void	PainterDraw(QPainter* painter, QRect frame_rect);
	void	Draw(ImagePtr img);

	void	setBeginColor(QColor);
	void	setMidColor(QColor);
	void	setBorderColor(QColor);
	QColor	getBeginColor() const;
	QColor	getMidColor() const;
	QColor	getBorderColor() const;

private:
	
	QPixmap coner_pixmap_LT_;
	QPixmap coner_pixmap_RT_;
	QPixmap coner_pixmap_LB_;
	QPixmap coner_pixmap_RB_;
	
	QColor begin_color_;
	QColor mid_color_;
	QColor border_color_;

};


//////////////////////////////////////////////////////////////////////////


class Text
{
public:
	Text();
	virtual ~Text();
	Text& operator=(Text &tt)
	{
		text_color_				= tt.text_color_;
		squre_border_color_		= tt.squre_border_color_;
		squre_background_color_	= tt.squre_background_color_;
		use_cur_pen_brush_for_next_paint_draw_ = tt.use_cur_pen_brush_for_next_paint_draw_;
		return *this;
	}
	Text(const Text& tt)
	{
		text_color_				= tt.text_color_;
		squre_border_color_		= tt.squre_border_color_;
		squre_background_color_	= tt.squre_background_color_;
		use_cur_pen_brush_for_next_paint_draw_ = tt.use_cur_pen_brush_for_next_paint_draw_;
	}


	QColor	getTextColor() const {return text_color_;}
	QColor	getSqureBorderColor() const {return squre_border_color_;}
	QColor	getSqureBackgroundColor() const {return squre_background_color_;}

	void	setTextColor(QColor color);
	void	setSqureBorderColor(QColor color);
	void	setSqureBackgroundColor(QColor color);

	QSize	calcTextSize(QString text, QFont font);
	void	PainterDraw(QPainter* pnt, QRect rect, QString text, QFont font, Qt::AlignmentFlag aligment = Qt::AlignCenter);
	void	PainterDrawSqure(QPainter* pnt, QRect rect, QString text, QFont font, Qt::AlignmentFlag aligment = Qt::AlignCenter);
	void	Draw(ImagePtr img,  QString text, QFont font); 

	void	useCurPenBrushForNextPaintDraw_singleshot();

private:
	QColor	text_color_;
	QColor	squre_border_color_;
	QColor	squre_background_color_;
	bool	use_cur_pen_brush_for_next_paint_draw_;
};



//////////////////////////////////////////////////////////////////////////

class ArrowText
{
public:
	ArrowText();
	virtual ~ArrowText();

	void setTextColor(QColor color);
	void setArrowColor(QColor color);
	void setTextBackgroundColor(QColor color);

	//dstRect - это целиковый рект который надо отрисовать
	//screenRect - это рект ограничивающего экрана, как правило QRect(QPoint(0,0),getItemSize());
	void PainterDraw(QPainter* pnt, QRect dstRect, QRect screenRect, QString text, QFont font,Qt::Orientation orient, bool top_or_right = false);

private:
	
	// [arr1.f]<-----[arr1.s]    [arr2.f]----->[arr2.s]   
	void DrawArrow(QPainter* pnt, QPair<int,int> arr1,QPair<int,int> arr2, int lineXY_pos,Qt::Orientation orient);

	Text text_drawer_;
};






}//namespace drawers
}//namespace mg2
}//namespace gui

#endif // MG2_DRAWER_H
