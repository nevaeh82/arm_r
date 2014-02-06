#include "../../../stdafx.h"
#include "spectrum_material.h"
#include "mg_spectrum_interface.h"
#include <QVBoxLayout>
#include <QtDebug>
#include <QMessageBox>
#include <QSettings>

//#define MG_STYLE1 ;
//#define MG_STYLE2 ;

Q_MG_SpectrumInterface::Q_MG_SpectrumInterface(QWidget *parent, QString wallPaperFileName) : QWidget(parent)
{
    _material2_visible = false;
	MainLayout = new QVBoxLayout(this);
	MainLayout->setSpacing(2);
	MainLayout->setContentsMargins(0, 0, 0, 0);
	align = 0;
	ZoomOutMaxOnDataSet = false;

	emitSignalOnProgrammChangeOffViewPort_ = false;
	haveRelations_ = false;

	TimerId = 0;
	if (!wallPaperFileName.isEmpty())
	{
		wallPaper.setPixmap(QPixmap(wallPaperFileName));
	}
	HorLabel = tr("Гц");
	InitMainGraf();
	
	

	MainLayout->insertWidget(0,MainGraf);
}


void Q_MG_SpectrumInterface::InitMainGraf()
{
	spectrumActivated_ = false;

	MainGraf = new Q_MultiGraphics(this);
	MainGraf->installEventFilter(this);
	MainGraf->SpectrumMode = true;
	MainGraf->VertStep = 1.15;
	MainGraf->HorStep = 1.15;

	material_1 = new SpectrumMaterial();
	connect(material_1,SIGNAL(sendCommand(QString,QString,QVariant)),MainGraf->InfoLayer,SLOT(getCommand(QString,QString,QVariant)));
	connect(material_1,SIGNAL(ViewPortChanged(double,double,double,double)),this,SLOT(on_ViewPortChanged(double,double,double,double)));

	material_2 = new SpectrumMaterial();
	//connect(material_1,SIGNAL(sendCommand(QString,QString,QVariant)),MainGraf->InfoLayer,SLOT(getCommand(QString,QString,QVariant)));
	//connect(material_1,SIGNAL(ViewPortChanged(double,double,double,double)),this,SLOT(on_ViewPortChanged(double,double,double,double)));

	//установка параметров компонента
	MainGraf->Grid_Layer->Hor_RoundValue = 3;
	MainGraf->Grid_Layer->Vert_RoundValue = 3;
	MainGraf->Grid_Layer->flag_GridAligment = 1;

	MainGraf->Grid_Layer->VertInterpretSum = 0;
	MainGraf->Grid_Layer->HorInterpretSum = 0;
	MainGraf->Grid_Layer->DrawHBaseLine = true;
	MainGraf->Grid_Layer->DrawVBaseLine = true;
	MainGraf->Grid_Layer->DrawVSubDiv	= true;
	MainGraf->Grid_Layer->DrawHSubDiv	= true;
	MainGraf->Grid_Layer->LabelHAxis = HorLabel;
	MainGraf->Grid_Layer->EnableKiloMath = true;

	MainGraf->MouseLayer->Hor_RoundValue = 4;
	MainGraf->MouseLayer->Vert_RoundValue = 4;
	MainGraf->MouseLayer->EnableKiloMath = true;
	MainGraf->MouseLayer->EnableInterpretAddLabel = true;
	MainGraf->MouseLayer->Hor_InterpretAddLabel = HorLabel;

#ifdef MG_STYLE1
	MainGraf->MouseLayer->showBigGrafCursor = true;
	MainGraf->MouseLayer->showMiniGrafCursor = false;
#else
	MainGraf->MouseLayer->showBigGrafCursor = false;
	MainGraf->MouseLayer->showMiniGrafCursor = true;	
	MainGraf->MouseLayer->showStrangeCross = false;
#ifdef MG_STYLE2
	MainGraf->MouseLayer->GlowEffect = false;
#endif
#endif

    MainGraf->MouseLayer->CurSelectionType = 1;
	MainGraf->MouseLayer->SetMultySelectEnabled(false);
	MainGraf->MouseLayer->ShowSelectText = true;
	MainGraf->MouseLayer->DrawTextDecorateMidLine = true;
	connect(MainGraf->MouseLayer,SIGNAL(selectionCleared()),this,SLOT(on_selectionCleared()));
	connect(MainGraf->MouseLayer,SIGNAL(deselectFinished()),this,SIGNAL(DeselectFinished()));
	connect(MainGraf->MouseLayer,SIGNAL(selectedPixelChange(double,double,double,double)),this,SLOT(on_selectedPixelChangeSpectrum(double,double,double,double)));
    connect(MainGraf->MouseLayer,SIGNAL(selectionFinished(double, double,double,double)),this,SLOT( on_selectionFinished(double, double,double,double)));
    connect(MainGraf->MouseLayer,SIGNAL(selectionFinishedRedLine(double)),this,SLOT( on_selectionFinishedRedLine(double)));

    connect(MainGraf->MouseLayer,SIGNAL(StrangeCrossMoved()),this,SIGNAL(CrossMoved()));
	connect(MainGraf->MouseLayer,SIGNAL(markerMooved(int,double,bool)),this,SLOT(on_markerMoved(int,double,bool)));
	connect(MainGraf->MouseLayer,SIGNAL(markersChanged(QList<double>,bool)),this,SLOT(on_markersChanged(QList<double>,bool)));
	connect(MainGraf->MouseLayer,SIGNAL(DoubleClicked(double,double)),SLOT(on_doubleClicked(double,double)));

	MainGraf->InfoLayer->Hor_RoundValue = 4;
	MainGraf->InfoLayer->Vert_RoundValue = 4;
	MainGraf->InfoLayer->EnableKiloMath = true;
	MainGraf->InfoLayer->EnableInterpretAddLabel = true;
	MainGraf->InfoLayer->Hor_InterpretAddLabel = HorLabel;

	QVector<InfoWidgetData> selectInfoData;
	selectInfoData.resize(8);
	selectInfoData[0].name = QObject::tr("Начало");
	selectInfoData[1].name = QObject::tr("Центр");
	selectInfoData[2].name = QObject::tr("Конец");
	selectInfoData[3].name = QObject::tr("Выделено");
	selectInfoData[4].name = QObject::tr("Макс.");
	selectInfoData[5].name = QObject::tr("Макс.частота");
	selectInfoData[6].name = QObject::tr("Мин.");
	selectInfoData[7].name = QObject::tr("Отсчетов");
	MainGraf->InfoLayer->setUserData(selectInfoData);

	//делаем пустой экран
    /*MainGraf->InfoLayer->setVisible(false);
	MainGraf->InfoLayer->setEnabled(false);
	MainGraf->Grid_Layer->setVisible(false);
	MainGraf->MouseLayer->setVisible(false);
    */

	MainGraf->MouseLayer->CopyMatrix(MainGraf->Grid_Layer);

	material_1->EnableKiloMath = true;
	material_1->Hor_RoundValue = 0;
	material_1->Vert_RoundValue = 0;
	material_1->EnableInterpretAddLabel = true;
	material_1->Hor_InterpretAddLabel = HorLabel;

	material_2->EnableKiloMath = true;
	material_2->Hor_RoundValue = 0;
	material_2->Vert_RoundValue = 0;
	material_2->EnableInterpretAddLabel = true;
	material_2->Hor_InterpretAddLabel = HorLabel;

	wasProgrammChangeOffViewPort_ = false;
	skip_signal_on_selection_change_ = false;
	skip_signal_on_selection_clear_ = false;

	//размещение обоев
	if (!wallPaper.pixmap().isNull())
	{
		wallPaper.setOpacity(0);
		MainGraf->setWallPaper(&wallPaper);
		if (TimerId != 0) killTimer(TimerId);
		TimerId = startTimer(100);
	}
		
	bandwidth_hz_ = 1;
}

