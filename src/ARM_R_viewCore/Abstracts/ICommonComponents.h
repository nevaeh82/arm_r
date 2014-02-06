#ifndef ICOMMONCOMPONENTS_H
#define ICOMMONCOMPONENTS_H

class QString;
#include <QWidget>
class IGraphicWidget;

class ICommonComponents : public QWidget
{
public:
	virtual ~ICommonComponents(){}
    virtual int init(QString path_to_ini_file)              = 0;
    virtual int init(int count)                             = 0;
    virtual int set(int id, IGraphicWidget* widget)         = 0;
    virtual IGraphicWidget *get(int id)                     = 0;
    virtual int count(int id)                               = 0;
    virtual QMap<int, IGraphicWidget* >* get_components()   = 0;
};

#endif // ICOMMONCOMPONENTS_H
