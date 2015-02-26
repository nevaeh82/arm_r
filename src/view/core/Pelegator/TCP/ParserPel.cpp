#include "ParserPel.h"

ParserPel::ParserPel()
{
}

ParserPel::~ParserPel()
{
}

/* serialization  */

bool ParserPel::ser_DFStopRequest(int id)
{
    Storm::DFStopRequest ssr;
    ssr.set_id(id);
    QByteArray ba;
    return true;
}
