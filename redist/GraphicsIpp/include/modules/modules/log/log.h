#ifndef MODULES_LOG_H
#define MODULES_LOG_H

/************************************************************************/
/*                   Модуль ведения отчетов (логов) v0.1                */
/************************************************************************/

#include <string>
#include <deque>
#include <stack>
#include <algorithm>
#include <wchar.h>
#include <memory>
#include <utility>
#include <vector>

#include "../../modules/windows/windows_tools.h"  //  Mutex, ErrorReporting, File

CHECK_MODULE_VERSION ( MODULES_WINDOWS_TOOLS_H_, 1 )

namespace logging
{

typedef win32::ExceptionErrorReportingPolicy DefaultErrorReporting;

#pragma region Scope class

//  всё лежит в одной куче, т.к. должно быть inline
class Scope
{
public:

    Scope(const std::wstring& name)
    {
        if(!instance_)
        {
            instance_.set(new std::stack<std::pair<std::wstring, std::wstring> >());
            if(instance_)
                instance_->push(std::make_pair(name, name));
        }
        else
            instance_->push(std::make_pair(name, instance_->top().second + _T(".") + name));      
    }

#if defined(QT_VERSION)
    Scope(const QString& qt_name)
    {
        const std::wstring&& name = qt_name.toStdWString();
        if(!instance_)
        {
            instance_.set(new std::stack<std::pair<std::wstring, std::wstring> >());
            if(instance_)
                instance_->push(std::make_pair(name, name));
        }
        else
            instance_->push(std::make_pair(name, instance_->top().second + _T(".") + name));      
    }
#endif

    ~Scope()
    {
        if(instance_)
        {
            instance_->pop();
            if(instance_->empty())
            {
                delete instance_;
                instance_.reset();
            }
        }
    }

    static std::wstring CurrentScope()
    {
        if(!instance_)return std::wstring();
        return instance_->top().second;
    }
    
    static bool IsEmpty()
    {
        return !instance_;
    }

private:

    //  TODO: если не понадобится первый член, заменить pair на wstring
    //  std::stack<std::pair<std::wstring, std::wstring> для каждого потока свой
    static win32::TLSValueTemplate<std::stack<std::pair<std::wstring, std::wstring> > >  instance_;
};

#pragma endregion

class CommitInterface
{
public:
    virtual void Commit( const std::wstring& str )  = 0;
#if defined(QT_VERSION)
    virtual void Commit( const QString& str )       { this->Commit(str.toStdWString()); }
#endif

    virtual ~CommitInterface() {}
};

class ODSCommiter: public CommitInterface
{
public:
    virtual void Commit( const std::wstring& str )
    {
        OutputDebugStringW(str.c_str());
    }
};

template<class ErrorReportingPolicy = DefaultErrorReporting>
class FileCommiter: public CommitInterface, public ErrorReportingPolicy
{
public:
    FileCommiter(const wchar_t* file_name, const wchar_t* file_header): file_(file_name)
    {
        if(!file_.Open(false, true, false, true))
        {
            OnError((L"Can't create log file " + std::wstring(file_name)).c_str());
        }
        else
        {
            uint_least16_t magic = 0xFEFF;
            file_.Write(&magic, 2);
            Commit((std::wstring(file_header) + L"\n"));
        }
    }

#if defined(QT_VERSION)
    FileCommiter(const QString& file_name, const QString& file_header): file_(file_name.toStdWString())
    {
        if(!file_.Open(false, true, false, true))
        {
            OnError((L"Can't create log file " + file_name.toStdWString()).c_str());
        }
        else
        {
            uint_least16_t magic = 0xFEFF;
            file_.Write(&magic, 2);
            CommitInterface::Commit((file_header + QChar('\n')));
        }
    }
#endif

    virtual void Commit( const std::wstring& str )
    {
        if(file_.is_valid())
            file_.Write(str.c_str(), str.size()*sizeof(std::wstring::value_type));
    }

private:
    win32::File     file_;
};

#pragma region Log types

enum LogType { ltGeneral = 0, ltDebug = 1, ltExtended = 2, ltTrace = 3 };

//  если никто не определил DEFAULT_LOG_LEVEL, то будет ltDebug в Debug'е
#ifndef DEFAULT_LOG_LEVEL
#ifdef _DEBUG
#define DEFAULT_LOG_LEVEL ltDebug
#else
#define DEFAULT_LOG_LEVEL ltGeneral
#endif // _DEBUG
#endif

#pragma endregion

class LoggerInterface
{
public:
    //  записать в log LogType отформатированную строку str
    virtual void Commit( const wchar_t* str, LogType )      = 0;

