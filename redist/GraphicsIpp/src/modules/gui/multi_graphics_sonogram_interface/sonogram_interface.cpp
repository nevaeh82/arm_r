#include "../../../stdafx.h"
#include "sonogram_interface.h"
#include <QVBoxLayout>
#include <QtDebug>
#include <QFile>
#include <QMessageBox>
#include <QSettings>


SonogramInterface::SonogramInterface(QWidget *parent,QSettings* settings) : QWidget(parent)
{
	SonogramActivated = false;
	ZoomOutMaxOnDataSet = false;
	MainLayout = new QVBoxLayout(this); 
	MainLayout->setSpacing(2);
	MainLayout->setContentsMargins(0, 0, 0, 0);
	
	settings_ = settings;
	TimerId = 0;
	wallPaper.setPixmap(QPixmap(":/Q_MultiGraphics/Resources/WallPaperSono.png"));

	emitSignalOnProgrammChangeOffViewPort_ = false;
	haveRelations_ = false;
	maximum_ram_of_system_ = 0;
	InitMainGraf();
	
	MainLayout->insertWidget(0,MainGraf);
	ZoomOutMaxOnDataSet = false;	
}

SonogramInterface::~SonogramInterface()
{
	MainGraf->removeWallPaper();
	delete sonorenderThread;
	delete material_1;
}




