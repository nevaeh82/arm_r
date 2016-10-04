#pragma once
#include "qwt_global.h"
#include "qwt_plot_item.h"

class QWT_EXPORT QwtPixmapItem: public QwtPlotItem
{
public:
	explicit QwtPixmapItem( const QString& title = QString::null );
	virtual ~QwtPlotSvgItem();

	virtual QRectF boundingRect() const;

	virtual void draw( QPainter *p,
		const QwtScaleMap &xMap, const QwtScaleMap &yMap,
		const QRectF &rect ) const;

	virtual int rtti() const;

protected:
	const QSvgRenderer &renderer() const;
	QSvgRenderer &renderer();

	void render( QPainter *painter,
		const QRectF &viewBox, const QRectF &rect ) const;

	QRectF viewBox( const QRectF &area ) const;

private:
	void init();

	class PrivateData;
	PrivateData *d_data;
};

#endif
