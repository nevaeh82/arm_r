#include "MainWindow.h"
#include "ui_MainWindow.h"

#define SERVER_NAME "ARM_R_Server"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	log_info("Started");

	this->setWindowTitle(tr("Zaviruha"));
	setWindowIcon(QIcon(":/images/icons/ARM_R.png"));



	QLineSeries *series = new QLineSeries();
	for (int i = 0; i < 500; i++) {
		QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);
		p.ry() += qrand() % 20;
		*series << p;
	}
//![1]

	Chart *chart = new Chart();
	chart->addSeries(series);
	chart->setTitle("Zoom in/out example");
	chart->setAnimationOptions(QChart::SeriesAnimations);
	chart->legend()->hide();
	chart->createDefaultAxes();

	ui->widget->resize(400, 300);
	ui->widget->grabGesture(Qt::PanGesture);
	ui->widget->grabGesture(Qt::PinchGesture);

	ChartView *chartView = new ChartView(chart, ui->widget);
	chartView->setRenderHint(QPainter::Antialiasing);


	this->showMaximized();

	init();
}

MainWindow::~MainWindow()
{
	delete ui;
}

ControlPanelWidget *MainWindow::getControlPanelWidget() const
{
	return ui->controlWidget;
}

QTabWidget *MainWindow::getWorkTabsWidget() const
{
	return ui->worlTabsWidget;
}

QStackedWidget* MainWindow::getStackedWidget() const
{
	return ui->stackedWidget;
}

void MainWindow::init()
{
	connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui->actionLists, SIGNAL(triggered()), this, SIGNAL(signalShowLists()));
	connect(ui->actionSolverLogs, SIGNAL(triggered()), this, SIGNAL(signalShowSolverLog()));
	connect(ui->actionSolverErrors, SIGNAL(triggered()), this, SIGNAL(signalShowSolverErrors()));

	connect(ui->actionRestartServer, SIGNAL(triggered()), this, SIGNAL(signalResetSerevr()));


}
