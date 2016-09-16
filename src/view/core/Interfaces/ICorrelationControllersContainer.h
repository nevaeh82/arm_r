#ifndef ICORRELATIONCONTROLLERSCONTAINER_H
#define ICORRELATIONCONTROLLERSCONTAINER_H

class QString;
class ICorrelationWidget;

class ICorrelationControllersContainer
{
public:
	virtual ~ICorrelationControllersContainer(){}
	virtual int init(int count)									= 0;
	virtual ICorrelationWidget *get(int id)						= 0;
	virtual int count()											= 0;
};

#endif // ICORRELATIONCONTROLLERSCONTAINER_H
