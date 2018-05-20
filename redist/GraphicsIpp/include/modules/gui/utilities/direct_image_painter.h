#ifndef DIRECT_IMAGE_PAINTER_H
#define DIRECT_IMAGE_PAINTER_H

#include <QObject>
#include <QImage>
#include <QColor>
#include <memory>

namespace gui
{

//����� ������� ��� ��������� �� QImage, ������������ ���������� � ��� ������ ���� ����� ����������� Image (�.�. ��� ������������)
//����������� ���������� ������������������ ��������� � 4 ����, ������� ����� ��������� 10 �������� ���������.
class DirectImagePainter : public QObject
{
	Q_OBJECT

public:
	DirectImagePainter( std::shared_ptr<QImage> paint_dst);//������ ������ ���� 32 ���� �� �������
	~DirectImagePainter();

	void	SetColor(QColor col);
	void	Fill(QColor col);//�������� ���� Image �������� ������ (���������� ������ ������������)
	void	SetPixel(QPoint pixel);//��������� (�� �� ��������� ��� QPainter;) ) ����� �� �� ��������, ���������� ������������ ������ �� ������
	void	FillRect(QRect rect);
	void	FillRect(QRectF rect);
	void	FillRect(QRect rect, QColor col);
	void	SetEnableAlphaBlending(bool enable);//����� ���������� ������ (��-��������� ����.). ��������� ������ � ��� ������ ���� ���� � ��� �� Image ������������. ��� ��������� ���� ����� ��������� ����� �������� ���������� QPainter (�� ���, �� ������) (���� ������ ���� �� �������������)
	bool	DrawLine( QPoint p1,QPoint p2 );//������ �����, ����� �������� ���� ������� ����� ���������������
	void	DrawImage( QPoint pos, std::shared_ptr<QImage> src_img);//������ src_img. ���� �������� ����������� ������, ���������� ��������, ������� ����������� ������ ��������� (�4)
	
private:
	
	std::shared_ptr<QImage> image_;
	QRect					image_rect_;
	int						bytes_per_line_;
	int						bytes_per_pixel_;
	uchar*					img_bits_;
	
	bool					use_alpha_blending_;
	uint					cur_color_;		//���� ������������ (���� ������ Format_ARGB32_Premultiplied, �� ���������� �� original_color_ )
	uint					original_color_;//���� �������� (������������ ��� ���������� ������)

	inline uchar*			GetScanLine(int y);//������� �� ������ ������ �������� ��� ���� ����� ���������� �����, �������� ��� Y �� ������ - ���� ������ �����
	inline void				FillXLine(int from_x, int to_x, int y);//�������� ����� �� ��� X, ���������� cached_data_ ��� �������� ������������� ��������� ����� � ���� ����� �� ������ Y �����������, ���� cached_data ������ �� ������������ �����
	QByteArray				cached_data_;

	bool					lines_intersection(QPointF start1, QPointF end1, QPointF start2, QPointF end2, QPointF *out_intersection);
	uint					ColorBlending(uint c1_i, uint c2_i);
};


}

#endif // DIRECT_IMAGE_PAINTER_H
