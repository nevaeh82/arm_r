#ifndef IROUTER_H
#define IROUTER_H

class IQueue;
class IController;
class ITCPController;
class ISubscriber;

class IRouter
{
public:
    virtual ~IRouter(){};
    virtual void set_queue(IQueue *queue)                               = 0;
    virtual void set_tcp_controller(ITCPController *tcp_controller)     = 0;
    virtual IQueue *get_queue()                                         = 0;
    virtual ITCPController *get_tcp_controller()                        = 0;
    virtual void set_subscriber(ISubscriber *scr)                       = 0;
    virtual ISubscriber* get_subscriber()                               = 0;
};

#endif // IROUTER_H
