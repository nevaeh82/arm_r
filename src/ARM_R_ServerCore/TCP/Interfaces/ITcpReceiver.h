#ifndef ITCPLISTENER_H
#define ITCPLISTENER_H

class QVariant;

class ITcpReceiver
{

public:
	virtual ~ITcpReceiver() {}
	virtual void onDataReceived(const QVariant& argument)	= 0;
};

#endif // ITCPLISTENER_H
