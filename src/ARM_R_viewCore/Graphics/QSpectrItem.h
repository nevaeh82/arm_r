#ifndef QSPECTRITEM_H
#define QSPECTRITEM_H


#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>
#include <QFont>

#include <QtGlobal>

#include <QAction>
#include <QMenu>

#include "RSpectr.h"
//#include "GraphicWidget.h"

#include "IGraphicWidget.h"

#include "../Common/CommandMessage.h"

class SpectrumWidget;

class QSpectrItem : public QObject, public QGraphicsItem
{
    Q_OBJECT

private:
    SpectrumWidget *prm300;

    QString adcIP;
    bool adcConnectFlag;
    quint16 adcPort;
    QString prmIP;
    bool prmConnectFlag;
    int prmFreq;
    int prmAtt1;
    int prmAtt2;
    QRectF selfRectF;
    QRectF graphRectF;
    QPen mainAxePen;
    QPen secondaryAxePen;
    QPen spectrPen;
    QPen maxLevelPen;
    QPen minLevelPen;
    QPen selectPen;
    QBrush selectBrush;
    QPen freqPrmPen;
    QFont axeFont;
    QGraphicsSimpleTextItem statePRMText;
    QGraphicsSimpleTextItem axeYText[MAX_AXES_Y + 1];
    QGraphicsSimpleTextItem axeXText[MAX_AXES_X + 1];
    int numMainAxesX;
    int numSecondaryAxesX;
    int numMainAxesY;
    int numSecondaryAxesY;
/////
    double heightSC;
    double widthSC;
    double scaleX;
    double scaleY;
    double scaleSelfX;
    double scaleSelfY;
    double scaleFont;
    double scaleStretch;
    double scaleLevel;
    double scaleFreq;
    double lf;
    double rt;
    double up;
    double dn;
/////
    unsigned stepX;
    unsigned stepY;
    unsigned startViewFreq;
    unsigned endViewFreq;
/////##
    int startViewLevel;
    int endViewLevel;
/////
    unsigned field_dn;
    unsigned field_up;
    unsigned field_lf;
    unsigned field_rt;
/////
    bool flagMinLevel;
    bool flagMaxLevel;
    bool flagViewAll;
    int minLevel;
    QRectF rectMinLevel;
    QRectF rectAxeYLeft;
    QRectF rectAxeYRight;
    QRectF rectAxeX;
/////
    RSpectr *rSpectr;
////
    bool isModeMinLevelMove;
    bool isModeAxeYChange;
    bool isModeAxeXChange;
    QPointF ptfClickStart;
    int endViewLevelClick;
    int startViewFreqClick;
//// Select
    bool isSelect;
    bool isSelectMove;
    unsigned startSelectFreq;
    unsigned endSelectFreq;
    QRectF selectRect;
    QRectF prmCtrlRect;
//// Test
    bool testFlag;
    int temp;
    int tempPt0;
    int tempPt1;
    int tempPt2;
public:
    QSpectrItem(IGraphicWidget *graphic_widget, QGraphicsItem *parent = 0);
    ~QSpectrItem();
    QRectF boundingRect(void) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *pe);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *pe);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *pe);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *pe);
    virtual void wheelEvent ( QGraphicsSceneWheelEvent * event );

    void SetRSpectr(RSpectr *r);
    void SetPrm300Widget(SpectrumWidget *w);
    void SetPrmData(QString ip, bool connect, int freq, int at1, int at2);
    void SetADCData(QString ip, quint16 port, bool connect);

    void EnableMinLevel(bool);
    void EnableMaxLevel(bool);
    void EnableViewAll(bool);

private:
    IGraphicWidget  *_graphic_widget;

signals:
    void signalGraphicContextMenu(const QPointF&);

private slots:
    void _slotShowGraphicContextMenu(const QPointF&);

private:
    bool _action_KM;

private slots:
    void _slotAddToBlackList();
    void _slotAddToWhiteList();
    void _slotRecognizeSignal();
    void _slotKM();

};
#endif // QSPECTRITEM_H
