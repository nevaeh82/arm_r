#ifndef _MODULES_CORE_STD_THREAD_CLASS_INL_H_
#define _MODULES_CORE_STD_THREAD_CLASS_INL_H_

//  пока основан на tbb
#include <tbb/compat/thread>

namespace threading
{

//  поток, частично совместим с QThread
class Thread
{
public:

    Thread()
    {
        stop_flag_ = false;
    }

    virtual ~Thread()
    {
        try
        {
            stop();
        }
        catch(...)
        {
        }
    }

    void stop()
    {
        stop_flag_ = true;
        wait();
    }

    void wait()
    {
        if(thread_.joinable())
            thread_.join();
    }

    void start()
    {
        thread_ = std::thread(RunHelper, this);
    }

    bool is_running()
    {
        return thread_.joinable();
    }

protected:    

    virtual void run() = 0;

    tbb::atomic<bool> stop_flag_;

private:

    std::thread thread_;

    Thread(Thread const&);
    Thread& operator=(Thread const&);

    static void RunHelper(Thread* thread_class)  { thread_class->run(); }
};


}

#endif