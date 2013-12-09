#include "QSpectrItem.h"

#include <QDebug>

QSpectrItem::QSpectrItem(IGraphicWidget* graphic_widget, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      _action_KM(false)
{

    _graphic_widget = graphic_widget;

    prm300 = 0;

    adcIP = "127.0.0.1";
    adcPort = 1030;
    adcConnectFlag=false;
    prmIP="127.0.0.1";
    prmConnectFlag=false;
    prmFreq=0;
    prmAtt1=0;
    prmAtt2=0;

    mainAxePen = QPen(QColor(192,192,192,128));
    secondaryAxePen = QPen(QColor(96,96,96,128));
//	secondaryAxePen.setStyle(Qt::DashLine);
    spectrPen = QPen(QColor(Qt::green));
//	maxLevelPen = QPen(QColor(Qt::red));
    maxLevelPen = QPen(QColor(255,0,0,144));
    minLevelPen = QPen(QColor(Qt::blue));
    selectPen =  QPen(QColor(144,0,144,144));
    selectBrush = QBrush(QColor(128,0,128,128));
    freqPrmPen =  QPen(QColor(0,0,255,192));

    axeFont = QFont("Arial",100);
//	axeFont.setBold (true);

    numMainAxesX = MAX_AXES_X;
    numSecondaryAxesX = MAX_AXES_X*5;
    numMainAxesY = MAX_AXES_Y;
    numSecondaryAxesY = MAX_AXES_Y*5;

    stepX = STEP_X_MAX;
    stepY = STEP_Y_MAX;
    startViewFreq = (unsigned)FREQ_MIN;
    endViewFreq = startViewFreq+MAX_AXES_X*stepX;//(unsigned)FREQ_MAX;
    startViewLevel = (int)DB_MIN + MAX_AXES_Y*stepY;//(int)DB_MAX;
    endViewLevel = (int)DB_MIN;

    field_dn = FIELD_DN;
    field_up = FIELD_UP;
    field_lf = FIELD_LF;
    field_rt = FIELD_RT;

    flagMinLevel=false;
    flagMaxLevel=false;
    flagViewAll=false;
    minLevel = MIN_LEVEL;
    rSpectr = 0;

    isModeMinLevelMove = false;
    isModeAxeYChange = false;
    isModeAxeXChange = false;

    isSelect = false;
    isSelectMove = false;
    startSelectFreq = 0;
    endSelectFreq = 0;
    selectRect = QRectF(0,0,0,0);

    testFlag = false;

    connect(this, SIGNAL(signalGraphicContextMenu(const QPointF&)), this, SLOT(_slotShowGraphicContextMenu(const QPointF&)));
}

QSpectrItem::~QSpectrItem()
{
}

/// set rect for painting
QRectF QSpectrItem::boundingRect(void) const
{
    QPointF ptf(0,0);
//	QSizeF szf(abs((int)endViewFreq-(int)startViewFreq),abs((int)endViewLevel-(int)startViewLevel));
    QSizeF szf(scene()->sceneRect().width(),scene()->sceneRect().height());
    return QRectF(ptf,szf);
}

void QSpectrItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int i;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::TextAntialiasing);

    double height = boundingRect().height();
    double width = boundingRect().width();

    selfRectF.setRect(
        scene()->sceneRect().x() + (double)field_lf,
        scene()->sceneRect().y() + (double)field_up,
        scene()->sceneRect().width() - (double)field_rt - (double)field_lf,
        scene()->sceneRect().height() - (double)field_dn - (double)field_up);

    heightSC = scene()->sceneRect().height();
    widthSC = scene()->sceneRect().width();
    scaleX = widthSC/width;
    scaleY = heightSC/height;
    scaleSelfX = widthSC/selfRectF.width();
    scaleSelfY = heightSC/selfRectF.height();
    lf = field_lf/scaleX;
    rt = field_rt/scaleX;
    up = field_up/scaleY;
    dn = field_dn/scaleY;
    graphRectF.setRect(lf,up,width - rt - lf, height - dn - up);
    scaleLevel = graphRectF.height()/(startViewLevel - endViewLevel);
    scaleFreq = graphRectF.width()/(double)(endViewFreq - startViewFreq);

    scaleFont = (double)(DB_MAX-DB_MIN)*100/(startViewLevel - endViewLevel);
    scaleStretch = (double)(FREQ_MAX-FREQ_MIN)/(endViewFreq - startViewFreq);
    axeFont.setPointSizeF(FONT_POINT_SIZE);
    axeFont.setStretch(FONT_STRETCH_SIZE);
