QT += core network
TARGET   = ARM_R_ServerCore
TEMPLATE = lib
ProjectDir = $$PWD

# or EXPORT class need define.
CONFIG += staticlib

include(../../specs/project.pri)
include(../ARM_R_Common/reference.pri)
include(../../specs/qxt.pri)
include(../../specs/Solver.pri)
include(../../specs/protobuf.pri)
include(../../specs/logger.pri)

INCLUDEPATH += $$PWD

SOURCES += \
	RPC/RPCServer.cpp \
	Common/Subscriber.cpp \
	TCP/TCPController.cpp \
	TCP/TCPClient.cpp \
	Common/CRCs.cpp \
	TCP/ParserFlakon.cpp \
	TCP/ParserPRM300.cpp \
	Common/Router.cpp \
	ARM_R_Srv.cpp \
	RPC/RPCClientFlakon.cpp \
	RPC/Message.cpp \
	ARM_R_Application.cpp \
	Atlant/EMS/storm.pb.cc \
	Atlant/AtlantController.cpp \
	Flakon/FlakonCoordinateCounter.cpp \
	Flakon/StationProperty.cpp \
	RPC/RPCClient_OD.cpp \
	TCP/ParserAtlant.cpp \
	Atlant/EMS/proto/EagleMessageProto.pb.cc \
	Flakon/Correction/ZDR.cpp \
	Flakon/Correction/ZCorrector.cpp \
	Flakon/Correction/ZCoord.cpp \
	Flakon/Server/PServer.cpp \
	IBDB/DBIBController.cpp \
    TCP/BaseClasses/BaseTcpClient.cpp \
    TCP/BaseClasses/BaseTcpDeviceController.cpp \
    TCP/BaseClasses/BaseTcpDeviceCoder.cpp \
    TCP/TcpFlakonController.cpp \
    TCP/TcpFlakonCoder.cpp \
    TCP/TcpManager.cpp \
    TCP/TcpPRM300Controller.cpp \
    TCP/TcpPRM300Coder.cpp \
    TCP/TcpAtlantController.cpp \
    TCP/TcpAtlantCoder.cpp \
    Flakon/CoordinateCounter.cpp

HEADERS += \
	RPC/RPCServer.h \
	RPC/IRPC.h \
	Common/Subscriber.h \
	Common/CRCs.h \
	TCP/TCPPacketStruct.h \
	TCP/TCPController.h \
	TCP/TCPClient.h \
	TCP/ITCPController.h \
	TCP/ITCPParser.h \
	TCP/ITCPClient.h \
	TCP/ParserFlakon.h \
	TCP/ParserPRM300.h \
	Common/Router.h \
	Common/IRouter.h \
	Common/IQueue.h \
	ARM_R_Srv.h \
	Common/IClient.h \
	RPC/RPCClientFlakon.h \
	Common/IMessage.h \
	Common/ISubscriber.h \
	RPC/Message.h \
	ARM_R_Application.h \
	Atlant/EMS/storm.pb.h \
	Atlant/AtlantController.h \
	Flakon/FlakonCoordinateCounter.h \
	Flakon/StationProperty.h \
	RPC/RPCClient_OD.h \
	TCP/ParserAtlant.h \
	Atlant/EMS/proto/EagleMessageProto.pb.h \
	Flakon/Correction/ZDR.h \
	Flakon/Correction/ZCorrector.h \
	Flakon/Correction/ZCoord.h \
	Flakon/Server/PServer.h \
	IBDB/nodb_exports.h \
	IBDB/DBIBController.h \
    TCP/BaseClasses/BaseTcpClient.h \
    TCP/Interfaces/ITcpClient.h \
    TCP/Interfaces/ITcpDeviceCoder.h \
    TCP/Interfaces/ITcpDeviceController.h \
    TCP/Interfaces/ITcpReceiver.h \
    TCP/BaseClasses/BaseTcpDeviceController.h \
    TCP/BaseClasses/BaseTcpDeviceCoder.h \
    TCP/TcpFlakonController.h \
    TCP/TcpFlakonCoder.h \
    TCP/Interfaces/ITcpManager.h \
    TCP/TcpManager.h \
    TCP/TcpPRM300Controller.h \
    TCP/TcpPRM300Coder.h \
    TCP/TcpAtlantController.h \
    TCP/TcpAtlantCoder.h \
    TCP/TcpDefines.h \
    TCP/Interfaces/ITcpListener.h \
    Flakon/CoordinateCounter.h \
    Flakon/Interfaces/ICoordinateCounter.h
