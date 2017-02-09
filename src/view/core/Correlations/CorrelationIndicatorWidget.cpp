#include "CorrelationIndicatorWidget.h"

#include "ui_CorrelationIndicatorWidget.h"

#include "Logger/Logger.h"

#define PIXMAP_SIZE 32

CorrelationIndicatorWidget::CorrelationIndicatorWidget(QWidget *parent, Qt::WFlags flags):
	BaseCorrelationWidget(parent, flags),
    ui(new Ui::CorrelationIndicatorWidget)
{
	ui->setupUi(this);

	m_red = new QPixmap(":/images/bullet_red.png");
	m_green = new QPixmap(":/images/bullet_green.png");

    ui->labelIndicator->setFixedSize(PIXMAP_SIZE, PIXMAP_SIZE);
    ui->labelIndicator->setPixmap(m_red->scaled(PIXMAP_SIZE, PIXMAP_SIZE, Qt::KeepAspectRatio));

	//this->setFixedSize(50, 50);

    QString borderStyle = QString("QWidget#CorrelationIndicatorWidget{ \
                                    border: 2px solid black; \
                                    border-radius: 1; \
                                    }" );

    setStyleSheet(borderStyle);

	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(signalExpand()));
}

CorrelationIndicatorWidget::~CorrelationIndicatorWidget()
{
	delete m_red;
	delete m_green;
	//log_debug("<<<<<<");
}

void CorrelationIndicatorWidget::reset()
{

}

void CorrelationIndicatorWidget::setDataSetup(double bandwidth, int m_pointCount, float *spectrum, float *spectrum_peak_hold, float sko)
{

}

void CorrelationIndicatorWidget::permanentSetup(float *spectrum, float *spectrum_peak_hold, float sko)
{
	float skoM = sko* 3e8;
	if( skoM > m_limit ) {
        ui->labelIndicator->setFixedSize(PIXMAP_SIZE, PIXMAP_SIZE);
        ui->labelIndicator->setPixmap(m_red->scaled(PIXMAP_SIZE, PIXMAP_SIZE, Qt::KeepAspectRatio));
	} else {
        ui->labelIndicator->setFixedSize(PIXMAP_SIZE, PIXMAP_SIZE);
        ui->labelIndicator->setPixmap(m_green->scaled(PIXMAP_SIZE, PIXMAP_SIZE,Qt::KeepAspectRatio));
	}
}

void CorrelationIndicatorWidget::setupDoppler(const QQueue<double> &data)
{
	return;
}

void CorrelationIndicatorWidget::setLabel(const QString &label)
{
    QStringList labels = label.split(" - ");
    if(labels.size() > 1) {
        ui->labelTitle1->setText(labels.at(0));
        ui->labelTitle2->setText(labels.at(1));
    }
}

void CorrelationIndicatorWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	emit signalExpand();
}

bool CorrelationIndicatorWidget::isVisible() const
{
	return true;
}

void CorrelationIndicatorWidget::paintEvent(QPaintEvent *)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}