Q_MG_SpectrumInterface::~Q_MG_SpectrumInterface()
{
	MainGraf->removeWallPaper();
	delete material_1;
	delete material_2;
}

bool Q_MG_SpectrumInterface::Setup(bool is_complex, double bandwidth_hz, QString vertical_dial_label, const float* spectrum_1, size_t items_count_1, const float* spectrum_2, size_t items_count_2, bool inline_memory_1, bool inline_memory_2, float minv_, float maxv_)
{
	spectrumActivated_ = false;
	if (bandwidth_hz == 0) return spectrumActivated_;
	if (items_count_1 == 0) return spectrumActivated_;
	
	bool EnabelSecondGraph = !((spectrum_2 == NULL) || (items_count_2 == 0));

	MaterialFloatData matData_1;
	MaterialFloatData matData_2;
	if (!inline_memory_1)
	{
		if (!matData_1.Allocate(static_cast<unsigned int>(items_count_1))) {QMessageBox::critical(this,"Memory allocation error","No memory");return spectrumActivated_ ;}
	}

	if (!inline_memory_2)
	{
		if (EnabelSecondGraph)
			if (!matData_2.Allocate(static_cast<unsigned int>(items_count_2))) {QMessageBox::critical(this,"Memory allocation error","No memory");return spectrumActivated_ ;}
	}

    float minv;float maxv;
    float minv_1;float maxv_1;
    float minv_2;float maxv_2;


	ippsMinMax_32f(spectrum_1,static_cast<int>(items_count_1),&minv_1,&maxv_1);


	if (!inline_memory_1)
		memcpy(matData_1.values,spectrum_1,items_count_1*sizeof(float));
	
	if (EnabelSecondGraph)
	{
        ippsMinMax_32f(spectrum_2,static_cast<int>(items_count_2),&minv_2,&maxv_2);
		if (!inline_memory_2)
			memcpy(matData_2.values,spectrum_2,items_count_2*sizeof(float));
		minv = qMin(minv_1,minv_2);
		maxv = qMax(maxv_1,maxv_2);
	}
	else
	{
		minv = minv_1;maxv = maxv_1;
	}
	

    if(minv_ == 0 && maxv_ == 1)
    {
        minv = minv_;
        maxv = maxv_;
    }


	double raznTop = (maxv-minv)*0.2;
	double raznBot = raznTop;
	switch (align)
	{
		case 1: raznTop = 0;break;
		case 2: raznBot = 0;break;
		case 3: raznTop = 0;raznBot = 0;break;
	}
		
//    if(minv_ == 0 && maxv_ == 0)
//    {
//    }
//    else
//    {
//        raznTop = 0;
//        raznBot = 0;
//    }
	MainGraf->Grid_Layer->Reset();
	InitSomeHiddenProperties();

	MainGraf->Grid_Layer->LabelVAxis = vertical_dial_label;

	MainGraf->MouseLayer->Vert_InterpretAddLabel = vertical_dial_label;
	MainGraf->InfoLayer->Vert_InterpretAddLabel = vertical_dial_label;
	material_1->Vert_InterpretAddLabel = vertical_dial_label;
	material_2->Vert_InterpretAddLabel = vertical_dial_label;

    is_complex_ = is_complex;
	bandwidth_hz_ = bandwidth_hz;
    if (is_complex_)
	{
        qreal ttt = bandwidth_hz_/2;
		MainGraf->Grid_Layer->setMinAndMaxTransPoint(QPointF(ttt,maxv+raznTop),QPointF(-ttt,minv-raznBot));
	}
    else MainGraf->Grid_Layer->setMinAndMaxTransPoint(QPointF(bandwidth_hz_,maxv+raznTop),QPointF(0,minv-raznBot));

	material_1->Hor_kf_inc = (items_count_1)/bandwidth_hz_;
	if (EnabelSecondGraph) material_2->Hor_kf_inc = (items_count_2)/bandwidth_hz_;
	
	MainGraf->Grid_Layer->MaximumZoom = QPointF(1/material_1->Hor_kf_inc*0.005, raznTop * 0.00001);

	MainGraf->MouseLayer->ClearAllSelections();
	MainGraf->MouseLayer->ClearAllMarkers(true);
	MainGraf->MouseLayer->ClearAllMarkers(false);
	MainGraf->MouseLayer->CopyMatrix(MainGraf->Grid_Layer);
	MainGraf->InfoLayer->CopyMatrix(MainGraf->Grid_Layer);

	MainGraf->addMaterial(material_1);
	if (EnabelSecondGraph) {MainGraf->addMaterial(material_2);material_2->setVisible(true);}
	else material_2->setVisible(false);
	
	if (haveRelations_) wasProgrammChangeOffViewPort_ = true;
	bool needUpdateOnDataSet = !( haveRelations_ || ZoomOutMaxOnDataSet || autoscaleY_);

	if (!inline_memory_1)
	{
		material_1->setData(matData_1, needUpdateOnDataSet );
		matData_1.DeleteMemory();
	}
    else material_1->setInlineData((float*)spectrum_1,static_cast<unsigned int>(items_count_1));

	if (EnabelSecondGraph)
	{
		if (!inline_memory_2)
		{
			material_2->setData(matData_2, needUpdateOnDataSet );
			matData_2.DeleteMemory();
		}
        else material_2->setInlineData((float*)spectrum_2,static_cast<unsigned int>(items_count_2));
	}
	

	spectrumActivated_ = true;
	
	MainGraf->InfoLayer->setUserText("");
	MainGraf->InfoLayer->setPrintTextMode(0);
	MainGraf->InfoLayer->PrepareAndUpdateStandartText();

	MainGraf->removeWallPaper();
	if (TimerId > 0) killTimer(TimerId);
	TimerId = 0;

	if (ZoomOutMaxOnDataSet) ZoomOutFull();
	else if (autoscaleY_) MainGraf->ZoomOutFull(false,true);
	emit SpectrumActivated();
	return spectrumActivated_;
}

