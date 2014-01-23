#ifndef ITCPPARSER_H
#define ITCPPARSER_H

#include <QByteArray>

class ITCPParser
{
public:
    virtual ~ITCPParser(){};

    virtual void parsing(QByteArray data)       = 0;
    virtual void distruct()                     = 0;
};

#endif // ITCPPARSER_H
