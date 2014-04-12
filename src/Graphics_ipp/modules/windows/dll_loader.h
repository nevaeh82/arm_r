#ifndef MODULES_WINDOWS_DLL_LOADER_H_
#define MODULES_WINDOWS_DLL_LOADER_H_

#include "modules/core/preprocessor.h"  //  DISALLOW_COPY_AND_ASSIGN
#include "modules/core/strings_inl.h"   //  tstring
#include "modules/core/strings.h"       //  widen
#include <windows.h>
#include <vector>

namespace win32
{

#pragma warning(disable:4702)   //  unreachable code - действительно так, если OnError генерирует исключение, но OnError определяется в стратегии ErrorReporting

template<class UserFunctionsStructure, class InternalFunctionsStructure>
class InternalToUserConverterPolicy
{
public:
    UserFunctionsStructure operator()(const InternalFunctionsStructure& src)
    {
        return src;
    }
};

template<
        class UserFunctionsStructure,
        class InternalFunctionsStructure = UserFunctionsStructure,
        class ErrorReporting = SilentErrorReportingPolicy,
        template<class, class> class ConverterPolicy = InternalToUserConverterPolicy
        >
class LibraryLoader: public ErrorReporting
{
public:
    typedef HMODULE HandleType;

    LibraryLoader(const tstring& dll_name, const tstring& path, const std::string& function_names, bool unload_on_exit = true):
        external_functions_     (),
        internal_functions_     (),
        handle_                 (),
        dll_name_               (dll_name),
        path_                   (path),
        unload_on_exit_         (unload_on_exit),
        function_names_         (function_names),
        pointers_start_offset_  (0)
    {}

    bool Load()
    {
        return InternalLoad<false>();
    }

    bool LoadOrFail()
    {
        return InternalLoad<true>();
    }

    bool Unload()
    {
        if(handle_ != HandleType())
            if(FreeLibrary(handle_))
            {
                handle_ = HandleType();
                return true;
            }
        return false;
    }

    bool UnloadOrFail()
    {
        if(handle_ != HandleType())
        {
            BOOL result = FreeLibrary(handle_);
            if(result == 0)
                OnError(_T("Can't unload library ") + dll_name_ + _T(" : ") + GetLastErrorMessage());
            handle_ = HandleType();
            return true;
        }
        else
        {
            OnError(_T("Library isn't loaded ") + dll_name_);
            return false;
        }
    }

    ~LibraryLoader()
    {
        if(unload_on_exit_)
        {
            if(handle_)
                UnloadOrFail();
        }
    }

    virtual bool        IsValid() const     { return handle_ != HandleType(); }
    const tstring&      dll_name() const    { return dll_name_; }
    const tstring&      path() const        { return path_; }

    const UserFunctionsStructure* operator->() const
    {
        if(!IsValid())
        {
            OnError(_T("Functions structure is not valid"));
            return NULL;
        }
        return &external_functions_;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(LibraryLoader);

    UserFunctionsStructure      external_functions_;
    InternalFunctionsStructure  internal_functions_;    
    HandleType                  handle_;
    tstring                     dll_name_;
    tstring                     path_;
    bool                        unload_on_exit_;
    std::string                 function_names_;
    std::ptrdiff_t              pointers_start_offset_;

#pragma warning(disable:4127)   //  сделано чтобы не было дублирования кода, поэтому в if-ах стоят константы
    template<const bool call_error>
    bool InternalLoad()
    {
        if(handle_ != HandleType())
        {
            if(call_error)
                OnError(_T("Library must be unloaded"));
            return false;
        }
        if(dll_name_.empty())
        {
            if(call_error)
                OnError(_T("DLL name can't be empty"));
            return false;
        }

        tstring previous_directory;
        if(!path_.empty())
        {
            previous_directory = GetCurrentDirectory();
            if(!SetCurrentDirectory(path_))
            {
                if(call_error)
                    OnError(_T("Can't set current directory to ") + path());
                return false;
            }
        }

        auto restore_saved_directory = [&]() -> bool
        {
            if(!path_.empty())
            {
                if(!SetCurrentDirectory(previous_directory))
                    return false;
                return true;
            }
            else
                return true;
        };

        HandleType temp_handle = LoadLibrary(path_.empty() ? dll_name_.c_str() : (path_ + _T("/") + dll_name_).c_str());
        if(temp_handle)
        {
            if(restore_saved_directory() && ResolveAllSymbols(temp_handle))
            {
                handle_ = temp_handle;
                return true;
            }
            else
            {
                FreeLibrary(temp_handle);
                if(call_error)
                    OnError(_T("Can't restore current directory to ") + previous_directory);
            }
        }
        else
        {
            if(call_error)
                OnError(_T("Can't load library ") + dll_name() + _T(" : ") + win32::GetLastErrorMessage());
        }

        restore_saved_directory();
        return false;
    }
#pragma warning(default:4127)
#pragma warning(default:4702)

    bool ResolveAllSymbols(HandleType test_handle)
    {
        if(function_names_.empty())
        {
            OnError(_T("Function names string is empty"));
            return false;
        }

        // C - way...
        InternalFunctionsStructure  temp_functions;
        std::ptrdiff_t* current_func_ptr = static_cast<std::ptrdiff_t*>(static_cast<void*>(&temp_functions)) + pointers_start_offset_;

        std::vector<std::string> names;
        strings::split(function_names_, ',', names);

        if(names.empty())
        {
            OnError(_T("Can't parse names from string '") + strings::widen(function_names_) + _T("'"));
            return false;
        }

        if(sizeof(temp_functions) < sizeof(std::ptrdiff_t)*names.size())
        {
            OnError(_T("Too many functions to resolve"));
            return false;
        }

        bool all_resolved = true;
        for(auto i = names.cbegin(); i != names.cend(); ++i)
        {
            void* pointer = GetProcAddress(test_handle, i->c_str());
            *current_func_ptr = reinterpret_cast<std::ptrdiff_t>(pointer);
            ++current_func_ptr;

            all_resolved = all_resolved & (pointer != NULL);
        }

        if(all_resolved && temp_functions.IsValid())
        {
            internal_functions_ = temp_functions;
            external_functions_ = ConverterPolicy<UserFunctionsStructure, InternalFunctionsStructure>()(internal_functions_);
            return true;
        }
        return false;
    }
};

}

#endif