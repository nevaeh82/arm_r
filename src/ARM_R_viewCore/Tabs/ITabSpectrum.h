#ifndef ITABSPECTRUM_H
#define ITABSPECTRUM_H

#include <QVector>
#include <QMap>
#include <QVariant>
#include <QLabel>

#include "Common/IMessage.h"

enum TypeCommand
{
	unknown = -1,
	graphic = 1,
	panorama = 2
};

class ITabSpectrum
{
public:
	virtual ~ITabSpectrum(){}

    virtual void set_selected_area(QMap<int, QVariant> data)        = 0; //1 - selection, 2 - central freq, 3 - begin freq, 4 - end freq

	virtual void set_command(TypeCommand type, IMessage *msg)       = 0;

    virtual void set_points_rpc(QVector<QPointF> points)            = 0;
    virtual void set_thershold(double y)                            = 0;

    virtual void set_indicator(int state)                           = 0;
	//virtual QLabel* get_indicator()                                 = 0;

    virtual void set_show_controlPRM(bool state)                    = 0;

    virtual void set_double_clicked(int id, double, double)         = 0;

    virtual void check_status()                                     = 0;

	virtual void set_panorama(bool state)							= 0;

	virtual double get_current_frequency()							= 0;
};

#endif // ITABSPECTRUM_H
