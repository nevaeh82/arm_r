#ifndef TCPDEVICESDEFINES_H
#define TCPDEVICESDEFINES_H

#define	BASE_TCP_DEVICE "baseTcpDevice"

//#define FLAKON_TCP_DEVICE "flakonTcpDevice"
//#define ATLANT_TCP_DEVICE "atlantTcpDevice"
//#define PRM300_TCP_DEVICE "prm300TcpDevice"
//#define ARMR_TCP_SERVER "armrTcpServer"

enum DeviceTypesEnum
{
	RETRRANSLATOR_TCP_DEVICE = -1,
	FLAKON_TCP_DEVICE = 1,
	PRM300_TCP_DEVICE = 2,
	ATLANT_TCP_DEVICE = 3,
	ARMR_TCP_SERVER = 4
};

#define FLAKON_COORDINATE_COUNTER "flakonCoordinateCounter"

#endif // TCPDEVICESDEFINES_H
