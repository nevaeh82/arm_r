#include "StdAfx.h"
#include <QImage>
#include <QPainter>
#include "direct_image_painter.h"

namespace gui
{

DirectImagePainter::DirectImagePainter(std::shared_ptr<QImage> paint_dst): QObject(nullptr)
{
	if ((paint_dst->format() != QImage::Format_ARGB32) && (paint_dst->format() != QImage::Format_ARGB32_Premultiplied) &&  (paint_dst->format() != QImage::Format_RGB32)  )
	{
		 Q_ASSERT_X(false,"DirectImagePainter","Image format not supported");
		 return;
	}
	if ((paint_dst->rect().width() <= 0) || (paint_dst->rect().height() <= 0))
	{
		Q_ASSERT_X(false,"DirectImagePainter","Image rect too small <= 0");
		return;
	}

	image_			= paint_dst;
	
	image_rect_		= image_->rect();
	bytes_per_line_ = image_->bytesPerLine();
	bytes_per_pixel_ = 4;//static_cast<int>( (double)image_->bytesPerLine()/(double)image_->width() );
	img_bits_		= image_->bits();

	use_alpha_blending_ = false;
}

DirectImagePainter::~DirectImagePainter()
{

}

void DirectImagePainter::Fill( QColor col )
{
	image_->fill(col);
}

void DirectImagePainter::SetColor( QColor col )
{
	if ((image_->format() == QImage::Format_ARGB32_Premultiplied) && (col.alpha() < 255))
	{
		QColor rgbpre = QColor(0,0,0,0);
		if (col.alpha() > 0)
		{
			double scale_factor = 255.0/(double)col.alpha();
			rgbpre.setRed(col.red()/scale_factor);
			rgbpre.setGreen(col.green()/scale_factor);
			rgbpre.setBlue(col.blue()/scale_factor);
			rgbpre.setAlpha(col.alpha());	
		}
		cur_color_ = rgbpre.rgba();
	}
	else cur_color_ = col.rgba();

	original_color_ = col.rgba();
	cached_data_.clear();
}

void DirectImagePainter::FillRect( QRect rect )
{
	rect = rect.normalized();
	rect = rect.intersect(image_rect_);
	if (use_alpha_blending_)
	{
		//рисование QT-шными средствами у них смешивание блоков цвета быстрее
		QColor a = QColor::fromRgba(original_color_);
		QPainter pnt(image_.get());
		pnt.setPen(Qt::NoPen);
		pnt.setBrush(a);
		pnt.drawRect(rect);
		pnt.end();
		return;
	}
	else
	{
		if (rect.size() == QSize(1,1))
		{
			SetPixel(rect.topLeft());
			return;
		}
				
		for (int y = rect.top(); y <= rect.bottom();y++)
			FillXLine(rect.left(),rect.right(),y);
	}
		
}

void DirectImagePainter::FillRect( QRect rect, QColor col )
{
	SetColor(col);
	FillRect(rect);
}

void DirectImagePainter::FillRect( QRectF rect )
{
	//QRect t_rect(static_cast<int>( rect.x() ), static_cast<int>( rect.y() ), static_cast<int>( rect.width() ), static_cast<int>( rect.height() ));
	//QRect t_rect(qRound( rect.x() ), qRound( rect.y() ), qRound( rect.width() ), qRound( rect.height() ));
	FillRect(rect.toRect());
}

uchar* DirectImagePainter::GetScanLine( int y )
{
	return (uchar *)(img_bits_+ y *bytes_per_line_); 
}

void DirectImagePainter::FillXLine( int from_x, int to_x, int y)
{
	uchar *sLine = GetScanLine(y);
		
	int pixels_count = (to_x-from_x)+1;

	if ( (cached_data_.size() == 0) || (cached_data_.size() < pixels_count*bytes_per_pixel_) )
	{
		//генерим новую линию
		cached_data_.resize( pixels_count * bytes_per_pixel_ );
		char*	c_data = cached_data_.data();
		for (int i = 0; i < pixels_count;i++)
			((uint*)c_data)[i] = cur_color_;
	}

	memcpy(&sLine[from_x*bytes_per_pixel_],cached_data_.data(),pixels_count*bytes_per_pixel_);
}

uint DirectImagePainter::ColorBlending( uint c1_i, uint c2_i )
{
	QColor result;
	double a1 = (double) qAlpha(c1_i) / 255.0;
	double a2 = (double) qAlpha(c2_i) / 255.0;
	result.setRed( (int) (a1 * qRed(c1_i) + a2 * (1 - a1) * qRed(c2_i)) );
	result.setGreen( (int) (a1 * qGreen(c1_i) + a2 * (1 - a1) * qGreen(c2_i)) );
	result.setBlue( (int) (a1 * qBlue(c1_i) + a2 * (1 - a1) * qBlue(c2_i)) );
	result.setAlpha( (int) (255 * (a1 + a2 * (1 - a1))) );
	return result.rgba();
}

void DirectImagePainter::SetPixel( QPoint pixel )
{
	if (!image_rect_.contains(pixel)) return;
	uchar *sLine = GetScanLine(pixel.y());

	if (use_alpha_blending_)
	{
		uint pixel_color = ((uint*)sLine)[pixel.x()];
		((uint*)sLine)[pixel.x()] = ColorBlending(original_color_,pixel_color);
	}
	else
	{
		((uint*)sLine)[pixel.x()] = cur_color_;
	}
}

void DirectImagePainter::SetEnableAlphaBlending( bool enable )
{
	use_alpha_blending_ = enable;
}

bool DirectImagePainter::lines_intersection( QPointF start1, QPointF end1, QPointF start2, QPointF end2, QPointF *out_intersection )
{
	QPointF dir1 = end1 - start1;
	QPointF dir2 = end2 - start2;

	//считаем уравнения прямых проходящих через отрезки
	double a1 = -dir1.y();
	double b1 = +dir1.x();
	double d1 = -(a1*start1.x() + b1*start1.y());

	double a2 = -dir2.y();
	double b2 = +dir2.x();
	double d2 = -(a2*start2.x() + b2*start2.y());

	//подставляем концы отрезков, для выяснения в каких полуплоскотях они
	double seg1_line2_start = a2*start1.x() + b2*start1.y() + d2;
	double seg1_line2_end = a2*end1.x() + b2*end1.y() + d2;

	double seg2_line1_start = a1*start2.x() + b1*start2.y() + d1;
	float seg2_line1_end = a1*end2.x() + b1*end2.y() + d1;

	//если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
	if (seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0) 
		return false;

	float u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
	*out_intersection =  start1 + u*dir1;

	return true;
}

bool DirectImagePainter::DrawLine( QPoint p1,QPoint p2 )
{
	bool retValPaint = false;
	QSize imgSize = image_rect_.size();
	
	uchar *sLine;
	
	//если это точка, проверяем что не вылезла за холст, шлепаем ее по бырику и выходим
	if (p1 == p2)
	{
		int x = p1.x();
		int y = p1.y();
		if ((x < 0) || (y < 0) || (x >= imgSize.width()) ||  (y >= imgSize.height())) return retValPaint;
		sLine = GetScanLine(y);
		if (use_alpha_blending_)
			((uint *)sLine)[x] = ColorBlending( original_color_, ((uint *)sLine)[x]  );
		else
			((uint *)sLine)[x] = cur_color_;
		retValPaint = true;
		return retValPaint;
	}


	auto checkLinesValues = [&]()->bool
	{
		QRectF screen (QPointF(-1,-1), QSizeF(imgSize.width()+2,imgSize.height()+2));

		bool res1 = false;
		bool res2 = false;
		bool haveAnyIntersect = false;
		QPoint resP1;
		QPoint resP2;

		if (screen.contains(p1)) {resP1 = p1;res1 = true;haveAnyIntersect = true;}
		if (screen.contains(p2)) {resP2 = p2;res2 = true;haveAnyIntersect = true;}

		if (res1 && res2) return haveAnyIntersect;

		QPointF out;
		auto CalcResForLine = [&](QPointF lp1, QPointF lp2)
		{
			if (!res1 || !res2)
				if (lines_intersection( lp1, lp2 , p1, p2,&out))
				{
					haveAnyIntersect = true;
					if (!res1)	{resP1 = out.toPoint();res1 = true;}
					else
						if (!res2)	{resP2 = out.toPoint();res2 = true;}
				}
		};

		CalcResForLine(screen.topLeft(), screen.bottomLeft());
		CalcResForLine(screen.topLeft(), screen.topRight());
		CalcResForLine(screen.topRight(), screen.bottomRight());
		CalcResForLine(screen.bottomLeft(), screen.bottomRight());

		if (res1) p1 = resP1;
		if (res2) p2 = resP2;

		return haveAnyIntersect;
	};

	if (!checkLinesValues()) return retValPaint;

	int p1x = p1.x();
	int p1y = p1.y();
	int p2x = p2.x();
	int p2y = p2.y();

	
	bool Invert = false;
	if (qAbs(p2x-p1x) < qAbs(p2y-p1y)) Invert = true;

	int Opty_Y = (p2y-p1y);
	int Opty_X = (p2x-p1x);

	if (!Invert)
	{
		if (p1x>p2x)
		{
			std::swap(p1x,p2x);
			std::swap(p1y,p2y);
		}

		for (int x = p1x;x < p2x;x++)
		{
			int y = ( (double)((x-p1x)*Opty_Y) / (double)(Opty_X) +p1y );//qRound64
			if ((x < 0) || (y < 0) || (x >= imgSize.width()) ||  (y >= imgSize.height())) continue;

			sLine = GetScanLine(y);
			if (use_alpha_blending_)
			{
				((uint *)sLine)[x] = ColorBlending( original_color_, ((uint *)sLine)[x]  );
			}
			else
			{
				((uint *)sLine)[x] = cur_color_;
			}
			retValPaint = true;
		}
	}
	else
	{
		if (p1y>p2y)
		{
			std::swap(p1x,p2x);
			std::swap(p1y,p2y);
		}

		for (int y = p1y;y < p2y;y++)
		{
			int x = ( (double)(Opty_X*(y-p1y)) / (double)(Opty_Y) +p1x );//qRound64
			if ((x < 0) || (y < 0) || (x >= imgSize.width()) ||  (y >= imgSize.height())) continue;

			sLine = GetScanLine(y);
			if (use_alpha_blending_)
			{
				((uint *)sLine)[x] = ColorBlending( original_color_, ((uint *)sLine)[x]  );
			}
			else
			{
				((uint *)sLine)[x] = cur_color_;
			}
			retValPaint = true;
		}
	}
	return retValPaint;
}

void DirectImagePainter::DrawImage( QPoint pos, std::shared_ptr<QImage> src_img )
{
	Q_ASSERT_X(image_->format() == src_img->format(),"DirectImagePainter","Format of src image != fromat of dst image");
	if (use_alpha_blending_)
	{
		QPainter pnt(image_.get());
		pnt.drawImage(pos,*src_img.get());
		pnt.end();
	}
	else
	{
		QRect src_rect;
		QRect dst_rect;
		{
			src_rect = src_img->rect();
			src_rect.moveTopLeft(pos);
			dst_rect = image_rect_.intersect(src_rect);
			QRect temp = image_rect_;
			temp.moveTopLeft(pos * -1);
			src_rect = src_img->rect();
			src_rect = src_rect.intersect(temp);
			Q_ASSERT_X(src_rect.size() == dst_rect.size(),"DirectImagePainter","src_rect != dst_rect. Internal error");
			if (dst_rect.size() == QSize(0,0)) return;
		}
			
		uchar*	src_sLine;
		uchar*	src_iBits			= src_img->bits();
		int		src_bytesPerLine	= src_img->bytesPerLine();
		uchar*	dst_sLine;

		for (int Y = dst_rect.top();Y < dst_rect.bottom();Y++)
		{
			dst_sLine = GetScanLine(Y);
			src_sLine = (uchar *)(src_iBits+(Y-dst_rect.top()+src_rect.top())*src_bytesPerLine); 
			memcpy( (uchar *)&dst_sLine[dst_rect.left()*bytes_per_pixel_],
				    (uchar *)&src_sLine[src_rect.left()*bytes_per_pixel_],
					dst_rect.width()*bytes_per_pixel_
					);
		}
	}	
}



}