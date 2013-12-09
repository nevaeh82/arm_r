#ifndef RADIAL_PROGRESS_BAR_H
#define RADIAL_PROGRESS_BAR_H

#include <QWidget>

namespace gui
{

class RadialProgressBar : public QWidget
{
	Q_OBJECT

public:
	RadialProgressBar(QWidget *parent= nullptr);
	~RadialProgressBar();
			
	unsigned int value() const;
	unsigned int maximum() const;
	
	//рендеры
	QPixmap	RenderToPixmap(QSize pixmap_size);
	QIcon	RenderToIcon(QSize icon_size);


	//Цвета и внешний вид
	void	SetGeometryParams(qreal param_1,qreal param_2,qreal param_3,qreal param_4);//от 0 до 1, лучше смотреть на практике (по умолчанию 0.04 0.15 0.07 0.07)
	void	SetFillColors(QColor center,QColor edge);
	void	SetBackgroundColors(QColor center,QColor edge);
	void	SetBorderColor(QColor border_color);
		
	void	GetFillColors(QColor& center, QColor& edge) const;
	void	GetBackgroundColors(QColor& center, QColor& edge) const;
	QColor	GetBorderColor() const;
	void	SetDrawBorder(bool draw_border);

public slots:
	//основные
	void	IncrementProgressBy(unsigned int inc_value);
	void	SetValue(unsigned int value);
	void	SetMaximum(unsigned int maximum);

signals:
	void	MaximumReached();//когда значение прогресса достигает максимума


private:
	void	paintEvent ( QPaintEvent * event );

	QPixmap Render(QSize sz);

	qreal geom_param_1_;
	qreal geom_param_2_;
	qreal geom_param_3_;
	qreal geom_param_4_;

	QColor fill_center_;
	QColor fill_edge_;
	QColor back_center_;
	QColor back_edge_;
	QColor border_color_;

	unsigned int value_;
	unsigned int maximum_;
	bool draw_border_;

};

}//namespace gui

#endif // RADIAL_PROGRESS_BAR_H