//	tempPt0 = axeFont.pointSizeF();
    QTransform qtransform;
    qtransform.scale(scaleX,scaleY);
    setTransform(qtransform);
//	tempPt1 = axeFont.stretch();
//	tempPt2 = (height-up-dn)/numMainAxesX;0;

/////////////  Calc Axes

    if(boundingRect().height()/MAX_AXES_Y < 20)
        numMainAxesY = MIN_AXES_Y;
    else
        numMainAxesY = MAX_AXES_Y;
    if(boundingRect().width()/MAX_AXES_X < 50)
        numMainAxesY = MIN_AXES_X;
    else
        numMainAxesX = MAX_AXES_X;

    painter->setPen(maxLevelPen);
    if(testFlag)
    {
        painter->drawLine(lf,height - dn, width - rt, up);
        painter->drawLine(lf,up,width - rt, height - dn);
    }

    painter->setPen(minLevelPen);
    painter->setFont(axeFont);
    QString s, s0, s1, s2;

    if(prmConnectFlag)
    {
        painter->setPen(spectrPen);
    }
    else
    {
        painter->setPen(maxLevelPen);
    }
    painter->setFont(axeFont);
    s = "Prm300=";
    s0 = prmIP;
    s += s0;
    if(prmConnectFlag)
    {
        s += "  частота = ";
        s0.setNum(prmFreq,10); s += s0; s += "МГц  атт1 = ";
        s0.setNum(prmAtt1); s += s0; s += "дБ  атт2 = ";
        s0.setNum(prmAtt2); s += s0; s += "дБ";
    }
    else
    {
        s += " нет связи";
    }
//// Test
//	s.setNum(startSelectFreq);
//	s += ":";
//	s0.setNum(endSelectFreq);
//	s += s0;
////
    painter->drawText(lf,up*0.9, s);

    if(adcConnectFlag)
    {
        painter->setPen(spectrPen);
    }
    else
    {
        painter->setPen(maxLevelPen);
    }
    painter->setFont(axeFont);
    adcIP = "127.0.0.1";
    s = "Завируха=";
    s0 = adcIP;
    s += s0;
    if(adcConnectFlag) s += "  коннект";
    else s += " нет связи";
    painter->drawText(lf,up*0.5, s);

    double step;
//	painter->setPen( secondaryAxePen);
//	if(numSecondaryAxesY)
//	{
//		step = (width-lf-rt)/numSecondaryAxesY;
//		for(i=0; i<numSecondaryAxesY+1; i++) painter->drawLine(lf+(double)i*step,up,lf+(double)i*step,height-up);
//	}
    painter->setPen(mainAxePen);
    step = (width - lf - rt)/numMainAxesY;
    QString qstr;
    for(i = 0; i < numMainAxesY + 1; i++)
    {
        painter->drawLine(lf + (double)i*step,up,lf + (double)i*step,height - dn);
    }
    rectAxeX.setRect(lf, height-dn, width-rt-lf, dn);
//	painter->setPen(maxLevelPen); // test mouse
//	painter->drawRect(rectAxeX);
    double viewFreqRange = endViewFreq - startViewFreq;
    double stepNum = viewFreqRange/numMainAxesY;
    painter->setFont(axeFont);
    int tempFreq = 0;
    for(i = 0; i < numMainAxesY + 1; i++)
    {
//		painter->setPen(mainAxePen); ////
        tempFreq=startViewFreq + stepNum*i;
        qstr.setNum(tempFreq*VIEW_LABEL_SCALE); //## 16.09.13 для учета в подписи 10КГц  //qstr.setNum(tempFreq);
//		qstr += "МГц";
        if(i % 2) painter->drawText(QRectF(lf + (double)(i - 1)*step, height - dn*0.9, step*1.8, dn), Qt::AlignHCenter, qstr);
        else painter->drawText(QRectF(lf + (double)(i - 1)*step,height - dn*0.5, step*1.8,dn), Qt::AlignHCenter, qstr);
//	painter->setPen(maxLevelPen); // test mouse
//	painter->drawRect(QRectF(lf+(double)(i-0.5)*step,height-dn,step*0.9,dn)); // test mouse
    }

//	painter->setPen( secondaryAxePen);
//	if(numSecondaryAxesX)
//	{
//		step = (height-up-dn)/numSecondaryAxesX;
//		for(i=0; i<numSecondaryAxesX+1; i++) painter->drawLine(lf, up+(double)i*step, width-lf, up+(double)i*step);
//	}
    painter->setPen(mainAxePen);
    step = (height - up - dn)/numMainAxesX;

    for(i = 0; i < numMainAxesX + 1; i++)
    {
        painter->drawLine(lf, up + (double)i*step, width - rt, up + (double)i*step);
    }