void SonogramInterface::InitMainGraf()
{
	MainGraf = new Q_MultiGraphics(this);
	MainGraf->installEventFilter(this);
	MainGraf->SpectrumMode = false;
	MainGraf->VertStep = 2;
	MainGraf->HorStep = 2;

	sonorenderThread = new SonogramGraphicsRenderThread();
	material_1 = new SonogramMaterial(sonorenderThread);
	sonorenderThread->setParent(material_1);
	connect(material_1,SIGNAL(sendCommand(QString,QString,QVariant)),MainGraf->InfoLayer,SLOT(getCommand(QString,QString,QVariant)));
	connect(material_1,SIGNAL(ViewPortChanged(double,double,double,double)),this,SLOT(on_ViewPortChanged(double,double,double,double)));
	connect(material_1,SIGNAL(ScreenSpectrumsChanged(FFTLine_32f,FFTLine)),this,SIGNAL(SonogramScreenSpectrumsChanged(FFTLine_32f,FFTLine)));
	connect(material_1,SIGNAL( CalcPallateFinished_signal(QByteArray) ),this,SIGNAL( CalcPallateFinished(QByteArray) ));

	//установка параметров компонента
	MainGraf->Grid_Layer->flag_GridAligment = 1;
	MainGraf->Grid_Layer->InverseVAxis = false;
	MainGraf->Grid_Layer->InverseHAxis = false;
	MainGraf->Grid_Layer->DrawVSubDiv = false;
	MainGraf->Grid_Layer->DrawHSubDiv = false;
	MainGraf->Grid_Layer->DrawVBaseLine = false;
	MainGraf->Grid_Layer->DrawHBaseLine = false;
		
	MainGraf->MouseLayer->CurSelectionType = 3;
	MainGraf->MouseLayer->GlowEffect = false;
	MainGraf->MouseLayer->showBigGrafCursor = false;
	MainGraf->MouseLayer->showMiniGrafCursor = true;
	MainGraf->MouseLayer->SetMultySelectEnabled(false);
	MainGraf->MouseLayer->showStrangeCross = false;
	MainGraf->MouseLayer->ShowSelectText = true;
	MainGraf->MouseLayer->DrawTextDecorateMidLine = true;

	//делаем пустой экран
	MainGraf->InfoLayer->setEnabled(false);
	MainGraf->InfoLayer->setVisible(false);
	MainGraf->Grid_Layer->setVisible(false);
	MainGraf->MouseLayer->setVisible(false);

	//установка параметров компонента для интерпритации значений
	MainGraf->Grid_Layer->Hor_RoundValue = 3;
	MainGraf->Grid_Layer->LabelHAxis = tr("Гц");
	MainGraf->Grid_Layer->LabelVAxis = tr("ч:м:с");
	MainGraf->Grid_Layer->EnableKiloMath = true;
	MainGraf->Grid_Layer->Vert_RoundValue = 0;
	MainGraf->Grid_Layer->EnableVertTimerConvertor = true;

	MainGraf->MouseLayer->Hor_RoundValue = 0;
	MainGraf->MouseLayer->InterpretShowFullTime = true;
	MainGraf->MouseLayer->EnableKiloMath = true;
	MainGraf->MouseLayer->EnableInterpretAddLabel = true;
	MainGraf->MouseLayer->Hor_InterpretAddLabel = tr("Гц");
	MainGraf->MouseLayer->Vert_RoundValue = 0;
	MainGraf->MouseLayer->EnableVertTimerConvertor = true;
	MainGraf->MouseLayer->Vert_InterpretAddLabel = tr("");

	MainGraf->InfoLayer->Hor_RoundValue = 4;
	MainGraf->InfoLayer->EnableKiloMath = true;
	MainGraf->InfoLayer->EnableInterpretAddLabel = true;
	MainGraf->InfoLayer->Hor_InterpretAddLabel = tr("Гц");
	MainGraf->InfoLayer->Vert_RoundValue = 0;
	MainGraf->InfoLayer->EnableVertTimerConvertor = true;
	MainGraf->InfoLayer->Vert_InterpretAddLabel = tr("");
	
	//установка данных для инфо виджета
	QVector<InfoWidgetData> selectInfoData;
	selectInfoData.resize(6);
	selectInfoData[0].name = "Начало";
	selectInfoData[1].name = "Конец";
	selectInfoData[2].name = "Выделено";
	selectInfoData[3].name = "Начало (время)";
	selectInfoData[4].name = "Конец (время)";
	selectInfoData[5].name = "Выделено (время)";
	MainGraf->InfoLayer->setUserData(selectInfoData);
	MainGraf->InfoLayer->InterpretShowFullTime = true;
	
	MainGraf->MouseLayer->CopyMatrix(MainGraf->Grid_Layer);
	
	skip_signal_on_selection_change_ = false;
	skip_signal_on_selection_clear_ = false;

	wasProgrammChangeOffViewPort_ = false;
	material_1->EnableKiloMath = true;
	material_1->Hor_RoundValue = 0;
	material_1->EnableInterpretAddLabel = true;
	material_1->Hor_InterpretAddLabel = tr("Гц");
	material_1->Vert_RoundValue = 0;
	material_1->EnableVertTimerConvertor = true;
	material_1->Vert_InterpretAddLabel = tr("");

	connect(MainGraf->MouseLayer,SIGNAL(deselectFinished()),this,SIGNAL(DeselectFinished()));
	connect(MainGraf->MouseLayer,SIGNAL(selectedPixelChange(double,double,double,double)),this,SLOT(on_selectedPixelChangeSonogram(double,double,double,double)));
	connect(MainGraf->MouseLayer,SIGNAL(selectionFinished(double, double,double,double)),this,SLOT( on_selectionFinished(double, double,double,double)));
	connect(MainGraf->MouseLayer,SIGNAL(dragScreenFinish()),this,SIGNAL(dragScreenFinish()));
	connect(MainGraf->MouseLayer,SIGNAL(markerMooved(int,double,bool)),this,SLOT(on_markerMoved(int,double,bool)));
	connect(MainGraf->MouseLayer,SIGNAL(markersChanged(QList<double>,bool)),this,SLOT(on_markersChanged(QList<double>,bool)));
	connect(MainGraf->MouseLayer,SIGNAL(selectionCleared()),this,SLOT(on_selectionCleared()));

	MainGraf->MouseLayer->CopyMatrix(MainGraf->Grid_Layer);

	//размещение обоев
	wallPaper.setOpacity(0);
	MainGraf->setWallPaper(&wallPaper);
	if (TimerId != 0) {killTimer(TimerId);TimerId = 0;}
	TimerId = startTimer(100);
	bandwidth_hz_ = 1;
}



