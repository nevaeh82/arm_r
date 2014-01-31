#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Tabs/TabManager.h"

#include "ControlPanelWidget.h"


typedef QVector<QPointF>         rpc_send_points_vector;
typedef QByteArray               rpc_send_atlant_data;


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
    
private:
	Ui::MainWindow*     ui;

    /// logger
	Pw::Logger::ILogger* m_logger;


private:
	void init();
};

Q_DECLARE_METATYPE(QPointF)
Q_DECLARE_METATYPE(rpc_send_points_vector)
Q_DECLARE_METATYPE(quint32)
Q_DECLARE_METATYPE(rpc_send_atlant_data)


#endif // MAINWINDOW_H
