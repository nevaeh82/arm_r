#include "stdafx.h"
#include "ipc_message_pipe.h"
#include "modules/core/preprocessor.h"
#include "modules/core/strings.h"

#define ENABLE_LOGGING
// #define TRACE_DATA_RECEIVING
// #define TRACE_DATA_TRANSMITTING

#ifdef ENABLE_LOGGING
#include "modules/log/log.h"
#else
#define LSCOPE(a)
#define LSCOPE_S(a)
#define LTRACE(a)
#define LTRACE_S(a)
#define LEXTENDED(a)
#define LEXTENDED_S(a)
#define LDEBUG(a)
#define LDEBUG_S(a)
#define LOG(a)
#define LOG_S(a)
#endif

namespace win32 {

namespace ipc {

DWORD WINAPI PipeIPC::StaticThreadProc( LPVOID lpParam )
{
    LSCOPE("PipeIPC");
    return reinterpret_cast<PipeIPC*>(lpParam)->ThreadProc();
}

PipeIPC::PipeIPC():
    mode_           (modeNotSet),
    section_        (),
    thread_handle_  (INVALID_HANDLE_VALUE),
    max_clients_    (),
    events_         (1)
{
    LSCOPE("PipeIPC");
    LTRACE("Constructor");

    InitializeCriticalSection(&section_);

    events_[0]      = CreateEvent( NULL, TRUE, FALSE, _T("Pipe thread term event") );
    
    if(events_[0] == INVALID_HANDLE_VALUE)
        LOG("Failed to create event");

    thread_handle_  = CreateThread(NULL, 0, StaticThreadProc, this, CREATE_SUSPENDED, NULL);
    if(thread_handle_ == INVALID_HANDLE_VALUE)
        LOG("Failed to create thread");
}

PipeIPC::~PipeIPC()
{
    LSCOPE("PipeIPC");

    TerminateThread(true);

    LTRACE("Cleaning up...");

    CloseHandle(thread_handle_);

    if(!events_.empty())
    {
        CloseHandle(events_[0]);
        events_[0] = INVALID_HANDLE_VALUE;
    }

    for(std::vector<ConnectionHandler*>::iterator i = handlers_.begin(); i != handlers_.end(); ++i)
        delete *i;
    handlers_.clear();

    DeleteCriticalSection(&section_);
}

bool PipeIPC::Transmit( size_t client_index, const void* message, DWORD size )
{
    if(client_index >= handlers_.size())
        return false;

    return handlers_[client_index]->Transmit(message, size);
}

void PipeIPC::FreeMemoryBlock( const void* message ) const
{
    ConnectionHandler::FreeMemoryBlock(message);
}

DWORD PipeIPC::ThreadProc()
{
    LSCOPE("ThreadProc");

    LTRACE("Starting...");

    _DBG_CONDITION(!events_.empty());

    while(true)
    {
        //  спокойно ждем событий...
        DWORD wait_result = WaitForMultipleObjects(events_.size(), events_.data(), FALSE, INFINITE);

        if((wait_result - WAIT_OBJECT_0) == 0)
        {
            LTRACE("Exit event signal captured");
            break;
        }

        if(wait_result == WAIT_TIMEOUT)
        {
            LTRACE("Wait timeout?");
            continue;
        }

        if(wait_result >= WAIT_ABANDONED_0 && wait_result < (WAIT_ABANDONED+events_.size()-1))
        {
            LOG("Wait abandoned!");
            //  TODO: разобраться как такое может быть?
            break;
        }

        if(wait_result >= WAIT_OBJECT_0 && wait_result < (WAIT_OBJECT_0+events_.size()))
        {
            DWORD   handler_index   = ((wait_result-WAIT_OBJECT_0)-1)/2;
            bool    read_event      = ((wait_result-WAIT_OBJECT_0)-1)%2 == 0;
            _DBG_CONDITION(handler_index < handlers_.size());
            handlers_[handler_index]->OnEvent(read_event);
        }
    }

    LTRACE("Finishing...");
    {
        CriticalSectionLock lock(section_);
        mode_   =   modeNotSet;
    }
    return 0;
}

bool PipeIPC::StartServerThread( const TCHAR* pipe_name, IPCMessageReceiverInterface* receiver, DWORD max_clients, bool use_message_header /*= false*/ )
{
    LSCOPE("StartServerThread");

    if(mode_ != modeNotSet)
    {
        LOG("Pipe thread is running already...");
        return false;
    }

    if(receiver == NULL)
    {
        LOG("Receiver is NULL");
        return false;
    }

    if(pipe_name == NULL)
    {
        LOG("Pipe name is NULL");
        return false;
    }

    if(use_message_header == true)
    {
        LOG("Not implemented");
        return false;
    }

    if(max_clients > 1000)
    {
        LOG("Usually, system can't handle more than 1000 clients... It wasn't tested.");
        return false;
    }

    std::vector<ConnectionHandler*> handlers;
    std::vector<HANDLE>             events(events_);

    for(DWORD client = DWORD(); client != max_clients; ++client)
    {
        HANDLE              pipe_handle = CreateNamedPipe( pipe_name, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, max_clients, kDefaultPipeBufferSize, kDefaultPipeBufferSize, kDefaultTimeout, NULL );
        HANDLE              read_event  = CreateEvent( NULL, TRUE, TRUE, NULL );
        HANDLE              write_event = CreateEvent( NULL, TRUE, FALSE, NULL );
        ConnectionHandler*  handler     = NULL;

        if(pipe_handle != INVALID_HANDLE_VALUE && read_event != INVALID_HANDLE_VALUE && write_event != INVALID_HANDLE_VALUE)
        {
            handler = new ConnectionHandler(read_event, write_event, pipe_handle, handlers.size(), receiver, this, use_message_header);
            if(handler && handler->StartServerInstance())
            {
                events.push_back(read_event);
                events.push_back(write_event);
                handlers.push_back(handler);
            }
        }
        else
        {
            LOG("Can't create named pipe");
            for(DWORD k = DWORD(); k != client; ++k)
            {
                delete handlers[k];
                handlers[k] = NULL;
            }
            handlers.clear();
            CloseHandle(read_event);
            CloseHandle(write_event);
            return false;
        }
    }

    max_clients_    = handlers.size();
    handlers_       = handlers;
    events_         = events;

    mode_           = modeServer;
    bool    ok      = ResumeThread(thread_handle_) != (DWORD)-1;
    return ok;
}

bool PipeIPC::StartClientThread( const TCHAR* pipe_name, IPCMessageReceiverInterface* receiver, bool use_message_header /*= false*/ )
{
    LSCOPE("StartServerThread");

    if(mode_ != modeNotSet)
    {
        LOG("Pipe thread is running already...");
        return false;
    }

    if(receiver == NULL)
    {
        LOG("Receiver is NULL");
        return false;
    }

    if(pipe_name == NULL)
    {
        LOG("Pipe name is NULL");
        return false;
    }

    if(use_message_header == true)
    {
        LOG("Not implemented");
        return false;
    }

    std::vector<ConnectionHandler*> handlers;
    std::vector<HANDLE>             events(events_);

    HANDLE              pipe_handle = CreateFile( pipe_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL );
    DWORD               new_mode    = PIPE_READMODE_MESSAGE;

    if(!SetNamedPipeHandleState(pipe_handle, &new_mode, NULL, NULL))
    {
        std::basic_ostringstream<TCHAR> code;
        code << GetLastError();
        LOG_S(_T("SetNamedPipeHandleState failed. GLE = ") + code.str());
        CloseHandle(pipe_handle);
        return false;
    }

    HANDLE              read_event  = CreateEvent( NULL, TRUE, TRUE, NULL );
    HANDLE              write_event = CreateEvent( NULL, TRUE, FALSE, NULL );
    ConnectionHandler*  handler     = NULL;


    if(pipe_handle != INVALID_HANDLE_VALUE && read_event != INVALID_HANDLE_VALUE && write_event != INVALID_HANDLE_VALUE)
    {
        handler = new ConnectionHandler(read_event, write_event, pipe_handle, handlers.size(), receiver, this, use_message_header);
        if(handler->StartClientInstance())
        {
            events.push_back(read_event);
            events.push_back(write_event);
            handlers.push_back(handler);
        }
    }
    else
    {
        LOG("Can't create named pipe");
        delete handler;
        handlers.clear();
        CloseHandle(read_event);
        CloseHandle(write_event);
        return false;
    }

    max_clients_    = handlers.size();
    handlers_       = handlers;
    events_         = events;

    mode_           = modeClient;
    bool    ok      = ResumeThread(thread_handle_) != (DWORD)-1;
    return ok;
}

void PipeIPC::TerminateThread(bool wait)
{
    if(mode_ != modeNotSet)
    {
        LTRACE("Setting up termination event...");
        SetEvent(events_[0]);
        if(wait)
            Wait();
    }
}

void PipeIPC::Wait()
{
    LTRACE("Waiting for thread...");
    WaitForSingleObject(thread_handle_, INFINITE);
    LTRACE("Thread terminated");
}

PipeIPC::ConnectionHandler::ConnectionHandler( HANDLE read_event_handle, HANDLE write_event_handle, HANDLE pipe_handle, size_t client_index, IPCMessageReceiverInterface* receiver, IPCMessageTransmitterInterface* transmitter, bool use_message_header ):
    receiver_                   (receiver),
    transmitter_                (transmitter),
    client_index_               (client_index),
    in_overlapped_              (),
    out_overlapped_             (),
    handle_                     (pipe_handle),
    state_                      (kDisconnected),
    read_pending_               (),
    write_pending_              (),
    use_message_header_         (use_message_header),
    server_mode_                (),
    current_message_            (),
    current_message_size_       (),
    current_message_allocated_  (),
    section_                    (),
    total_tx_                   (),
    total_rx_                   ()
{
    InitializeCriticalSection(&section_);
    memset(in_buffer_, 0, sizeof(in_buffer_));
    in_overlapped_.hEvent   = read_event_handle;
    out_overlapped_.hEvent  = write_event_handle;
}

PipeIPC::ConnectionHandler::~ConnectionHandler()
{
    if(state_ != kDisconnected && state_ != kConnecting)
        if(receiver_)
            receiver_->OnDisconnect(client_index_);

    CloseHandle(in_overlapped_.hEvent);
    CloseHandle(out_overlapped_.hEvent);
    CloseHandle(handle_);
    FreeMemoryBlock(current_message_);
    DeleteCriticalSection(&section_);
}

bool PipeIPC::ConnectionHandler::StartServerInstance()
{
    CriticalSectionLock lock(section_);
    if(state_ != kDisconnected)
        return false;

    server_mode_    = true;

    BOOL connected  = ConnectNamedPipe(handle_, &in_overlapped_);
    if(connected)
    {
        LOG("ConnectNamedPipe failed");
        return false;
    }

    switch(GetLastError())
    { 
    case ERROR_IO_PENDING:
        read_pending_ = true;
        break; 

    case ERROR_PIPE_CONNECTED:
        if(receiver_)
            receiver_->OnConnect(client_index_);
        if (SetEvent(in_overlapped_.hEvent))
            break;

    default:
        LOG("ConnectNamedPipe failed");
        state_ = kDisconnected;
        return false;
    }

    state_ = kConnecting;

    return true;
}

bool PipeIPC::ConnectionHandler::Transmit( const void* const message, DWORD size )
{
    if(message == NULL && size == 0)
        return true;
    if(message == NULL && size != 0)
        return false;
    if(message != NULL && size == 0)
        return false;

    LSCOPE("Transmit");
    {
        CriticalSectionLock lock(section_);
        if(state_ == kDisconnected)
            return false;       //  или может в очередь его закинуть?..

        if(state_ == kConnecting || write_pending_ || !messages_queue_.empty())
        {
            std::vector<BYTE>   data(size);
            memcpy(data.data(), message, size);
            messages_queue_.push(data);
#ifdef TRACE_DATA_TRANSMITTING
            if(server_mode_)LTRACE("Server: putting message in queue");
            else            LTRACE("Client: putting message in queue");
#endif // TRACE_DATA_TRANSMITTING

            if(!write_pending_)
                PopAndSendMessage();
        }
        else
        {
#ifdef TRACE_DATA_TRANSMITTING
            if(server_mode_)LTRACE("Server: direct send");
            else            LTRACE("Client: direct send");
#endif // TRACE_DATA_TRANSMITTING
            SendOverlapped(message, size);
        }
        return true;
    }
}

void PipeIPC::ConnectionHandler::OnEvent( bool read )
{
    CriticalSectionLock lock(section_);
    if(read)
    {
        if(read_pending_)
        {
            if(state_ == kConnecting || state_ == kReceivingMessage)
            {
                DWORD   read_bytes              = DWORD();
                BOOL    read_status             = GetOverlappedResult(handle_, &in_overlapped_, &read_bytes, FALSE);

                if(state_ == kConnecting)
                {
                    if(!read_status)
                    {
                        DWORD GLE = GetLastError();
                        LOG("Connecting error");
                        state_                  = kDisconnected;
                        Reconnect();
                    }
                    else
                    {
                        state_                  = kEndOfMessage;
                        if(server_mode_)
                            LTRACE("Server: client connected");
                        else
                            LTRACE("Client: connected to server");

                        if(receiver_)
                            receiver_->OnConnect(client_index_);

                        if(!messages_queue_.empty())
                            PopAndSendMessage();
                    }
                }
                else    //  if(state_ == kConnecting)
                {
                    AppendMessageToBuffer(in_buffer_, read_bytes, true);

                    read_pending_   = false;

                    if(read_status)
                    {
                        state_  = kEndOfMessage;
                        MessageToReceiver();
                    }
                    else
                    if(read_bytes == 0)
                    {
                        if(server_mode_)
                            LTRACE("Server: error, reconnecting...");
                        else
                            LTRACE("Client: error, reconnecting...");
                        Reconnect();
                    }
                }   //  if(state_ == kConnecting)
            }   //  if(state_ == kConnecting || state_ == kReceivingMessage)
        }   //  if(read_pending_)
        else
        {
            if(state_ != kEndOfMessage && state_ != kReceivingMessage)
            {
                DWORD   last_error      = GetLastError();
                if(server_mode_)
                    LOG("Server: unexpected read event");
                else
                    LOG("Client: unexpected read event");
                Reconnect();
            }
        }

        if(state_ == kEndOfMessage || state_ == kReceivingMessage)
        {
            BOOL    read_success    = ReadFile(handle_, in_buffer_, sizeof(in_buffer_), NULL, &in_overlapped_);
            DWORD   read_error      = GetLastError();

            if(read_error == ERROR_IO_PENDING)
            {
                read_pending_       = true;
                state_              = kReceivingMessage;
            }
            else
            {
                read_pending_       = false;
                DWORD   read_bytes              = DWORD();
                BOOL    read_status             = GetOverlappedResult(handle_, &in_overlapped_, &read_bytes, FALSE);
                DWORD   overlapped_result_error = GetLastError();

                _DBG_CONDITION(read_error == overlapped_result_error);

                if(read_status == FALSE && read_error != ERROR_MORE_DATA)
                {
                    LDEBUG("Unexpected overlapped result"); //  TODO: разобраться как сюда попасть
                    if(overlapped_result_error == ERROR_IO_INCOMPLETE)
                    {
                        LOG("IO incomplete");
                        Reconnect();
                    }
                }   //  if(read_status == FALSE && read_error != ERROR_MORE_DATA)
                else
                {
                    if(overlapped_result_error == ERROR_IO_INCOMPLETE || read_error == ERROR_BROKEN_PIPE)
                    {
                        if(read_error == ERROR_BROKEN_PIPE)
                        {
                            if(server_mode_)
                                LOG("Server: the pipe has been ended");
                            else
                                LOG("Client: the pipe has been ended");
                        }
                        else
                        {
                            if(server_mode_)
                                LOG("Server: IO incomplete");
                            else
                                LOG("Client: IO incomplete");
                        }
                        Reconnect();
                    }

#ifdef TRACE_DATA_RECEIVING
                    LTRACE_S(std::wstring(server_mode_ ? L"S: " : L"C: ") + L"RE: " + strings::IntToStr((int)read_error) + L" size: " + strings::IntToStr((int)read_bytes));
#endif // TRACE_DATA_RECEIVING

                    AppendMessageToBuffer(in_buffer_, read_bytes, false);

                    if(read_success == FALSE)
                    {
                        if(read_error == ERROR_MORE_DATA)
                        {
                            //read_pending_   = true;
                            state_          = kReceivingMessage;
                        }
                        else
                        {
                            if(server_mode_)
                                LOG("Server: read error");
                            else
                                LOG("Client: read error");
                            Reconnect();
                        }
                    }
                    else
                    {
                        state_              = kEndOfMessage;

                        MessageToReceiver();
                    }
                }
            }
        }   //  if(state_ == kEndOfMessage || state_ == kReceivingMessage)

        if(!messages_queue_.empty() && !write_pending_)
            PopAndSendMessage();
    }   //  if(read)
    else
    {
        if(write_pending_)
        {
            DWORD   write_bytes             = DWORD();
            BOOL    write_status            = GetOverlappedResult(handle_, &out_overlapped_, &write_bytes, FALSE);
            DWORD   overlapped_result_error = GetLastError();
            if(write_status == TRUE)
            {
                ResetEvent(out_overlapped_.hEvent);
                write_pending_              = false;
            }
            else
            {
                if(!(!server_mode_ && overlapped_result_error == ERROR_OPERATION_ABORTED))
                {
                    if(overlapped_result_error != ERROR_IO_INCOMPLETE)  //  это вообще странно - винда разбудила поток чтобы сообщить, что еще не закончила работу?..
                    {
                        if(server_mode_)    LOG_S(L"Server: write error " + std::to_wstring(long long(overlapped_result_error)));
                        else                LOG_S(L"Client: write error " + std::to_wstring(long long(overlapped_result_error)));
                        Reconnect();
                    }
                }
                else
                    LOG("Client: write error - operation aborted");
            }
        }
        if(!messages_queue_.empty() && !write_pending_ && state_ != kDisconnected)
            PopAndSendMessage();
    }
}

void PipeIPC::ConnectionHandler::AppendMessageToBuffer(const void* message, size_t size, bool pending)
{
    if(size)
    {
        if(current_message_size_ + size > current_message_allocated_)
        {
            current_message_allocated_  = current_message_size_ + size;
            current_message_            = reinterpret_cast<BYTE*>(_aligned_realloc(current_message_, current_message_allocated_, 16));
        }
        memcpy(&current_message_[current_message_size_], message, size);
        current_message_size_          += size;
        total_rx_.QuadPart             += size;

#ifdef TRACE_DATA_RECEIVING
        std::wstring temp_short_dump;
        if(size > 5)
            temp_short_dump = (strings::IntToStr(((char*)message)[2]) + L" " + strings::IntToStr(((char*)message)[3]) + L" " + strings::IntToStr(((char*)message)[4]) + L" " + strings::IntToStr(((char*)message)[5]));

        if(server_mode_)    LTRACE_S(L"Server: appending to message " + temp_short_dump + L" +" + strings::IntToStr(int(size)) + L", total: " + strings::IntToStr(int(current_message_size_)) + (pending ? L" p" : L"") + L" total rx: " + strings::IntToStr(int(total_rx_.QuadPart)));
        else                LTRACE_S(L"Client: appending to message " + temp_short_dump + L" +" + strings::IntToStr(int(size)) + L", total: " + strings::IntToStr(int(current_message_size_)) + (pending ? L" p" : L"") + L" total rx: " + strings::IntToStr(int(total_rx_.QuadPart)));
#endif // TRACE_DATA_RECEIVING
    }
}

bool PipeIPC::ConnectionHandler::SendOverlapped( const void* message, DWORD size )
{
    CriticalSectionLock lock(section_);
    LSCOPE("SendOverlapped");

#ifdef TRACE_DATA_TRANSMITTING
    std::wstring temp_short_dump;
    if(size > 5)
        temp_short_dump = (strings::IntToStr(((char*)message)[2]) + L" " + strings::IntToStr(((char*)message)[3]) + L" " + strings::IntToStr(((char*)message)[4]) + L" " + strings::IntToStr(((char*)message)[5]));

    if(server_mode_)    LTRACE_S(L"Server: writting to pipe " + strings::IntToStr(size) + L" bytes " + temp_short_dump);
    else                LTRACE_S(L"Client: writting to pipe " + strings::IntToStr(size) + L" bytes " + temp_short_dump);
#endif // TRACE_DATA_TRANSMITTING

    BOOL                write_success   = WriteFile(handle_, message, size, NULL, &out_overlapped_);
    DWORD               write_error     = GetLastError();

    if(write_error == ERROR_IO_PENDING || write_success == TRUE)
    {
        write_pending_  = write_success == FALSE;
#ifdef TRACE_DATA_TRANSMITTING
        if(write_pending_)
        {
            if(server_mode_)    LTRACE("Server: write pending");
            else                LTRACE("Client: write pending");
        }
#endif // TRACE_DATA_TRANSMITTING

        if(write_success)
        {
            DWORD   write_bytes             = DWORD();
            BOOL    write_status            = GetOverlappedResult(handle_, &out_overlapped_, &write_bytes, FALSE);
            DWORD   overlapped_result_error = GetLastError();
            overlapped_result_error         = -1;
            ResetEvent(out_overlapped_.hEvent);
        }

        return true;
    }

    //  write_success = FALSE и write_error ERROR_IO_PENDING

    if(write_error == ERROR_BROKEN_PIPE)
    {
        if(server_mode_)
            LOG("Server: the pipe has been ended");
        else
            LOG("Client: the pipe has been ended");
        Reconnect();
    }
    else
    {
        if(server_mode_)
            LOG("Server: unknown write error");
        else
            LOG("Client: unknown write error");
        Reconnect();
    }

    return false;
}

void PipeIPC::ConnectionHandler::FreeMemoryBlock( const void* message )
{
    _aligned_free(const_cast<void*>(message));
}

void PipeIPC::ConnectionHandler::Reconnect()
{
    LSCOPE("Reconnect");

    bool call_on_disconnect = state_ != kDisconnected && state_ != kConnecting;
    if(state_ != kDisconnected)
    {
        if(server_mode_)
            LTRACE("Server: disconnecting...");
        else
            LTRACE("Client: disconnecting...");
    }

    if(server_mode_)
        LTRACE("Server: sleeping...");
    else
        LTRACE("Client: sleeping...");

    Sleep(100);

    if(handle_ != INVALID_HANDLE_VALUE)
    {
        if(!DisconnectNamedPipe(handle_))
        {
            LOG("Failed to disconnect named pipe");
        }
        handle_ = INVALID_HANDLE_VALUE;
    }

    state_  = kDisconnected;

    if(call_on_disconnect && receiver_)
        receiver_->OnDisconnect(client_index_);

    if(server_mode_)
        StartServerInstance();
    else
        StartClientInstance();
}

void PipeIPC::ConnectionHandler::MessageToReceiver()
{
    _DBG_CONDITION(state_ == kEndOfMessage);
    _DBG_CONDITION(read_pending_ == false);

    if(receiver_)
    {
#ifdef TRACE_DATA_RECEIVING
        if(server_mode_)    LTRACE_S(L"Server: message to receiver " + strings::IntToStr(int(current_message_size_)) + L" bytes");
        else                LTRACE_S(L"Client: message to receiver " + strings::IntToStr(int(current_message_size_)) + L" bytes");
#endif // TRACE_DATA_RECEIVING

        receiver_->ReceiveIPCMessage(transmitter_, client_index_, current_message_, current_message_size_);
        current_message_            = NULL;
        current_message_size_       = 0;
        current_message_allocated_  = 0;
    }
}

bool PipeIPC::ConnectionHandler::StartClientInstance()
{
    CriticalSectionLock lock(section_);
    if(state_ != kDisconnected)
        return false;

    read_pending_   = true;
    state_          = kConnecting;

    return true;
}

void PipeIPC::ConnectionHandler::PopAndSendMessage()
{
#ifdef TRACE_DATA_TRANSMITTING
    LTRACE("PopAndSendMessage");
#endif // TRACE_DATA_TRANSMITTING
    CriticalSectionLock lock(section_);
    _DBG_CONDITION(write_pending_ == false);

    std::vector<BYTE>&  message         = messages_queue_.front();
    SendOverlapped(message.data(), message.size());
    messages_queue_.pop();
}

}   //  namespace ipc

}   //  namespace win32