bool SonogramInterface::Setup(const FileType& file, int fft_Order, QByteArray color_level_data /*= QByteArray()*/)
{
	SonogramActivated               = false;

	if ((fft_Order < 10) || (fft_Order >= 20))
        return SonogramActivated;

	unsigned int sonogram_step      = 1 << fft_Order;
	
    if ((!file->IsOpened()) || (file->SampleCount()-sonogram_step < 10))
        return SonogramActivated;

	unsigned __int64 items_count    = file->SampleCount();

	if (items_count < sonogram_step)
        return SonogramActivated;

	MainGraf->Grid_Layer->Reset();
	
	//инициализируем с пустого экрана
	MainGraf->InfoLayer->setEnabled(true);
    MainGraf->InfoLayer->setVisible(true);
	MainGraf->Grid_Layer->setVisible(true);
	MainGraf->MouseLayer->setVisible(true);
	
	unsigned __int64 LinesCount     = items_count-sonogram_step;

   bandwidth_hz_ = file->IsComplex() ? file->SampleRate() : file->SampleRate()*0.5;
	
	//определение размеров материала
	if (file->IsComplex())
	{
		qreal ttt = bandwidth_hz_/2;
		MainGraf->Grid_Layer->setMinAndMaxTransPoint(QPointF(ttt,LinesCount),QPointF(-ttt,0));
	}
	else MainGraf->Grid_Layer->setMinAndMaxTransPoint(QPointF(bandwidth_hz_,LinesCount),QPointF(0,0));

	MainGraf->Grid_Layer->VertTimerKf = (bandwidth_hz_/1000000000);//расчет в нано секундах
	MainGraf->Grid_Layer->VertInterpretSum = +sonogram_step*0.5;//корректировка по времени

	MainGraf->MouseLayer->ClearAllSelections();
	MainGraf->MouseLayer->ClearAllMarkers(true);
	MainGraf->MouseLayer->ClearAllMarkers(false);
	MainGraf->MouseLayer->CopyMatrix(MainGraf->Grid_Layer);
	MainGraf->InfoLayer->CopyMatrix(MainGraf->Grid_Layer);

	double aa = 1/(((sonogram_step)/bandwidth_hz_)*3);//10
	MainGraf->Grid_Layer->MaximumZoom = QPointF(aa,100);
	
	MainGraf->addMaterial(material_1);
	material_1->Hor_kf_inc  = 1;
	material_1->Vert_kf_inc = 1;

	MainGraf->InfoLayer->setUserText("");
	MainGraf->InfoLayer->setPrintTextMode(0);
	MainGraf->InfoLayer->PrepareAndUpdateStandartText();

	file->Seek(0);

	wasProgrammChangeOffViewPort_ = false;//тут проверка на haveRelation не нужны ибо сонограмма должна всегда эмитить при установке данных
	bool needUpdateOnDataSet = !(ZoomOutMaxOnDataSet);
	
	if (!material_1->setData(file, fft_Order, color_level_data, needUpdateOnDataSet,maximum_ram_of_system_))
	{
		MainGraf->InfoLayer->setUserText("Неверные данные или недостаточно оперативной памяти для отрисовки");
		return false;
	}

	MainGraf->SetDirectKf(QPointF(aa,1));

	SonogramActivated = true;


	if (ZoomOutMaxOnDataSet) ZoomOutFull();

	MainGraf->removeWallPaper();
	if (TimerId > 0) killTimer(TimerId);
	TimerId = 0;

	return SonogramActivated;
}


