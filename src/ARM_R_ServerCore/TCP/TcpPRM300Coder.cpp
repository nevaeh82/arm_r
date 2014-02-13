#include "TcpPRM300Coder.h"

TcpPRM300Coder::TcpPRM300Coder(QObject* parent) :
	BaseTcpDeviceCoder(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpPRM300Coder)), parent)
{
}

TcpPRM300Coder::~TcpPRM300Coder()
{
}