void Q_MG_SpectrumInterface::ZoomOutFull()
{
	if (!spectrumActivated_) return;
	MainGraf->ZoomOutFull();
}

void Q_MG_SpectrumInterface::SetUserTextToInfo( QString txt,bool IsHtmlFormat)
{
	MainGraf->InfoLayer->setUserText(txt,IsHtmlFormat);
}

void Q_MG_SpectrumInterface::SetVisibleInfoWidget( bool isVisible /*= true*/ )
{
	MainGraf->InfoLayer->setEnabled(isVisible);
    MainGraf->InfoLayer->setVisible(isVisible);
}

void Q_MG_SpectrumInterface::SetZeroFrequencyHz( double valX )
{
	SetVirtualOffsetXY(valX,MainGraf->Grid_Layer->VertInterpretSum);
}

void Q_MG_SpectrumInterface::SetVirtualOffsetY( double valY )
{
	SetVirtualOffsetXY(MainGraf->Grid_Layer->HorInterpretSum,valY);
}

void Q_MG_SpectrumInterface::SetVirtualOffsetXY( double valX,double valY )
{
	MainGraf->Grid_Layer->VertInterpretSum = valY;
	MainGraf->Grid_Layer->HorInterpretSum = valX;
	MainGraf->Grid_Layer->ReDraw();
	/*MainGraf->MouseLayer->CopyMatrix(MainGraf->Grid_Layer);
	MainGraf->InfoLayer->CopyMatrix(MainGraf->Grid_Layer);
	if ((SpectrumActivated) && (MainGraf->Main_Layer != NULL)) MainGraf->Main_Layer->CopyMatrix(MainGraf->Grid_Layer);*/
	MainGraf->AllCopyMatrixByGridPublic();
	MainGraf->InfoLayer->PrepareAndUpdateStandartText();
}

void Q_MG_SpectrumInterface::SetInfoWidgetPrintTextMode(int _PrintTextMode)
{
	MainGraf->InfoLayer->setPrintTextMode(_PrintTextMode);
}

void Q_MG_SpectrumInterface::SetShowOptionsForStarndartLabels(QString name /*= ""*/, bool showparam /*= false*/ )
{
	MainGraf->InfoLayer->setShowOptionsForStarndartLabels(name, showparam);
}

