#ifndef MODULES_WINDOWS_TOOLS_H_
#define MODULES_WINDOWS_TOOLS_H_ 1

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <stdint.h>                     //  для int64_t
#include "../../modules/core/preprocessor.h"  //  DISALLOW_COPY_AND_ASSIGN
#include "../../modules/core/templates_inl.h" //  ProcessDataByBlocks
#include "../../modules/core/strings_inl.h"
#include "../../modules/windows/error_reporting_policy_inl.h"

namespace win32
{

UINT            GetMSIInstallerVersion(tstring& version, tstring& language);
void            SetThreadName(const char* thread_name, DWORD thread_id = -1);
std::wstring    WStringFromLocal8bit(const std::string& str);
std::wstring    WStringFromLocal8bit(const char* str);

template<class ErrorReporting = ODSErrorReportingPolicy>
class Mutex
{
public:
    Mutex()         {   InitializeCriticalSection(&section_);   }
    ~Mutex()        {   DeleteCriticalSection(&section_);       }
    void Lock()     {   EnterCriticalSection(&section_);        }
    void Unlock()   {   LeaveCriticalSection(&section_);        }

private:
    CRITICAL_SECTION    section_;

    DISALLOW_COPY_AND_ASSIGN(Mutex);
};

template<class ErrorReporting = SilentErrorReportingPolicy>
class NoLockingMutex
{
public:
    NoLockingMutex()    {}
    ~NoLockingMutex()   {}
    void Lock()         {}
    void Unlock()       {}
};

template<
            class ErrorReporting                = SilentErrorReportingPolicy,
            template<class> class MutexClass    = Mutex
        >
class FileTemplate: public ErrorReporting
{
public:
    typedef ScopedLock<Mutex<ErrorReporting> >  Locker;
    typedef MutexClass<ErrorReporting>          MutexType;

    FileTemplate(const tstring& file_name):
        is_valid_(false),
        handle_(INVALID_HANDLE_VALUE),
        read_only_(false),
        file_name_(file_name)
    {
    }

    ~FileTemplate()
    {
        Locker locker(mutex_);
        if(is_valid())
            Close();
    }

    int64_t size() const
    {
        Locker locker(mutex_);
        if(!Validate(_T("File must be opened")))
            return 0;

        LARGE_INTEGER Size = {0};
        GetFileSizeEx(handle_, &Size);
        return (LONGLONG)Size.QuadPart;
    }

    bool set_position(int64_t new_position)
    {
        Locker locker(mutex_);
        if(new_position < 0)
        {
            OnError(_T("New position must be positive"));
            return false;
        }

        if(!Validate(_T("File must be opened")))
            return false;

        if(read_only())
        {
            if(new_position > size())
            {
                OnError(_T("Wrong new position"));
                return false;
            }
        }

        LARGE_INTEGER win_new_position;
        win_new_position.QuadPart = (LONGLONG)new_position;
        BOOL result = SetFilePointerEx(handle_, win_new_position, NULL, FILE_BEGIN);
        return SUCCEEDED(result);
    }

    int64_t position() const
    {
        Locker locker(mutex_);
        if(!Validate(_T("File must be opened")))
            return 0;

        LARGE_INTEGER position = { 0 };
        LARGE_INTEGER new_position = { 0 };
        BOOL result = SetFilePointerEx(handle_, new_position, &position, FILE_CURRENT);
        if(SUCCEEDED(result))
        {
            return (int64_t)position.QuadPart;
        }
        else
            return 0;
    }

    bool Open(bool read_only_flag = true, bool share_read = false, bool share_write = false, bool truncate = true)
    {
        Locker locker(mutex_);
        if (is_valid())
        {
            OnError(_T("File must be closed"));
            return false;
        }

        DWORD creation_disposition;
        //если только для чтения то открываем исключительно существующий
        if (read_only_flag) creation_disposition = OPEN_EXISTING;
        else
        {
            //если может быть записан
            if (truncate) creation_disposition = CREATE_ALWAYS;//всегда создаем новый
            else creation_disposition = OPEN_ALWAYS;//открываем существующий или создаем новый, если не существует
        }
                
        HANDLE handle = CreateFile(file_name_.c_str(), read_only_flag ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE, (share_read ? FILE_SHARE_READ : 0) + (share_write ? FILE_SHARE_WRITE : 0), NULL, creation_disposition , FILE_ATTRIBUTE_NORMAL, 0);

        if(handle == INVALID_HANDLE_VALUE)
        {
            OnError(_T("Can't open file ") + file_name_);
            return false;
        }
        else
        {
            handle_     = handle;
            is_valid_   = true;
            read_only_  = read_only_flag;
            return true;
        }
    }

