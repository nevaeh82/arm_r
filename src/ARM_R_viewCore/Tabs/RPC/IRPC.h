#ifndef IRPC_H
#define IRPC_H

class IRPC
{
public:
	virtual ~IRPC(){}

	virtual int slotStart()     = 0;
	virtual int slotStop()      = 0;

};

#endif // IRPC_H
