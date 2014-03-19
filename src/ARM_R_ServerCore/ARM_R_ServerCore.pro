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
include($$SolutionDir/specs/CISCommonLib.pri )
include(../../specs/logger.pri)

INCLUDEPATH += $$PWD

SOURCES += \
    RPC/RPCServer.cpp \
    ARM_R_Srv.cpp \
    ARM_R_Application.cpp \
    Atlant/EMS/storm.pb.cc \
    Atlant/EMS/proto/EagleMessageProto.pb.cc \
    Flakon/Correction/ZDR.cpp \
    Flakon/Correction/ZCorrector.cpp \
    Flakon/Correction/ZCoord.cpp \
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
    Flakon/CoordinateCounter.cpp \
    TCP/TcpSettingsManager.cpp \
    TCP/Server/TcpServer.cpp

HEADERS += \
    RPC/RPCServer.h \
    RPC/IRPC.h \
    ARM_R_Srv.h \
    ARM_R_Application.h \
    Atlant/EMS/storm.pb.h \
    Atlant/EMS/proto/EagleMessageProto.pb.h \
    Flakon/Correction/ZDR.h \
    Flakon/Correction/ZCorrector.h \
    Flakon/Correction/ZCoord.h \
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
    Flakon/Interfaces/ICoordinateCounter.h \
    TCP/TcpSettingsManager.h \
    TCP/Interfaces/ITcpSettingsManager.h \
    MessageSP.h \
    TCP/Server/TcpServer.h
