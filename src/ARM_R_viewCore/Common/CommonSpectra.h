#ifndef COMMONSPECTRA_H
#define COMMONSPECTRA_H

#include <QString>
#include <QMap>

#include "../Abstracts/ICommonComponents.h"

#include "../GraphicWidget.h"

class CommonSpectra: public ICommonComponents
{
public:
    CommonSpectra();
    ~CommonSpectra();

public:
    virtual int init(QString path_to_ini_file);
    virtual int init(int count);
    virtual int set(int id, IGraphicWidget *widget);
    virtual IGraphicWidget* get(int id);
    virtual int count(int id);
    virtual QMap<int, IGraphicWidget* >* get_components();

private:
    QMap<int, IGraphicWidget* >*      _map_spectrum_widgets;
};

#endif // COMMONSPECTRA_H