void SonogramInterface::Reset()
{
	if (SonogramActivated)
	{
		bool mem_multy_select = MainGraf->MouseLayer->IsMultySelectEnabled();

		delete sonorenderThread;
		delete material_1;
		delete MainGraf;
		SonogramActivated = false;
		InitMainGraf();

		MainGraf->MouseLayer->SetMultySelectEnabled(mem_multy_select);
		MainGraf->setContextMenu(contextMenu_);
		MainLayout->insertWidget(0,MainGraf);
	}
}

void SonogramInterface::ZoomOutFull()
{
	if (!SonogramActivated) return;
	MainGraf->ZoomOutFull();
}

void SonogramInterface::SetVirtualOffsetX( double valX )
{
	MainGraf->Grid_Layer->HorInterpretSum = valX;
	MainGraf->Grid_Layer->ReDraw();
	/*MainGraf->MouseLayer->CopyMatrix(MainGraf->Grid_Layer);
	MainGraf->InfoLayer->CopyMatrix(MainGraf->Grid_Layer);
	if ((SonogramActivated) && (MainGraf->Main_Layer != NULL)) MainGraf->Main_Layer->CopyMatrix(MainGraf->Grid_Layer);*/
	MainGraf->AllCopyMatrixByGridPublic();
	MainGraf->InfoLayer->PrepareAndUpdateStandartText();
}

void SonogramInterface::SetUsetTextToInfo( QString txt, bool IsHtmlFormat /*= false*/ )
{
	MainGraf->InfoLayer->setUserText(txt,IsHtmlFormat);
}

void SonogramInterface::SetInfoWidgetPrintTextMode( int _PrintTextMode )
{
	MainGraf->InfoLayer->setPrintTextMode(_PrintTextMode);
}

void SonogramInterface::SetVisibleInfoWidget( bool isVisible /*= true*/ )
{
	MainGraf->InfoLayer->setEnabled(isVisible);
    MainGraf->InfoLayer->setVisible(isVisible);
}

void SonogramInterface::SetShowOptionsForStarndartLabels(QString name /*= ""*/, bool showparam /*= false*/ )
{
	MainGraf->InfoLayer->setShowOptionsForStarndartLabels( name, showparam);
}

void SonogramInterface::on_selectedPixelChangeSonogram( double x1, double y1, double x2, double y2)
{
	
#define OutAvaibleTextAndReturn {MainGraf->InfoLayer->PrepareAndUpdateStandartText();break;}

	if (x1 > x2) {double t=x1;x1=x2;x2=t;}
	if (y1 > y2) {double t=y2;y2=y1;y1=t;}

	
	while(1)
	{
		double v1x = x1;
		double v2x = x2;
		double v1y = y1;
		double v2y = y2;
		v1x = MainGraf->Grid_Layer->GetPixelByValue(x1,false);
		v2x = MainGraf->Grid_Layer->GetPixelByValue(x2,false);
		v1y = MainGraf->Grid_Layer->GetPixelByValue(y1,true);
		v2y = MainGraf->Grid_Layer->GetPixelByValue(y2,true);
				
		MainGraf->InfoLayer->SetValid(MainGraf->InfoLayer->UserData,false);
		if (v1x != v2x)
		{
			MainGraf->InfoLayer->UserData[0].setValueAndValid(x1,false);
			MainGraf->InfoLayer->UserData[1].setValueAndValid(x2,false);
			MainGraf->InfoLayer->UserData[2].setValueAndValid(qAbs(x2-x1),false,true,true,true);
		}
		if (v1y != v2y)
		{
			MainGraf->InfoLayer->UserData[3].setValueAndValid(y1,true);
			MainGraf->InfoLayer->UserData[4].setValueAndValid(y2,true);
			MainGraf->InfoLayer->UserData[5].setValueAndValid(qAbs(y2-y1),true,true,true,true);
		}
		OutAvaibleTextAndReturn;
	}


#undef OutAvaibleTextAndReturn 

		
	
	if (x1 > x2) {double t=x1;x1=x2;x2=t;}
	if (y1 > y2) {double t=y2;y2=y1;y1=t;}
	x1 = MainGraf->Grid_Layer->ApplyInterpretSum(x1,false);
	x2 = MainGraf->Grid_Layer->ApplyInterpretSum(x2,false);
	//ось Y возвращаем как есть (ибо InterpretSum для времени не утанавливается)
	if (MainGraf->MouseLayer->CurSelectionType <= 2)
	{
		bool isVertical = (MainGraf->MouseLayer->CurSelectionType == 2);
		if (isVertical) emit HorizontalSelectionChanged(x1,x2);
		else emit VerticalSelectionChanged(y1,y2);
	}
	if (!skip_signal_on_selection_change_) emit SelectionChangedTo( x1, y1, x2, y2 );
	else skip_signal_on_selection_change_ = false;
}