void Q_MG_SpectrumInterface::on_selectedPixelChangeSpectrum( double x1, double y1, double x2, double y2 )
{

#define OutAvaibleTextAndReturn {MainGraf->InfoLayer->PrepareAndUpdateStandartText();break;}
	
	if (x1 > x2) {double t=x1;x1=x2;x2=t;}
	if (y1 > y2) {double t=y2;y2=y1;y1=t;}

	if (MainGraf->Materials_LayersList.count() == 0) return;
	if (MainGraf->Materials_LayersList.count() > 2) Q_ASSERT_X(NULL,"Q_MultiGraphics::on_selectedPixelChange","Поправить тут, не рассчитано было на большее кол-во материалов");
	bool EnabelSecondGraph = MainGraf->Materials_LayersList.count()==2;
	SpectrumMaterial* mat1 = dynamic_cast<SpectrumMaterial*> (MainGraf->Materials_LayersList[0]);
	if (mat1 == NULL) return;

	SpectrumMaterial* mat2 = NULL;
	if (EnabelSecondGraph)
	{
		mat2 = dynamic_cast<SpectrumMaterial*> (MainGraf->Materials_LayersList[1]);
		if (mat2 == NULL) return;
	}

	while(1)
	{
        if ((MainGraf->MouseLayer->CurSelectionType > 2) && (MainGraf->MouseLayer->CurSelectionType != 4)) return;
		bool isVertical = (MainGraf->MouseLayer->CurSelectionType == 2);
		double v1x = x1;
		double v2x = x2;
		v1x = MainGraf->Grid_Layer->GetPixelByValue(x1,isVertical);
		v2x = MainGraf->Grid_Layer->GetPixelByValue(x2,isVertical);
		if (qAbs(v1x - v2x) <= 1) return;

		MainGraf->InfoLayer->SetValid(MainGraf->InfoLayer->UserData,false);
		MainGraf->InfoLayer->UserData[0].setValueAndValid(x1,isVertical);
		MainGraf->InfoLayer->UserData[1].setValueAndValid(x1+qAbs((x2-x1)/2.0),isVertical);
		MainGraf->InfoLayer->UserData[2].setValueAndValid(x2,isVertical);
		MainGraf->InfoLayer->UserData[3].setValueAndValid(qAbs(x2-x1),isVertical,true,true,true);

		if (isVertical) OutAvaibleTextAndReturn;
		if (v1x == v2x) OutAvaibleTextAndReturn;
		size_t arrSize1 = 0;
		float* selData1 = mat1->GetMaterialDataArrayByPixels(v1x,v2x,&arrSize1);
		if (selData1 == NULL) OutAvaibleTextAndReturn;

		size_t arrSize2 = 0;
		float* selData2;

		float minv;float maxv;
		int index_minv;int index_maxv;

		float minv_1;float maxv_1;
		int index_minv_1;int index_maxv_1;
		float minv_2;float maxv_2;
		int index_minv_2;int index_maxv_2;
        ippsMinMaxIndx_32f(selData1,static_cast<int>(arrSize1),&minv_1, &index_minv_1,&maxv_1, &index_maxv_1);
		SpectrumMaterial* usedMatMax;
		if (EnabelSecondGraph)
		{
			selData2 = mat2->GetMaterialDataArrayByPixels(v1x,v2x,&arrSize2);
            ippsMinMaxIndx_32f(selData2,static_cast<int>(arrSize2),&minv_2,&index_minv_2,&maxv_2,&index_maxv_2);
			minv = qMin(minv_1,minv_2);
			maxv = qMax(maxv_1,maxv_2);
			if (minv == minv_1) index_minv = index_minv_1;
			else index_minv = index_minv_2;
				
			if (maxv == maxv_1)
			{
				usedMatMax = mat1;
				index_maxv = index_maxv_1;
			}
			else
			{
				index_maxv = index_maxv_2;
				usedMatMax = mat2;
			}
		}
		else
		{
			usedMatMax = mat1;
			minv = minv_1;maxv = maxv_1;
			index_minv = index_minv_1;
			index_maxv = index_maxv_1;
		}
		
		MainGraf->InfoLayer->UserData[4].setValueAndValid(maxv,!isVertical);
		
		double maxHz = 0;
		{//рассчет , какая же там частота 
			double globP1x = v1x;
			if (v1x > v2x) { globP1x = v2x;}
			double StartPixelX = (usedMatMax->pos().x()*-1)+globP1x;
			qreal key_kf = usedMatMax->getCurKf().x()*usedMatMax->Hor_kf_inc;
			__int64 MassGlobPos = (StartPixelX*key_kf)+1+index_maxv;
			if (MassGlobPos < 0) MassGlobPos = 0;
			qreal cur_PX = MassGlobPos/key_kf;
			maxHz = usedMatMax->GetValueByPixelOnMaterial(cur_PX,0).x();
		}
		

		MainGraf->InfoLayer->UserData[5].setValueAndValid(maxHz,isVertical);
		MainGraf->InfoLayer->UserData[6].setValueAndValid(minv,!isVertical);
		MainGraf->InfoLayer->UserData[7].setValueAndValid(arrSize1,isVertical,false);
		OutAvaibleTextAndReturn;
	}
	

#undef OutAvaibleTextAndReturn 


//        qDebug() << "on_selectedPixelChangeSpectrum";

	//преобразуем реальные в виртуальные
	x1 = MainGraf->Grid_Layer->ApplyInterpretSum(x1,false);
	y1 = MainGraf->Grid_Layer->ApplyInterpretSum(y1,true);
	x2 = MainGraf->Grid_Layer->ApplyInterpretSum(x2,false);
	y2 = MainGraf->Grid_Layer->ApplyInterpretSum(y2,true);
	
	if (MainGraf->MouseLayer->CurSelectionType <= 2)
	{
		bool isVertical = (MainGraf->MouseLayer->CurSelectionType == 2);
		if (isVertical) emit HorizontalSelectionChanged(x1,x2);
		else emit VerticalSelectionChanged(y1,y2);
	}
    if (MainGraf->MouseLayer->CurSelectionType == 4)
    {
//        qDebug() <<  "VerticalSelectionChanged";
        emit VerticalSelectionChanged(y1,y2);
    }
    if (!skip_signal_on_selection_change_)
    {
//        qDebug() << "!skip_signal_on_selection_change_";
        emit SelectionChangedTo( x1, y1, x2, y2 );
    }
	else skip_signal_on_selection_change_ = false;
}

void Q_MG_SpectrumInterface::on_selectionFinished( double x1, double y1, double x2, double y2 )
{
    if (x1 > x2) {double t=x1;x1=x2;x2=t;}
	if (y1 > y2) {double t=y2;y2=y1;y1=t;}
	x1 = MainGraf->Grid_Layer->ApplyInterpretSum(x1,false);
	x2 = MainGraf->Grid_Layer->ApplyInterpretSum(x2,false);
	y1 = MainGraf->Grid_Layer->ApplyInterpretSum(y1,true);
	y2 = MainGraf->Grid_Layer->ApplyInterpretSum(y2,true);



	emit SelectionFinished( x1, y1, x2, y2 );
}

void Q_MG_SpectrumInterface::on_selectionFinishedRedLine(double y)
{
    y = MainGraf->Grid_Layer->ApplyInterpretSum(y,true);
    emit selectionFinishedRedLine(y);
}

void Q_MG_SpectrumInterface::SetStartAndFinishHz(QPointF start, QPointF finish)
{
    MainGraf->Grid_Layer->setMinAndMaxTransPoint(start, finish);
}

/* пока выключено, нет времени разбираться и править, надо уточнить как она используется
void Q_MG_SpectrumInterface::SetupNewData( QByteArray inc_ba)
{
	SpectrumActivated = false;
	unsigned __int64 items_count = inc_ba.size()/4;
	if (inc_ba.size() == 0) return;
	
	MaterialFloatData matData;
	if (!matData.Allocate(items_count)) {QMessageBox::critical(this,"Memory allocation error","No memory");return;}
	
	memcpy(matData.values,inc_ba.data(),items_count*sizeof(float));

	float minv;float maxv;
	ippsMinMax_32f(matData.values,items_count,&minv,&maxv);
		
	double raznTop = (maxv-minv)*0.2;
	double raznBot = raznTop;
	switch (align)
	{
		case 1: raznTop = 0;break;
		case 2: raznBot = 0;break;
		case 3: raznTop = 0;raznBot = 0;break;
	}
		
	MainGraf->Grid_Layer->Reset();
	InitSomeHiddenProperties();
	
	if (is_complex_)
	{
		qreal ttt = bandwidth_hz_/2;
		MainGraf->Grid_Layer->setMinAndMaxTransPoint(QPointF(ttt,maxv+raznTop),QPointF(-ttt,minv-raznBot));
	}
	else MainGraf->Grid_Layer->setMinAndMaxTransPoint(QPointF(bandwidth_hz_,maxv+raznTop),QPointF(0,minv-raznBot));

	material_1->Hor_kf_inc = (items_count-1)/bandwidth_hz_;
	MainGraf->Grid_Layer->MaximumZoom = QPointF(1/material_1->Hor_kf_inc*0.005,0.00001);

	MainGraf->MouseLayer->ClearAllSelections();
//	MainGraf->MouseLayer->clearMarkers(true);
//	MainGraf->MouseLayer->clearMarkers(false);
	MainGraf->MouseLayer->CopyMatrix(MainGraf->Grid_Layer);
	MainGraf->InfoLayer->CopyMatrix(MainGraf->Grid_Layer);

	MainGraf->setMaterialClass(material_1);	
	material_1->setData(matData);
	matData.DeleteMemory();
	
	MainGraf->InfoLayer->setUserText("");
	MainGraf->InfoLayer->setPrintTextMode(0);
	MainGraf->InfoLayer->PrepareAndUpdateStandartText();

	SpectrumActivated = true; 

	if (SpectrumActivated) MainGraf->removeWallPaper();

	if (ZoomOutMaxOnDataSet) ZoomOutFull();
	else if (autoscaleY_) MainGraf->ZoomOutFull(false,true);
}*/

