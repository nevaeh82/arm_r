#ifndef ICOMMONCOMPONENTS_H
#define ICOMMONCOMPONENTS_H

#include <QMap>
class QString;
class ICorrelationWidget;

class ICommonComponents
{
public:
	virtual ~ICommonComponents(){}
	//virtual int init(QString path_to_ini_file)					= 0;
	virtual int init(int count)									= 0;
	//virtual int set(int id, ICorrelationWidget* widget)			= 0;
	virtual ICorrelationWidget *get(int id)						= 0;
	virtual int count()									= 0;
	//virtual QMap<int, ICorrelationWidget* >* get_components()   = 0;
};

#endif // ICOMMONCOMPONENTS_H
