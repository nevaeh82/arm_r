#include "GraphicWidget.h"

SpectrumWidget::SpectrumWidget(QString ipprm300, QString ipadc, quint16 portadc, ITabSpectrum *tab, QWidget *parent)
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

    rSpectr->CreateTestData(); // ������ ��� ������� ������� ���������� ���������� ������� ����� �������
//    rSpectr->CreateTestData(); // ������ ��� ������� ������� ���������� ���������� ������� ����� �������
//    rSpectr->CreateTestData(); // ������ ��� ������� ������� ���������� ���������� ������� ����� �������
//    rSpectr->CreateTestData(); // ������ ��� ������� ������� ���������� ���������� ������� ����� �������

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

    cbMinLevel = new QCheckBox(QString("�����"));
    cbMaxHistLevel = new QCheckBox(QString("����. �������"));
    cbAllView = new QCheckBox(QString("�������� ���"));
    pbResetMax = new QPushButton(QString("����� ����"));
    pbAttUp = new QPushButton(QString("���1 +"));
    pbAttDn = new QPushButton(QString("���1 -"));
    pbAtt2Up = new QPushButton(QString("���2 +"));
    pbAtt2Dn = new QPushButton(QString("���2 -"));
    pbStartScan = new QPushButton(QString("����� ����"));
    pbStopScan = new QPushButton(QString("���� ����"));

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
// �� Init
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

SpectrumWidget::~SpectrumWidget()
{
    emit signalFinished();
    delete rSpectr;
}

void SpectrumWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << event;
//	QRectF widgetRect = qSpectrView->childrenRect();
    qDebug() << "parent rect = " << this->width() << this->height();
    QRectF widgetRect = qSpectrView->rect();
    scene->setSceneRect(widgetRect);
    scene->update();
}

void SpectrumWidget::mousePressEvent(QMouseEvent *)
{
//	QRectF widgetRect = qSpectrView->childrenRect();
    QRectF widgetRect = qSpectrView->rect();
    scene->setSceneRect(widgetRect);
    scene->update();
}

/// to server
 void SpectrumWidget::slotReadyReadADC()
 {
 }

///// ��������
 /// to server set freq ADC
void SpectrumWidget::SendSetFreqADC(unsigned short aFreq)
{
}

/// to server
void SpectrumWidget::SendSetFiltrADC(int aFiltr)
{
}

/// to server
void SpectrumWidget::SendSetShift(float aShift)
{
}

/// to server
void SpectrumWidget::SendStartADC(bool aIsStart)
{
}



void SpectrumWidget::slotMinLevel()
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

void SpectrumWidget::slotMaxHistLevel()
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

void SpectrumWidget::slotAllView()
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

void SpectrumWidget::slotResetMax()
{
    rSpectr->ResetMax();
    scene->update();
}

void SpectrumWidget::slotAttUp()
{
    SendSetAtt(1);
    SendGetFreqAtt();
}

void SpectrumWidget::slotAttDn()
{
    SendSetAtt(0);
    SendGetFreqAtt();
}

void SpectrumWidget::slotAtt2Up()
{
    if(m_att2 < 15)
    {
        SendSetAtt2(m_att2 + 1);
    }
    SendGetFreqAtt();
}

void SpectrumWidget::slotAtt2Dn()
{
    if(m_att2 > 0)
    {
        SendSetAtt2(m_att2-1);
    }
    SendGetFreqAtt();
}

void SpectrumWidget::slotStartScan()
{
    isScan=true;
}

void SpectrumWidget::slotStopScan()
{
    isScan=false;
}

/// change indecation about connect to PRM
void SpectrumWidget::slotIpConnectedPRM()
{
    isConnectedPRM = true;
    qSpectrItem->SetPrmData(ipPrm300, isConnectedPRM, m_frequ, 0, 0);
    qSpectrItem->SetADCData(ipAdc, portAdc, isConnectedADC);
    scene->update();
}

/// change indecation about connect to ADC
void SpectrumWidget::slotIpConnectedADC()
{
    isConnectedADC = true;
    SendStartADC(true);
    qSpectrItem->SetPrmData(ipPrm300, isConnectedPRM, m_frequ, 0, 0);
    qSpectrItem->SetADCData(ipAdc, portAdc, isConnectedADC);
    scene->update();
}

void SpectrumWidget::set_command(IMessage *msg)
{
    _tab->set_command(msg);
}

/// change indecation about connect to PRM (disconnected)
void SpectrumWidget::slotIpErrorPRM()
{
    isConnectedPRM = false;
    qSpectrItem->SetPrmData(ipPrm300, isConnectedPRM, m_frequ, 0, 0);
    qSpectrItem->SetADCData(ipAdc, portAdc, isConnectedADC);
    scene->update();
}

/// change indecation about connect to ADC (disconnected)
void SpectrumWidget::slotIpErrorADC()
{
    isConnectedADC = false;
    qSpectrItem->SetPrmData(ipPrm300, isConnectedPRM, m_frequ, 0, 0);
    qSpectrItem->SetADCData(ipAdc, portAdc, isConnectedADC);
    scene->update();

}

void SpectrumWidget::timerEvent(QTimerEvent *)
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

void SpectrumWidget::Deal()
{
    Protocol();
}

/// to server
void SpectrumWidget::SendGetFreqAtt()
{
}

/// to server
void SpectrumWidget::SendSetAtt(unsigned char att)
{
}

/// to server
void SpectrumWidget::SendSetAtt2(unsigned char att)
{
}

/// to server
void SpectrumWidget::SendSetFreq(unsigned short freq)
{
}

/// from server ?
void SpectrumWidget::Protocol()
{
}


void SpectrumWidget::set(QVector<double> data)
{
    _tab->set_selected_area(data);
}