void Q_MG_SpectrumInterface::SetScaleY_1to1()
{
	QPointF kf = MainGraf->Grid_Layer->getCurKf();
	kf.setY(1);
	MainGraf->SetDirectKf(kf);
}

void Q_MG_SpectrumInterface::ClearAllSelections(bool noSignal)
{
	if (noSignal) skip_signal_on_selection_clear_ = true;
	else skip_signal_on_selection_clear_ = false;
	MainGraf->ClearAllSelections();
}

void Q_MG_SpectrumInterface::SetCross( double X, double Y)
{
	MainGraf->SetStrangeCross(X,Y);
}

void Q_MG_SpectrumInterface::SetShowCross( bool value )
{
	MainGraf->SetShowStrangeCross(value,MainGraf->MouseLayer->showStrangeCrossHorText,MainGraf->MouseLayer->showStrangeCrossVertText);
}

void Q_MG_SpectrumInterface::SetShowCrossHorizontalText( bool visible )
{
	MainGraf->SetShowStrangeCross(MainGraf->MouseLayer->showStrangeCross,visible,MainGraf->MouseLayer->showStrangeCrossVertText);
}

void Q_MG_SpectrumInterface::SetShowCrossVerticalText( bool visible )
{
	MainGraf->SetShowStrangeCross(MainGraf->MouseLayer->showStrangeCross,MainGraf->MouseLayer->showStrangeCrossHorText,visible);
}

void Q_MG_SpectrumInterface::ShowMarkers()
{
	MainGraf->ShowMarkers();
}

void Q_MG_SpectrumInterface::HideMarkers()
{
	MainGraf->HideMarkers();
}

void Q_MG_SpectrumInterface::SetMoveAllMarkers()
{
	MainGraf->SetMoveAllMarkers();
}

void Q_MG_SpectrumInterface::SetMoveOneMarker()
{
	MainGraf->SetMoveOneMarker();
}


// Бордюры оси сетка

bool Q_MG_SpectrumInterface::ShowHorizGrid()
{
	return MainGraf->Grid_Layer->DrawHBaseLine;
}

void Q_MG_SpectrumInterface::SetShowHorizGrid( bool value )
{
	MainGraf->Grid_Layer->DrawHBaseLine = value;
	MainGraf->Grid_Layer->ReDraw();
}

bool Q_MG_SpectrumInterface::ShowVerticalGrid()
{
	return MainGraf->Grid_Layer->DrawVBaseLine;
}

void Q_MG_SpectrumInterface::SetShowVerticalGrid( bool value )
{
	MainGraf->Grid_Layer->DrawVBaseLine = value;
	MainGraf->Grid_Layer->ReDraw();
}

bool Q_MG_SpectrumInterface::ShowLevelBar()
{
	return MainGraf->Grid_Layer->flag_DrawVAxis;
}

void Q_MG_SpectrumInterface::SetShowLevelBar( bool value /*= true*/ )
{
	MainGraf->Grid_Layer->flag_DrawVAxis = value;
	MainGraf->MouseLayer->showVValues = value;
	MainGraf->Grid_Layer->ReDraw();
}

bool Q_MG_SpectrumInterface::ShowHorizontalBar()
{
	return MainGraf->Grid_Layer->flag_DrawHAxis;
}

void Q_MG_SpectrumInterface::SetShowHorizontalBar( bool value /*= true*/ )
{
	MainGraf->Grid_Layer->flag_DrawHAxis = value;
	MainGraf->MouseLayer->showHValues = value;
	MainGraf->Grid_Layer->ReDraw();
}


bool Q_MG_SpectrumInterface::ShowHorGridBorder()
{
	return MainGraf->Grid_Layer->DrawHGridBorders;
}

void Q_MG_SpectrumInterface::SetShowHorGridBorder( bool value /*= true*/ )
{
	MainGraf->Grid_Layer->DrawHGridBorders = value;
	MainGraf->Grid_Layer->ReDraw();
}

bool Q_MG_SpectrumInterface::ShowVertGridBorder()
{
	return MainGraf->Grid_Layer->DrawVGridBorders;
}

void Q_MG_SpectrumInterface::SetShowVertGridBorder( bool value /*= true*/ )
{
	MainGraf->Grid_Layer->DrawVGridBorders = value;
	MainGraf->Grid_Layer->ReDraw();
}

// конец Бордюры оси сетка

int Q_MG_SpectrumInterface::MarkersCount() const
{
	return MainGraf->MarkersCount(false);
}

bool Q_MG_SpectrumInterface::MarkersVisible() const
{
	return MainGraf->MarkersVisible(false);
}

QList<double> Q_MG_SpectrumInterface::GetMarkers() const
{
	return  MainGraf->MouseLayer->getMarkers(false);
}

bool Q_MG_SpectrumInterface::MoveAll() const
{
	return MainGraf->MoveAllMarkers();
}

void Q_MG_SpectrumInterface::SetMarkers( QVector<double> inclist,bool isVert,bool noSignal)
{
	MainGraf->MouseLayer->SetMarkers(inclist.toList(),isVert, noSignal);
}

void Q_MG_SpectrumInterface::UpdateMarker( int index, double value, bool isVert)
{
	MainGraf->MouseLayer->UpdateMarker(index,value,isVert);
}