void SonogramInterface::on_selectionFinished( double x1, double y1, double x2, double y2 )
{
	if (x1 > x2) {double t=x1;x1=x2;x2=t;}
	if (y1 > y2) {double t=y2;y2=y1;y1=t;}
	x1 = MainGraf->Grid_Layer->ApplyInterpretSum(x1,false);
	x2 = MainGraf->Grid_Layer->ApplyInterpretSum(x2,false);
	//ось Y возвращаем как есть (ибо InterpretSum для времени не утанавливается)
	emit selectionFinished( x1, y1, x2, y2 );
}

void SonogramInterface::ZoomToSelection()
{
	MainGraf->ZoomToSelection();
}

void SonogramInterface::on_ViewPortChanged( double value_startX, double value_startY,double value_endX, double value_endY )
{
	value_startX = MainGraf->Grid_Layer->ApplyInterpretSum(value_startX,false);
	value_endX = MainGraf->Grid_Layer->ApplyInterpretSum(value_endX,false);
	
	//qDebug() << "SonogramInterface viewport ch SX" << value_startX << " SY" << value_startY << " EX" << value_endX << " EY" << value_endY;
	if ((wasProgrammChangeOffViewPort_) && (!emitSignalOnProgrammChangeOffViewPort_)) {wasProgrammChangeOffViewPort_ = false;return;}
	//qDebug() << "emit SonogramInterface viewport ch";
	emit viewPortChanged(value_startX,value_startY,value_endX,value_endY);
}

void SonogramInterface::SetViewport( double start_hz, double start_smpl, double end_hz, double end_smpl )
{
	//qDebug() << "SonogramInterface set viewport SX" << start_hz << " SY" << start_smpl << " EX" << end_hz << " EY" << end_smpl;
	start_smpl = MainGraf->Grid_Layer->ApplyInterpretSum(start_smpl,true);
	end_smpl = MainGraf->Grid_Layer->ApplyInterpretSum(end_smpl,true);	
	wasProgrammChangeOffViewPort_ = true;
	MainGraf->SetViewportForValues(start_hz,start_smpl,end_hz,end_smpl);
}

void SonogramInterface::ClearAllSelections(bool noSignal)
{
	if (noSignal) skip_signal_on_selection_clear_ = true;
	else skip_signal_on_selection_clear_ = false;
	MainGraf->ClearAllSelections();
}

void SonogramInterface::SetSelection( double x1,double y1,double x2,double y2, bool noSignal )
{
	y1 = MainGraf->Grid_Layer->ApplyInterpretSum(y1,true);
	y2 = MainGraf->Grid_Layer->ApplyInterpretSum(y2,true);
	if (noSignal) skip_signal_on_selection_change_ = true;
	else skip_signal_on_selection_change_ = false;
	MainGraf->MouseLayer->SetSelection(x1,y1,x2,y2);
}

bool SonogramInterface::IsSomethingSelected() const
{
	return MainGraf->MouseLayer->HasSelection();
}

