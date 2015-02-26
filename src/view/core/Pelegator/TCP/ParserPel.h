#ifndef PARSERPEL_H
#define PARSERPEL_H

#include <QObject>
#include "../proto/StormProto.pb.h"
#include "../proto/StormProto.pb.cc"

class ParserPel
{
public:
    ParserPel();
    ~ParserPel();

public:
    bool ser_DirectionFindingRequest();
    bool ser_DFStopRequest(int id);
    bool ser_ScanRequest();
    bool ser_ScanStopRequest();
    bool ser_DirectionAnswerMessage();
    bool ser_PositionAnswerMessage();
    bool ser_StatusMessage();

};

#endif // PARSERPEL_H