    bool Close()
    {
        Locker locker(mutex_);
        if(!Validate(_T("File must be opened")))
            return false;

        CloseHandle(handle_);
        handle_ = INVALID_HANDLE_VALUE;
        is_valid_ = false;
        return true;
    }

    int64_t Read(void* data, int64_t length)
    {
        Locker locker(mutex_);
        if(!Validate(_T("File must be opened")))
            return 0;

        if(length > int64_t(((size_t)-1) >> 1))
        {
            OnError(_T("Wrong data length"));
            return 0;
        }

        if(data == NULL || length == 0)
        {
            OnError(_T("Invalid parameters"));
            return 0;
        }

        int64_t read = 0;
        ProcessDataByBlocks(static_cast<unsigned char*>(data), length, (int64_t)INT_MAX, [&](unsigned char* block_data, int64_t block_len, bool) -> bool
        {
            DWORD IO;
            BOOL result = ReadFile(handle_, block_data, (DWORD)block_len, &IO, NULL);
            read += IO;
            return SUCCEEDED(result);
        });
        return read;
    }

    int64_t ReadFromPosition(int64_t start_position, void* data, int64_t length, int64_t* new_position = NULL)
    {
        Locker locker(mutex_);
        if(set_position(start_position))
        {
            auto read_result= Read(data, length);
            if(new_position)
                *new_position = position();
            return read_result;
        }
        return 0;
    }

    int64_t Write(const void* data, int64_t length)
    {
        Locker locker(mutex_);
        if(!Validate(_T("File must be opened")))
            return 0;

        if(length > int64_t(((size_t)-1) >> 1))
        {
            OnError(_T("Wrong data length"));
            return 0;
        }

        if(data == NULL || length == 0)
        {
            OnError(_T("Invalid parameters"));
            return 0;
        }

        if (read_only())
        {
            OnError(_T("Can't write to read only file"));
            return 0;
        }

        int64_t Written = 0;
        ProcessDataByBlocks(static_cast<const unsigned char*>(data), length, (int64_t)INT_MAX, [&](const unsigned char* block_data, int64_t block_len, bool) -> bool
        {
            DWORD IO;
            BOOL result = WriteFile(handle_, block_data, (DWORD)block_len, &IO, NULL);
            Written += IO;
            return SUCCEEDED(result);
        });
        return Written;
    }

    int64_t Append(const void* data, int64_t length)//файловая позиция остается на старом месте, а не перемещается в конец файла
    {
        Locker locker(mutex_);
        int64_t old_pos = position();
        if(set_position(size()))
        {
            auto write_result = Write(data, length);
            set_position(old_pos);
            return write_result;
        }
        return 0;
    }

    operator bool()             { return is_valid();                        }
    bool is_valid() const       { Locker locker(mutex_); return is_valid_;  }
    bool read_only() const      { Locker locker(mutex_); return read_only_; }
    tstring file_name() const   { Locker locker(mutex_); return file_name_; }

private:
    bool                is_valid_;
    HANDLE              handle_;
    mutable MutexType   mutex_;     //  mutex'ы можно лочить в константных функциях, вообще, mutex здесь 
    bool                read_only_;
    tstring             file_name_;

    bool Validate(const TCHAR* error_string) const
    {
        if(!is_valid())
        {
            OnError(error_string);
            return false;
        }
        return true;
    }

    DISALLOW_COPY_AND_ASSIGN(FileTemplate);
};

typedef FileTemplate<>  File;

template<
    typename T,
    class ErrorReporting = ExceptionErrorReportingPolicy
>
class TLSValueTemplate: public ErrorReporting
{
public:
    TLSValueTemplate();
    TLSValueTemplate( const T& initial_value );

    ~TLSValueTemplate();

    operator T* ()  const;
    void set( T* value);
    T* operator->() const;
    operator bool() const;
    void reset();

private:

    DWORD   index_;
    DISALLOW_COPY_AND_ASSIGN(TLSValueTemplate);

