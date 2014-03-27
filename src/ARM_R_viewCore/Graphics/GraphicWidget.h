#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/QtNetwork>
#include <QTime>

//#include "../Tabs/TabsProperty.h"

#include "QSpectrView.h"
#include "QSpectrItem.h"
#include "Structs.h"
#include "Templates.h"
#include "PacketStruct.h"

#include "../Tabs/ITabSpectrum.h"

#include "IGraphicWidget.h"

class GraphicWidget: public QWidget, public IGraphicWidget
{
    Q_OBJECT
public:
    GraphicWidget(QString ipprm300, QString ipadc, quint16 portadc, ITabSpectrum *tab, QWidget* parent = 0);
    ~GraphicWidget();

private:
    ITabSpectrum    *_tab;

public:
    virtual void set(QVector<double> data);

public:

    void timerEvent(QTimerEvent *);
    void Deal();
    void mousePressEvent(QMouseEvent *) ;
    void resizeEvent(QResizeEvent *);
///// Prm300 Ctrl
    void SendGetState();
    void SendSetFreq(unsigned short);
    void SendSetAtt(unsigned char);
    void SendSetAtt2(unsigned char);
    void SendGetFreqAtt();
    void SendSetReject(unsigned char);
    void SendSetFiltr(unsigned char);

    void SetScanMaxMin(double scan_min, double scan_max); //## 16.09.13 в МГц

    bool IsScan() { return isScan; }
    void StartScan() { isScan = true; }
    void StopScan() { isScan = false; }
    unsigned char Att1() { return m_att; }
    unsigned char Att2() { return m_att2; }
    unsigned short Freq() { return m_frequ; }
///// Завируха
    void SendSetFreqADC(unsigned short);
    void SendSetFiltrADC(int aFiltr);
    void SendSetShift(float aShift);
    void SendStartADC(bool aIsStart);
protected:
    QHBoxLayout* qHBoxLayout;
    QVBoxLayout* qVBoxLayout;

private:

    double scanMin; //## 16.09.13  в МГц
    double scanMax; //## 16.09.13   в МГц

    QString ipPrm300;
    QString ipAdc;
    quint16 portAdc;

    QSpectrView *qSpectrView;
    QCheckBox *cbMinLevel;
    QCheckBox *cbMaxHistLevel;
    QCheckBox *cbAllView;
    QPushButton *pbResetMax;
    QPushButton *pbAttUp;
    QPushButton *pbAttDn;
    QPushButton *pbAtt2Up;
    QPushButton *pbAtt2Dn;
    QPushButton *pbStartScan;
    QPushButton *pbStopScan;

    QGraphicsScene *scene;
    QSpectrItem *qSpectrItem;

    QTcpSocket *qSocketPRM;
    QTcpSocket *qSocketADC;
// PrmCtrl
    unsigned short freqPRM;
// PrmPorotocol
    NovomarMsg novomarMsg;
    unsigned long counterIn;
    unsigned long counterOut;
    int mode5A;
    int isFree;
    int	isSendCommand;
    int isCtrlPresent;
    NvDataInit nvDataInit;

    QTime Time;
    unsigned scanPause;
    unsigned m_frequ;
    unsigned m_step;
    unsigned char m_att;
    unsigned char m_att2;
    unsigned m_numOfSend;

    unsigned long ireg;
    unsigned long ivalue;

    bool dirUp;
    bool isModify;
    bool isScan;
    bool isRcv;
    bool initFlag;
    long stateSerial;
    unsigned char incByte;

    long statSend;
    long statBad;
    long statGood;
    long statAll;

    TRBuffer<NovomarMsg,100> RecvMsgBuff;
    TRBuffer<NovomarMsg,100> SendMsgBuff;

    void Protocol();

    void SaveDataInit();
    void SendDataInit();
    void LoadDataInit();

    bool isConnectedPRM;
    bool isConnectedADC;
//////
    RSpectr *rSpectr;
///// Завируха
    unsigned int blockSize;
    int headerType;
    bool newData;
    QTime Time1;

private slots:
//// Управление приемником
    void slotIpConnectedPRM();
    void slotIpErrorPRM();
//// Управление отображением
    void slotMinLevel();
    void slotMaxHistLevel();
    void slotAllView();
    void slotResetMax();
    void slotAttUp();
    void slotAttDn();
    void slotAtt2Up();
    void slotAtt2Dn();
    void slotStartScan();
    void slotStopScan();
//// Завируха
    //! Слот чтения данных
    void slotReadyReadADC();
    //! Слот определения ошибок
///	void slotIpErrorADC(QAbstractSocket::SocketError);
    void slotIpErrorADC();
    //! Слот при подключении к серверу
    void slotIpConnectedADC();

public:
    virtual void set_command(IMessage *msg);

signals:
    void signalFinished();

//private:
//    TabsProperty    *_tab_property;
};

#endif // GRAPHICWIDGET_H