void Q_MG_SpectrumInterface::SetSelection( double x1,double y1,double x2,double y2, bool noSignal )
{
	if (noSignal) skip_signal_on_selection_change_ = true;
	else skip_signal_on_selection_change_ = false;
	MainGraf->MouseLayer->SetSelection(x1,y1,x2,y2);
}

double Q_MG_SpectrumInterface::SelStartHz() const
{
	QList<QPointF> retPoint = MainGraf->MouseLayer->getHVSelected(false);
	if (retPoint.count() == 0) return 0;
	double SelectionHz = retPoint[0].x();
	return SelectionHz;
}

double Q_MG_SpectrumInterface::SelEndHz() const
{
	QList<QPointF> retPoint = MainGraf->MouseLayer->getHVSelected(false);
	if (retPoint.count() == 0) return 0;
	double SelectionHz = retPoint[0].y();
	return SelectionHz;
}

/*НАХ
double Q_MG_SpectrumInterface::SelStartPix() const
{
	return MainGraf->getSelectionStartPix();
}

double Q_MG_SpectrumInterface::SelEndPix() const
{
	return MainGraf->getSelectionEndPix();
}
*/


bool Q_MG_SpectrumInterface::HasSelection() const
{
	return MainGraf->MouseLayer->HasSelection();
}

void Q_MG_SpectrumInterface::SetFillSpectrum()
{
	material_1->SetFillSpectrum(true);
}

bool Q_MG_SpectrumInterface::FilledSpectrum() const
{
	return material_1->isFilledSpectrum();
}

void Q_MG_SpectrumInterface::SetLinesOnlySpectrum()
{
	material_1->SetFillSpectrum(false);
}

double Q_MG_SpectrumInterface::CrossHz() const
{
	return MainGraf->MouseLayer->ApplyInterpretSum(MainGraf->MouseLayer->StrangeCrossValX,false);
}

double Q_MG_SpectrumInterface::CrossPix() const
{
	//double z = MainGraf->MouseLayer->ApplyInterpretSum(MainGraf->MouseLayer->StrangeCrossValX,false);
	return MainGraf->MouseLayer->GetPixelByValuePublic(MainGraf->MouseLayer->StrangeCrossValX,false);
}

void Q_MG_SpectrumInterface::on_markerMoved( int index, double val,bool isVert)
{
	emit MarkerMoved(index,MainGraf->MouseLayer->ApplyInterpretSum(val,isVert),isVert);
}

void Q_MG_SpectrumInterface::on_markersChanged( QList<double> inc_list,bool isVert)
{
	QVector<double> retVal(inc_list.count());
	for (int i = 0; i < inc_list.count();i++) retVal[i] = MainGraf->MouseLayer->ApplyInterpretSum(inc_list[i],!isVert);
	emit MarkersChanged(retVal, isVert);
}

void Q_MG_SpectrumInterface::on_doubleClicked( double val1, double val2)
{
    val1 = MainGraf->MouseLayer->ApplyInterpretSum(val1,false);
    val2 = MainGraf->MouseLayer->ApplyInterpretSum(val2,true);
	emit DoubleClicked(val1,val2);
}

void Q_MG_SpectrumInterface::SetProgress( double a)
{
	MainGraf->InfoLayer->SetProgress(a);
}

bool Q_MG_SpectrumInterface::ShowProgress()
{
	return MainGraf->InfoLayer->ShowProgress();
}

void Q_MG_SpectrumInterface::SetShowProgress( bool value )
{
	MainGraf->InfoLayer->SetShowProgress(value);
}

void Q_MG_SpectrumInterface::SetViewport( double start_hz, double start_smpl, double end_hz, double end_smpl )
{
	//qDebug() << "SpectrumInterface set viewport SX" << start_hz << " SY" << start_smpl << " EX" << end_hz << " EY" << end_smpl;
	wasProgrammChangeOffViewPort_ = true;
	MainGraf->SetViewportForValues(start_hz,start_smpl,end_hz,end_smpl);
}

void Q_MG_SpectrumInterface::ZoomToSelection()
{
	MainGraf->ZoomToSelection();
}

