#include "stdafx.h"

#include <shlobj.h>
#include <shlwapi.h>
#include <Msi.h>

#include "windows_tools.h"

#pragma comment(lib, "Msi.lib")         //  MsiGetFileVersion
#pragma comment(lib, "shlwapi.lib")     //  PathAppend

namespace win32
{

UINT GetMSIInstallerVersion(std::wstring& version, std::wstring& language)
{
    TCHAR szPath[MAX_PATH];

    if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, 0, szPath))) 
    {
        PathAppend(szPath, TEXT("MSI.DLL"));

        TCHAR VersionBuffer[MAX_PATH] = {0};
        TCHAR LangBuffer[MAX_PATH] = {0};

        DWORD VersionBufferSize = sizeof(VersionBuffer)/sizeof(VersionBuffer[0])-1; //Хотя бы 1 ноль останется в конце
        DWORD LangBufferSize = sizeof(LangBuffer)/sizeof(LangBuffer[0])-1;

        UINT result;
        if(SUCCEEDED(result = MsiGetFileVersion(szPath, VersionBuffer, &VersionBufferSize, LangBuffer, &LangBufferSize)))
        {
            version = VersionBuffer;
            language = LangBuffer;
        }
        return result;
    }
    return ERROR_INVALID_DATA;
}

//  http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
#define MS_VC_EXCEPTION 0x406d1388

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType;        // must be 0x1000
    LPCSTR szName;       // pointer to name (in same addr space)
    DWORD dwThreadID;    // thread ID (-1 caller thread)
    DWORD dwFlags;       // reserved for future use, most be zero
} THREADNAME_INFO;
#pragma pack(pop)

void SetThreadName( const char* thread_name, DWORD thread_id /*= -1*/ )
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = thread_name;
    info.dwThreadID = thread_id;
    info.dwFlags = 0;

    __try
    {
        RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
    }
}

std::wstring WStringFromLocal8bit( const std::string& str )
{
    const auto len = str.size();
    WCHAR* unicode_str = (WCHAR*)_aligned_malloc((len+1)*2, 64);

    MultiByteToWideChar(CP_ACP, 0, str.c_str(), int(len+1), unicode_str, int(len+1));

    std::wstring result(unicode_str);

    _aligned_free(unicode_str);

    return result;    
}

std::wstring WStringFromLocal8bit( const char* str )
{
    const auto len = strlen(str);
    WCHAR* unicode_str = (WCHAR*)_aligned_malloc((len+1)*2, 64);

    MultiByteToWideChar(CP_ACP, 0, str, int(len+1), unicode_str, int(len+1));

    std::wstring result(unicode_str);

    _aligned_free(unicode_str);

    return result;
}

}  //  win32