void SonogramInterface::GetSelection( double &start_x, double &start_y, double &end_x, double &end_y ) const
{
#define swap if (start_x > end_x) {double t = start_x;start_x = end_x;end_x = t;} if (start_y > end_y) {double t = start_y;start_y = end_y;end_y = t;}
	
	bool a = IsSomethingSelected();
	Q_ASSERT_X(a, "SonogramInterface::GetSelection", "Unable to get the selection, nothing is selected, you should check function IsSomethingSelected()");
	if (!a) return;

	//мульти селект не поддерживается поэтому выбираем то что есть
	/*if (MainGraf->MouseLayer->CurSelectionType <= 2)
	{
		bool isVertical = (MainGraf->MouseLayer->CurSelectionType == 2);
		if (isVertical)
		{
			QList<QPointF> lp = MainGraf->MouseLayer->getHVSelected(true);
			if (lp.count() > 0)
			{
				QPointF pp = lp.at(0);
				start_y = pp.x()-MainGraf->MouseLayer->VertInterpretSum;
				end_y = pp.y()-MainGraf->MouseLayer->VertInterpretSum;
				start_x = 0;
				end_x = 0;
				swap
			}
			return;
		}
		else
		{
			QList<QPointF> lp = MainGraf->MouseLayer->getHVSelected(false);
			if (lp.count() > 0)
			{
				QPointF pp = lp.at(0);
				start_x = pp.x();
				end_x = pp.y();
				start_y = 0;
				end_y = 0;
				swap
			}
			return;
		}
	}*/

	//ищем где же выделенно, ибо CurSelectionType может не соответствовать выделенному
	{
		QList<QPointF> lp = MainGraf->MouseLayer->getHVSelected(true);
		if (lp.count() > 0)
		{
			QPointF pp = lp.at(0);
			start_x = 0;
			end_x = 0;
			start_y = pp.x()-MainGraf->MouseLayer->VertInterpretSum;
			end_y = pp.y()-MainGraf->MouseLayer->VertInterpretSum;
			swap
			return;
		}
	}
	

	{
		QList<QPointF> lp = MainGraf->MouseLayer->getHVSelected(false);
		if (lp.count() > 0)
		{
			QPointF pp = lp.at(0);
			start_x = pp.x();
			end_x = pp.y();
			start_y = 0;
			end_y = 0;
			swap
			return;
		}
	}
	
	
	{
		QList<QRectF> lp = MainGraf->MouseLayer->getRectSelected();
		if (lp.count() > 0)
		{
			QRectF rect = lp.at(0);
			start_y = rect.top()-MainGraf->MouseLayer->VertInterpretSum;
			end_y = rect.bottom()-MainGraf->MouseLayer->VertInterpretSum;
			start_x = rect.left();
			end_x = rect.right();
			swap
			return;
		}
	}
	
}

void SonogramInterface::timerEvent( QTimerEvent * ev )
{
	if (SonogramActivated)	{	killTimer(TimerId);TimerId = 0;	}

	if (wallPaper.opacity() >= 0.7) {killTimer(TimerId);TimerId = 0;}
	wallPaper.setOpacity(wallPaper.opacity()+0.005);
}

void SonogramInterface::GetCurrentViewport(double &start_hz, double &start_smpl, double &end_hz, double &end_smpl)
{
	double a = 0;
	double b = 0;
	double c = 0;
	double d = 0; 
	if (material_1 != NULL) material_1->GetViewPort(a,b,c,d);
	start_hz = MainGraf->Grid_Layer->ApplyInterpretSum(a,false);
	end_hz = MainGraf->Grid_Layer->ApplyInterpretSum(c,false);
	start_smpl = b;
	end_smpl = d;
}

void SonogramInterface::on_selectionCleared()
{
	MainGraf->InfoLayer->SetValid(MainGraf->InfoLayer->UserData,false);
	MainGraf->InfoLayer->PrepareAndUpdateStandartText();
	if (!skip_signal_on_selection_clear_) emit SelectionCleared();
	else skip_signal_on_selection_clear_ = false;
}


void SonogramInterface::on_markerMoved(int index, double val,bool isVert )
{
	emit MarkerMoved(index,MainGraf->MouseLayer->ApplyInterpretSum(val,isVert),isVert);
}

