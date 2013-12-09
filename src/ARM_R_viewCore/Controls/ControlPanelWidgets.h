#ifndef CONTROLPANELWIDGETS_H
#define CONTROLPANELWIDGETS_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QToolButton>

#include <QPropertyAnimation>

class ControlPanelWidgets : public QWidget
{
    Q_OBJECT
public:
    ControlPanelWidgets(QWidget *parent = 0);
    ~ControlPanelWidgets();

public:
    QToolButton         *tb_map;
    QToolButton         *tb_spectra;
    QToolButton         *tb_spectrum;
    QToolButton         *tb_correlations;

private:
    QWidget             *_panel_controls_widget;
    QVBoxLayout         *_vbox_panel;
    QPropertyAnimation  *_animation;


private:
    void _add_widgets_to_panel(QWidget *);

    void leaveEvent(QEvent *);

public slots:
    void slotShow();
    void slotSetSize(int width, int height);

};

#endif // CONTROLPANELWIDGETS_H