bool Q_MG_SpectrumInterface::PermanentDataSetup( const float* spectrum_1, const float* spectrum_2, float minv_, float maxv_)
{
	if (!isActivated())
	{
		qDebug() << "ERROR (ОШИБКА). SpectrumInterface::PermanentDataSetup : вызов PermanentDataSetup до функции Setup ! Функция не выполнена.";
		return false;
	}
	
	bool EnableSeconsSpectr = spectrum_2;

	unsigned int items_count_1 = material_1->PointsCount();
	unsigned int items_count_2 = 0;
	if (EnableSeconsSpectr) items_count_2 = material_2->PointsCount();

	if (items_count_1 == 0) return false;

	/* КОСТЫТЬ ЗАКОММЕНТИРОВАН (детали в карточке №417)
    bool spectrum_1_ok;
    bool spectrum_2_ok = !EnableSeconsSpectr;

    auto is_spectrum_empty = [](const float* data, unsigned int item_count) -> bool
    {
        bool all_zeros = true;
        for(unsigned int i = 0; i < item_count && all_zeros; ++i)
            all_zeros &= data[i] == 0.0f;

        return all_zeros;
    };

    spectrum_1_ok = !is_spectrum_empty(spectrum_1, items_count_1);
    if(EnableSeconsSpectr)
        spectrum_2_ok = !is_spectrum_empty(spectrum_2, items_count_2);

    if(!spectrum_1_ok || !spectrum_2_ok)
    {
        qDebug() << "FIXME: пустой спектр";
        return false;
    }//*/
	
	MaterialFloatData matData_1;
	MaterialFloatData matData_2;
	if (!matData_1.Allocate(items_count_1)) {QMessageBox::critical(this,"PermanentDataSetup: Memory allocation error","No memory");return false;}
	memcpy(matData_1.values,spectrum_1,items_count_1*sizeof(float));

	if (EnableSeconsSpectr)
	{
		if (!matData_2.Allocate(items_count_2)) {QMessageBox::critical(this,"PermanentDataSetup: Memory allocation error","No memory");return false;}
		memcpy(matData_2.values,spectrum_2,items_count_2*sizeof(float));
	}

    float minv;
    float maxv;
    minv = minv_;
    maxv = maxv_;

        float minv_1;float maxv_1;
        float minv_2;float maxv_2;
        ippsMinMax_32f(spectrum_1,items_count_1,&minv_1,&maxv_1);

        if (EnableSeconsSpectr)
        {
            ippsMinMax_32f(spectrum_2,items_count_2,&minv_2,&maxv_2);
            minv = qMin(minv_1,minv_2);
            maxv = qMax(maxv_1,maxv_2);
        }
        else
        {
//            minv = minv_1;maxv = maxv_1;
            if(minv_ == 0 && maxv_ == 1)
            {
                minv = minv_1;
                maxv = maxv_1;
            }

        }

    if(minv_ == 0 && maxv_ == 1)
    {
        minv = minv_;
        maxv = maxv_;
    }

	double raznTop = (maxv-minv)*0.2;
	double raznBot = raznTop;
	switch (align)
	{
		case 1: raznTop = 0;break;
		case 2: raznBot = 0;break;
		case 3: raznTop = 0;raznBot = 0;break;
	}



//    if(minv_ == 0 && maxv_ == 0)
//    {
//    }
//    else
//    {
//        raznTop = 10;
//        raznBot = 10;
//    }

//    qDebug() << "Per is_complex! = " << is_complex_ << minv << maxv << bandwidth_hz_;
    if (is_complex_)
    {
        qreal ttt = bandwidth_hz_/2;
        MainGraf->Grid_Layer->setMinAndMaxTransPoint(QPointF(ttt,maxv+raznTop),QPointF(-ttt,minv-raznBot));
    }
    else MainGraf->Grid_Layer->setMinAndMaxTransPoint(QPointF(bandwidth_hz_,maxv+raznTop),QPointF(0,minv-raznBot));


	if (haveRelations_) wasProgrammChangeOffViewPort_ = true;
    bool needUpdateOnDataSet = !( haveRelations_ || autoscaleY_);
	material_1->setData(matData_1,needUpdateOnDataSet);
	matData_1.DeleteMemory();

	if (EnableSeconsSpectr)
	{
		material_2->setData(matData_2,needUpdateOnDataSet);
		matData_2.DeleteMemory();
//		material_2->setVisible(_material2_visible);
	}
//	else material_2->setVisible(false);
    material_2->setVisible(_material2_visible);
	

	MainGraf->InfoLayer->setUserText("");
	MainGraf->InfoLayer->setPrintTextMode(0);
    if (IsSomethingSelected())
    {
        double x1 = SelStartHz();
        double x2 = SelEndHz();
        x1 = MainGraf->Grid_Layer->DisApplyInterpretSum(x1,false);
        x2 = MainGraf->Grid_Layer->DisApplyInterpretSum(x2,false);
        skip_signal_on_selection_change_ = true;
        on_selectedPixelChangeSpectrum( x1, 0, x2, 0);
    }
    else MainGraf->InfoLayer->PrepareAndUpdateStandartText();
	
//    MainGraf->show();
//    MainGraf->update();
//    if (autoscaleY_) MainGraf->ZoomOutFull(false,false);
	return true;
}

void Q_MG_SpectrumInterface::Reset()
{
	if (spectrumActivated_)
	{
		bool mem_multy_select = MainGraf->MouseLayer->IsMultySelectEnabled();

		delete material_1;
		delete MainGraf;
		InitMainGraf();

		MainGraf->MouseLayer->SetMultySelectEnabled(mem_multy_select);
		MainGraf->setContextMenu(contextMenu_);
		MainLayout->insertWidget(0,MainGraf);
	}
}

void Q_MG_SpectrumInterface::timerEvent( QTimerEvent * ev )
{
	if (spectrumActivated_)	{	killTimer(TimerId);TimerId = 0;	}

	if (wallPaper.opacity() >= 0.7) {killTimer(TimerId);TimerId = 0;}
	wallPaper.setOpacity(wallPaper.opacity()+0.005);
}

void Q_MG_SpectrumInterface::InitSomeHiddenProperties()
{
	MainGraf->InfoLayer->setEnabled(true);
    MainGraf->InfoLayer->setVisible(true);
	MainGraf->Grid_Layer->setVisible(true);
	MainGraf->MouseLayer->setVisible(true);
}

void Q_MG_SpectrumInterface::on_ViewPortChanged( double value_startX, double value_startY,double value_endX, double value_endY )
{
	value_startX = MainGraf->Grid_Layer->ApplyInterpretSum(value_startX,false);
	value_endX = MainGraf->Grid_Layer->ApplyInterpretSum(value_endX,false);
	value_startY = MainGraf->Grid_Layer->ApplyInterpretSum(value_startY,true);
	value_endY = MainGraf->Grid_Layer->ApplyInterpretSum(value_endY,true);

	//qDebug() << "SpectrumInterface viewport ch";

	if ((wasProgrammChangeOffViewPort_) && (!emitSignalOnProgrammChangeOffViewPort_)) {wasProgrammChangeOffViewPort_ = false;return;}
	//qDebug() << "emit SpectrumInterface viewport ch";
	emit viewPortChanged(value_startX,value_startY,value_endX,value_endY);
}

void Q_MG_SpectrumInterface::GetCurrentViewport( double &start_hz, double &start_Y, double &end_hz, double &end_Y )
{
	double a = 0;
	double b = 0;
	double c = 0;
	double d = 0;
	if (material_1 != NULL) material_1->GetViewPort(a,b,c,d);
	start_hz	= MainGraf->Grid_Layer->ApplyInterpretSum(a,false);
	end_hz		= MainGraf->Grid_Layer->ApplyInterpretSum(c,false);
	start_Y		= MainGraf->Grid_Layer->ApplyInterpretSum(b,true);
	end_Y		= MainGraf->Grid_Layer->ApplyInterpretSum(d,true);
}

void Q_MG_SpectrumInterface::on_selectionCleared()
{
	MainGraf->InfoLayer->SetValid(MainGraf->InfoLayer->UserData,false);
	MainGraf->InfoLayer->PrepareAndUpdateStandartText();
	if (!skip_signal_on_selection_clear_) emit SelectionCleared();
	else skip_signal_on_selection_clear_ = false;
}

bool Q_MG_SpectrumInterface::IsSomethingSelected() const
{
	return MainGraf->MouseLayer->HasSelection();
}

