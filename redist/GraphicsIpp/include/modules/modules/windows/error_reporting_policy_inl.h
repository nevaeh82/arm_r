#ifndef MODULES_WINDOWS_ERROR_REPORTING_POLICY_H_
#define MODULES_WINDOWS_ERROR_REPORTING_POLICY_H_

namespace win32 {



class SilentErrorReportingPolicy
{
public:
    void OnError(const tstring&) const {}
};



#ifdef MODULES_CORE_STRINGS_INL_H_

class ExceptionErrorReportingPolicy
{
public:
    void OnError(const tstring& text) const
    {
        throw tstring(text);
    }
};

#endif // MODULES_CORE_STRINGS_INL_H_



#ifdef _INC_WINDOWS

class ODSErrorReportingPolicy
{
public:
    void OnError(const tstring& text) const
    {
        OutputDebugString((text + _T("\n")).c_str());
    }
};

#endif // _INC_WINDOWS



}  //  namespace win32

#endif