//	painter->setPen(maxLevelPen); // test mouse
//	painter->drawRect(rectAxeY); // test mouse
//	painter->setPen(mainAxePen);

    double viewLevelRange = endViewLevel - startViewLevel;
    stepNum = viewLevelRange/numMainAxesX;
    painter->setFont(axeFont);

    for(i = 0; i < numMainAxesX + 1; i++)
    {
//	painter->setPen(mainAxePen); ////
    qstr.setNum((startViewLevel + (int)stepNum*i));
        painter->drawText(QRectF(0, ((double)i - 0.45)*step + up, lf*0.9, 0.9*step), Qt::AlignRight|Qt::AlignVCenter, qstr);
        painter->drawText(QRectF(width - rt*0.9,((double)i - 0.45)*step + up, rt*0.8, 0.9*step), Qt::AlignRight|Qt::AlignVCenter, qstr);
//	painter->setPen(maxLevelPen); // test mouse
//	painter->drawRect(QRectF(0,((double)i-0.45)*step+up,lf*0.9,0.9*step)); // test mouse
    }
    rectAxeYLeft.setRect(0, up, lf, height - dn - up);
    rectAxeYRight.setRect(width - rt, up, width + lf + rt, height - dn - up);
///// Порог
    if(flagMinLevel)
    {
        minLevelPen.setWidthF(MIN_LEVEL_WIDTH_PX/scaleY);
        painter->setPen(minLevelPen);
        if((minLevel < startViewLevel)&&(minLevel>endViewLevel))
            painter->drawLine(lf, up - (minLevel - startViewLevel)*scaleLevel, width - rt, up - (minLevel - startViewLevel)*scaleLevel);

        rectMinLevel.setRect(lf, up - (minLevel - startViewLevel)*scaleLevel - MIN_LEVEL_HITSIZE_PX/scaleY/2, width - rt - lf, MIN_LEVEL_HITSIZE_PX/scaleY);
    }
///// Спектр
    double temp, temp1;
    if(rSpectr != 0)
    {
        step = (width - lf - rt)/(endViewFreq - startViewFreq);
        painter->setPen(spectrPen);
        int widthFreq = selfRectF.width();
        double scaleGetSpectr = (endViewFreq - startViewFreq)/selfRectF.width();
        for(i = 0; i < widthFreq; i++)
        {
            temp = rSpectr->GetPoint(startViewFreq + i*scaleGetSpectr);
            if(temp>startViewLevel)
            {
                temp = startViewLevel;
            }
            if(temp<endViewLevel)
            {
                temp = endViewLevel;
            }
            if(!flagViewAll)
            {
                if(temp<minLevel)
                {
                    temp = minLevel;
                }
            }
            temp1 = rSpectr->GetPoint(startViewFreq+(i+1)*scaleGetSpectr);
            if(temp1>startViewLevel)
            {
                temp1 = startViewLevel;
            }
            if(temp1<endViewLevel)
            {
                temp1 = endViewLevel;
            }
            if(!flagViewAll)
            {
                if(temp1<minLevel)
                {
                    temp1 = minLevel;
                }
            }
            painter->drawLine(lf + ((double)i*scaleGetSpectr)*scaleFreq, up - (temp - startViewLevel)*scaleLevel, lf + ((double)(i + 1.0)*scaleGetSpectr)*scaleFreq, up - (temp1 - startViewLevel)*scaleLevel);
        }
    }
