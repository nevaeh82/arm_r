#ifndef _GUI_UTILITIES_ENUM_TRANSITION_H_
#define _GUI_UTILITIES_ENUM_TRANSITION_H_

namespace utilities {

    template<int Id>
    class EnumTransition: public QAbstractTransition
    {
    public:
        struct Event: public QEvent
        {
            Event(): QEvent(QEvent::Type(QEvent::User + Id)) {}
        };

    protected:
        virtual bool eventTest(QEvent* event)
        {
            return event->type() == QEvent::Type(QEvent::User + Id);
        }
        virtual void onTransition(QEvent *)
        {
        }
    public:
        EnumTransition(QAbstractState* TargetState): QAbstractTransition()
        {
            setTargetState(TargetState);
        }
    };

}  //  namespace utilities

#endif