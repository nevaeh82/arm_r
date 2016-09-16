#pragma once

#include <qwt_raster_data.h>

class RasterData: public QwtRasterData
{
public:
	RasterData()
	{
		setInterval( Qt::XAxis, QwtInterval( -3.0, 3.0 ) );
		setInterval( Qt::YAxis, QwtInterval( -3.0, 3.0 ) );
		setInterval( Qt::ZAxis, QwtInterval( 0.0, 10.0 ) );
	}


	virtual double value( double x, double y ) const
	{
		return  ( cos(x*x) + y*y);
	}
};