    bool IsIndexValidNoFail() const
    {
        return index_ < 1088;
    }
};

template<typename T, class ErrorReporting>
TLSValueTemplate<T, ErrorReporting>::TLSValueTemplate(): index_(TLS_OUT_OF_INDEXES)
{
    auto temp   = TlsAlloc();
    if(temp == TLS_OUT_OF_INDEXES)
        OnError(_T("Can't allocate TLS"));

    index_      = temp;
}

template<typename T, class ErrorReporting>
TLSValueTemplate<T, ErrorReporting>::TLSValueTemplate( const T& initial_value ): index_(TLS_OUT_OF_INDEXES)
{
    auto temp   = TlsAlloc();
    if(temp == TLS_OUT_OF_INDEXES)
        OnError(_T("Can't allocate TLS"));

    index_      = temp;

    operator=(initial_value);
}

template<typename T, class ErrorReporting>
TLSValueTemplate<T, ErrorReporting>::~TLSValueTemplate()
{
    if(IsIndexValidNoFail())
        TlsFree(index_);

    index_ = TLS_OUT_OF_INDEXES;
}

template<typename T, class ErrorReporting>
TLSValueTemplate<T, ErrorReporting>::operator T*() const
{
    if(!IsIndexValidNoFail())
    {
        OnError(_T("Reading value from wrong TLS"));
        return nullptr;
    }
    else
    {
        T* value = reinterpret_cast<T*>( TlsGetValue(index_) );
        if(value == nullptr)
            OnError(_T("TLS value is nullptr"));
        return value;
    }
}

template<typename T, class ErrorReporting>
TLSValueTemplate<T, ErrorReporting>::operator bool() const
{
    T* value = reinterpret_cast<T*>( TlsGetValue(index_) );
    return value != nullptr;
}

template<typename T, class ErrorReporting>
void TLSValueTemplate<T, ErrorReporting>::reset()
{
    if(!IsIndexValidNoFail())
    {
        OnError(_T("Writing value from wrong TLS"));
        return;
    }
    else
    {
        BOOL result = TlsSetValue(index_, nullptr);
        if(result == FALSE)
            OnError(_T("Failed to set TLS value"));
    }
}

template<typename T, class ErrorReporting>
void TLSValueTemplate<T, ErrorReporting>::set( T* value )
{
    if(!IsIndexValidNoFail())
    {
        OnError(_T("Writing value from wrong TLS"));
        return;
    }
    else
    {
        if(value == nullptr)
            OnError(_T("Setting nullptr to TLS value"));

        BOOL result = TlsSetValue(index_, value);
        if(result == FALSE)
            OnError(_T("Failed to set TLS value"));
    }
}

template<typename T, class ErrorReporting>
T* TLSValueTemplate<T, ErrorReporting>::operator->() const
{
    return operator T*();
}

inline tstring GetLastErrorMessage()
{
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL);

    tstring result = (LPTSTR)lpMsgBuf;
    LocalFree(lpMsgBuf);
    return result;
}

inline tstring GetCurrentDirectory()
{
    TCHAR buffer[MAX_PATH] = {0};

    DWORD call_result = ::GetCurrentDirectory(MAX_PATH, buffer);
    if(call_result == 0)
        return tstring();
    if(call_result > sizeof(buffer)/sizeof(buffer[0]))
        return tstring();

    return buffer;
}

inline bool SetCurrentDirectory(const tstring& directory)
{
    return ::SetCurrentDirectory(directory.c_str()) == 1;
}

}

#ifdef UNICODE
    #define DebugString  DebugStringW
#else
    #define DebugString  DebugStringA
#endif // !UNICODE

void    inline  DebugStringA ( char *format, ... )
{
	va_list     argptr;

	va_start( argptr, format );

	size_t	msg_size    = 4096;
	char*   msg         = new char[ msg_size ];
			msg[0]		= 0;

	vsprintf_s( msg, msg_size, format, argptr );

	OutputDebugStringA( msg );

	delete[]    msg;

	va_end( argptr );
}

void    inline  DebugStringW ( wchar_t *format, ... )
{
	va_list     argptr;

	va_start( argptr, format );

	size_t	msg_size    = 4096;
	wchar_t*  msg       = new wchar_t[ msg_size ];
			msg[0]		= 0;

	vswprintf_s( msg, msg_size, format, argptr );

	OutputDebugStringW( msg );

	delete[]    msg;

	va_end( argptr );
}

#endif