    //  оторвать этот logger от мастера
    virtual void DetachFromMaster()                         = 0;

    //  добавить зависимый лог в список
    virtual void AddChildToList( LoggerInterface* )         = 0;

    //  удалить зависимый лог из списка
    virtual void RemoveChildFromList( LoggerInterface* )    = 0;

    //  вернуть текущий уровень логирования
    virtual LogType         log_level()                     = 0;

    //  вернуть версию лога
    virtual unsigned int    version()                       = 0;
};

template<
    class ErrorReportingPolicy          = DefaultErrorReporting,
    template<class> class MutexClass	= win32::Mutex,
    class DefaultCommiter               = ODSCommiter
>
class Logger: public LoggerInterface, public ErrorReportingPolicy
{
public:

    void                    operator()          ( const std::wstring& str ) {   if(log_level() >= ltGeneral)    WriteToLog(str, ltGeneral);                         }
    void                    Debug               ( const std::wstring& str ) {   if(log_level() >= ltDebug)      WriteToLog(str, ltDebug);                           }
    void                    Extended            ( const std::wstring& str ) {   if(log_level() >= ltExtended)   WriteToLog(str, ltExtended);                        }
    void                    Trace               ( const std::wstring& str ) {   if(log_level() >= ltTrace)      WriteToLog(str, ltTrace);                           }

#if defined(QT_VERSION)
    void                    operator()          ( const QString& str )      {   this->operator()  (str.toStdWString());                                             }
    void                    Debug               ( const QString& str )      {   this->Debug       (str.toStdWString());                                             }
    void                    Extended            ( const QString& str )      {   this->Extended    (str.toStdWString());                                             }
    void                    Trace               ( const QString& str )      {   this->Trace       (str.toStdWString());                                             }
#endif

    bool                    AttachToMaster      ( LoggerInterface* master );
    void                    SetMaster           ( bool new_master );
    void                    set_log_level       ( LogType log_level )       { log_level_ = log_level;                                                               }

    virtual LogType         log_level()                                     { Locker locker(mutex_); return master_ptr_ ? master_ptr_->log_level() : log_level_;    }
    virtual void            DetachFromMaster();
    virtual void            AddChildToList      ( LoggerInterface* logger );
    virtual void            RemoveChildFromList ( LoggerInterface* logger );
    virtual unsigned int    version             ()                          { return 1;                                                                             }

    void                    ReplaceCommiters    ( const std::shared_ptr<CommitInterface>& new_commiter );
    void                    AddCommiter         ( const std::shared_ptr<CommitInterface>& new_commiter );

    static Logger&          instance()                                      { return instance_;                                                                     }

private:
    Logger();
    ~Logger();

    //  главный ли это лог
    bool                                        master_;

    //  сообщения ниже какого уровня не выводить
    LogType                                     log_level_;

    //  определения типов
    typedef MutexClass<ErrorReportingPolicy>    Mutex;
    typedef ScopedLock<Mutex>	                Locker;
    typedef std::pair<std::wstring, LogType>    QueueType;
    typedef std::shared_ptr<CommitInterface>    CommiterType;

    //  mutex на master_ptr_ и children_
    Mutex                                       mutex_;

    //  кому слать сообщения
    LoggerInterface*                            master_ptr_;

    //  зависимые логгеры
    std::deque<LoggerInterface*>                children_;

    //  циклический буфер локальных сообщений (пока мастер не подконнектится)
    std::deque<QueueType>                       local_temp_;
    bool                                        local_temp_overflow_;

    std::vector<CommiterType>                   commiters_;

    //  статический указатель на себя
    static Logger                               instance_;

    void WriteToLog( const std::wstring& str, LogType );

