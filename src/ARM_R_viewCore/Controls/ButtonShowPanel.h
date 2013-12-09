#ifndef BUTTONSHOWPANEL_H
#define BUTTONSHOWPANEL_H

#include <QToolButton>

class ButtonShowPanel : public QToolButton
{
    Q_OBJECT
public:
    ButtonShowPanel(QWidget *parent);
    ~ButtonShowPanel();
    void sh();
signals:
    void signalPanelShow();
    
public slots:

protected:
    void enterEvent(QEvent *);

};

#endif // BUTTONSHOWPANEL_H
