#include "GraphicWidget.h"

GraphicWidget::GraphicWidget(QString ipprm300, QString ipadc, quint16 portadc, ITabSpectrum *tab, QWidget *parent)
    : QWidget( parent )
{

    _tab = tab;
//    this->setLayout(parent->layout());
//    _tab_property = prop;
    ipPrm300 = ipprm300;//_tab_property->get_ip_prm300();

    ipAdc = ipadc;//_tab_property->get_ip_adc();
    portAdc = portadc;//_tab_property->get_port_adc();

//    int i;
 //   resize( QSize(500, 400).expandedTo(minimumSizeHint()) );
//    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QRect widgetRect = parent->rect();
    qDebug() << parent->width() << parent->height();
    scene = new QGraphicsScene(QRectF(widgetRect));// (0, 0, 500, 300));
    scene->setBackgroundBrush(Qt::black);

    rSpectr = new RSpectr;

    rSpectr->CreateTestData(); // Только для отладки графики заполнение случайными числами всего массива
//    rSpectr->CreateTestData(); // Только для отладки графики заполнение случайными числами всего массива
//    rSpectr->CreateTestData(); // Только для отладки графики заполнение случайными числами всего массива
//    rSpectr->CreateTestData(); // Только для отладки графики заполнение случайными числами всего массива

    qSpectrItem = new QSpectrItem(this);
    qSpectrItem->SetRSpectr(rSpectr);
    qSpectrItem->SetPrm300Widget(this);

    scene->addItem(qSpectrItem);
    qSpectrItem->setPos(0,0);

    qSpectrView = new QSpectrView(scene);

    qHBoxLayout = new QHBoxLayout(this);
    qVBoxLayout = new QVBoxLayout(this);
    qHBoxLayout->addWidget(qSpectrView);

    qHBoxLayout->addLayout(qVBoxLayout);
//	qHBoxLayout->addStretch(1);

    cbMinLevel = new QCheckBox(QString("Порог"));
    cbMaxHistLevel = new QCheckBox(QString("Макс. уровень"));
    cbAllView = new QCheckBox(QString("Показать все"));
    pbResetMax = new QPushButton(QString("Сброс Макс"));
    pbAttUp = new QPushButton(QString("Атт1 +"));
    pbAttDn = new QPushButton(QString("Атт1 -"));
    pbAtt2Up = new QPushButton(QString("Атт2 +"));
    pbAtt2Dn = new QPushButton(QString("Атт2 -"));
    pbStartScan = new QPushButton(QString("Старт скан"));
    pbStopScan = new QPushButton(QString("Стоп скан"));

    qVBoxLayout->addWidget(cbAllView);
    qVBoxLayout->addWidget(cbMinLevel);
    qVBoxLayout->addWidget(cbMaxHistLevel);
    qVBoxLayout->addWidget(pbResetMax);
    qVBoxLayout->addStretch(1);
    qVBoxLayout->addWidget(pbAttUp);
    qVBoxLayout->addWidget(pbAttDn);
    qVBoxLayout->addWidget(pbAtt2Up);
    qVBoxLayout->addWidget(pbAtt2Dn);
    qVBoxLayout->addWidget(pbStartScan);
    qVBoxLayout->addWidget(pbStopScan);

//    this->setLayout(qHBoxLayout);

/// 	setCacheMode(QGraphicsView::CacheBackground);
///	setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing,true);

    ///////////////////////////////
    // RPC have to realese
    ///////////////////////////////
//    qSocketPRM = new QTcpSocket(this);
//    qSocketPRM->connectToHost(QHostAddress(ipPRM300), RAD_PORT);
//    qSocketPRM->setReadBufferSize(1544);

//    qSocketADC = new QTcpSocket(this);
//    qSocketADC->connectToHost(QHostAddress(ipADC), portADC);


//	qSocketADC->setReadBufferSize(1544);

    ///????????
//    connect(qSocketPRM, SIGNAL(connected()), SLOT(slotIpConnectedPRM()));
//    connect(qSocketADC, SIGNAL(connected()), SLOT(slotIpConnectedADC()));
//    connect(qSocketPRM, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotIpErrorPRM()));
//    connect(qSocketADC, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotIpErrorADC()));
//    connect(qSocketADC, SIGNAL(readyRead()), SLOT(slotReadyReadADC()));

    connect(cbMinLevel, SIGNAL( clicked() ), SLOT( slotMinLevel() ) );
    connect(cbMaxHistLevel, SIGNAL( clicked() ), SLOT( slotMaxHistLevel() ) );
    connect(cbAllView, SIGNAL( clicked() ), SLOT( slotAllView() ) );
    connect(pbResetMax, SIGNAL( clicked() ), SLOT( slotResetMax() ) );

    connect(pbAttUp, SIGNAL( clicked() ), SLOT( slotAttUp() ) );
    connect(pbAttDn, SIGNAL( clicked() ), SLOT( slotAttDn() ) );
    connect(pbAtt2Up, SIGNAL( clicked() ), SLOT( slotAtt2Up() ) );
    connect(pbAtt2Dn, SIGNAL( clicked() ), SLOT( slotAtt2Dn() ) );
    connect(pbStartScan, SIGNAL( clicked() ), SLOT( slotStartScan() ) );
    connect(pbStopScan, SIGNAL( clicked() ), SLOT( slotStopScan() ) );

    //////////////////////////////////////////////////
    // /RPC
    //////////////////////////////////////////////////
    startTimer(55);
    Time = QTime::currentTime();
/// PrmPorotocol
    isModify=true;
    isScan=false;
    scanPause = SCAN_PAUSE;
////////////
//	m_level.SetStartFinish(FREQ_MIN,FREQ_MAX);
    m_att=0;
    m_att2=0;
    stateSerial=0;
    counterIn=0;
    counterOut=0;
    isRcv=0;// false
    dirUp=true;
////////////
    m_numOfSend=5;
////////////
    statBad=statAll=statGood=statSend=0;
///
// Из Init
    RecvMsgBuff.Init();
    SendMsgBuff.Init();

    isConnectedPRM = false;
    isConnectedADC = false;
    qSpectrItem->SetPrmData(ipPrm300, isConnectedPRM, m_frequ, 0, 0);
    qSpectrItem->SetADCData(ipAdc, portAdc, isConnectedADC);

//	LoadDataInit();
//	freqPRM=20;
    blockSize=0;
    newData=true;
}