    virtual void Commit( const wchar_t* str, LogType );
};

template<class ErrorReportingPolicy, template<class>class MutexClass, class DefaultCommiter>
Logger<ErrorReportingPolicy, MutexClass, DefaultCommiter>::Logger():
    master_(false),
    log_level_(DEFAULT_LOG_LEVEL),
    master_ptr_(nullptr),
    local_temp_overflow_(false)
{
    commiters_.push_back(CommiterType(new DefaultCommiter()));
}

template<class ErrorReportingPolicy, template<class>class MutexClass, class DefaultCommiter>
Logger<ErrorReportingPolicy, MutexClass, DefaultCommiter>::~Logger()
{
    Locker locker(mutex_);

    auto old_children_list = children_; //  нельзя работать с children_, т.к. DetachFromMaster удаляет из него элементы (it invalidates iterators)
    std::for_each(old_children_list.begin(), old_children_list.end(), [](LoggerInterface* i) { i->DetachFromMaster(); });
    _DBG_CONDITION( children_.empty() );

    if(master_ptr_)
        master_ptr_->RemoveChildFromList(this);

    if(master_)
        Trace(L"Master log destroyed");
}

template<class ErrorReportingPolicy, template<class>class MutexClass, class DefaultCommiter>
void Logger<ErrorReportingPolicy, MutexClass, DefaultCommiter>::WriteToLog( const std::wstring& str, LogType type )
{
    wchar_t                                         current_thread_id[16];
    swprintf_s(current_thread_id, L" <%.8X> ", GetCurrentThreadId());

    std::wstring                                    formated_string         = Scope::IsEmpty() ? (current_thread_id + str + L"\n") : ( current_thread_id + Scope::CurrentScope() + L" :: " + str + L"\n" );
    static const std::deque<QueueType>::size_type   max_local_strings       = 128;  
    
    if(master_) //  если я главный - сохраняю данные
        Commit(formated_string.c_str(), type);
    else
    {
        Locker locker(mutex_);
                //  если я не главный...
        if(master_ptr_)  //  но знаю кто главный - отправляю ему
            master_ptr_->Commit(formated_string.c_str(), type);
        else
        {       
                //  иначе, накручиваем у себя лог
            if(local_temp_.size() >= max_local_strings )
            {
                local_temp_overflow_ = true;
                local_temp_.pop_front();    //  с надеждой на эффективную реализацию дэка
            }

            local_temp_.push_back(std::make_pair(formated_string, type));
        }
    }
}

template<class ErrorReportingPolicy, template<class>class MutexClass, class DefaultCommiter>
void Logger<ErrorReportingPolicy, MutexClass, DefaultCommiter>::Commit( const wchar_t* str, LogType type )
{
    if(log_level() < type)
        return;

    Locker locker(mutex_);

    std::for_each(commiters_.begin(), commiters_.end(), [&type, &str, this](CommiterType commiter_)
    {
        switch(type)
        {
        case ltGeneral:     commiter_->Commit(L"[ General ]"   + std::wstring(L" ") + std::wstring(str)); break;
        case ltDebug:       commiter_->Commit(L"[  Debug  ]"   + std::wstring(L" ") + std::wstring(str)); break;
        case ltExtended:    commiter_->Commit(L"[   Ext   ]"   + std::wstring(L" ") + std::wstring(str)); break;
        case ltTrace:       commiter_->Commit(L"[  Trace  ]"   + std::wstring(L" ") + std::wstring(str)); break;
        default:
            this->OnError(L"Not implemented log type");
        }
    });
}

template<class ErrorReportingPolicy, template<class>class MutexClass, class DefaultCommiter>
bool Logger<ErrorReportingPolicy, MutexClass, DefaultCommiter>::AttachToMaster( LoggerInterface* master )
{
    _DBG_POINTER( master );
    if(master_)
    {
        OnError(L"Can't attach master to master");
        return false;
    }

    if(master->version() != version())
    {
        OnError(L"Unsupported logger version");
        return false;
    }

    {
        Locker locker(mutex_);
        master_ptr_ = master;
        master_ptr_->AddChildToList(this);

        if(local_temp_overflow_)
            master_ptr_->Commit(L"...", ltGeneral);

        //  передаем всё накопленное в лог
        for(auto i = local_temp_.cbegin(); i != local_temp_.cend(); ++i)
            master_ptr_->Commit(i->first.c_str(), i->second);

        local_temp_.clear();
        local_temp_overflow_ = false;
    }
    return true;
}

template<class ErrorReportingPolicy, template<class>class MutexClass, class DefaultCommiter>
void Logger<ErrorReportingPolicy, MutexClass, DefaultCommiter>::DetachFromMaster()
{
    Locker locker(mutex_);
    if(master_ptr_)
        master_ptr_->RemoveChildFromList(this);

    master_ptr_ = nullptr;
}

template<class ErrorReportingPolicy, template<class>class MutexClass, class DefaultCommiter>
void Logger<ErrorReportingPolicy, MutexClass, DefaultCommiter>::AddChildToList( LoggerInterface* logger )
{
    if(std::find(children_.cbegin(), children_.cend(), logger) == children_.cend())
        children_.push_back(logger);
}

template<class ErrorReportingPolicy, template<class>class MutexClass, class DefaultCommiter>
void Logger<ErrorReportingPolicy, MutexClass, DefaultCommiter>::RemoveChildFromList( LoggerInterface* logger )
{
    if(std::find(children_.cbegin(), children_.cend(), logger) == children_.cend())
        operator()(L"Can't remove unknown child");
    else
        children_.erase(std::remove(children_.begin(), children_.end(), logger), children_.end());
}

template<class ErrorReportingPolicy, template<class>class MutexClass, class DefaultCommiter>
void Logger<ErrorReportingPolicy, MutexClass, DefaultCommiter>::SetMaster(bool new_master)
{
    Locker locker(mutex_);

    if(!master_)
    {
        //  если был не мастер, а стал мастер
        if(new_master)
        {
            master_ = true;

            //  передаем всё накопленное в лог
            for(auto i = local_temp_.cbegin(); i != local_temp_.cend(); ++i)
                Commit(i->first.c_str(), i->second);

            local_temp_.clear();
            local_temp_overflow_ = false;
        }
    }
    else
    {
        //  если был мастер, а стал не мастер
        master_ = false;
    }
}

template<class ErrorReportingPolicy, template<class>class MutexClass, class DefaultCommiter>
void Logger<ErrorReportingPolicy, MutexClass, DefaultCommiter>::ReplaceCommiters( const std::shared_ptr<CommitInterface>& new_commiter )
{
    Locker locker(mutex_);
    commiters_.clear();
    commiters_.push_back(new_commiter);
}

template<class ErrorReportingPolicy, template<class>class MutexClass, class DefaultCommiter>
void Logger<ErrorReportingPolicy, MutexClass, DefaultCommiter>::AddCommiter( const std::shared_ptr<CommitInterface>& new_commiter )
{
    Locker locker(mutex_);
    commiters_.push_back(new_commiter);
}

}