void Q_MG_SpectrumInterface::SetHorizontalLabel( QString label )
{
	HorLabel = label;
	MainGraf->Grid_Layer->LabelHAxis = label;
	MainGraf->MouseLayer->Hor_InterpretAddLabel = label;
	MainGraf->InfoLayer->Hor_InterpretAddLabel = label;
	material_1->Hor_InterpretAddLabel = label;
	material_2->Hor_InterpretAddLabel = label;

}

void Q_MG_SpectrumInterface::SetScaleStepByVertical( double vert_wheel_step )
{
	MainGraf->VertStep = vert_wheel_step;
}

void Q_MG_SpectrumInterface::SetScaleStepByHorizontal( double hor_wheel_step )
{
	MainGraf->HorStep = hor_wheel_step;
}

void Q_MG_SpectrumInterface::ZoomInX()
{
	MainGraf->ScaleOnSteps(1,true,false);
}

void Q_MG_SpectrumInterface::ZoomOutX()
{
	MainGraf->ScaleOnSteps(-1,true,false);
}

void Q_MG_SpectrumInterface::ZoomInY()
{
	MainGraf->ScaleOnSteps(1,false,true);
}

void Q_MG_SpectrumInterface::ZoomOutY()
{
	MainGraf->ScaleOnSteps(-1,false,true);
}

void Q_MG_SpectrumInterface::ClearAllLabels()
{
	MainGraf->MouseLayer->ClearAllFredomLabels();
}

void Q_MG_SpectrumInterface::SetLabel( double valX_b,QString text,QString name,bool immediately_update/* = false*/, QColor label_color )
{
	if (!isActivated()) return;
	if (MainGraf->Materials_LayersList.count() == 0) return;
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->SetFredomLabel(valX_b,text,name,immediately_update, label_color);
}

void Q_MG_SpectrumInterface::ChangeLabelText( QString name,QString new_text, bool immediately_update /*= false*/ )
{
	if (!isActivated()) return;
	if (MainGraf->Materials_LayersList.count() == 0) return;
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->ChangeLabelText(name,new_text,immediately_update);
}

void Q_MG_SpectrumInterface::ChangeLabelPos( QString name,double new_pos, bool immediately_update /*= false*/ )
{
	if (!isActivated()) return;
	if (MainGraf->Materials_LayersList.count() == 0) return;
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->ChangeLabelPos(name,new_pos,immediately_update);
}

void Q_MG_SpectrumInterface::RemoveLabel( QString name, bool immediately_update /*= false*/ )
{
	if (!isActivated()) return;
	if (MainGraf->Materials_LayersList.count() == 0) return;
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->RemoveLabel(name,immediately_update);
}


void Q_MG_SpectrumInterface::SetLabelsUnionsText( QString union_text )
{
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->SetLabelsUnionText(union_text);
}

void Q_MG_SpectrumInterface::SetLabelsUnionsShowHint( bool show_hint )
{
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->SetLabelsUnionShowHint(show_hint);
}


bool Q_MG_SpectrumInterface::eventFilter( QObject* obj, QEvent* ev)
{
	if (ev->type() == QEvent::Resize)
	{
		if (haveRelations_) wasProgrammChangeOffViewPort_ = true;
		else
		{
			if (autoscaleY_) MainGraf->ZoomOutFull(false,true);
		}
	}
	return false;
}

void Q_MG_SpectrumInterface::MoveScreenByPixels( QPoint pixels, bool emit_viewPortChangeSignal )
{
	wasProgrammChangeOffViewPort_ = !emit_viewPortChangeSignal;
    getBaseEngineClass()->ScreenDragByMouse( pixels);
}

/// show/hide spectrum
void Q_MG_SpectrumInterface::SetSpectrumVisible(int spectr, bool visible)
{
    switch(spectr)
    {
    case 1:
        material_1->setVisible(visible);
        break;
    case 2:
        material_2->setVisible(visible);
        _material2_visible = visible;
        break;
    default:
        break;
    }
}

void Q_MG_SpectrumInterface::SetMultySelectEnabled( bool enable )
{
	if (MainGraf->MouseLayer == nullptr) return;
	//ClearAllSelections(false);
	MainGraf->MouseLayer->SetMultySelectEnabled(enable);
}

QList<QPointF> Q_MG_SpectrumInterface::GetAllHorSelections() const
{
	if (MainGraf->MouseLayer == nullptr) return QList<QPointF>();
	return MainGraf->MouseLayer->getHVSelected(false);
}

QList<QPointF> Q_MG_SpectrumInterface::GetAllVertSelections() const
{
	if (MainGraf->MouseLayer == nullptr) return QList<QPointF>();
	return MainGraf->MouseLayer->getHVSelected(true);
}

QList<QRectF> Q_MG_SpectrumInterface::GetAllRectSelections() const
{
	if (MainGraf->MouseLayer == nullptr) return QList<QRectF>();
	return MainGraf->MouseLayer->getRectSelected();
}

bool Q_MG_SpectrumInterface::HasRectSelection() const
{
	if (MainGraf->MouseLayer == nullptr) return false;
	return MainGraf->MouseLayer->HasRectSelection();
}

bool Q_MG_SpectrumInterface::HasHorSelection() const
{
	if (MainGraf->MouseLayer == nullptr) return false;
	return MainGraf->MouseLayer->HasHorSelection();
}

bool Q_MG_SpectrumInterface::HasVertSelection() const
{
	if (MainGraf->MouseLayer == nullptr) return false;
	return MainGraf->MouseLayer->HasVertSelection();
}

void Q_MG_SpectrumInterface::RemoveLastSelection()
{
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->RemoveLastSelection();
}

void Q_MG_SpectrumInterface::SetButtonCtrlClickIsSingle( bool is_single /*= true*/ )
{
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->SetButtonCtrlClickIsSingle(is_single);
}

void Q_MG_SpectrumInterface::ClearAllMarkers()
{
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->ClearAllMarkers(true);
	MainGraf->MouseLayer->ClearAllMarkers(false);
}

/// change current selection type
void Q_MG_SpectrumInterface::slotCurSelectionChanged(int type)
{
    MainGraf->MouseLayer->ClearAllSelections();
    MainGraf->MouseLayer->CurSelectionType = type;
}