GraphicWidget::~GraphicWidget()
{
    emit signalFinished();
    delete rSpectr;
}

void GraphicWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << event;
//	QRectF widgetRect = qSpectrView->childrenRect();
    qDebug() << "parent rect = " << this->width() << this->height();
    QRectF widgetRect = qSpectrView->rect();
    scene->setSceneRect(widgetRect);
    scene->update();
}

void GraphicWidget::mousePressEvent(QMouseEvent *)
{
//	QRectF widgetRect = qSpectrView->childrenRect();
    QRectF widgetRect = qSpectrView->rect();
    scene->setSceneRect(widgetRect);
    scene->update();
}

/// to server
 void GraphicWidget::slotReadyReadADC()
 {
 }

///// Завируха
 /// to server set freq ADC
void GraphicWidget::SendSetFreqADC(unsigned short aFreq)
{
}

/// to server
void GraphicWidget::SendSetFiltrADC(int aFiltr)
{
}

/// to server
void GraphicWidget::SendSetShift(float aShift)
{
}

/// to server
void GraphicWidget::SendStartADC(bool aIsStart)
{
}



void GraphicWidget::slotMinLevel()
{
    if(cbMinLevel->isChecked())
    {
        qSpectrItem->EnableMinLevel(true);
    }
    else
    {
        qSpectrItem->EnableMinLevel(false);
    }
}