/////// Частота
    if(prmConnectFlag)
    {
        painter->setPen(freqPrmPen);
        painter->drawLine(lf + (prmFreq - startViewFreq)*scaleFreq, up, lf + (prmFreq - startViewFreq)*scaleFreq, height - dn);
    }
    if(flagMaxLevel)
    {
        if(rSpectr != 0)
        {
            step = (width - lf - rt)/(endViewFreq - startViewFreq);
            painter->setPen(maxLevelPen);
            int widthFreq = selfRectF.width();
            double scaleGetSpectr = (endViewFreq - startViewFreq)/selfRectF.width();
            for(i = 0; i < widthFreq; i++)
            {
                temp = rSpectr->GetPointMax(startViewFreq + i*scaleGetSpectr);
                if(temp>startViewLevel)
                {
                    temp = startViewLevel;
                }
                if(temp<endViewLevel)
                {
                    temp = endViewLevel;
                }
                if(!flagViewAll)
                {
                    if(temp<minLevel)
                    {
                        temp = minLevel;
                    }
                }
                temp1 = rSpectr->GetPointMax(startViewFreq + (i + 1)*scaleGetSpectr);
                if(temp1>startViewLevel)
                {
                    temp1 = startViewLevel;
                }
                if(temp1 < endViewLevel)
                {
                    temp1 = endViewLevel;
                }
                if(!flagViewAll)
                {
                    if(temp1<minLevel)
                    {
                        temp1 = minLevel;
                    }
                }
                painter->drawLine(lf + ((double)i*scaleGetSpectr)*scaleFreq, up - (temp - startViewLevel)*scaleLevel, lf + ((double)(i + 1.0)*scaleGetSpectr)*scaleFreq, up - (temp1 - startViewLevel)*scaleLevel);
            }
        }
    }
    if(isSelect)
    {
        int st, end;
        painter->setPen(selectPen);
        painter->setBrush(selectBrush );
        if((endSelectFreq - startSelectFreq) > 0)
        {
            st = startSelectFreq;
            end = endSelectFreq;
        }
        else
        {
            st = endSelectFreq;
            end = startSelectFreq;
        }
        if(st > endViewFreq)
        {
            st=endViewFreq;
        }
        if(st < startViewFreq)
        {
            st=startViewFreq;
        }
        if(end > endViewFreq)
        {
            end=endViewFreq;
        }
        if(end < startViewFreq)
        {
            end=startViewFreq;
        }
        painter->drawRect(lf + (st - startViewFreq)*scaleFreq, up, (end - st)*scaleFreq, height - dn - up);
        selectRect = QRectF(lf + (st - startViewFreq)*scaleFreq, up, (end - st)*scaleFreq, height - dn - up);
    }
    painter->restore();
}

/// mouse events
void QSpectrItem::mousePressEvent(QGraphicsSceneMouseEvent *pe)
{
//	testFlag = true;
//	update( boundingRect() );
    QPointF pt = pe->pos();
    if(pe->button() == Qt::LeftButton)
    {
        if(flagMinLevel)
        {
            if(rectMinLevel.contains(pt))
            {
                isModeMinLevelMove = true;
            }
        }
        if(rectAxeYLeft.contains(pt) || rectAxeYRight.contains(pt))
        {
            isModeAxeYChange = true;
            ptfClickStart = pt;
            endViewLevelClick = endViewLevel;
        }
        if(rectAxeX.contains(pt))
        {
            isModeAxeXChange = true;
            ptfClickStart = pt;
            startViewFreqClick = startViewFreq;
        }
        if((graphRectF.contains(pt) && (!rectMinLevel.contains(pt))))
        {
            isSelect = true;
            isSelectMove = true;
            startSelectFreq = endSelectFreq = (int)((pt.x() - graphRectF.x())/scaleFreq) + startViewFreq;
        }
    }
    if(pe->button() == Qt::RightButton)
    {
        emit signalGraphicContextMenu(pt);
        /// must realesed context menu
    }
    if(pe->button() == Qt::MidButton )
    {
        /// ?
    }
    if(pe->button() == Qt::NoButton )
    {
        /// ?
    }
}

