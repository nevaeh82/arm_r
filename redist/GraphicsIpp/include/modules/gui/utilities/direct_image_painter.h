#ifndef DIRECT_IMAGE_PAINTER_H
#define DIRECT_IMAGE_PAINTER_H

#include <QObject>
#include <QImage>
#include <QColor>
#include <memory>

namespace gui
{

//класс заточен под рисование на QImage, использовать желательно в том случае если нужен однослойный Image (т.е. фон игнорируется)
//минимальное увеличение производительности увеличено в 4 раза, местами может достигать 10 кратного ускорения.
class DirectImagePainter : public QObject
{
	Q_OBJECT

public:
	DirectImagePainter( std::shared_ptr<QImage> paint_dst);//формат должен быть 32 бита на пиксель
	~DirectImagePainter();

	void	SetColor(QColor col);
	void	Fill(QColor col);//заливает весь Image заданным цветом (смешивание цветов игнорируется)
	void	SetPixel(QPoint pixel);//медленная (но не медленнее чем QPainter;) ) много ей не рисовать, жедательно использовать другую из набора
	void	FillRect(QRect rect);
	void	FillRect(QRectF rect);
	void	FillRect(QRect rect, QColor col);
	void	SetEnableAlphaBlending(bool enable);//альфа смешивание цветов (по-умолчанию выкл.). Необходим только в том случае если один и тот же Image многослойный. При включении этой опции компонент будет рисовать средствами QPainter (не все, но многое) (этот момент пока не оптимизирован)
	bool	DrawLine( QPoint p1,QPoint p2 );//рисует линию, можно подавать МЕГА длинные линии оптимизированно
	void	DrawImage( QPoint pos, std::shared_ptr<QImage> src_img);//рисует src_img. Если рисуемое изображение больше, произойдет усечение, форматы изображений должны совпадать (х4)
	
private:
	
	std::shared_ptr<QImage> image_;
	QRect					image_rect_;
	int						bytes_per_line_;
	int						bytes_per_pixel_;
	uchar*					img_bits_;
	
	bool					use_alpha_blending_;
	uint					cur_color_;		//цвет установленый (если формат Format_ARGB32_Premultiplied, то отличается от original_color_ )
	uint					original_color_;//цвет поданный (использовать при смешивании цветов)

	inline uchar*			GetScanLine(int y);//функция не делает лишних проверок ибо надо чтобы вызывалось часто, выяснять что Y не входит - надо решать ранее
	inline void				FillXLine(int from_x, int to_x, int y);//заливает линию по оси X, использует cached_data_ для быстрого многократного рисования одной и тоже линии на разных Y координатах, если cached_data пустой то генерируется новый
	QByteArray				cached_data_;

	bool					lines_intersection(QPointF start1, QPointF end1, QPointF start2, QPointF end2, QPointF *out_intersection);
	uint					ColorBlending(uint c1_i, uint c2_i);
};


}

#endif // DIRECT_IMAGE_PAINTER_H
