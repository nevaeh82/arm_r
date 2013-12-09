#ifndef _MODULES_WINDOWS_IPC_MESSAGE_PIPE_H_
#define _MODULES_WINDOWS_IPC_MESSAGE_PIPE_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <vector>
#include <queue>

//#define USE_TEST_DELAYS

namespace win32
{

namespace ipc   //  interprocess communication
{

class IPCMessageTransmitterInterface
{
public:
    virtual bool Transmit       (size_t client_index, const void* message, DWORD size) = 0;
    virtual void TerminateThread(bool wait) = 0;
    virtual void FreeMemoryBlock(const void* message) const = 0;
    virtual ~IPCMessageTransmitterInterface() {}
};

class IPCMessageReceiverInterface
{
public:
    //  ���������� ��������� ������ ���� ������� � ������� IPCMessageTransmitterInterface::FreeMemoryBlock
    //  ��� ������, ����� use_message_header = false
    virtual bool ReceiveIPCMessage          (IPCMessageTransmitterInterface* transmitter, size_t client_index, const void* message, DWORD size) = 0;

    //  ����������� ��� ����������� �������
    virtual void OnConnect                  (size_t client_index) = 0;

    //  ����������� ��� ���������� �������
    virtual void OnDisconnect               (size_t client_index) = 0;

    virtual ~IPCMessageReceiverInterface() {}
};

//  ������� �� ���������� ������ � Overlapped IO ���, ����� ������������ ������ � ������ � ����� �� ����������� ���� �����

class PipeIPC: public IPCMessageTransmitterInterface
{
public:
    PipeIPC();
    ~PipeIPC();

            //  use_message_header - ���� ������ ���� ���������� �� ������� � �� �������
            //                       ��������� ����������� ��������� � �������� ���������
            //                       ���� ������ ��������� ������� �������� - ����� ����� �������� ����� ������������ �������
            bool    StartServerThread(const TCHAR* pipe_name, IPCMessageReceiverInterface*, DWORD max_clients, bool use_message_header = false);
            bool    StartClientThread(const TCHAR* pipe_name, IPCMessageReceiverInterface*, bool use_message_header = false);

    virtual bool    Transmit(size_t client_index, const void* message, DWORD size);
    virtual void    FreeMemoryBlock(const void* message) const;
    virtual void    TerminateThread(bool wait);

            void    Wait();

            static const size_t kDefaultPipeBufferSize  = 1*1024*1024;
            static const size_t kDefaultTimeout         = 50000;

private:
    PipeIPC(const PipeIPC&);
    PipeIPC& operator=(const PipeIPC&);

    enum Mode   {   modeNotSet  = 0,    modeServer  = 1,    modeClient  = 2 };

    class ConnectionHandler
    {
    public:

                ConnectionHandler(HANDLE read_event_handle, HANDLE write_event_handle, HANDLE pipe_handle, size_t client_index, IPCMessageReceiverInterface* receiver, IPCMessageTransmitterInterface* transmitter, bool use_message_header);
                ~ConnectionHandler();

                bool    StartServerInstance();
                bool    StartClientInstance();

                bool    Transmit(const void* const message, DWORD size);      //  ���� ���� IO, �� ����� ������ � �������, ����� ���������� �����
                void    OnEvent(bool read);                             //  �������� IPCMessageReceiverInterface::ReceiveIPCMessage

        static  void    FreeMemoryBlock(const void* message);

    private:

        ConnectionHandler(const ConnectionHandler&);
        ConnectionHandler& operator=(const ConnectionHandler&);
        bool    SendOverlapped(const void*, DWORD size);
        void    Reconnect();
        void    AppendMessageToBuffer(const void* message, size_t size, bool pending);
        void    MessageToReceiver();
        void    PopAndSendMessage();

        enum State { kDisconnected = 0, kConnecting = 1, kEndOfMessage = 2, kReceivingMessage = 3 };

        IPCMessageReceiverInterface*    receiver_;
        IPCMessageTransmitterInterface* transmitter_;
        size_t                          client_index_;

        OVERLAPPED                      in_overlapped_;
        HANDLE                          handle_;
        bool                            read_pending_;
        bool                            use_message_header_;
        bool                            server_mode_;

        BYTE*                           current_message_;
        DWORD                           current_message_size_;
        DWORD                           current_message_allocated_;

        BYTE                            in_buffer_[kDefaultPipeBufferSize];

        CRITICAL_SECTION                section_;               //  �������� state_, write_pending_, out_overlapped_, messages_queue_
        State                           state_;
        bool                            write_pending_;
        OVERLAPPED                      out_overlapped_;
        std::queue<std::vector<BYTE>>   messages_queue_;

        LARGE_INTEGER                   total_tx_;
        LARGE_INTEGER                   total_rx_;
    };

    class CriticalSectionLock
    {
    public:
        CriticalSectionLock(CRITICAL_SECTION& section):
            section_(section)
        {
            EnterCriticalSection(&section_);
        }
        ~CriticalSectionLock()
        {
            LeaveCriticalSection(&section_);
        }
    private:
        CRITICAL_SECTION& section_;
    };

    Mode                                mode_;
    CRITICAL_SECTION                    section_;
    HANDLE                              thread_handle_;
    DWORD                               max_clients_;
    std::vector<HANDLE>                 events_;        //  ������ �������, ������� ���� ����� (� �.�. ������� �� ���������� ������ ������ - �0), ������ �������� = max_clients*2+1 ��� ������� � 2*1+1 ��� �������
    std::vector<ConnectionHandler*>     handlers_;

    bool TryToConnectToServer();

    DWORD ThreadProc();
    static DWORD WINAPI StaticThreadProc(LPVOID lpParam);
};

}

}

#endif