/// wheel zoom event
void QSpectrItem::wheelEvent( QGraphicsSceneWheelEvent * ev )
{
    QPointF pt = ev->pos();

    if(rectAxeYLeft.contains(pt) || rectAxeYRight.contains(pt))
    {
        // stepY = 5...1
        if(ev->delta()>0)
        {
            if(stepY>1)
            {
                endViewLevel += stepY*2;
                if(endViewLevel > DB_MIN + MAX_AXES_Y - 2)
                {
                    endViewLevel = DB_MIN + MAX_AXES_Y - 2;
                }
                stepY--;
                startViewLevel = endViewLevel + MAX_AXES_Y*stepY;
                update( boundingRect() );
            }
            else
            {
                endViewLevel = DB_MIN;
            }
        }
        else
        {
            if(stepY < STEP_Y_MAX)
            {
                stepY++;
                endViewLevel -= stepY*2;
                if(endViewLevel < DB_MIN)
                {
                    endViewLevel = DB_MIN;
                }
                startViewLevel = endViewLevel + MAX_AXES_Y*stepY;
                if(startViewLevel > DB_MAX)
                {
                    endViewLevel = DB_MAX - MAX_AXES_Y*stepY;
                    startViewLevel = endViewLevel + MAX_AXES_Y*stepY;
                }
                update( boundingRect() );
            }
            else
            {
                endViewLevel = DB_MIN;
            }
        }
    }

    if(rectAxeX.contains(pt))
    {
        if(ev->delta() > 0)
        {
            if(stepX > 1)
            {
                endViewFreq -= stepX;
                if(stepX <= 10)
                {
                    stepX--;
                }
                else
                {
                    stepX-=10;
                }
                if(stepX<0)
                {
                    stepX=1; ///## 16/09/13
                }
                startViewFreq = endViewFreq - MAX_AXES_X*stepX;
                if(startViewFreq < FREQ_MIN)
                {
                    startViewFreq=FREQ_MIN;
                }
                endViewFreq = startViewFreq + MAX_AXES_X*stepX;
                if(endViewFreq > FREQ_MAX)
                {
                    startViewFreq = FREQ_MAX - MAX_AXES_X*stepX;
                    endViewFreq = startViewFreq + MAX_AXES_X*stepX;
                }
                update( boundingRect() );
            }
        }
        else
        {
            if(stepX < STEP_X_MAX)
            {
                endViewFreq += stepX;
                if(stepX < 10)
                {
                    stepX++;
                }
                else
                {
                    stepX += 10;
                }
                startViewFreq = endViewFreq - MAX_AXES_X*stepX;
                if((int)startViewFreq < (int)FREQ_MIN)
                {
                    startViewFreq = FREQ_MIN;
                }
                endViewFreq = startViewFreq + MAX_AXES_X*stepX;
                if(endViewFreq > FREQ_MAX)
                {
                    startViewFreq=FREQ_MAX-MAX_AXES_X*stepX;
                    endViewFreq = FREQ_MAX;
                }
                update( boundingRect() );
            }
        }
    }
    ev->accept();
}

/// release mouse (have to translate struct to tree)
void QSpectrItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *pe)
{

    /// to tree and database
    double sel;
    if(startSelectFreq >= endSelectFreq)
    {
        sel = startSelectFreq - endSelectFreq;
    }
    else
    {
        sel = endSelectFreq - startSelectFreq;
    }
    QVector<double> data;
    data.push_back(sel);
    data.push_back(sel/2 + startSelectFreq);
    data.push_back(startSelectFreq);
    data.push_back(endSelectFreq);
    _graphic_widget->set(data);
    ///

    if(isModeMinLevelMove)
    {
        isModeMinLevelMove = false;
    }
    if(isModeAxeYChange)
    {
        isModeAxeYChange = false;
    }
    if(isModeAxeXChange)
    {
        isModeAxeXChange = false;
    }
    if(isSelectMove)
    {
        isSelectMove = false;
    }
    if((startSelectFreq == endSelectFreq) && isSelect)
    {
        isSelect = false;
    }
    update( boundingRect() );
}

/// move mouse
void QSpectrItem::mouseMoveEvent(QGraphicsSceneMouseEvent *pe)
{
    if(isModeMinLevelMove)
    {
        QPointF pt = pe->pos();
        minLevel = (int)graphRectF.y()/scaleLevel - (int)pt.y()/scaleLevel + startViewLevel;
        if(minLevel > startViewLevel)
        {
            minLevel = startViewLevel;
        }
        if(minLevel < endViewLevel)
        {
            minLevel=endViewLevel;
        }
        update( boundingRect() );
    }
    if(isModeAxeYChange)
    {
        QPointF pt = pe->pos();
        endViewLevel = endViewLevelClick - (ptfClickStart.y() - pt.y())/scaleLevel;
        if(endViewLevel > DB_MAX - MAX_AXES_Y*stepY)
        {
            endViewLevel = DB_MAX - MAX_AXES_Y*stepY;
        }
        if(endViewLevel < DB_MIN)
        {
            endViewLevel = DB_MIN;
        }

        startViewLevel = endViewLevel + MAX_AXES_Y*stepY;
        update( boundingRect() );
    }
    if(isModeAxeXChange)
    {
        QPointF pt = pe->pos();
        startViewFreq = startViewFreqClick + (ptfClickStart.x() - pt.x())/scaleFreq;
                if((int)startViewFreq < (int)FREQ_MIN)
                {
                    startViewFreq = FREQ_MIN;
                }
                endViewFreq = startViewFreq + MAX_AXES_X*stepX;
                if(endViewFreq > FREQ_MAX)
                {
                    startViewFreq = FREQ_MAX - MAX_AXES_X*stepX;
                    endViewFreq = FREQ_MAX;
                }
        update( boundingRect() );
    }
    if(isSelectMove)
    {
        QPointF pt = pe->pos();
        endSelectFreq = (int)((pt.x() - graphRectF.x())/scaleFreq) + startViewFreq;
        if(endSelectFreq < startViewFreq)
        {
            endSelectFreq = startViewFreq;
        }
        if(endSelectFreq > endViewFreq)
        {
            endSelectFreq = endViewFreq;
        }
        update( boundingRect() );
    }
}

void QSpectrItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *pe)
{
//	testFlag = false;
//	update( boundingRect() );
    QPointF pt = pe->pos();
    if(pe->button() == Qt::RightButton)
    {
        if(selectRect.contains(pt))
        {
            // Сохранение в базу
        }
    }
    if(pe->button() == Qt::LeftButton)
    {
        if(prmCtrlRect.contains(pt))
        {
            // Ctrl Att Prm
        }
        if(graphRectF.contains(pt))
        {
            if(prm300!=0)
            {
                ///????????
//                prm300->SendSetFreq((unsigned short)((pt.x()-graphRectF.x())/scaleFreq)+startViewFreq);
//                prm300->SendSetFreqADC((unsigned short)((pt.x()-graphRectF.x())/scaleFreq)+startViewFreq);
//                prm300->SendGetFreqAtt();
            }
        }
    }
}

void QSpectrItem::EnableMinLevel(bool b)
{
    flagMinLevel = b;
    update( boundingRect() );
}
void QSpectrItem::EnableMaxLevel(bool b)
{
    flagMaxLevel = b;
    update( boundingRect() );
}

void QSpectrItem::EnableViewAll(bool b)
{
    flagViewAll = b;
    update( boundingRect() );
}

void QSpectrItem::SetRSpectr(RSpectr *r)
{
    rSpectr = r;
}

void QSpectrItem::SetPrm300Widget(GraphicWidget *w)
{
    prm300 = w;
}

void QSpectrItem::SetPrmData(QString ip, bool connect, int freq, int at1, int at2)
{
    prmIP = ip;
    prmConnectFlag = connect;
    prmFreq = freq;
    prmAtt1 = at1;
    prmAtt2 = at2;
}

void QSpectrItem::SetADCData(QString ip, quint16 port, bool connect)
{
    adcIP = ip;
    adcPort = port;
    adcConnectFlag = connect;
}


/// show context menu og graphic
void QSpectrItem::_slotShowGraphicContextMenu(const QPointF& pos)
{
    QGraphicsView *v = this->scene()->views().first();
    QPointF globalPos = v->viewport()->mapToGlobal(pos.toPoint());

    QMenu myMenu;
    myMenu.addAction(tr("В черный список"), this, SLOT(_slotAddToBlackList()));
    myMenu.addAction(tr("В белый список"), this, SLOT(_slotAddToWhiteList()));
    myMenu.addAction(tr("Распознать сигнал"), this, SLOT(_slotRecognizeSignal()));
    QString km;
    if(_action_KM == false)
        km = tr("Начать координатометрию");
    else
        km = tr("Остановить координатометрию");

    myMenu.addAction(km, this, SLOT(_slotKM()));


    QAction* selectedItem = myMenu.exec(globalPos.toPoint());
    if (selectedItem)
    {
        qDebug() << "SHOW CONTEXT MENU" << selectedItem->text();
    }
    else
    {
        // nothing was chosen
    }
}


void QSpectrItem::_slotAddToBlackList()
{
    CommandMessage *msg = new CommandMessage(COMMAND_TOBLACKLIST, QVariant());
    _graphic_widget->set_command(msg);
    qDebug() << "BLACK";
}

void QSpectrItem::_slotAddToWhiteList()
{
    CommandMessage *msg = new CommandMessage(COMMAND_TOWHITELIST, QVariant());
    _graphic_widget->set_command(msg);
    qDebug() << "WHITE";

}

void QSpectrItem::_slotRecognizeSignal()
{
    CommandMessage *msg = new CommandMessage(COMMAND_RECOGNIZESIGNAL, QVariant());
    _graphic_widget->set_command(msg);
    qDebug() << "RECOGNIZE";

}

void QSpectrItem::_slotKM()
{
    CommandMessage *msg = new CommandMessage(COMMAND_KM, _action_KM);
    _graphic_widget->set_command(msg);

    qDebug() << "KM";
    _action_KM = !_action_KM;

}
