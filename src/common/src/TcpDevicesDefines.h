#ifndef TCPDEVICESDEFINES_H
#define TCPDEVICESDEFINES_H

#define	BASE_TCP_DEVICE "baseTcpDevice"

//#define FLAKON_TCP_DEVICE "flakonTcpDevice"
//#define PRM300_TCP_DEVICE "prm300TcpDevice"
//#define ARMR_TCP_SERVER "armrTcpServer"

enum DeviceTypesEnum
{
	RETRRANSLATOR_TCP_DEVICE = -1,
	FLAKON_TCP_DEVICE = 1,
	PRM300_TCP_DEVICE = 2,
	ARMR_TCP_SERVER = 4,
	CLIENT_TCP_SERVER = 5,
	RDS_TCP_DEVICE = 6,
	SOLVER_CLIENT_DEVICE = 7
};

#define FLAKON_COORDINATE_COUNTER "flakonCoordinateCounter"
#define RDS_NAME "RDS"

#define FLAKON_ROUTE_ID		201
#define RDS_ROUTE_ID		203
#define SOLVER_ROUTE_ID		204

#endif // TCPDEVICESDEFINES_H