#pragma region Helpers

#ifndef NO_LOG
    #define LOG(a)          ::logging::Logger<>::instance()(L ## a)
    #define LOG_S(a)        ::logging::Logger<>::instance()(a)
#else
    #define LOG(a)
    #define LOG_S(a)
#endif

#ifndef NO_LOG_DEBUG
    #define LDEBUG(a)       ::logging::Logger<>::instance().Debug(L ## a)
    #define LDEBUG_S(a)     ::logging::Logger<>::instance().Debug(a)
#else
    #define LDEBUG(a)
    #define LDEBUG_S(a)
#endif

#ifndef NO_LOG_EXTENDED
    #define LEXTENDED(a)    ::logging::Logger<>::instance().Extended(L ## a)
    #define LEXTENDED_S(a)  ::logging::Logger<>::instance().Extended(a)
#else
    #define LEXTENDED(a)
    #define LEXTENDED_S(a)
#endif

#ifndef NO_LOG_TRACE
    #define LTRACE(a)       ::logging::Logger<>::instance().Trace(L ## a)
    #define LTRACE_S(a)     ::logging::Logger<>::instance().Trace(a)
#else
    #define LTRACE(a)
    #define LTRACE_S(a)
#endif

#ifndef NO_LOG_SCOPE
    #define LSCOPE(a)       ::logging::Scope scope (L ## a)
    #define LSCOPE_S(a)     ::logging::Scope scope (a)
    #define LSCOPE_A        ::logging::Scope scope (_T(__FUNCTION__))
#else
    #define LSCOPE(a)
    #define LSCOPE_S(a)
    #define LSCOPE_A
#endif

#define SET_MASTER_LOG(a)   ::logging::Logger<>::instance().SetMaster(a)

#define LOG_TO_FILE(name, header)           \
                            ::logging::Logger<>::instance().ReplaceCommiters(std::shared_ptr<logging::CommitInterface>(new logging::FileCommiter<>(L ## name, L ## header)))

#define LOG_TO_FILE_S(name, header)         \
                            ::logging::Logger<>::instance().ReplaceCommiters(std::shared_ptr<logging::CommitInterface>(new logging::FileCommiter<>(name, header)))

#define ADD_LOG_TO_FILE(name, header)       \
                            ::logging::Logger<>::instance().AddCommiter     (std::shared_ptr<logging::CommitInterface>(new logging::FileCommiter<>(L ## name, L ## header)))

#define ADD_LOG_TO_FILE_S(name, header)     \
                            ::logging::Logger<>::instance().AddCommiter     (std::shared_ptr<logging::CommitInterface>(new logging::FileCommiter<>(name, header)))


#define SET_LOG_LEVEL(level)                \
                            ::logging::Logger<>::instance().set_log_level   (level)

#define ATTACH_LOG_TO_MASTER(master_log)    \
                            ::logging::Logger<>::instance().AttachToMaster  (master_log)

#pragma endregion

#endif
