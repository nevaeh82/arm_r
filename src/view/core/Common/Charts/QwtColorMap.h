#include<qwt_color_map.h>

class ColorMap: public QwtLinearColorMap 
{ 
	public: 
	ColorMap(): QwtLinearColorMap(Qt::black, Qt::green){}
};