void GraphicWidget::slotMaxHistLevel()
{
    if(cbMaxHistLevel->isChecked())
    {
        qSpectrItem->EnableMaxLevel(true);
    }
    else
    {
        qSpectrItem->EnableMaxLevel(false);
    }
}

void GraphicWidget::slotAllView()
{
    if(cbAllView->isChecked())
    {
        qSpectrItem->EnableViewAll(true);
    }
    else
    {
        qSpectrItem->EnableViewAll(false);
    }
}

void GraphicWidget::slotResetMax()
{
    rSpectr->ResetMax();
    scene->update();
}

void GraphicWidget::slotAttUp()
{
    SendSetAtt(1);
    SendGetFreqAtt();
}

void GraphicWidget::slotAttDn()
{
    SendSetAtt(0);
    SendGetFreqAtt();
}

void GraphicWidget::slotAtt2Up()
{
    if(m_att2 < 15)
    {
        SendSetAtt2(m_att2 + 1);
    }
    SendGetFreqAtt();
}

void GraphicWidget::slotAtt2Dn()
{
    if(m_att2 > 0)
    {
        SendSetAtt2(m_att2-1);
    }
    SendGetFreqAtt();
}

void GraphicWidget::slotStartScan()
{
    isScan=true;
}

void GraphicWidget::slotStopScan()
{
    isScan=false;
}

/// change indecation about connect to PRM
void GraphicWidget::slotIpConnectedPRM()
{
    isConnectedPRM = true;
    qSpectrItem->SetPrmData(ipPrm300, isConnectedPRM, m_frequ, 0, 0);
    qSpectrItem->SetADCData(ipAdc, portAdc, isConnectedADC);
    scene->update();
}

/// change indecation about connect to ADC
void GraphicWidget::slotIpConnectedADC()
{
    isConnectedADC = true;
    SendStartADC(true);
    qSpectrItem->SetPrmData(ipPrm300, isConnectedPRM, m_frequ, 0, 0);
    qSpectrItem->SetADCData(ipAdc, portAdc, isConnectedADC);
    scene->update();
}

void GraphicWidget::set_command(IMessage *msg)
{
    _tab->set_command(msg);
}

/// change indecation about connect to PRM (disconnected)
void GraphicWidget::slotIpErrorPRM()
{
    isConnectedPRM = false;
    qSpectrItem->SetPrmData(ipPrm300, isConnectedPRM, m_frequ, 0, 0);
    qSpectrItem->SetADCData(ipAdc, portAdc, isConnectedADC);
    scene->update();
}

/// change indecation about connect to ADC (disconnected)
void GraphicWidget::slotIpErrorADC()
{
    isConnectedADC = false;
    qSpectrItem->SetPrmData(ipPrm300, isConnectedPRM, m_frequ, 0, 0);
    qSpectrItem->SetADCData(ipAdc, portAdc, isConnectedADC);
    scene->update();

}

void GraphicWidget::timerEvent(QTimerEvent *)
{
    if(isScan)
    {
        if(Time.msecsTo(QTime::currentTime())>=scanPause)
        {
            Time = QTime::currentTime();
            m_frequ += SCAN_STEP;
            if(m_frequ >= FREQ_MAX)
            {
                m_frequ=scanMin; //## 16.09.13 //FREQ_MIN;
            }
            SendSetFreq(m_frequ);
            SendGetFreqAtt();
        }
    }
    Deal();
}

void GraphicWidget::Deal()
{
    Protocol();
}

/// to server
void GraphicWidget::SendGetFreqAtt()
{
}

/// to server
void GraphicWidget::SendSetAtt(unsigned char att)
{
}

/// to server
void GraphicWidget::SendSetAtt2(unsigned char att)
{
}

/// to server
void GraphicWidget::SendSetFreq(unsigned short freq)
{
}

/// from server ?
void GraphicWidget::Protocol()
{
}


void GraphicWidget::set(QVector<double> data)
{
    _tab->set_selected_area(data);
}