void SonogramInterface::on_markersChanged( QList<double> inc_list,bool isVert )
{
	QVector<double> retVal(inc_list.count());
	for (int i = 0; i < inc_list.count();i++) retVal[i] = MainGraf->MouseLayer->ApplyInterpretSum(inc_list[i],!isVert);
	emit MarkersChanged(retVal, isVert);
}

void SonogramInterface::ShowMarkers()
{
	MainGraf->ShowMarkers();
}

void SonogramInterface::HideMarkers()
{
	MainGraf->HideMarkers();
}

void SonogramInterface::SetMoveAllMarkers()
{
	MainGraf->SetMoveAllMarkers();
}

void SonogramInterface::SetMoveOneMarker()
{
	MainGraf->SetMoveOneMarker();
}

void SonogramInterface::SetMarkers( QVector<double> inclist,bool isVert,bool noSignal /*= false*/ )
{
	MainGraf->MouseLayer->SetMarkers(inclist.toList(),isVert, noSignal);
}

void SonogramInterface::UpdateMarker( int index, double value,bool isVert)
{
	MainGraf->MouseLayer->UpdateMarker(index,value,isVert);
}

void SonogramInterface::SetCurSelectionType( int type )
{
	MainGraf->MouseLayer->CurSelectionType = type;
}

void SonogramInterface::SetMultySelectEnabled( bool enable )
{
	//ClearAllSelections(false);
	MainGraf->MouseLayer->SetMultySelectEnabled(enable);
}

QList<QPointF> SonogramInterface::GetAllHorSelections() const
{
	if (MainGraf->MouseLayer == nullptr) return QList<QPointF>();
	return MainGraf->MouseLayer->getHVSelected(false);
}

QList<QPointF> SonogramInterface::GetAllVertSelections() const
{
	if (MainGraf->MouseLayer == nullptr) return QList<QPointF>();
	return MainGraf->MouseLayer->getHVSelected(true);
}

QList<QRectF> SonogramInterface::GetAllRectSelections() const
{
	if (MainGraf->MouseLayer == nullptr) return QList<QRectF>();
	return MainGraf->MouseLayer->getRectSelected();
}

bool SonogramInterface::HasRectSelection() const
{
	if (MainGraf->MouseLayer == nullptr) return false;
	return MainGraf->MouseLayer->HasRectSelection();
}

bool SonogramInterface::HasHorSelection() const
{
	if (MainGraf->MouseLayer == nullptr) return false;
	return MainGraf->MouseLayer->HasHorSelection();
}

bool SonogramInterface::HasVertSelection() const
{
	if (MainGraf->MouseLayer == nullptr) return false;
	return MainGraf->MouseLayer->HasVertSelection();
}

void SonogramInterface::RemoveLastSelection()
{
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->RemoveLastSelection();
}

void SonogramInterface::SetButtonCtrlClickIsSingle( bool is_single /*= true*/ )
{
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->SetButtonCtrlClickIsSingle(is_single);
}


int SonogramInterface::curSelectionType()
{
	return MainGraf->MouseLayer->CurSelectionType;
}

bool SonogramInterface::eventFilter( QObject* obj, QEvent* ev )
{
	if (ev->type() == QEvent::Resize)
	{
		if (haveRelations_) wasProgrammChangeOffViewPort_ = true;
	}
	return false;
}

void SonogramInterface::MoveScreenByPixels( QPoint pixels,bool emit_viewPortChangeSignal /*= true*/ )
{
	wasProgrammChangeOffViewPort_ = !emit_viewPortChangeSignal;
	getBaseEngineClass()->ScreenDragByMouse( pixels);
}

void SonogramInterface::ClearAllMarkers()
{
	if (MainGraf->MouseLayer == nullptr) return;
	MainGraf->MouseLayer->ClearAllMarkers(true);
	MainGraf->MouseLayer->ClearAllMarkers(false);
}
