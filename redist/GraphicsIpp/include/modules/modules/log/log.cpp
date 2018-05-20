#include "../../stdafx.h"
#include "../../modules/log/log.h"

namespace logging
{

win32::TLSValueTemplate<std::stack<std::pair<std::wstring, std::wstring> > > Scope::instance_;
Logger<> logging::Logger<>::instance_;


}
