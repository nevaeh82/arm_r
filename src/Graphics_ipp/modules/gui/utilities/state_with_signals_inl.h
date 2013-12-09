#ifndef _GUI_UTILITIES_STATE_WITH_SIGNALS_H
#define _GUI_UTILITIES_STATE_WITH_SIGNALS_H

#include <QState>
namespace utilities {

class StateWithSignals: public QState
{

    Q_OBJECT

public:
    StateWithSignals(QState* parent = NULL): QState(parent), active_(false) {}
    bool active() const { return active_; }

protected:
    virtual void onEntry(QEvent* e)
    {
        active_ = true;
        emit OnEntry(e);
        QState::onEntry(e);
    }

    virtual void onExit(QEvent* e)
    {
        emit OnExit(e);
        QState::onExit(e);
        active_ = false;
    }

private:

    bool active_;

signals:
    void OnEntry(QEvent*);
    void OnExit(QEvent*);
};

}  //  namespace utilities

#endif
