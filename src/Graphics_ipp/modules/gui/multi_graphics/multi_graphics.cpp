#include "../../../stdafx.h"
#include "multi_graphics.h"
#include "mg_fastlab_style.h"//цветовой стиль по умолчанию
#include <QApplication>
#include <QLayout>
#include <QLinearGradient>
#include <QBitmap>
#include <QPlainTextEdit>
#include <QTransform>
#include <QtDebug>
#include <QMessageBox>
#include <math.h>
#include <ippi.h>
#include <stdlib.h>
#include <algorithm>

#ifdef OMEGA
#include "third_party/crashrpt/include/CrashRpt.h"
#endif // OMEGA


#if 0
#define MG_Print(T)
#ifdef MG_EnableDebugOutput
	void Print(QString asd);
	#undef MG_Print
	#define MG_Print(T) Print(T);
#endif
#endif

//#define ScaleFocusToCenter //раскомментировать чтобы при Zoom желаемый объект смещался к центру

Q_MultiGraphics::Q_MultiGraphics(QWidget *parent,QSettings* settings) : QGraphicsView(parent)
{
	setTransformationAnchor(QGraphicsView::NoAnchor);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setCacheMode(QGraphicsView::CacheBackground);
	setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing,true);
	setOptimizationFlag(QGraphicsView::DontSavePainterState,true);
	setAttribute(Qt::WA_TranslucentBackground, false);
	//setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	
	setFrameStyle(0);

	MainSettings = new Q_MG_Style(settings);
	
	if (MainSettings == NULL) {QMessageBox::critical(this,tr("Init error"),tr("Settings file or pointer == NULL.")); return;}

	QLinearGradient BackGrad(0.0, 0.0, 0.0, 500);
	BackGrad.setColorAt(0.0, MainSettings->BackGroundBrush_BeginColor);
	BackGrad.setColorAt(0.5, MainSettings->BackGroundBrush_MidColor);
	BackGrad.setColorAt(1.0, MainSettings->BackGroundBrush_EndColor);
	setBackgroundBrush(BackGrad);
	setMouseTracking(false);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setStyleSheet("QToolTip "+MainSettings->ToolTipStyleSheet);
	
	MouseLayer = NULL;
	Grid_Layer = NULL;
	GridBack_Layer = NULL;
	InfoLayer = NULL;
	wallp_ = NULL;

	EnableHorZoom = true;
	EnableVertZoom  = true;
	SpectrumMode = false;
	VertStep = 2;
	HorStep = 2;

	QPixmapCache::setCacheLimit(1024*500);//500 метров КЭШ

	verticalScrollBar()->setMinimum(0);
	horizontalScrollBar()->setMinimum(0);

	PrepareScene();
}

Q_MultiGraphics::~Q_MultiGraphics()
{
	delete MainSettings;
}

void Q_MultiGraphics::PrepareScene()
{
	MainScene = new QGraphicsScene(this);
	setScene(MainScene);

	if (GridBack_Layer != NULL) delete GridBack_Layer;
	if (MouseLayer != NULL) delete MouseLayer;
	if (Grid_Layer != NULL) delete Grid_Layer;
	if (InfoLayer != NULL) delete InfoLayer;

	GridBack_Layer = new Q_MG_Back_Grid();
	Q_ASSERT_X(GridBack_Layer,"Q_MultiGraphics::PrepareScene","Error in initialization of scene, GridBack_Layer");
	GridBack_Layer->setPos(0,0);
	GridBack_Layer->settings = MainSettings;
	GridBack_Layer->setZValue(1);
	MainScene->addItem(GridBack_Layer);
	
	Grid_Layer = new Q_MG_Grid(GridBack_Layer);
	Q_ASSERT_X(Grid_Layer ,"Q_MultiGraphics::PrepareScene","Error in initialization of scene, Grid_Layer");
	Grid_Layer->setPos(0,0);
	Grid_Layer->settings = MainSettings;
	Grid_Layer->setZValue(4);
	MainScene->addItem(Grid_Layer);

	MouseLayer = new Q_MG_MouseCursor();
	Q_ASSERT_X(MouseLayer,"Q_MultiGraphics::PrepareScene","Error in initialization of scene, MouseLayer");
	MouseLayer->setPos(0,0);
	MouseLayer->setZValue(3);
	MouseLayer->settings = MainSettings;
	connect(MouseLayer,SIGNAL(dragScreen(QPointF)),this,SLOT(ScreenDragByMouse(QPointF)));
	connect(MouseLayer,SIGNAL(scaleScreen(QPointF,QPointF)),this,SLOT(ScreenScaleByMidMouse(QPointF,QPointF)));
	connect(MouseLayer,SIGNAL(wantContextMenu()),this,SLOT(on_wantContextMenu()));

	MainScene->addItem(MouseLayer);

	InfoLayer = new Q_MG_InfoWidget(MainSettings);
	Q_ASSERT_X(InfoLayer,"Q_MultiGraphics::PrepareScene","Error in initialization of scene, InfoLayer");
	InfoLayer->setZValue(3.5);
	InfoLayer->ErrasedbackGround = GridBack_Layer;
    //MainScene->addItem(InfoLayer);

    InfoLayer->hide();
    //InfoLayer->setVisible(false);


	//законнекчивание объектов в общую сеть
	connect(Grid_Layer,SIGNAL(sendCommand(QString,QString,QVariant)),MouseLayer,SLOT(getCommand(QString,QString,QVariant)));
	connect(Grid_Layer,SIGNAL(sendCommand(QString,QString,QVariant)),InfoLayer,SLOT(getCommand(QString,QString,QVariant)));
	
	connect(MouseLayer,SIGNAL(sendCommand(QString,QString,QVariant)),Grid_Layer,SLOT(getCommand(QString,QString,QVariant)));
	connect(MouseLayer,SIGNAL(sendCommand(QString,QString,QVariant)),InfoLayer,SLOT(getCommand(QString,QString,QVariant)));
	
	connect(InfoLayer,SIGNAL(sendCommand(QString,QString,QVariant)),MouseLayer,SLOT(getCommand(QString,QString,QVariant)));
	connect(InfoLayer,SIGNAL(sendCommand(QString,QString,QVariant)),Grid_Layer,SLOT(getCommand(QString,QString,QVariant)));

}


void Q_MultiGraphics::wheelEvent( QWheelEvent * event )
{
	int numDegrees = event->delta() / 8;
	double numSteps = numDegrees / 15;

	

// 	QPointF curKf = Grid_Layer->getCurKf();
	QPointF scalePoint(0,0);
	QPointF MousePos = event->pos();

	if (SpectrumMode)
	{
		//qDebug() << "SpectrumMode wheelEvent";
		if ((event->modifiers() == Qt::NoModifier) && (EnableHorZoom))
		{
			if (RelationBaseClass_)
			{//передача эвента скейла в сонограмму
				QWheelEvent sonoEvent(event->pos(),event->delta(),event->buttons(),Qt::ShiftModifier);
				RelationBaseClass_->wheelEvent(&sonoEvent);
				return;
			}
			scalePoint.setX(HorStep*(-numSteps));
		}
		
		if (((event->modifiers() & Qt::ControlModifier)) && (EnableVertZoom))
		{
			scalePoint.setY(VertStep*(-numSteps));
		}
	}
	else
	{
		//qDebug() << "SonogramMode wheelEvent";
		if (((event->modifiers() & Qt::ShiftModifier) || (event->modifiers() == Qt::NoModifier)) && (EnableHorZoom))
			scalePoint.setX(HorStep*(-numSteps));
		
		if (((event->modifiers() & Qt::ControlModifier) || (event->modifiers() == Qt::NoModifier)) && (EnableVertZoom))
			scalePoint.setY(VertStep*(-numSteps));
	}
	

	if (!scalePoint.isNull())
	{
		Grid_Layer->ScaleEvent(scalePoint,MousePos);
		AllCopyMatrixByGrid();
	}	
}

void Q_MultiGraphics::ScaleOnSteps( int steps, bool byX,bool byY )
{
	QPointF scalePoint(0,0);
	QPoint MousePos = QPoint(width()/2,height()/2);
	
	if (byX) scalePoint.setX(HorStep*(-(double)steps));
	if (byY) scalePoint.setY(VertStep*(-(double)steps));
	if (!scalePoint.isNull())
	{
		Grid_Layer->ScaleEvent(scalePoint,MousePos);
		AllCopyMatrixByGrid();
	}
}

void Q_MultiGraphics::resizeEvent( QResizeEvent * event )
{
	posWallPaper();

	Grid_Layer->ResizeEvent(QPoint(qMax(0, event->size().width()-2), qMax(0, event->size().height()-2)));
	AllCopyMatrixByGrid();

	QLinearGradient BackGrad(0.0, 0.0, 0.0, event->size().height());
	BackGrad.setColorAt(0.0, MainSettings->BackGroundBrush_BeginColor);
	BackGrad.setColorAt(0.5, MainSettings->BackGroundBrush_MidColor);
	BackGrad.setColorAt(1.0, MainSettings->BackGroundBrush_EndColor);
	setBackgroundBrush(BackGrad);
	
    event->ignore();
    this->ZoomOutFull();
}

void Q_MultiGraphics::keyPressEvent( QKeyEvent * event )
{
#ifdef _DEBUG
	qreal step = 10;
	if (event->key() == Qt::Key_Up)
	{
		Grid_Layer->MoveEvent(Grid_Layer->getCurTrans()+QPointF(0,-step));
		AllCopyMatrixByGrid();
	}
	if (event->key() == Qt::Key_Down)
	{
		Grid_Layer->MoveEvent(Grid_Layer->getCurTrans()+QPointF(0,step));
		AllCopyMatrixByGrid();
	}

	if (event->key() == Qt::Key_Left)
	{
		Grid_Layer->MoveEvent(Grid_Layer->getCurTrans()+QPointF(-step,0));
		AllCopyMatrixByGrid();
	}
	if (event->key() == Qt::Key_Right)
	{
		Grid_Layer->MoveEvent(Grid_Layer->getCurTrans()+QPointF(step,0));
		AllCopyMatrixByGrid();
	}

	/*if (event->key() == Qt::Key_Space)
	{
		Grid_Layer->DrawVGridBorders = !Grid_Layer->DrawVGridBorders;
		Grid_Layer->DrawHGridBorders = !Grid_Layer->DrawHGridBorders;
		Grid_Layer->ReDraw();
	}//*/

	/*if (event->key() == Qt::Key_1)
	{
		GridBack_Layer->setVisible(!GridBack_Layer->isVisible());
		Grid_Layer->ReDraw();
	}
	if (event->key() == Qt::Key_2)
	{
		Grid_Layer->DrawVSubDiv = !Grid_Layer->DrawVSubDiv;
		Grid_Layer->DrawHSubDiv = !Grid_Layer->DrawHSubDiv;
		Grid_Layer->ReDraw();
	}
	if (event->key() == Qt::Key_0) MouseLayer->CurSelectionType = 1;
	if (event->key() == Qt::Key_9) MouseLayer->CurSelectionType = 2;
	if (event->key() == Qt::Key_8) MouseLayer->CurSelectionType = 3;//*/

	//if (event->key() == Qt::Key_R) {Grid_Layer->ScaleAllToScreen();MouseLayer->CopyMatrix(Grid_Layer);if (Main_Layer != NULL) Main_Layer->CopyMatrix(Grid_Layer);}
	

	/*if (event->key() == Qt::Key_W) {Grid_Layer->flag_GridAligment = 0;Grid_Layer->ReDraw();MouseLayer->ReDraw();if (Main_Layer != NULL)  Main_Layer->ReDraw();}
	if (event->key() == Qt::Key_A) {Grid_Layer->flag_GridAligment = 1;Grid_Layer->ReDraw();MouseLayer->ReDraw();if (Main_Layer != NULL) Main_Layer->ReDraw();}
	if (event->key() == Qt::Key_D) {Grid_Layer->flag_GridAligment = 2;Grid_Layer->ReDraw();MouseLayer->ReDraw();if (Main_Layer != NULL) Main_Layer->ReDraw();}
	if (event->key() == Qt::Key_S) {Grid_Layer->flag_GridAligment = 3;Grid_Layer->ReDraw();MouseLayer->ReDraw();if (Main_Layer != NULL) Main_Layer->ReDraw();}//*/
	
	if ((event->key() == Qt::Key_Plus) || (event->key() == Qt::Key_Equal))
	{
		QWheelEvent* ev = new QWheelEvent(QPoint(width()/2,height()/2),120,Qt::NoButton,Qt::NoModifier);
		wheelEvent(ev);
		delete ev;
	}

	if (event->key() == Qt::Key_Minus)
	{
		QWheelEvent* ev = new QWheelEvent(QPoint(width()/2,height()/2),-120,Qt::NoButton,Qt::NoModifier);
		wheelEvent(ev);
		delete ev;
	}//*/

#endif

	event->ignore();
}

void Q_MultiGraphics::keyReleaseEvent( QKeyEvent * event )
{
	event->ignore();
}

void Q_MultiGraphics::ScreenDragByMouse( QPointF pf )
{
	QPointF TransPoint = pf;
	if (Grid_Layer->InverseHAxis) TransPoint.setX(-pf.x());
	if (Grid_Layer->InverseVAxis) TransPoint.setY(-pf.y());
	Grid_Layer->MoveEvent(Grid_Layer->getCurTrans()+TransPoint);
	
	AllCopyMatrixByGrid();
}

void Q_MultiGraphics::ScreenScaleByMidMouse( QPointF pf, QPointF MousePos )
{
	if (RelationBaseClass_)
	{
		RelationBaseClass_->ScreenScaleByMidMouse(pf,MousePos);
		return;
	}
	int stepX = 0;
	if (pf.x() > 0) stepX = 1;
	if (pf.x() < 0) stepX = -1;
	double scStep = 1.15;
// 	QPointF curKf = Grid_Layer->getCurKf();
	QPointF scalePoint(0,0);
	if (stepX != 0)
	{
		if (stepX > 0) scalePoint.setX(scStep);
		else scalePoint.setX(-scStep);
	}
	if (!scalePoint.isNull())
	{
		Grid_Layer->ScaleEvent(scalePoint,MousePos);
		AllCopyMatrixByGrid();
	}
}

void Q_MultiGraphics::addMaterial( Q_MG_Material* mat )
{
	if (mat == NULL) return;
	if (!Materials_LayersList.contains(mat))
	{
		Materials_LayersList.append(mat);
		
		mat->setPos(0,0);
		mat->setZValue((qreal)3 - ((qreal)1/(qreal)Materials_LayersList.count()));
		mat->MaterialID = Materials_LayersList.count();
		MainScene->addItem(mat);
		mat->settings = MainSettings;

		connect(Grid_Layer,SIGNAL(sendCommand(QString,QString,QVariant)),mat,SLOT(getCommand(QString,QString,QVariant)));
		connect(MouseLayer,SIGNAL(sendCommand(QString,QString,QVariant)),mat,SLOT(getCommand(QString,QString,QVariant)));
		connect(mat,SIGNAL(sendCommand(QString,QString,QVariant)),Grid_Layer,SLOT(getCommand(QString,QString,QVariant)));
		connect(mat,SIGNAL(sendCommand(QString,QString,QVariant)),MouseLayer,SLOT(getCommand(QString,QString,QVariant)));
	}

	mat->CopyMatrix(Grid_Layer);
	mat->ClearEl();
}

void Q_MultiGraphics::AllCopyMatrixByGrid()
{
	 MouseLayer->CopyMatrix(Grid_Layer);
	 InfoLayer->CopyMatrix(Grid_Layer);
	 for (int z = 0; z < Materials_LayersList.count();z++) Materials_LayersList[z]->CopyMatrix(Grid_Layer);
}

void Q_MultiGraphics::ZoomToSelection( int selIndex /*= 0*/ )
{
	if (!MouseLayer->HasSelection()) return;
	double Left = 0;
	double Right = 0;
	double Top = 0;
	double Bottom = 0;

	if (MouseLayer->CurSelectionType == 1) //!vert
	{
		QList<QPointF> selList = MouseLayer->getHVSelected(false);
		if ((selList.size() <= selIndex) || (selIndex < 0)) return;
		Left = selList.at(selIndex).x();
		Right = selList.at(selIndex).y();
	}

	if (MouseLayer->CurSelectionType == 2)
	{
		QList<QPointF> selList = MouseLayer->getHVSelected(true);
		if ((selList.size() <= selIndex) || (selIndex < 0)) return;
		Top = selList.at(selIndex).x();
		Bottom = selList.at(selIndex).y();
	}

	if (MouseLayer->CurSelectionType == 3)
	{
		QList<QRectF> selList = MouseLayer->getRectSelected();
		if ((selList.size() <= selIndex) || (selIndex < 0)) return;
		Left = selList.at(selIndex).x();
		Top = selList.at(selIndex).y();
		Right = selList.at(selIndex).right();
		Bottom = selList.at(selIndex).bottom();
	}

	SetViewportForValues(Left,Top,Right,Bottom);
}

void Q_MultiGraphics::ZoomOutFull(bool x, bool y)
{
	Grid_Layer->ScaleAllToScreen(x,y);
    AllCopyMatrixByGrid();
}

void Q_MultiGraphics::ClearAllSelections()
{
	 MouseLayer->ClearAllSelections();
}

void Q_MultiGraphics::SetDirectKf( QPointF newKf )
{
	Grid_Layer->setKf(newKf);
	AllCopyMatrixByGrid();
}

void Q_MultiGraphics::SetStrangeCross( double X, double Y)
{
	X -= MouseLayer->HorInterpretSum;
	Y -= MouseLayer->VertInterpretSum;
	MouseLayer->StrangeCrossValX = X;
	MouseLayer->StrangeCrossValY = Y;
	MouseLayer->ReDraw();
}

void Q_MultiGraphics::SetShowStrangeCross( bool value,bool horLabel,bool vertLabel )
{
	MouseLayer->showStrangeCross = value;
	MouseLayer->showStrangeCrossHorText = horLabel;
	MouseLayer->showStrangeCrossVertText = vertLabel;
	MouseLayer->ReDraw();
}

void Q_MultiGraphics::ShowMarkers()
{
	MouseLayer->showHMarkers = true;
	MouseLayer->showVMarkers = true;
	MouseLayer->ReDraw();
}

void Q_MultiGraphics::HideMarkers()
{
	MouseLayer->showHMarkers = false;
	MouseLayer->showVMarkers = false;
	MouseLayer->ReDraw();
}

void Q_MultiGraphics::SetMoveAllMarkers()
{
	MouseLayer->mutiMarkerMoove = true;
}

void Q_MultiGraphics::SetMoveOneMarker()
{
	MouseLayer->mutiMarkerMoove = false;
}

int Q_MultiGraphics::MarkersCount( bool isVertical ) const
{
	return MouseLayer->getMarkers(isVertical).size();
}

bool Q_MultiGraphics::MarkersVisible( bool isVertical ) const
{
	if (isVertical) return MouseLayer->showVMarkers;
	else return MouseLayer->showHMarkers;
}

bool Q_MultiGraphics::MoveAllMarkers() const
{
	return MouseLayer->mutiMarkerMoove;
}


void Q_MultiGraphics::SetViewportForValues( double LeftVal,double TopVal,double RightVal,double BottomVal )//в пользовательских (виртуальных) бубнах
{
	int mode = 0;
	if (TopVal == BottomVal) mode = 1;
	if (LeftVal == RightVal) mode = 2;
	LeftVal = Grid_Layer->DisApplyInterpretSum(LeftVal,false);
	RightVal = Grid_Layer->DisApplyInterpretSum(RightVal,false);
	TopVal = Grid_Layer->DisApplyInterpretSum(TopVal,true);
	BottomVal = Grid_Layer->DisApplyInterpretSum(BottomVal ,true);
	Grid_Layer->SetViewportForValues(LeftVal,TopVal,RightVal,BottomVal,mode);
	AllCopyMatrixByGrid();
}

void Q_MultiGraphics::scrollContentsBy(int a, int b)
{
	//  ничего не делаем если просят подвинуть сцену
}


/////////// Context Menu
void Q_MultiGraphics::on_wantContextMenu()
{
	if (contextMenu_ == NULL) return;
	if (contextMenu_->actions().size() > 0)
		contextMenu_->exec(QCursor::pos());
}

void Q_MultiGraphics::on_menuAboutToShow()
{
	MouseLayer->setCursor(Qt::ArrowCursor);
}

void Q_MultiGraphics::on_menuAboutToHide()
{
	MouseLayer->setCursor(Qt::BlankCursor);
	MouseLayer->updateCursorPosition(mapFromGlobal(QCursor::pos()));
}

void Q_MultiGraphics::setContextMenu( QMenu* menu )
{
	delete contextMenu_;
	if (menu == NULL) return;
	contextMenu_ = menu;
	connect(contextMenu_,SIGNAL(aboutToShow ()),this,SLOT(on_menuAboutToShow()));
	connect(contextMenu_,SIGNAL(aboutToHide ()),this,SLOT(on_menuAboutToHide()));
}
///////// END Context Menu

void Q_MultiGraphics::setWallPaper( QGraphicsPixmapItem* wallp )
{
	wallp_ =  wallp;
	wallp_->setAcceptHoverEvents(true);
	wallp_->setAcceptedMouseButtons(Qt::NoButton);
	wallp_->setFlag(QGraphicsItem::ItemIsFocusable,false);
	wallp_->setFlag(QGraphicsItem::ItemIsMovable,false);
	wallp_->setFlag(QGraphicsItem::ItemIsSelectable,false);
	//wallp_->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	wallp_->setZValue(1.5);
	MainScene->addItem(wallp_);
	wallp_->show();
	posWallPaper();
}

void Q_MultiGraphics::posWallPaper()
{
	if (!wallp_) return;
	int x = qMax(0, width()-2)/2 - (wallp_->boundingRect().width()/2);
	int y = qMax(0, height()-2)/2 - (wallp_->boundingRect().height()/2);
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	wallp_->setPos(x,y);
}

void Q_MultiGraphics::removeWallPaper()
{
	if (!wallp_) return;
	wallp_->hide();
	MainScene->removeItem(wallp_);
	wallp_ = NULL;
}

bool Q_MultiGraphics::isSomeZoomIsOnMax( bool &X, bool& Y )
{
	return Grid_Layer->isSomeZoomIsOnMax(X,Y);
}

/*//свободные подписи
void Q_MultiGraphics::ClearAllLabels()
{
	MouseLayer->ClearAllFredomLabels();
}

void Q_MultiGraphics::SetLabel( double valX_b,double valY_b,QString text )
{
	MouseLayer->SetFredomLabel(valX_b,valY_b,text);
}*/

//конец свободные подписи

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//						Q_MG_GRID	&&	Q_MG_Back_Grid					//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Q_MG_Back_Grid::Q_MG_Back_Grid( QGraphicsItem *parent /*= NULL*/ ): QGraphicsItem(parent)
{
	setAcceptHoverEvents(false);
	setAcceptedMouseButtons(Qt::NoButton);
	setFlag(QGraphicsItem::ItemIsFocusable,false);
	setFlag(QGraphicsItem::ItemIsMovable,false);
	setFlag(QGraphicsItem::ItemIsSelectable,false);
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	HightLightX = -1;
	HightLightY = -1;
}

void Q_MG_Back_Grid::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	painter->setRenderHint(QPainter::SmoothPixmapTransform,	false);
	painter->setRenderHint(QPainter::Antialiasing,false);

	QPen pn (Qt::DotLine);
	pn.setColor(settings->BackGrid_SubLinesColor);//цвет доп. линий
	painter->setPen(pn);
	for (int i = 0 ; i < XSubDiv.count();i++) painter->drawLine(QLineF(XSubDiv[i],0,XSubDiv[i],sz.y()));
	for (int i = 0 ; i < YSubDiv.count();i++) painter->drawLine(QLineF(0.0,YSubDiv[i],(qreal)sz.x(),YSubDiv[i]));

	painter->setPen(settings->BackGrid_BaseLinesColor);//цвет основных линий
	for (int i = 0 ; i < YDiv.count();i++) painter->drawLine(QLineF(0.0,YDiv[i],(qreal)sz.x(),YDiv[i]));
	for (int i = 0 ; i < XDiv.count();i++) painter->drawLine(QLineF(XDiv[i],0,XDiv[i],sz.y()));

	painter->setPen(settings->BackGrid_HightLinesColor);//цвет выделенных линий
	if (HightLightY >= 0) painter->drawLine(QLineF(0.0,HightLightY,(qreal)sz.x(),HightLightY));
	if (HightLightX >= 0) painter->drawLine(QLineF(HightLightX,0,HightLightX,sz.y()));
}

void Q_MG_Back_Grid::ClearAll()
{
	XSubDiv.clear();
	YSubDiv.clear();
	YDiv.clear();
	XDiv.clear();
	HightLightX = -1;
	HightLightY = -1;
}
//////////////////////////////////////////////////////////////////////////

Q_MG_Grid::Q_MG_Grid( Q_MG_Back_Grid* _GridBack_Layer,QGraphicsItem *parent ) : Q_MG_BaseClass(parent)
{
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::MidButton | Qt::RightButton | Qt::LeftButton);

	setFlag(QGraphicsItem::ItemIsFocusable,false);
	setFlag(QGraphicsItem::ItemIsMovable,false);
	setFlag(QGraphicsItem::ItemIsSelectable,false);
	setCursor(Qt::ArrowCursor);
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	
	setObjectName("mg_Grid");

	flag_GridAligment = 3;// 0 - LT 1 - LB 2 - RT 3 - RB
	flag_DrawVAxis = true;
	flag_DrawHAxis = true;
	DrawVGridBorders = true;
	DrawHGridBorders = true;
	DrawVSubDiv = true;
	DrawHSubDiv = true;
	DrawVBaseLine = true;
	DrawHBaseLine = true;
	LabelVAxis = "";
	LabelHAxis = "";
	GridBack_Layer = _GridBack_Layer;

	HightLightVVal = 0;
	HightLightHVal = 0;
	HGridNumberStep = 30;
	VGridNumberStep = 30;
	BorderW = 0;
	BorderH = 0;
	
}

void Q_MG_Grid::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	//MG_Print("Paint Grid");
	painter->setRenderHint(QPainter::SmoothPixmapTransform,	false);
	painter->setRenderHint(QPainter::Antialiasing,false);
	DrawStandartGrid(painter);
}

void Q_MG_Grid::DrawStandartGrid(QPainter* pnt1)
{
#define ChechY_continue(Y,V)	Y = GetPixelByValue(V,true); \
								if ((Y < 0) || (Y >= sz.y())) continue;\
								if (flag_DrawHAxis && DrawHGridBorders)	{\
									if ((flag_GridAligment == 0) || (flag_GridAligment == 2)) {	if (Y < BorderH) continue; }\
									else if (Y > sz.y()-BorderH-1) continue;\
								}

#define ChechX_continue(X,V)	X = GetPixelByValue(V,false); \
								if ((X < 0) || (X >= sz.x())) continue;\
								if (flag_DrawVAxis && DrawVGridBorders) { \
									if ((flag_GridAligment == 0) || (flag_GridAligment == 1)) {	if (X < BorderW) continue; }\
									else if (X > sz.x()-BorderW-1) continue;\
								}
	//////////////////////////////////////////////////////////////////////////
	
	QFontMetrics fnt = pnt1->font();

	//необходимы доя правильной маркировки
	double VerStartNumV = (Trans.y()*kf.y())+VertInterpretSum;
	double VerEndNumV = ((Trans.y()+sz.y())*kf.y())+VertInterpretSum;
	double HorStartNumV = (Trans.x()*kf.x())+HorInterpretSum;
	double HorEndNumV = ( (Trans.x()+sz.x())*kf.x() )+HorInterpretSum;
	
	double HightLightVy = -1;
	double HightLightHx = -1;

	GridBack_Layer->ClearAll();

	if (flag_DrawVAxis)
	{
		if ((DrawVSubDiv) && (GridBack_Layer->isVisible()))
		{
			for (int i = 0;i < SubDivRetValV.count();i++)
			{
				qreal y;ChechY_continue(y,SubDivRetValV[i]);
				GridBack_Layer->YSubDiv.append(y);
			}
		}
	}

	if (flag_DrawHAxis)
	{
		if ((DrawHSubDiv) && (GridBack_Layer->isVisible()))
		{
			for (int i = 0;i < SubDivRetValH.count();i++)
			{
				qreal x;ChechX_continue(x,SubDivRetValH[i]);
				GridBack_Layer->XSubDiv.append(x);
			}
		}
	}

	/////////////////////////////////////////////////////////////////////
	// ОСЬ YYYY
	/////////////////////////////////////////////////////////////////////
	if (flag_DrawVAxis)
	{
		qreal TextTop = 0;
		qreal TextBotton = sz.y();
		if (DrawVGridBorders)
		{
			
			//определяем границы расования в зависимости от горизонтальной оси
			if (flag_DrawHAxis)
			{
				if ((flag_GridAligment == 0) || (flag_GridAligment == 2)) {	TextTop = BorderH; TextBotton = sz.y();}
				else {	TextTop = 0; TextBotton = sz.y()-BorderH;}
			}

			//рисование градиентов шкал
			pnt1->setPen(settings->GridBorder_LineColor);
			QLinearGradient BorderGrad(0.0, 0.0, 0.0, sz.y());
			BorderGrad.setColorAt(0.0, settings->GridBorder_BeginColor);BorderGrad.setColorAt(0.5, settings->GridBorder_MidColor);BorderGrad.setColorAt(1.0, settings->GridBorder_EndColor);
			if ((flag_GridAligment == 0) || (flag_GridAligment == 1))
			{	
				pnt1->drawLine(QPointF(BorderW,TextTop),QPointF(BorderW,TextBotton));
				pnt1->fillRect(QRectF(0,TextTop,BorderW,TextBotton+1),BorderGrad);
			}
			else
			{
				pnt1->drawLine(QPointF(sz.x()-BorderW-1,TextTop),QPointF(sz.x()-BorderW-1,TextBotton));			
				pnt1->fillRect(QRectF(sz.x()-BorderW,TextTop,sz.x()-BorderW,TextBotton+1),BorderGrad);
			}
		}

		pnt1->setPen(settings->GridText_Color);//цвет букв
		for (int i = 0;i < IntervalRetValV.count();i++)
		{
			double RcV = IntervalRetValV.at(i);
			double Rc = RcV-VertInterpretSum;
			qreal y = 0;ChechY_continue(y,Rc);
			//рисование на заднем фоне, если он виден, основных линий
			if ((GridBack_Layer->isVisible()) && (DrawVBaseLine))
			{
				if (RcV == HightLightVVal) HightLightVy = y;
				else GridBack_Layer->YDiv.append(y); 
			}
					
			if (DrawVGridBorders)
			{
				//вывод подписей
				qreal textY = y;
				qreal numberHalf = (fnt.ascent()/2)-1;			
				QString OutText = Interpret(RcV,true,true,&printLabelVAxis,LabelVAxis, VerStartNumV, VerEndNumV,true);
				if (y-numberHalf < TextTop) textY = TextTop+numberHalf;
				if (y+numberHalf > TextBotton) textY = TextBotton-numberHalf;

				if ((flag_GridAligment == 0) || (flag_GridAligment == 1)) pnt1->drawText(QPointF((BorderW-5-fnt.width(OutText)),numberHalf+textY),OutText);
				else pnt1->drawText(QPointF(sz.x()-BorderW+5,numberHalf+textY),OutText);
			}
		}//for 0 -> IntervalRetVal.count
		
	}

	/////////////////////////////////////////////////////////////////////
	// ОСЬ XXXX
	/////////////////////////////////////////////////////////////////////
	if (flag_DrawHAxis)
	{
		qreal TextLeft = 0;
		qreal TextRight = sz.x();
		if (DrawHGridBorders)
		{
			//определяем границы расования в зависимости от вертикальной оси
			if (flag_DrawVAxis)
			{
				if ((flag_GridAligment == 0) || (flag_GridAligment == 1)) {	TextLeft = BorderW+1; TextRight = sz.x();}
				else {	TextLeft = 0; TextRight = sz.x()-BorderW-1;}
			}

			//рисование градиентов шкал
			pnt1->setPen(settings->GridBorder_LineColor);
			QLinearGradient BorderGrad(0.0, 0.0, sz.x(), 0.0);
			BorderGrad.setColorAt(0.0, settings->GridBorder_BeginColor);BorderGrad.setColorAt(0.5, settings->GridBorder_MidColor);BorderGrad.setColorAt(1.0, settings->GridBorder_EndColor);
			if ((flag_GridAligment == 0) || (flag_GridAligment == 2))
			{	
				pnt1->drawLine(QPointF(TextLeft,BorderH),QPointF(TextRight,BorderH));
				pnt1->fillRect(QRectF(TextLeft-1,0,TextRight+2,BorderH),BorderGrad);
			}
			else
			{
				pnt1->drawLine(QPointF(TextLeft,sz.y()-BorderH),QPointF(TextRight,sz.y()-BorderH));
				pnt1->fillRect(QRectF(TextLeft-1,sz.y()-BorderH+1,TextRight+2,sz.y()-BorderH+1),BorderGrad);
			}
		}

		pnt1->setPen(settings->GridText_Color);//цвет букв
		qreal prev_text_item_right = -50000;//правая (x+w) позиция предыдущего текста
		qreal prev_text_item_left  = 50000;//левая (x) позиция предыдущего текста
		for (int i = 0;i < IntervalRetValH.count();i++)
		{
			double RcV = IntervalRetValH.at(i);
			double Rc = RcV-HorInterpretSum;
			qreal x;ChechX_continue(x,Rc);

			//рисование на заднем фоне, если он виден, основных линий
			if ((GridBack_Layer->isVisible()) && (DrawHBaseLine))
			{
				if (RcV == HightLightHVal) HightLightHx = x;
				else GridBack_Layer->XDiv.append(x);
			}

			if (DrawHGridBorders)
			{
				//вывод подписей
				qreal textX = x;
				QString OutText = Interpret(RcV,false,true,&printLabelHAxis,LabelHAxis, HorStartNumV, HorEndNumV,true);
				qreal numberHalf = (fnt.width(OutText)/2);
				if (x-numberHalf < TextLeft)
				{
					textX = TextLeft+numberHalf;
					if (textX + numberHalf > prev_text_item_left-5) continue;
				}
				if (x+numberHalf > TextRight)
				{
					textX = TextRight-numberHalf;
					if (textX - numberHalf < prev_text_item_right+5) continue;
				}

				if ((flag_GridAligment == 0) || (flag_GridAligment == 2)) pnt1->drawText(QPointF(textX-numberHalf,BorderH-5),OutText);
				else pnt1->drawText(QPointF(textX-numberHalf,sz.y()-(BorderH-fnt.height())),OutText);
				prev_text_item_left = textX-numberHalf;
				prev_text_item_right = textX-numberHalf+fnt.width(OutText);
			}
		}//for 0 -> IntervalRetVal.count

	}//if flag_DrawHAxis

	//рисование выделенных линий
	if (GridBack_Layer->isVisible())
	{
		GridBack_Layer->HightLightY = HightLightVy;
		GridBack_Layer->HightLightX = HightLightHx;
	}

	//рисование угловой области
	if (flag_DrawVAxis && flag_DrawHAxis && DrawHGridBorders && DrawVGridBorders)
	{
		QPixmap* parentPixmap = dynamic_cast<QPixmap*>(pnt1->device());
		QImage im = parentPixmap->toImage();
		QColor col;
		QRadialGradient ConerGrad;
		ConerGrad.setRadius(qMin(BorderW,BorderH));
		ConerGrad.setColorAt(0.0, settings->GridConer_Color);
		if (printLabelHAxis.isEmpty()) printLabelHAxis = LabelHAxis;
		if (printLabelVAxis.isEmpty()) printLabelVAxis = LabelVAxis;
		QString elidedHText = fnt.elidedText(printLabelHAxis,Qt::ElideRight,BorderW-2);
		QString elidedVText = fnt.elidedText(printLabelVAxis,Qt::ElideRight,BorderW-2);
		pnt1->setPen(settings->GridText_Color);

		switch (flag_GridAligment)
		{
			
			case 0: ConerGrad.setCenter(0.0, 0.0);
					ConerGrad.setFocalPoint(0.0, 0.0);
					col = im.pixel(1,BorderH+1);
					ConerGrad.setColorAt(0.8, col);
					pnt1->fillRect(QRectF(0,0,BorderW,BorderH),ConerGrad);
					pnt1->drawText(BorderW-fnt.width(elidedHText),fnt.ascent()-2,elidedHText);
					pnt1->drawText(BorderW/2-fnt.width(elidedVText)/2,BorderH-2,elidedVText);
					break;
			case 1:	ConerGrad.setCenter(0.0, sz.y());
					ConerGrad.setFocalPoint(0.0, sz.y());
					col = im.pixel(1,sz.y()-BorderH-1);
					ConerGrad.setColorAt(0.8, col);
					pnt1->fillRect(QRectF(0,sz.y()-BorderH+1,BorderW,BorderH),ConerGrad);
					pnt1->drawText(BorderW-fnt.width(elidedHText),sz.y()-2,elidedHText);
					pnt1->drawText(BorderW/2-fnt.width(elidedVText)/2,sz.y()-BorderH+fnt.ascent()-1,elidedVText);
					break;
			case 2:	ConerGrad.setCenter(sz.x(), 0.0);
					ConerGrad.setFocalPoint(sz.x(), 0.0);
					col = im.pixel(sz.x()-1,BorderH+1);
					ConerGrad.setColorAt(0.8, col);
					pnt1->fillRect(QRectF(sz.x()-BorderW,0,BorderW,BorderH),ConerGrad);
					pnt1->drawText(sz.x()-BorderW,fnt.ascent()-2,elidedHText);
					pnt1->drawText(sz.x()-BorderW/2-fnt.width(elidedVText)/2,BorderH-2,elidedVText);
					break;
			case 3:	ConerGrad.setCenter(sz.x(), sz.y());
					ConerGrad.setFocalPoint(sz.x(), sz.y());
					col = im.pixel(sz.x()-1,sz.y()-BorderH-1);
					ConerGrad.setColorAt(0.8, col);
					pnt1->fillRect(QRectF(sz.x()-BorderW,sz.y()-BorderH+1,BorderW,BorderH),ConerGrad);
					pnt1->drawText(sz.x()-BorderW,sz.y()-2,elidedHText);
					pnt1->drawText(sz.x()-BorderW/2-fnt.width(elidedVText)/2,sz.y()-BorderH+fnt.ascent()-1,elidedVText);
					break;
		}		
	}

	//завершения рисования на заднем фоне, если он виден
	if (GridBack_Layer->isVisible())
	{
		GridBack_Layer->sz = sz;
		GridBack_Layer->ReDraw();
	}

#undef ChechY_continue
#undef ChechX_continue
}

QVector<double> Q_MG_Grid::MyMarkInterval( double start,double end,double WidthPix, double MinSpacePix, QVector<double>* Subdivision, double SubdivRate, bool isVertical)
{
#define NormalizeStartEndV if (qAbs(startV) > qAbs(endV)) { double t = endV;endV = startV;startV = t; }
	QVector<double> retVal;
	double startV = start;
	double endV = end;
	if (isVertical)
	{
		startV = start+VertInterpretSum;
		endV = end+VertInterpretSum;
	}
	else
	{
		startV = start+HorInterpretSum;
		endV = end+HorInterpretSum;
	}

	if ((startV < 0) && (endV > 0))
	{
		if (qAbs(startV) > endV)
		{
			double wPix = WidthPix - GetPixelByValue(end,isVertical);
			if (wPix < MinSpacePix) wPix = MinSpacePix;
			
			QVector<double> tmpVal = MarkInterval(0,qAbs(startV),wPix,MinSpacePix,Subdivision,SubdivRate);	
			int Count = tmpVal.count();
			for (int i =0; i < Count;i++)
			{
				tmpVal[i] = -tmpVal[i];
				if (-tmpVal[i] > endV) continue;
				tmpVal.append(-tmpVal[i]);
			}
			if ((DrawVSubDiv) || (DrawHSubDiv))
			{
				Count = Subdivision->count();
				for (int i =0; i < Count;i++)
				{
					Subdivision->replace(i,-Subdivision->at(i));
					if (-Subdivision->at(i) > endV) continue;
					Subdivision->append(-Subdivision->at(i));
				}
			}
			return tmpVal;
		}
		else
		{
			double wPix = WidthPix - GetPixelByValue(end,isVertical);
			if (wPix < MinSpacePix) wPix = MinSpacePix;

			QVector<double> tmpVal = MarkInterval(0,endV,wPix,MinSpacePix,Subdivision,SubdivRate);	
			int Count = tmpVal.count();
			for (int i =0; i < Count;i++)
			{
				if (tmpVal[i] > qAbs(startV)) break;
				tmpVal.append(-tmpVal[i]);
			}
			if ((DrawVSubDiv) || (DrawHSubDiv))
			{
				Count = Subdivision->count();
				for (int i =0; i < Count;i++)
				{
					if (Subdivision->at(i) > qAbs(startV)) break;
					Subdivision->append(-Subdivision->at(i));
				}
			}
			return tmpVal;
		}
	}
	if ((startV >= 0) && (endV > 0))
	{
		NormalizeStartEndV
		return MarkInterval(startV,endV,WidthPix,MinSpacePix,Subdivision,SubdivRate);
	}
	if ((startV < 0) && (endV <= 0))
	{
		NormalizeStartEndV
		QVector<double> tmpVal = MarkInterval(qAbs(startV),qAbs(endV),WidthPix,MinSpacePix,Subdivision,SubdivRate);
		for (int i =0; i < tmpVal.count();i++)	tmpVal[i] = -tmpVal[i];
		if ((DrawVSubDiv) || (DrawHSubDiv))	for (int i =0; i < Subdivision->count();i++) Subdivision->replace(i,-Subdivision->at(i));
		return tmpVal;
	}
	return retVal;
#undef NormalizeStartEndV
}

void Q_MG_Grid::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	event->accept();
}

QPointF Q_MG_Grid::getBorderHightWidth()
{
	return QPointF(BorderW,BorderH);
}

QPainterPath Q_MG_Grid::shape() const
{
	QPainterPath path;
	switch (flag_GridAligment)
	{
		case 0: path.addRect(0,0,BorderW,sz.y());path.addRect(BorderW,0,sz.x()-BorderW,BorderH);break;
		case 1: path.addRect(0,0,BorderW,sz.y());path.addRect(BorderW,sz.y()-BorderH+1,sz.x()-BorderW,BorderH);break;
		case 2: path.addRect(0,0,sz.x()-BorderW,BorderH);path.addRect(sz.x()-BorderW,0,BorderW,sz.y());break;
		case 3: path.addRect(0,sz.y()-BorderH+1,sz.x()-BorderW,BorderH);path.addRect(sz.x()-BorderW,0,BorderW,sz.y());break;
	}
	return path;
}

void Q_MG_Grid::getCommand( QString ToObj,QString CommandName,QVariant params )
{
	if ((ToObj != objectName()) && (ToObj != "mg_ALL")) return;
}

int Q_MG_Grid::MarkIntervalAndCalcBorderSizes()
{
#define ChechY_continue(Y,V)	Y = GetPixelByValue(V,true); \
								if ((Y < 0) || (Y >= sz.y())) continue;\
								if (flag_DrawHAxis && DrawHGridBorders)	{\
									if ((flag_GridAligment == 0) || (flag_GridAligment == 2)) {	if (Y < BorderH) continue; }\
									else if (Y > sz.y()-BorderH-1) continue;\
								}

#define ChechX_continue(X,V)	X = GetPixelByValue(V,false); \
								if ((X < 0) || (X >= sz.x())) continue;\
								if (flag_DrawVAxis && DrawVGridBorders) { \
									if ((flag_GridAligment == 0) || (flag_GridAligment == 1)) {	if (X < BorderW) continue; }\
									else if (X > sz.x()-BorderW-1) continue;\
								}

	QFontMetrics fnt = QFont();
	
	IntervalRetValV.clear();
	IntervalRetValH.clear();
	SubDivRetValV.clear();
	SubDivRetValH.clear();

	BorderW = 0;//ширина вертикального бордюра
	if (flag_DrawHAxis && DrawHGridBorders) BorderH= fnt.height()*2-fnt.ascent()/2; //константная высота горизонтального бордюра
	else BorderH = 0;


	//подготовка, маркировка и рисование доп. линий на заднем фоне
	double VerStartNum = Trans.y()*kf.y();
	double VerEndNum = (Trans.y()+sz.y())*kf.y();
	double HorStartNum = Trans.x()*kf.x();
	double HorEndNum = (Trans.x()+sz.x())*kf.x();
	if ((VerStartNum != VerStartNum) || (VerEndNum != VerEndNum) || (HorStartNum != HorStartNum) || (HorEndNum != HorEndNum)) return 1;

	if ((sz.y() <= 0) || (qAbs(VerEndNum-VerStartNum) == 0)) return 2;
	if ((sz.x() <= 0) || (qAbs(HorEndNum-HorStartNum) == 0)) return 2;

	QString tempP;//передать хоть че либо

	if (flag_DrawVAxis)
	{
		IntervalRetValV = MyMarkInterval(VerStartNum,VerEndNum,sz.y(),VGridNumberStep,&SubDivRetValV,0.25,true);	
		if (IntervalRetValV.count() < 4)
		{
			IntervalRetValV = MyMarkInterval(VerStartNum,VerEndNum,sz.y()*2,VGridNumberStep*2,&SubDivRetValV,0.25,true);
		}
		if (IntervalRetValV.count() == 0) return 1;
		
		double maxTextWidth = fnt.width(Interpret(IntervalRetValV.at(0),true,true,&tempP,"",VerStartNum,VerEndNum,false));
		if (DrawVGridBorders)
		{
			//вычисляем длинну самой длинно возможной строчки, для вычисления ширины бордюра
			for (int i = 1;i  < IntervalRetValV.count();i++)
			{
				double RcV = IntervalRetValV.at(i);
				double Rc = RcV-VertInterpretSum;
				qreal y;ChechY_continue(y,Rc);
				double pret = fnt.width(Interpret(RcV,true,true,&tempP,"",VerStartNum,VerEndNum,false));
				if (pret > maxTextWidth) maxTextWidth = pret;				
			}
			BorderW = maxTextWidth+10;//ширина бордюра
		}
		if (IntervalRetValV.count() == 0) return 1;
	}

	if (flag_DrawHAxis)
	{
		IntervalRetValH = MyMarkInterval(HorStartNum,HorEndNum,sz.x(),50,&SubDivRetValH,0.25,false);
		if (IntervalRetValH.count() == 0) return false;
		
		//вычисляем длинну самой длинно возможной строчки, для корректировки шага
		double maxTextWidth = fnt.width(Interpret(IntervalRetValH.at(0),false,true,&tempP,"",HorStartNum,HorEndNum,false));
		for (int i = 1;i  < IntervalRetValH.count();i++)
		{
			double RcV = IntervalRetValH.at(i);
			double Rc = RcV-HorInterpretSum;
			qreal x;ChechX_continue(x,Rc);
			double pret = fnt.width(Interpret(RcV,false,true,&tempP,"",HorStartNum,HorEndNum,false));
			if (pret > maxTextWidth) maxTextWidth = pret;				
		}
		maxTextWidth += HGridNumberStep;
		if (maxTextWidth < 50) maxTextWidth = 50;
		else
		{
			SubDivRetValH.clear();IntervalRetValH = MyMarkInterval(HorStartNum,HorEndNum,sz.x(),maxTextWidth,&SubDivRetValH,0.25,false);
		}
		
		if (IntervalRetValH.count() < 4)
		{
			SubDivRetValH.clear();
			IntervalRetValH = MyMarkInterval(HorStartNum,HorEndNum,sz.x()*2,maxTextWidth*2,&SubDivRetValH,0.25,false);
		}
		if (IntervalRetValH.count() == 0) return 1;
	}

	GridChanged(QPointF(BorderW,BorderH),flag_GridAligment);

	return 0;
#undef ChechY_continue
#undef ChechX_continue
}

//////////////////////////////////////////////////////////////////////////
//			 Q_MG_BaseClass
//////////////////////////////////////////////////////////////////////////

Q_MG_BaseClass::Q_MG_BaseClass(QGraphicsItem *parent) : QGraphicsObject(parent)
{
	MaxTransPoint = QPointF( 1, 1);
	MinTransPoint = QPointF( 0, 0);
	InverseVAxis = true;
	InverseHAxis = false;
	LockHorMove = false;
	LockVerMove = false;
	Release_ScaleIncrementType = true;
	kf = QPointF(1,1);
	MinimumKf = QPointF(1,1);
	Trans = QPointF(0,0);
	sz = QPoint(300,300);
	UserTrans = QPointF(0,0);
	MaximumZoom = QPointF(0.0000001,0.0000001);
	
	Vert_RoundValue = 0;
	Hor_RoundValue = 0;
	EnableInterpretAddLabel = false;
	Vert_InterpretAddLabel = "";
	Hor_InterpretAddLabel = "";
	VertInterpretSum = 0;
	HorInterpretSum = 0;
	EnableKiloMath = false;
	EnableHorTimerConvertor = false;
	EnableVertTimerConvertor = false;
	InterpretShowFullTime = false;
	VertTimerKf = 1;
	HorTimerKf = 1;
	

	EnableFixedScale = false;

	curGridBhw = QPointF(0,0);
	curGridAlig = 0;
	GridBottomStep = 0;
	GridTopStep = 0;
	GridLeftStep = 0;
	GridRightStep = 0;
	
}

void Q_MG_BaseClass::ResizeEvent( QPoint _sz, bool MinMaxChanged /*= false*/ )
{
	//пересчет минимально возможного KF
	QPoint CalcScreenSize;
	CalcScreenSize.setX((_sz.x()-1)-(GridRightStep+GridLeftStep));
	CalcScreenSize.setY((_sz.y()-1)-(GridBottomStep+GridTopStep));
	if (CalcScreenSize.y() < 0) CalcScreenSize.setY(0);
    if (CalcScreenSize.x() < 0) CalcScreenSize.setX(0);
	
	double totalValues;
   	if ((MinMaxChanged) || (_sz.x() != sz.x()))
	{
		totalValues = qAbs(MaxTransPoint.x()-MinTransPoint.x() );// + CalcScreenSize.x()
		double temp = (double)CalcScreenSize.x()/(double)totalValues;
		if (temp == 0) MinimumKf.setX(0);
		else MinimumKf.setX(1.0/temp);
	}
	if ((MinMaxChanged) || (_sz.y() != sz.y()))
	{
		totalValues = qAbs(MaxTransPoint.y()-MinTransPoint.y() );// + CalcScreenSize.y()
		double temp = (double)CalcScreenSize.y()/(double)totalValues;
		if (temp == 0) MinimumKf.setY(0);
		else MinimumKf.setY(1.0/temp);
	}

	//MinimumKf = QPointF(1.0,1.0);

	sz = _sz;
    
	on_Resize();
	MoveEvent(UserTrans,true);
}

void Q_MG_BaseClass::MoveEvent( QPointF _Trans, bool NeedCalcKf /*= false */)
{
	//qDebug() << "BaseClass::MoveEvent";
	
	if ((kf.y() == 0) || (kf.x() == 0)) return;
	
	if (!NeedCalcKf)
	{
		if (LockHorMove) _Trans.setX(UserTrans.x());
		if (LockVerMove) _Trans.setY(UserTrans.y());
	}

	
	double MaxXVal_val = MaxTransPoint.x();	
	double MinXVal_val = MinTransPoint.x();
	double MaxYVal_val = MaxTransPoint.y();
	double MinYVal_val = MinTransPoint.y();
	if (InverseHAxis)
	{	double z = MaxXVal_val; MaxXVal_val = MinXVal_val;MinXVal_val = z;	}
	if (InverseVAxis)
	{	double z = MaxYVal_val; MaxYVal_val = MinYVal_val;MinYVal_val = z;	}

	str_Collision colstr;

	colstr = CheckCollisionForTrans(_Trans,kf); 
		
	//Compare XCoord
	if (!(LockHorMove && !NeedCalcKf) && (MinimumKf.x() > 0))
	{
		if (kf.x() >= MinimumKf.x())
		{
			 kf.setX(MinimumKf.x());
			_Trans.setX(GetTransByValueOnPixel(MinXVal_val,GridLeftStep,false));
			LockHorMove = true;
		}
		if (kf.x() < MinimumKf.x())
		{
			if (colstr.X1Col)
			{
				_Trans.setX(GetTransByValueOnPixel(MaxXVal_val,(sz.x()-1)-GridRightStep,false));
			}
			if (colstr.X2Col)
			{
				_Trans.setX(GetTransByValueOnPixel(MinXVal_val,GridLeftStep,false));
			}
			LockHorMove = false;
		}
	}

	//Compare YCoord
	if (!(LockVerMove && !NeedCalcKf) && (MinimumKf.y() > 0))
	{
		
		if (kf.y() >= MinimumKf.y())
		{
			kf.setY(MinimumKf.y());
			_Trans.setY(GetTransByValueOnPixel(MinYVal_val,GridTopStep,true));
			LockVerMove = true;
		}
		if (kf.y() < MinimumKf.y())
		{
			if (colstr.Y1Col)
			{
				_Trans.setY(GetTransByValueOnPixel(MaxYVal_val,(sz.y()-1)-GridBottomStep,true));
			}
			if (colstr.Y2Col)
			{
				_Trans.setY(GetTransByValueOnPixel(MinYVal_val,GridTopStep,true));
			}
			LockVerMove = false;
		}
	}//*/
	

	UserTrans = _Trans;

	Trans = _Trans;
    Q_ASSERT(kf.x() >= 0);
    Q_ASSERT(kf.y() >= 0);

	if (kf.x() < MaximumZoom.x()) kf.setX(MaximumZoom.x());
	if (kf.y() < MaximumZoom.y()) kf.setY(MaximumZoom.y());

	//MG_Print("Trans.x = "+QString::number(Trans.x(),'f',1)+"  ;  Trans.y = "+QString::number(Trans.y(),'f',1));
	on_Move();
	ReDraw();
}

void Q_MG_BaseClass::SetViewportForValues( double LeftVal,double TopVal,double RightVal,double BottomVal,int mode ) 
{
	//в "реальных" бубнах, mode: 0-просчитываем все оси, 1-только 0X 2-только 0Y
	double MaxXVal_val = RightVal;	
	double MinXVal_val = LeftVal;
	double MaxYVal_val = TopVal;
	double MinYVal_val = BottomVal;
	//корректировка введенных не правильных значений и пердобразования относительно инверсии осей
	if (MaxXVal_val < MinXVal_val)
	{	double z = MaxXVal_val;MaxXVal_val = MinXVal_val; MinXVal_val = z;	}
	if (MaxYVal_val < MinYVal_val)
	{	double z = MaxYVal_val;MaxYVal_val = MinYVal_val; MinYVal_val = z; }
	
	if (MaxXVal_val > MaxTransPoint.x()) MaxXVal_val = MaxTransPoint.x();	
	if (MinXVal_val < MinTransPoint.x()) MinXVal_val = MinTransPoint.x();
	if (MaxYVal_val > MaxTransPoint.y()) MaxYVal_val = MaxTransPoint.y();	
	if (MinYVal_val < MinTransPoint.y()) MinYVal_val = MinTransPoint.y();

	if (InverseHAxis)
	{	double z = MaxXVal_val; MaxXVal_val = MinXVal_val;MinXVal_val = z;	}
	if (InverseVAxis)
	{	double z = MaxYVal_val; MaxYVal_val = MinYVal_val;MinYVal_val = z;	}

	QPoint CalcScreenSize;
	CalcScreenSize.setX((sz.x()-1)-(GridRightStep+GridLeftStep));
	CalcScreenSize.setY((sz.y()-1)-(GridBottomStep+GridTopStep));

	double totalValues;//всего бубнов
	QPointF newTrans = UserTrans;

	if ((mode == 0) || (mode == 1))
	{
		totalValues = qAbs(MaxXVal_val-MinXVal_val);
		kf.setX(1/((double)CalcScreenSize.x()/(double)totalValues));
		if (kf.x() < MaximumZoom.x())
		{
			kf.setX(MaximumZoom.x());
			newTrans.setX(GetTransByValueOnPixel(MinXVal_val+totalValues/2,sz.x()/2,false));
		}
		else newTrans.setX(GetTransByValueOnPixel(MinXVal_val,GridLeftStep,false));
	}
	
	if ((mode == 0) || (mode == 2))
	{
		totalValues = qAbs(MaxYVal_val-MinYVal_val);
		kf.setY(1/((double)CalcScreenSize.y()/(double)totalValues));
		if (kf.y() < MaximumZoom.y())
		{
			kf.setY(MaximumZoom.y());
			newTrans.setY(GetTransByValueOnPixel(MinYVal_val+totalValues/2,sz.y()/2,true));
		}
		else newTrans.setY(GetTransByValueOnPixel(MinYVal_val,GridTopStep,true));
	}
	
	MoveEvent(newTrans,true);
}

QPointF Q_MG_BaseClass::ScaleEvent( QPointF scaleKf, QPointF scalePos )
{
	//MG_Print("BaseClass::ScaleEvent");
	
	QPointF newKf;
	if (Release_ScaleIncrementType)
	{
		if (scaleKf.x() == 0) scaleKf.setX(1);
		if (scaleKf.y() == 0) scaleKf.setY(1);
		if (scaleKf.x() < 0) scaleKf.setX(1/(scaleKf.x()*-1));
		if (scaleKf.y() < 0) scaleKf.setY(1/(scaleKf.y()*-1));
		newKf.setX(kf.x()*scaleKf.x());
		newKf.setY(kf.y()*scaleKf.y());
	}
	else newKf = kf+scaleKf;

	if (newKf.x() <= 0)
	{
		if (!Release_ScaleIncrementType) newKf.setX(kf.x()+scaleKf.x()/10);
		if (newKf.x() <= 0) newKf.setX(1);
	}
	if (newKf.y() <= 0)
	{
		if (!Release_ScaleIncrementType) newKf.setY(kf.y()+scaleKf.y()/10);
		if (newKf.y() <= 0) newKf.setY(1);
	}

	if (newKf.x() < MaximumZoom.x())
		newKf.setX(MaximumZoom.x());
	if (newKf.y() < MaximumZoom.y()) newKf.setY(MaximumZoom.y());

	double posValX = GetValueByPixel(scalePos.x(),false);
	double posValY = GetValueByPixel(scalePos.y(),true);
	
	//алгоритм округления kf
	/*if ((!LockHorMoove) && (newKf.x() > 4))
	{
		unsigned __int64 nx = qRound64(newKf.x());
		newKf.setX(nx);
	}
	if ((!LockVerMoove) && (newKf.y() > 4))
	{
		unsigned __int64 ny = qRound64(newKf.y());
		newKf.setY(ny);
	}//*/
		
	if ((EnableFixedScale) && (FixedScaleList.size() > 0))
	{
		if (newKf.x() < FixedScaleList.first())
			newKf.setX(FixedScaleList.first());
		if (newKf.y() < FixedScaleList.first())
			newKf.setY(FixedScaleList.first());

		if (newKf.x() > FixedScaleList.last())
			newKf.setX(FixedScaleList.last());
		if (newKf.y() > FixedScaleList.last())
			newKf.setY(FixedScaleList.last());

		int indexX = 0;
		int indexY = 0;
		for (int i = 1;i < FixedScaleList.size();i++)
		{
			if ( qAbs(newKf.x() - FixedScaleList[i]) < qAbs(newKf.x() - FixedScaleList[indexX]) ) indexX = i;
			if ( qAbs(newKf.y() - FixedScaleList[i]) < qAbs(newKf.y() - FixedScaleList[indexY]) ) indexY = i;
		}

		newKf.setX(FixedScaleList[indexX]);
		newKf.setY(FixedScaleList[indexY]);
	}

	kf = newKf;
	if ((kf.y() < 0) || (kf.x() < 0)) DebugBreak();
	on_Scale();
	on_Scale_or_MinMaxChange();

#ifdef ScaleFocusToCenter
	int scrHX = sz.x()/2;
	int scrHY = sz.y()/2;
	int kf1 = scrHX - scalePos.x();
	scalePos.setX(scalePos.x()+ kf1/50);
	kf1 = scrHY - scalePos.y();
	scalePos.setY(scalePos.y() + kf1/50);//*/
#endif

	MoveEvent_ValueOnPixel(posValX,posValY,scalePos.x(),scalePos.y());
	//MoveEvent_ValueOnCenter(QPointF(posValX,posValY));
	//MG_Print("kf.x = "+QString::number(kf.x(),'f',6)+"  ;  kf.y = "+QString::number(kf.y(),'f',6));
	return kf;
}

QPointF Q_MG_BaseClass::getCurTrans()   {   return UserTrans;   }
QPointF Q_MG_BaseClass::getCurKf()      {   return kf;          }

double Q_MG_BaseClass::GetPixelByValue( double val,bool isVertical )
{
	if ((kf.y() == 0) || (kf.x() == 0)) return 0;
	if (isVertical)
	{
		if (InverseVAxis) return sz.y()-(val/kf.y()-Trans.y());
		else return val/kf.y()-Trans.y();
	}
	else
	{
		if (InverseHAxis) return sz.x()-(val/kf.x()-Trans.x());
		else return val/kf.x()-Trans.x();
	}
}

double Q_MG_BaseClass::GetValueByPixel( double pixel,bool isVertical )
{
	if (isVertical)
	{
		if (InverseVAxis) return (Trans.y()+(sz.y()-pixel))*kf.y();
		else return (Trans.y()+pixel)*kf.y();
	}
	else
	{
		if (InverseHAxis) return (Trans.x()+(sz.x()-pixel))*kf.x();
		else return (Trans.x()+pixel)*kf.x();
	}
}

double Q_MG_BaseClass::GetPixelByValueForTrans( QPointF tans, QPointF _kf, double val,bool isVertical )
{
	if ((_kf.y() == 0) || (_kf.x() == 0)) return 0;
	if (isVertical)
	{
		if (InverseVAxis) return (double)sz.y()-(val/_kf.y()-tans.y());
		else return val/_kf.y()-tans.y();
	}
	else
	{
		if (InverseHAxis) return (double)sz.x()-(val/_kf.x()-tans.x());
		else return val/_kf.x()-tans.x();
	}
}

double Q_MG_BaseClass::GetTransByValueOnPixel( double val, double pointOnScreen, bool isVertical )
{
	if ((kf.y() == 0) || (kf.x() == 0)) return 0;
	if (isVertical)
	{
		if (InverseVAxis) return pointOnScreen-sz.y()+val/kf.y();
		else return val/kf.y()-pointOnScreen;
	}
	else
	{
		if (InverseHAxis) return pointOnScreen-sz.x()+val/kf.x();
		else return val/kf.x()-pointOnScreen;
	}
}

void Q_MG_BaseClass::MoveEvent_ValueOnCenter( QPointF Value)
{
	double newPosX;
	double newPosY;
 	newPosX = GetTransByValueOnPixel(Value.x(),sz.x()/2,false);
	newPosY = GetTransByValueOnPixel(Value.y(),sz.y()/2,true);
	
	MoveEvent(QPointF(newPosX,newPosY),true);
}

void Q_MG_BaseClass::MoveEvent_ValueOnPixel( double valX, double valY, double PixelX, double PixelY )
{
	double newPosX;
	double newPosY;
	newPosX = GetTransByValueOnPixel(valX,PixelX,false);
	newPosY = GetTransByValueOnPixel(valY,PixelY,true);
	
	MoveEvent(QPointF(newPosX,newPosY),true);
}

str_Collision Q_MG_BaseClass::CheckCollisionForTrans( QPointF trans,QPointF _kf )
{
	str_Collision retVal;

	double MaxXVal_pix = GetPixelByValueForTrans(trans,_kf,MaxTransPoint.x(),false);
	double MinXVal_pix = GetPixelByValueForTrans(trans,_kf,MinTransPoint.x(),false);
	double MaxYVal_pix = GetPixelByValueForTrans(trans,_kf,MaxTransPoint.y(),true);
	double MinYVal_pix = GetPixelByValueForTrans(trans,_kf,MinTransPoint.y(),true);
	if (InverseHAxis)
	{	double z = MaxXVal_pix;MaxXVal_pix = MinXVal_pix;MinXVal_pix = z; }
	if (InverseVAxis)
	{	double z = MaxYVal_pix;MaxYVal_pix = MinYVal_pix;MinYVal_pix = z; }
	if (MaxXVal_pix <= (sz.x()-1)-GridRightStep) retVal.X1Col = true;
	if (MinXVal_pix > GridLeftStep) retVal.X2Col = true;//-1
	if (MaxYVal_pix <= (sz.y()-1)-GridBottomStep) retVal.Y1Col = true;
	if (MinYVal_pix > GridTopStep) retVal.Y2Col = true;
	return retVal;
}

void Q_MG_BaseClass::setMinAndMaxTransPoint( QPointF _MaxTransPoint,QPointF  _MinTransPoint)
{
	if (_MaxTransPoint.x() < _MinTransPoint.x())
	{	double z = _MinTransPoint.x();_MinTransPoint.setX(MaxTransPoint.x());_MaxTransPoint.setX(z);	}
	if (_MaxTransPoint.y() < _MinTransPoint.y())
	{	double z = _MinTransPoint.y();_MinTransPoint.setY(MaxTransPoint.y());_MaxTransPoint.setY(z);	}
	MaxTransPoint = _MaxTransPoint;MinTransPoint = _MinTransPoint;
	
	on_MinMaxChage();
	on_Scale_or_MinMaxChange();
    ResizeEvent(sz,true);
}

void Q_MG_BaseClass::ScaleAllToScreen(bool x,bool y)
{
	QPointF mx;
	if (x) mx.setX((MinimumKf.x()+10000000.0)/kf.x());
	if (y) mx.setY((MinimumKf.y()+10000000.0)/kf.y() );
	ScaleEvent(mx,QPointF(sz.x()/2.0,sz.y()/2.0));	
}

void Q_MG_BaseClass::CopyMatrix( Q_MG_BaseClass* src )
{
	Release_ScaleIncrementType = src->Release_ScaleIncrementType;
	InverseVAxis = src->InverseVAxis;
	InverseHAxis = src->InverseHAxis;
	LockHorMove = src->LockHorMove;
	LockVerMove = src->LockVerMove;
	MaximumZoom = src->MaximumZoom;
	VertInterpretSum = src->VertInterpretSum;
	HorInterpretSum = src->HorInterpretSum;
	HorTimerKf = src->HorTimerKf;
	VertTimerKf = src->VertTimerKf;
	
	bool Scale_or_MinMax_Chage = false;
	bool NeedCallScale = false;
	bool NeedCallMinMax = false;
	bool NeedCallMove = false;
	bool NeedCallResize = false;
	
	if (kf != src->kf) { kf = src->kf;Scale_or_MinMax_Chage = true;NeedCallScale = true;}
	if (MaxTransPoint != src->MaxTransPoint) { MaxTransPoint = src->MaxTransPoint;NeedCallMinMax = true;Scale_or_MinMax_Chage = true;}
	if (MinTransPoint != src->MinTransPoint) { MinTransPoint = src->MinTransPoint;NeedCallMinMax = true;Scale_or_MinMax_Chage = true;}
	
	if (sz != src->sz) {sz = src->sz;if (!Scale_or_MinMax_Chage) NeedCallResize=true;}
	if (Trans != src->Trans) {Trans = src->Trans;if (!Scale_or_MinMax_Chage) NeedCallMove=true;}
	UserTrans = src->UserTrans;
	MinimumKf = src->MinimumKf;

	curGridBhw = src->curGridBhw;
	curGridAlig = src->curGridAlig;
	GridTopStep = src->GridTopStep;
	GridLeftStep = src->GridLeftStep;
	GridRightStep = src->GridRightStep;
	GridBottomStep = src->GridBottomStep;
	
	if (NeedCallScale) on_Scale();
	if (NeedCallMinMax) on_MinMaxChage();
	if (NeedCallResize) on_Resize();
	if (NeedCallMove) on_Move();
	if (Scale_or_MinMax_Chage) {on_Scale_or_MinMaxChange();on_Move();}
	
	ReDraw();
}

QString Q_MG_BaseClass::Interpret( double val, bool isVertical,bool isVirtual, QString* out,QString In,double start,double end,bool startEndIsVirtual )
{
	QString retVal;
	QString addedLabel;
	
	if (!isVirtual)	val = ApplyInterpretSum(val,isVertical);
	if (!startEndIsVirtual)
	{
		start = ApplyInterpretSum(start,isVertical);
		end = ApplyInterpretSum(end,isVertical);
	}
	
	double LineEtalon = qMax(qAbs(start),qAbs(end));
	if ((start == 0) && (end == 0)) LineEtalon = val;

	auto kiloMath = [&]()
	{
		if (((EnableKiloMath) && ( (isVertical) && (!EnableVertTimerConvertor) )) || ( (EnableKiloMath) && ( (!isVertical) && (!EnableHorTimerConvertor) )))
		{
			if (LineEtalon > 0)
			{
				if (LineEtalon >= 5000000000) {val = val/1000000000; addedLabel = QObject::tr("Г"); return;}
				if (LineEtalon >= 5000000) {val = val/1000000; addedLabel = QObject::tr("М");return;}
				if (LineEtalon >= 5000) {val = val/1000; addedLabel = QObject::tr("к");return;}

				if (LineEtalon <= 0.0000009) {val = val*1000000000; addedLabel = QObject::tr("н"); return;}
				if (LineEtalon <= 0.0009) {val = val*1000000; addedLabel = QObject::tr("мк");return;}
				if (LineEtalon <= 0.9) {val = val*1000; addedLabel = QObject::tr("м");return;}						
			}
			else
			{
				if (LineEtalon <= -5000000000) {val = val/1000000000; addedLabel = QObject::tr("Г");return;}
				if (LineEtalon <= -5000000) {val = val/1000000; addedLabel = QObject::tr("М");return;}
				if (LineEtalon <= -5000) {val = val/1000; addedLabel = QObject::tr("к");return;}

				if (LineEtalon >= -0.0000009) {val = val*1000000000; addedLabel = QObject::tr("н"); return;}
				if (LineEtalon >= -0.0009) {val = val*1000000; addedLabel = QObject::tr("мк");return;}
				if (LineEtalon >= -0.9) {val = val*1000; addedLabel = QObject::tr("м");return;}		
			}			
		}
		return;
	};

	kiloMath();
	

	if ((isVertical) && (Vert_RoundValue > 0) && (!EnableVertTimerConvertor)) 
	{
		if (((val < 0.1) && (val > 0)) || ((val > -0.1) && (val < 0)))
		{
			retVal  = QString::number(val,'f',6);
		}
		else
		{
			qreal tmpRV = pow(10.0,Vert_RoundValue-1);
			if (tmpRV == 0) tmpRV = 1;
			retVal  = QString::number((double)qRound64(val*tmpRV)/tmpRV,'f',Vert_RoundValue-1);
		}
	}
	if ((!isVertical) && (Hor_RoundValue > 0) && (!EnableHorTimerConvertor))
	{
		if (((val < 0.1) && (val > 0)) || ((val > -0.1) && (val < 0)))
		{
			retVal  = QString::number(val,'f',6);
		}
		else
		{
			qreal tmpRV = pow(10.0,Hor_RoundValue-1);
			if (tmpRV == 0) tmpRV = 1;
			retVal  = QString::number((double)qRound64(val*tmpRV)/tmpRV,'f',Hor_RoundValue-1);
		}
	}

	if (retVal.isEmpty())
	{
		QString TimeFormat;
		if ((isVertical) && (EnableVertTimerConvertor)) retVal = FormatNanSecs(val/VertTimerKf,start/VertTimerKf,end/VertTimerKf,TimeFormat, InterpretShowFullTime);
		if ((!isVertical) && (EnableHorTimerConvertor)) retVal = FormatNanSecs(val/HorTimerKf,start/HorTimerKf,end/HorTimerKf,TimeFormat, InterpretShowFullTime);
		if (retVal.isEmpty()) retVal = QString::number(val);
		else  if (out != NULL) *out = TimeFormat;
	}


	if ((!addedLabel.isEmpty()) && (out == NULL))
	{
		if (EnableInterpretAddLabel)
		{
			if (isVertical)
			{
				if (!Vert_InterpretAddLabel.isEmpty()) retVal += " "+addedLabel+Vert_InterpretAddLabel;
			}
			else if (!Hor_InterpretAddLabel.isEmpty()) retVal += " "+addedLabel+Hor_InterpretAddLabel;
		}
		else
		{
			retVal += " "+addedLabel;
		}
		return retVal;
	}

	if ((out != NULL) && !( (isVertical && EnableVertTimerConvertor) || (!isVertical && EnableHorTimerConvertor)  ) )
	{
		if (!In.isEmpty()) *out = addedLabel+In;
	}

	if (EnableInterpretAddLabel)	
	{
		if (isVertical)
		{
			if (!Vert_InterpretAddLabel.isEmpty()) retVal += " "+Vert_InterpretAddLabel;
		}
		else if (!Hor_InterpretAddLabel.isEmpty()) retVal += " "+Hor_InterpretAddLabel;
	}
	return retVal;
}

bool Q_MG_BaseClass::GridChanged( QPointF Hw,int aligment )
{
	if ((curGridBhw == Hw) && (curGridAlig == aligment)) return false;
	switch (aligment)
	{
		case 0: GridTopStep = Hw.y();GridLeftStep = Hw.x()+1;GridRightStep = 0;GridBottomStep = 0;break;
		case 1:	GridBottomStep = Hw.y()-1;GridLeftStep = Hw.x()+1;GridRightStep = 0;GridTopStep = 0;break;
		case 2: GridTopStep = Hw.y();GridRightStep = Hw.x();GridLeftStep = 0;GridBottomStep = 0;break;
		case 3: GridBottomStep = Hw.y()-1;GridRightStep = Hw.x();GridLeftStep = 0;GridTopStep = 0;break;
		default: GridTopStep = 0;GridLeftStep = 0;GridRightStep = 0;GridBottomStep = 0;break;
	}
	
	curGridBhw = Hw;
	curGridAlig = aligment;
	
	ResizeEvent(sz,true);
	emit sendCommand("mg_ALL","gridChanged",0);
	return true;
}

double Q_MG_BaseClass::ApplyInterpretSum( double val,bool isVertical ) const
{
	if (isVertical) return val+VertInterpretSum;
	else return val+HorInterpretSum;
}

double Q_MG_BaseClass::DisApplyInterpretSum( double val,bool isVertical ) const
{
	if (isVertical) return val-VertInterpretSum;
	else return val-HorInterpretSum;
}

void Q_MG_BaseClass::Reset()
{
	Trans = QPointF(0,0);
	UserTrans = QPointF(0,0);
	kf = QPointF(1,1);
	MinimumKf = QPointF(1,1);

	MaxTransPoint = QPointF( 1500, 1500);
	MinTransPoint = QPointF( 0,-1500);
}

QPointF Q_MG_BaseClass::setKf( QPointF inc_kf )
{
	kf = inc_kf;
	on_Scale();
	on_Scale_or_MinMaxChange();
	MoveEvent_ValueOnPixel(0,0,sz.x()/2,sz.y()/2);
	return kf;
}

double Q_MG_BaseClass::GetValueByPixelPublic( double pixel,bool isVertical )
{
	 return GetValueByPixel(pixel,isVertical);
}

double Q_MG_BaseClass::GetPixelByValuePublic( double val,bool isVertical )
{
	return GetPixelByValue(val,isVertical);
}

void Q_MG_BaseClass::setFixedScaleList( QList<double> FixedList )
{
	if (FixedList.count() <= 0) return;
	FixedScaleList = FixedList;
	EnableFixedScale = true;
}

bool Q_MG_BaseClass::isSomeZoomIsOnMax( bool &X, bool& Y )
{
	X = false;Y = false;
	if (kf.x() == MaximumZoom.x()) X = true;
	if (kf.y() == MaximumZoom.y()) Y = true;
	return (X || Y);
}

//////////////////////////////////////////////////////////////////////////
//		Q_MG_MouseCursor
//////////////////////////////////////////////////////////////////////////

Q_MG_MouseCursor::Q_MG_MouseCursor(QGraphicsItem *parent) : Q_MG_BaseClass(parent)
{
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::MidButton | Qt::RightButton | Qt::LeftButton);
	setFlag(QGraphicsItem::ItemIsFocusable,false);
	setFlag(QGraphicsItem::ItemIsMovable,false);
	setFlag(QGraphicsItem::ItemIsSelectable,false);
	HideCursor = false;
	//CurGrid =  NULL;
	CursorPosX = 0;
	CursorPosY = 0;
	setObjectName("mg_Mouse");
	
	setCursor(Qt::BlankCursor);
	//setCursor(Qt::ArrowCursor);
	showBigGrafCursor = true;
	showMiniGrafCursor = false;
	showVValues = true;
	showHValues = true;
	SizeAllCur = new QCursor(QPixmap("://Q_MultiGraphics/Resources/SizeAllCur.png"),12,12);//://Q_MultiGraphics/Resources/CloseHandCur.png",9,6)
	CursorJastReDroped = false;
	UseMouseCursorJump = true;
	GlowEffect = true;

	//выделение
	CurSelectionType			= 0;
	multi_select_enable_		= true;
	button_ctrl_click_is_single_ = false;
	OnSelection					= false;
	selection_deselection_started_ = false;
	is_deselect_				= false;
	ShowSelectText				= true;
	DrawTextDecorateMidLine			= true;
	EnableByStepSelection		= false;
	stepValX					= 100;
	stepValY					= 100;
	//////////////////////////////////////////////////////////////////////////
		
	//markers
	showVMarkers				= true;
	showHMarkers				= true;
	showMarkersText				= true;
	showMarkersDeff				= true;
	showMarkerCentralParam		= true;
	mutiMarkerMoove				= false;
	MarkerGlowEffect			= true;
	OnMarkerMoove				= false;
	UnderMouseMarkerIndexIsVert = false;
	UnderMouseMarkerIndex		= -1;
	NeedSortHorMark				= true;
	NeedSortVertMark			= true;
	VerCurPix.load("://Q_MultiGraphics/Resources/VerCur.png");
	HorCurPix.load("://Q_MultiGraphics/Resources/HorCur.png");
	//////////////////////////////////////////////////////////////////////////

	//StrangeCross
	showStrangeCrossVertText	= true;
	showStrangeCrossHorText		= true;
	showStrangeCross			= true;
	StrangeCrossValX			= 10;
	StrangeCrossValY			= 10;
	//////////////////////////////////////////////////////////////////////////

	screenStartPosScaleX = 0;
	screenStartPosScaleY = 0;

	IgnoreNextHoverEvent = false;

	labels_manager_ = new NoVisualLabelsManager(this);
}

QRectF Q_MG_MouseCursor::boundingRect() const
{
	return QRectF(0,0,sz.x()+1,sz.y()+1);
}

void Q_MG_MouseCursor::paint( QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget )
{
	DrawSelections(painter);
	DrawMarkers(painter);
	DrawFredomLabels(painter);
	DrawStrangeCross(painter);
	

	if (isUnderMouse())
	{
		if (!HideCursor)
		{
			if (UnderMouseMarkerIndex >= 0)
			{
				if (UnderMouseMarkerIndexIsVert)
					painter->drawPixmap(CursorPosX-12,CursorPosY-12,HorCurPix);
				else
					painter->drawPixmap(CursorPosX-12,CursorPosY-12,VerCurPix);
				return;
			}
			
			//рисование курсоров
			if ((showBigGrafCursor))
			{
				if (GlowEffect)
				{
					painter->setPen(settings->MouseCursor_GlowColor);//цвет свечения курсора
					painter->drawLine(QPointF(CursorPosX+1,0),QPointF(CursorPosX+1,sz.y()));
					painter->drawLine(QPointF(CursorPosX-1,0),QPointF(CursorPosX-1,sz.y()));
					painter->drawLine(QPointF(0,CursorPosY-1),QPointF(sz.x(),CursorPosY-1));
					painter->drawLine(QPointF(0,CursorPosY+1),QPointF(sz.x(),CursorPosY+1));//*/
				}
				painter->setPen(settings->MouseCursor_Color);//цвет курсора
				painter->drawLine(QPointF(CursorPosX,0),QPointF(CursorPosX,sz.y()));
				painter->drawLine(QPointF(0,CursorPosY),QPointF(sz.x(),CursorPosY));
			}
			else
				if ((showMiniGrafCursor))
				{
					double CursorW = 10;
					painter->setPen(settings->MouseCursor_MiniCursorColor);
					painter->drawLine(QPointF(CursorPosX-CursorW,CursorPosY),QPointF(CursorPosX+CursorW,CursorPosY));
					painter->drawLine(QPointF(CursorPosX,CursorPosY-CursorW),QPointF(CursorPosX,CursorPosY+CursorW));
				}


			//вывод текущих значений курсора
			double VerTextW = 0;
			double VerTextXpos = 0;
			double VerTextYpos = CursorPosY-2;	
			QFontMetrics fnt(painter->font());
			painter->setPen(settings->MouseCursor_TextColor);//цвет текста подписей
			//показывать значение на вертикальной оси
			if (showVValues)
			{
				double PrintNum = GetValueByPixel(CursorPosY,true);
				QString PrintText = Interpret(PrintNum,true,false);

				VerTextW = fnt.width(PrintText);
				if ((curGridAlig == 0) || (curGridAlig == 1)) VerTextXpos = curGridBhw.x()+2;
				else VerTextXpos = sz.x()-curGridBhw.x()-VerTextW-2;
				if ((curGridAlig == 0) || (curGridAlig == 2))
				{	if (VerTextYpos-fnt.height() < curGridBhw.y()) VerTextYpos = curGridBhw.y()+fnt.height();}
				else if (VerTextYpos-fnt.height() < 0) VerTextYpos = fnt.height();
				painter->drawText(VerTextXpos,VerTextYpos,PrintText);
			}//if (showVValues)
			
			//показывать значение на горизонтальной оси
			if (showHValues)
			{
				double PrintNum = GetValueByPixel(CursorPosX,false);
				QString PrintText = Interpret(PrintNum,false,false);			
				double TextXpos = CursorPosX+2;
				double TextYpos = 0;
				if ((curGridAlig == 0) || (curGridAlig == 2)) TextYpos = curGridBhw.y()+fnt.height()-1;
				else TextYpos = sz.y()-curGridBhw.y()-2;
				if ((curGridAlig == 2) || (curGridAlig == 3))
				{
					if ((VerTextXpos < TextXpos+fnt.width(PrintText)+2) && (VerTextYpos-fnt.height() < TextYpos) && (VerTextYpos > TextYpos-fnt.height()))
						TextXpos = VerTextXpos-fnt.width(PrintText)-2;
					if (TextXpos+fnt.width(PrintText) > sz.x()-curGridBhw.x()) TextXpos = sz.x()-curGridBhw.x()-fnt.width(PrintText);	
				}
				else
				{
					if ((VerTextXpos+VerTextW+2 > TextXpos) && (VerTextYpos-fnt.height() < TextYpos) && (VerTextYpos > TextYpos-fnt.height()))
						TextXpos = VerTextXpos+VerTextW+2;
					if (TextXpos+fnt.width(PrintText)+2 > sz.x()) TextXpos = sz.x()-fnt.width(PrintText)-2;
				}
				painter->drawText(TextXpos,TextYpos,PrintText);
			}//if (showHValues)
		} //(!HideCursor)

	}//if (isUnderMouse())

}

void Q_MG_MouseCursor::updateCursorPosition(QPointF pos)
{
	HideCursor = false;
	CursorPosX =  pos.x();
	CursorPosY = pos.y();
	IgnoreNextHoverEvent = true;
	ReDraw();
}

void Q_MG_MouseCursor::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
	if  (IgnoreNextHoverEvent) {IgnoreNextHoverEvent = false;return;}
	HideCursor = false;
	CursorPosX = event->pos().x();
	CursorPosY = event->pos().y();
	ReDraw();
}

void Q_MG_MouseCursor::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	HideCursor = true;
	ReDraw();
	//event->accept();
}

void Q_MG_MouseCursor::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	//временна и служит чисто для теста, пока не придумал как и кто должен заниматься расстановкой маркеров
	//////////////////////////////////////////////////////////////////////////
    if ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ShiftModifier))
	{
		//установка маркера
		if (event->modifiers() & Qt::ControlModifier)
		{
			if (showVMarkers) addMarker(ApplyInterpretSum(GetValueByPixel(CursorPosY,true),true),false);
		}
		else
		{
			if (showHMarkers) addMarker(ApplyInterpretSum(GetValueByPixel(CursorPosX,false),false),true);
		}
		ReDraw();
		event->accept();
		return;
	}//*/
	//////////////////////////////////////////////////////////////////////////
	
	if ((event->buttons() == Qt::LeftButton) && (event->modifiers() == Qt::AltModifier))//== чтобы вместе с контролом больше ничего небыло нажато
	{
		StrangeCrossValX = GetValueByPixel(CursorPosX,false);
		StrangeCrossValY = GetValueByPixel(CursorPosY,true);
		ReDraw();
		event->accept();
		return;
	}


	if (event->buttons() & Qt::MidButton)
	{
		screenStartPosScaleX = event->screenPos().x();
		screenStartPosScaleY = event->screenPos().y();
		sceneStartPosScale = event->pos();
		event->accept();
		return;
	}

	if ((OnMarkerMoove) || (selection_deselection_started_))
	{
		event->ignore();
		return;
	}

	if (event->buttons() == Qt::LeftButton)
	{
		//старт таскания маркера
		if (UnderMouseMarkerIndex >= 0) OnMarkerMoove = true;
		StartSelectionDeselection(event);
	}

	if ((event->buttons() == Qt::RightButton) && (event->modifiers() == Qt::ControlModifier))
	{
		//удалить маркер
		if (UnderMouseMarkerIndex >= 0) 
		{
			RemoveMarker(UnderMouseMarkerIndex,UnderMouseMarkerIndexIsVert);
		}
		StartSelectionDeselection(event);
	}

	if ((event->buttons() == Qt::RightButton) && (event->modifiers() == Qt::NoModifier))
	{
		setCursor(*SizeAllCur);
		HideCursor = true;
		
		//инициализация расчета кол-ва пикселей перемещения мыши для вызова контекстного меню
		deltaMoveX = 0;
		deltaMoveY = 0;
	}
	ReDraw();
	event->accept();//необходимо принять евент  чтобы объект захвател фокус мыши и работал Release и Move
}

void Q_MG_MouseCursor::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	if (event->button() == Qt::MidButton) 
	{
        SetCursorPos(screenStartPosScaleX,screenStartPosScaleY);
		event->accept();
		return;
	}
	
	if (event->button() == Qt::LeftButton)
	{
		//конец таскания маркера если даже оно и не было
		OnMarkerMoove = false;
	}

	if ((selection_deselection_started_) && (OnSelection))
	{
		if ((event->button() == Qt::LeftButton) && (is_deselect_)) return;
		if ((event->button() == Qt::RightButton) && (!is_deselect_)) return;
		
		//конец выделение
		if ((CurSelectionType == 1) || (CurSelectionType == 2))
			if (CurMoveSel.x() > CurMoveSel.y()) {double z = CurMoveSel.x();CurMoveSel.setX(CurMoveSel.y());CurMoveSel.setY(z);}
		
		if (!is_deselect_)
		{
			if (CurSelectionType == 1)
			{
				
				bool some_ch = CalcAddSelectionPoint(HAxisSelList,QPointF(CurMoveSel.x(),CurMoveSel.y()));
				if (some_ch) emit selectedPixelChange(HAxisSelList.last().x(),0,HAxisSelList.last().y(),0 );
				emit selectionFinished( HAxisSelList.last().x(),0,HAxisSelList.last().y(),0 );
				//HAxisSelList.append(CurMoveSel.topLeft());
				//emit selectionFinished( CurMoveSel.x(),0,CurMoveSel.y(),0 );
			}
			if (CurSelectionType == 2)
			{
				bool some_ch = CalcAddSelectionPoint(VAxisSelList,QPointF(CurMoveSel.x(),CurMoveSel.y()));
				if (some_ch) emit selectedPixelChange(0,VAxisSelList.last().x(),0,VAxisSelList.last().y());
				emit selectionFinished( 0, VAxisSelList.last().x(),0,VAxisSelList.last().y());

				//VAxisSelList.append(CurMoveSel.topLeft());
				//emit selectionFinished( 0, CurMoveSel.x(),0,CurMoveSel.y() );
			}
			if (CurSelectionType == 3) 
			{
				if (CurMoveSel.x() > CurMoveSel.right()) {double z = CurMoveSel.x(); CurMoveSel.setX(CurMoveSel.right());CurMoveSel.setRight(z);}
				if (CurMoveSel.y() > CurMoveSel.bottom()) {double z = CurMoveSel.y(); CurMoveSel.setY(CurMoveSel.bottom());CurMoveSel.setBottom(z);}
				bool some_ch = CalcAddSelectRect(RectSelList,CurMoveSel);
				if (some_ch) emit selectedPixelChange(RectSelList.last().x(),RectSelList.last().y(),RectSelList.last().right(),RectSelList.last().bottom());
				emit selectionFinished(RectSelList.last().x(),RectSelList.last().y(),RectSelList.last().right(),RectSelList.last().bottom());
				
				//RectSelList.append(CurMoveSel);
				//emit selectionFinished( CurMoveSel.x(),CurMoveSel.y(),CurMoveSel.right(),CurMoveSel.bottom() );
			}
            if (CurSelectionType == 4)
            {
//                qDebug() << "END SELECTION";
                bool some_ch = CalcAddSelectionPoint(VAxisSelList,QPointF(CurMoveSel.x(),CurMoveSel.y()));
                if (some_ch) emit selectedPixelChange(0,VAxisSelList.last().y(),1000,VAxisSelList.last().y());
                emit selectionFinishedRedLine(VAxisSelList.last().y());
               // emit selectionFinished(0,RectSelList.last().y(),1000,RectSelList.last().y());

//                if (CurMoveSel.x() > CurMoveSel.right()) {double z = CurMoveSel.x(); CurMoveSel.setX(CurMoveSel.right());CurMoveSel.setRight(z);}
//                if (CurMoveSel.y() > CurMoveSel.bottom()) {double z = CurMoveSel.y(); CurMoveSel.setY(CurMoveSel.bottom());CurMoveSel.setBottom(z);}
//                bool some_ch = CalcAddSelectRect(RectSelList,CurMoveSel);
//                if (some_ch) emit selectedPixelChange(RectSelList.last().x(),RectSelList.last().y(),RectSelList.last().right(),RectSelList.last().bottom());
//                emit selectionFinished(RectSelList.last().x(),RectSelList.last().y(),RectSelList.last().right(),RectSelList.last().bottom());
            }
		}
		else
		{
			bool some_ch = false;
			if (CurSelectionType == 1)
			{
				some_ch = CalcDeselectPoint(HAxisSelList,QPointF(CurMoveSel.x(),CurMoveSel.y()),multi_select_enable_);
				if (HasHorSelection())
					if (some_ch)emit selectedPixelChange(HAxisSelList.last().x(),0,HAxisSelList.last().y(),0 );				
			}
			if (CurSelectionType == 2)
			{
				some_ch = CalcDeselectPoint(VAxisSelList,QPointF(CurMoveSel.x(),CurMoveSel.y()),multi_select_enable_);
				if (HasVertSelection())
					if (some_ch) emit selectedPixelChange(0,VAxisSelList.last().x(),0,VAxisSelList.last().y());
			}
			if (CurSelectionType == 3) 
			{
				if (CurMoveSel.x() > CurMoveSel.right()) {double z = CurMoveSel.x(); CurMoveSel.setX(CurMoveSel.right());CurMoveSel.setRight(z);}
				if (CurMoveSel.y() > CurMoveSel.bottom()) {double z = CurMoveSel.y(); CurMoveSel.setY(CurMoveSel.bottom());CurMoveSel.setBottom(z);}
				some_ch = CalcDeselectRect(RectSelList,CurMoveSel);
				if (HasRectSelection())
					if (some_ch) emit selectedPixelChange(RectSelList.last().x(),RectSelList.last().y(),RectSelList.last().right(),RectSelList.last().bottom());
			}
            if (CurSelectionType == 4)
            {
                //qDebug() << "END SELECTION22";
                if (CurMoveSel.x() > CurMoveSel.right()) {double z = CurMoveSel.x(); CurMoveSel.setX(CurMoveSel.right());CurMoveSel.setRight(z);}
                if (CurMoveSel.y() > CurMoveSel.bottom()) {double z = CurMoveSel.y(); CurMoveSel.setY(CurMoveSel.bottom());CurMoveSel.setBottom(z);}
                some_ch = CalcDeselectRect(RectSelList,CurMoveSel);
                if (HasRectSelection())
                    if (some_ch) emit selectedPixelChange(RectSelList.last().x(),RectSelList.last().y(),RectSelList.last().right(),RectSelList.last().bottom());
            }
			if (some_ch)
			{
				if (!HasSelection())
					emit selectionCleared();
				emit deselectFinished();
			}
		}
		
		OnSelection = false;
		selection_deselection_started_ = false;
		ReDraw();
		event->accept();
		return;
	}
	
	if (selection_deselection_started_) 
	{
		selection_deselection_started_ = false;
		return;
	}
	
	if ((event->button() == Qt::RightButton) && (event->modifiers() == Qt::NoModifier))
	{
		setCursor(Qt::BlankCursor);
		if ((deltaMoveX > 0	) || (deltaMoveY > 0) )	emit dragScreenFinish();
		if ((deltaMoveX <= 2) && (deltaMoveY <= 2)) emit wantContextMenu(); 
	}
	ReDraw();
	event->accept();
}

void Q_MG_MouseCursor::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{    
    if (event->buttons() & Qt::MidButton)
    {
        emit scaleScreen(QPointF(event->lastPos() - event->pos()).toPoint(),sceneStartPosScale);
        return;
    }

    if ((event->buttons() & Qt::LeftButton) && (event->modifiers() == Qt::AltModifier))//== чтобы вместе с контролом больше ничего небыло нажато
    {
        CursorPosX = event->pos().x();
        CursorPosY = event->pos().y();
        StrangeCrossValX = GetValueByPixel(CursorPosX,false);
        StrangeCrossValY = GetValueByPixel(CursorPosY,true);
        ReDraw();
        event->accept();
        emit StrangeCrossMoved();
        return;
    }

    //маркер
    if (event->buttons() & Qt::LeftButton)
    {
        CursorPosX = event->pos().x();
        CursorPosY = event->pos().y();
		
        //двигаем маркер под курсором
        if ((UnderMouseMarkerIndex >= 0) && (OnMarkerMoove))
        {
            if (UnderMouseMarkerIndexIsVert)
            {
                if (UnderMouseMarkerIndex < VertMark.count())
                {
                    double newPos = GetValueByPixel(CursorPosX,false);
                    if (newPos > MaxTransPoint.x()) newPos = MaxTransPoint.x();
                    if (newPos < MinTransPoint.x()) newPos = MinTransPoint.x();
                    if (mutiMarkerMoove)
                    {
                        double oldpos = VertMark[UnderMouseMarkerIndex];
                        for (int i = 0;i < VertMark.count();i++)
                        {
                            VertMark[i] += newPos-oldpos;
                            emit markerMooved(i,VertMark[i],true);
                        }
                    }
                    else
                    {
                        VertMark[UnderMouseMarkerIndex] = newPos;
                        emit markerMooved(UnderMouseMarkerIndex,VertMark[UnderMouseMarkerIndex],true);
                    }
                    emit markersChanged(VertMark,true);
                    NeedSortVertMark = true;
                }
            }
            else
                if (UnderMouseMarkerIndex < HorMark.count())
                {
                    double newPos = GetValueByPixel(CursorPosY,true);
                    if (newPos > MaxTransPoint.y()) newPos = MaxTransPoint.y();
                    if (newPos < MinTransPoint.y()) newPos = MinTransPoint.y();
                    if (mutiMarkerMoove)
                    {
                        double oldpos = HorMark[UnderMouseMarkerIndex];
                        for (int i = 0;i < HorMark.count();i++)
                        {
                            HorMark[i] += newPos-oldpos;
                            emit markerMooved(i,HorMark[i],false);
                        }
                    }
                    else
                    {
                        HorMark[UnderMouseMarkerIndex] = newPos;
                        emit markerMooved(UnderMouseMarkerIndex,HorMark[UnderMouseMarkerIndex],false);
                    }
                    emit markersChanged(HorMark,false);
                    NeedSortHorMark = true;
                }
        }
    }
    //////////////////////////////////////////////////////////////////////////

    //выделение
    if (selection_deselection_started_)
    {
        CursorPosX = event->pos().x();
        CursorPosY = event->pos().y();
		
        if ((CurSelectionType > 0) && (UnderMouseMarkerIndex < 0))
        {
            const double minimum_selected_pixels = 3;
            if (CurSelectionType == 1) //||
            {
                double prevPixel = GetPixelByValue(CurMoveSel.x(),false);
                //что бы случайно не выделялось по малу пикселей
                if ((qAbs(CursorPosX-prevPixel) >= minimum_selected_pixels) || (OnSelection))
                {
                    double valX = GetValueByPixel(CursorPosX,false);
                    valX = Correct_ValueByStep_or_PixelByGrid(valX,CursorPosX,false);
                    if (valX > MaxTransPoint.x()) valX = MaxTransPoint.x();
                    if (valX < MinTransPoint.x()) valX = MinTransPoint.x();
                    CurMoveSel.setY(valX);
                    if (!is_deselect_) emit selectedPixelChange(CurMoveSel.x(),0,CurMoveSel.y(),0);
                    OnSelection = true;
                }
            }
            if (CurSelectionType == 2) //=
            {
                double prevPixel = GetPixelByValue(CurMoveSel.x(),true);
                //что бы случайно не выделялось по малу пикселей
                if ((qAbs(CursorPosY-prevPixel) >= minimum_selected_pixels) || (OnSelection))
                {
                    double valY = GetValueByPixel(CursorPosY,true);
                    valY = Correct_ValueByStep_or_PixelByGrid(valY,CursorPosY,true);
                    if (valY > MaxTransPoint.y()) valY = MaxTransPoint.y();
                    if (valY < MinTransPoint.y()) valY = MinTransPoint.y();
                    CurMoveSel.setY(valY);
                    if (!is_deselect_) emit selectedPixelChange(0,CurMoveSel.x(),0,CurMoveSel.y());
                    OnSelection = true;
                }
            }
            if (CurSelectionType == 3) //[]
            {
                double prevPixelX = GetPixelByValue(CurMoveSel.x(),false);
                double prevPixelY = GetPixelByValue(CurMoveSel.y(),true);
                //что бы случайно не выделялось по малу пикселей
                if ((qAbs(CursorPosX-prevPixelX)+qAbs(CursorPosY-prevPixelY) > minimum_selected_pixels*2) || (OnSelection))
                {
                    double valX = GetValueByPixel(CursorPosX,false);
                    valX = Correct_ValueByStep_or_PixelByGrid(valX,CursorPosX,false);
                    double valY = GetValueByPixel(CursorPosY,true);
                    valY = Correct_ValueByStep_or_PixelByGrid(valY,CursorPosY,true);
                    if (valX > MaxTransPoint.x()) valX = MaxTransPoint.x();
                    if (valX < MinTransPoint.x()) valX = MinTransPoint.x();
                    if (valY > MaxTransPoint.y()) valY = MaxTransPoint.y();
                    if (valY < MinTransPoint.y()) valY = MinTransPoint.y();
                    CurMoveSel.setBottomRight(QPointF(valX,valY));
                    if (!is_deselect_) emit selectedPixelChange(CurMoveSel.x(),CurMoveSel.y(),CurMoveSel.right(),CurMoveSel.bottom());
                    OnSelection = true;
                }
            }
            if (CurSelectionType == 4) //[]
            {
//                qDebug() << "MOVE!!!!44444444444444444";
                double valY = GetValueByPixel(CursorPosY,true);
                if (valY > MaxTransPoint.y()) valY = MaxTransPoint.y();
                if (valY < MinTransPoint.y()) valY = MinTransPoint.y();
                CurMoveSel.setY(valY);
                if (!is_deselect_) emit selectedPixelChange(0,CurMoveSel.y(),100,CurMoveSel.y());
                OnSelection = true;
            }
        }
        ReDraw();
        event->accept();
        return;
    }
    //////////////////////////////////////////////////////////////////////////
	
    //таскание экрана
    if ((event->buttons() == Qt::RightButton) && (event->modifiers() == Qt::NoModifier))
    {
        #define DropReturn CursorJastReDroped = true;event->accept();return;
        if (UseMouseCursorJump)
        {
            if (event->screenPos().x() >= Desktop.screenGeometry().width()-1)
            {	SetCursorPos(2,event->screenPos().y());	DropReturn	}
            if (event->screenPos().x() <= 1)
            {	SetCursorPos(Desktop.screenGeometry().width()-2,event->screenPos().y()); DropReturn	}
            if (event->screenPos().y() >= Desktop.screenGeometry().height()-1)
            {	SetCursorPos(event->screenPos().x(),2);	DropReturn	}
            if (event->screenPos().y() <= 1)
            {	SetCursorPos(event->screenPos().x(),Desktop.screenGeometry().height()-2); DropReturn }
        }
        if (!CursorJastReDroped)
        {
            QPoint pp = QPointF(event->lastPos() - event->pos()).toPoint();
            emit dragScreen(pp);
            deltaMoveX += qAbs(pp.x());
            deltaMoveX += qAbs(pp.y());
        }
        CursorJastReDroped = false;
        #undef DropReturn
    }
    else
    {
        CursorPosX = event->pos().x();
        CursorPosY = event->pos().y();
    }
    ReDraw();
    event->accept();
}

void Q_MG_MouseCursor::DrawSelections( QPainter *painter )
{
	if (CurSelectionType > 0)
	{
		QBrush oldBrush = painter->brush();
		QPen oldPen = painter->pen();
		QBrush SellFill = settings->SelectionFillColor;//цвет выделения
		QPen SellPen = settings->SelectionBorderColor;//цвет бордюра выделения

		//статическое
		for (int z = 0;z < HAxisSelList.count();z++)//1
		{
			QPointF sel = HAxisSelList[z];
			double FromX = GetPixelByValue(sel.x(),false);
			double ToX = GetPixelByValue(sel.y(),false);
			QRectF selFigure(QPointF(FromX,-1),QPointF(ToX,sz.y()+1));
			if (FigureIsVisible(selFigure))
			{
				painter->setBrush(SellFill);painter->setPen(SellPen);				
				painter->drawRect(selFigure);
				TextDecorate(painter,selFigure,sel,false);
			}			
		}

        for (int z = 0;z < VAxisSelList.count();z++)
		{
            if(CurSelectionType == 4)
            {
                QPointF sel = VAxisSelList[z];
                double FromY = GetPixelByValue(sel.x(),true);
                double ToY = GetPixelByValue(sel.y(),true);
//                QRectF selFigure(QPointF(-1,ToY-1),QPointF(sz.x()+1,ToY+1));
//                if (FigureIsVisible(selFigure))
//                {
//                    painter->setBrush(SellFill);
//                    painter->setPen(SellPen);
//                    painter->drawRect(selFigure);
                    painter->setPen(QColor(255,0,0));
                    painter->drawLine(-1, ToY, sz.x() + 1, ToY);
//                    TextDecorate(painter,selFigure,sel,true);
//                }
            }
            else
            {
                QPointF sel = VAxisSelList[z];
                double FromY = GetPixelByValue(sel.x(),true);
                double ToY = GetPixelByValue(sel.y(),true);
                QRectF selFigure(QPointF(-1,FromY),QPointF(sz.x()+1,ToY));
                if (FigureIsVisible(selFigure))
                {
                    painter->setBrush(SellFill);painter->setPen(SellPen);
                    painter->drawRect(selFigure);
                    TextDecorate(painter,selFigure,sel,true);
                }
            }
		}

		for (int z = 0;z < RectSelList.count();z++)
		{
			QRectF sel = RectSelList[z];
			double FromX = GetPixelByValue(sel.x(),false);
			double ToX = GetPixelByValue(sel.right(),false);
			double FromY = GetPixelByValue(sel.y(),true);
			double ToY = GetPixelByValue(sel.bottom(),true);
			QRectF selFigure(QPointF(FromX,FromY),QPointF(ToX,ToY));
			if (FigureIsVisible(selFigure))
			{
				painter->setBrush(SellFill);painter->setPen(SellPen);				
				painter->drawRect(selFigure);
				TextDecorate(painter,selFigure,QPointF(sel.x(),sel.right()),false);
				TextDecorate(painter,selFigure,QPointF(sel.y(),sel.bottom()),true);
			}
		}
		//////////////////////////////////////////////////////////////////////////
			
		
		//динамическое рисование
		if (OnSelection)
		{            
			int text_deco_style = 0;
			if (is_deselect_)
			{
				SellFill = settings->DeselectionFillColor;
				SellPen = settings->DeselectionBorderColor;
				text_deco_style = 2;
			}

			if (CurSelectionType == 1)
			{
				double FromX = GetPixelByValue(CurMoveSel.x(),false);
				double ToX = GetPixelByValue(CurMoveSel.y(),false);
				QRectF selFigure(QPointF(FromX,-1),QPointF(ToX,sz.y()+1));
				painter->setBrush(SellFill);painter->setPen(SellPen);				
				painter->drawRect(selFigure);
				TextDecorate(painter,selFigure,CurMoveSel.topLeft(),false,text_deco_style);
			}
			if (CurSelectionType == 2)
			{
				double FromY = GetPixelByValue(CurMoveSel.x(),true);
				double ToY = GetPixelByValue(CurMoveSel.y(),true);
				QRectF selFigure(QPointF(-1,FromY),QPointF(sz.x()+1,ToY));
				painter->setBrush(SellFill);painter->setPen(SellPen);				
				painter->drawRect(selFigure);
				TextDecorate(painter,selFigure,CurMoveSel.topLeft(),true,text_deco_style);
			}
			if (CurSelectionType == 3)
			{
				double FromX = GetPixelByValue(CurMoveSel.x(),false);
				double ToX = GetPixelByValue(CurMoveSel.right(),false);
				double FromY = GetPixelByValue(CurMoveSel.y(),true);
				double ToY = GetPixelByValue(CurMoveSel.bottom(),true);
				QRectF selFigure(QPointF(FromX,FromY),QPointF(ToX,ToY));
				painter->setBrush(SellFill);painter->setPen(SellPen);				
				painter->drawRect(selFigure);
				TextDecorate(painter,selFigure,QPointF(CurMoveSel.x(),CurMoveSel.right()),false,text_deco_style);
				TextDecorate(painter,selFigure,QPointF(CurMoveSel.y(),CurMoveSel.bottom()),true,text_deco_style);
			}
            ///paint dynamic red line
            if (CurSelectionType == 4)
            {
                double FromX = GetPixelByValue(CurMoveSel.x(),false);
                double ToX = GetPixelByValue(CurMoveSel.right(),false);
                double FromY = GetPixelByValue(CurMoveSel.y(),true);
//                double ToY = GetPixelByValue(CurMoveSel.bottom(),true);
//                QRectF selFigure(QPointF(FromX,FromY),QPointF(ToX,ToY));
//                painter->setBrush(SellFill);painter->setPen(SellPen);
                painter->setPen(QColor(255,0,0));
                painter->drawLine(-1, FromY, sz.x() + 1, FromY);
//                TextDecorate(painter,selFigure,QPointF(CurMoveSel.x(),CurMoveSel.right()),false,text_deco_style);
//                TextDecorate(painter,selFigure,QPointF(CurMoveSel.y(),CurMoveSel.bottom()),true,text_deco_style);
            }
		}
		//////////////////////////////////////////////////////////////////////////
		painter->setBrush(oldBrush);
		painter->setPen(oldPen);
	}
}

void Q_MG_MouseCursor::TextDecorate( QPainter *painter,const QRectF& points, const QPointF& value, bool isVertical,int style)
{
	double FromX = points.x();
	double FromY = points.y();
	double ToX = points.right();
	double ToY = points.bottom();
	double IntervalStart = 0;
	double IntervalEnd = 0;
	QString OutText;
	QFontMetrics fnt(painter->font());
	if (FromX > ToX) {double z = FromX; FromX = ToX;ToX = z;}
	if (FromY > ToY) {double z = FromY; FromY = ToY;ToY = z;}
	if (style > 2) style = 0;
	
	double val = qAbs(value.y()-value.x());
	OutText = Interpret(val,isVertical,true);
	
	QPen MidLinePen;//карандаш центрального пунктира
	if (style == 0) MidLinePen.setColor(settings->TextDecorate_MidLineColor0);
	if (style == 1) MidLinePen.setColor(settings->TextDecorate_MidLineColor1);
	if (style == 2) MidLinePen.setColor(settings->TextDecorate_MidLineColor2);
	MidLinePen.setStyle(Qt::DotLine);

	if (isVertical)
	{
		if (ToY-FromY < 5) return;
		IntervalStart = FromY;
		IntervalEnd = ToY;

		//рисование центрального пунктира
		if ((DrawTextDecorateMidLine) && (style != 2))
		{
			painter->setPen(MidLinePen);
			double Half = (ToY-FromY)/2+FromY;
			painter->drawLine(FromX,Half,ToX,Half);
			//подпись к центральному пунктиру
			if ((style == 1) && (showMarkerCentralParam) && (ToY-FromY > 23))
			{
				double val2 = GetValueByPixel(Half,true);
				QString OutText2 = Interpret(val2,true,false);
				double TextH_2= fnt.height()/2;
				double TextXpos;
				if ((curGridAlig == 0) || (curGridAlig == 1)) TextXpos = sz.x()-fnt.width(OutText2)-25;
				else TextXpos = 22;
				QRectF LabelRect(QPointF(TextXpos-3,Half-TextH_2),QPointF(TextXpos+fnt.width(OutText2)+2,Half+TextH_2));
				painter->setBrush(settings->TextDecorate_MidLine_TextFillColor);//цвет фона
				QPen pen(settings->TextDecorate_MidLine_TextBorderColor);
				pen.setStyle(Qt::DotLine);
				painter->setPen(pen);
				painter->drawRect(LabelRect);
				painter->setPen(settings->TextDecorate_MidLine_TextColor);//цвет текста
				painter->drawText(TextXpos,Half+TextH_2-1,OutText2);
			}
		}

		if ((!ShowSelectText) || (style == 2)) return;

		if ((curGridAlig == 0) || (curGridAlig == 2))
		{
			if (IntervalStart < curGridBhw.y()) IntervalStart = curGridBhw.y();
			if (IntervalEnd > sz.y()) IntervalEnd = sz.y();
		}
		else
		{ 
			if (IntervalStart < 0) IntervalStart = 0;
			if (IntervalEnd > sz.y()-curGridBhw.y()) IntervalEnd = sz.y()-curGridBhw.y();
		}

		double TextXpos = 0;
		double TextYpos = ((IntervalEnd-IntervalStart)/2)+IntervalStart;
		//рисование текста
		if ((curGridAlig == 0) || (curGridAlig == 1))	
		{	
			if (style == 0)	TextXpos = FromX+1;
			if (style == 2)	TextXpos = FromX+1;//TODO:
			if (style == 1)	TextXpos = ToX-fnt.height()-4;
			if (ToX-FromX < fnt.height()+fnt.height()/2) TextXpos -= fnt.height()+fnt.height()/2;
			if (TextXpos < curGridBhw.x()+2) TextXpos = curGridBhw.x()+2;
			if (TextXpos > sz.x()-fnt.height()-7) TextXpos = sz.x()-fnt.height()-7;
		}
		else
		{	
			if (style == 0)	TextXpos = ToX-fnt.height()-4;
			if (style == 2)	TextXpos = ToX-fnt.height()-4;//TODO:
			if (style == 1) TextXpos = FromX+1;
			if (ToX-FromX < fnt.height()+fnt.height()/2) TextXpos += fnt.height()+fnt.height()/2+1;
			if (TextXpos > sz.x()-curGridBhw.x()-fnt.height()-5) TextXpos = sz.x()-curGridBhw.x()-fnt.height()-5;
			if (TextXpos < 2) TextXpos = 2;
		}

		int SpaceVal = 0;
		if (IntervalEnd-IntervalStart > fnt.width(OutText)+15)
		{
			TextYpos -= (fnt.width(OutText)/2);
			SpaceVal = 4;
		}
		else OutText = "";

		if (ToY-FromY < 20) return;
		//рисование Линии со стрелками
		double LineXPos = TextXpos+fnt.height()/2+1;
		//рисование направляющих если значение выскачило за пределы квадрата
		if (!points.contains(QPointF(LineXPos,FromY)))
		{
			QPen pen(settings->TextDecorate_AdditionLinesColor);
			pen.setStyle(Qt::DashLine);
			painter->setPen(pen);
			painter->drawLine(QLineF(LineXPos,FromY,FromX,FromY));
			painter->drawLine(QLineF(LineXPos,ToY,ToX,ToY));
		}

		QRectF LineRect(QPointF(LineXPos-fnt.height()/2,TextYpos-SpaceVal),QPointF(LineXPos+fnt.height()/2+1,TextYpos+SpaceVal+fnt.width(OutText)-1));
		//цвет линий
		if (style == 0) painter->setPen(settings->TextDecorate_BaseLineColor0);
		if (style == 1) painter->setPen(settings->TextDecorate_BaseLineColor1);
		if (style == 2) painter->setPen(settings->TextDecorate_BaseLineColor2);
		painter->drawLine(QLineF(LineXPos,FromY,LineXPos,LineRect.y()));
		painter->drawLine(QLineF(LineXPos,LineRect.bottom(),LineXPos,ToY));
		painter->drawLine(QLineF(LineXPos,FromY,LineXPos-2,FromY+4));
		painter->drawLine(QLineF(LineXPos,FromY,LineXPos+2,FromY+4));
		painter->drawLine(QLineF(LineXPos,ToY-1,LineXPos-2,ToY-4));
		painter->drawLine(QLineF(LineXPos,ToY-1,LineXPos+2,ToY-4));
		
		if (!OutText.isEmpty())
		{
			//цвет фона
			if (style == 0) painter->setBrush(settings->TextDecorate_BaseFillColor0);
			if (style == 1) painter->setBrush(settings->TextDecorate_BaseFillColor1);
			if (style == 2) painter->setBrush(settings->TextDecorate_BaseFillColor2);
			painter->drawRect(LineRect);	
			painter->setPen(settings->TextDecorate_BaseTextColor);//цвет текста
			//поворот и рисование
			if ((curGridAlig == 0) || (curGridAlig == 1))
			{
				painter->rotate(90);
				painter->drawText(TextYpos,-TextXpos-fnt.height()/2+2,OutText);
			}
			else
			{
				painter->rotate(-90);
				painter->drawText(-TextYpos-fnt.width(OutText),TextXpos+fnt.height()-1,OutText);
			}
			painter->resetTransform();
		}		
	}
	else //if (isVertical)
	{
		
		if (ToX-FromX < 5) return;
		IntervalStart = FromX;
		IntervalEnd = ToX;

		//рисование центрального пунктира
		if ((DrawTextDecorateMidLine) && (style != 2))
		{
			painter->setPen(MidLinePen);
			double Half = (ToX-FromX)/2+FromX;
			painter->drawLine(Half,FromY,Half,ToY);
			//подпись к центральному пунктиру
			if ((style == 1) && (showMarkerCentralParam) && (ToX-FromX > 23))
			{
				double val2 = GetValueByPixel(Half,false);
				QString OutText2 = Interpret(val2,false,false);
				double TextW_2= fnt.width(OutText2)/2;
				double TextYpos;
				if ((curGridAlig == 0) || (curGridAlig == 2)) TextYpos = sz.y()-7;
				else TextYpos = fnt.height()+3;
				QRectF LabelRect(QPointF(Half-TextW_2-2,TextYpos-fnt.height()),QPointF(Half+TextW_2+1,TextYpos+3));
				painter->setBrush(settings->TextDecorate_MidLine_TextFillColor);//цвет фона
				QPen pen(settings->TextDecorate_MidLine_TextBorderColor);
				pen.setStyle(Qt::DotLine);
				painter->setPen(pen);
				painter->drawRect(LabelRect);
				painter->setPen(settings->TextDecorate_MidLine_TextColor);//цвет текста
				painter->drawText(Half-TextW_2,TextYpos,OutText2);
			}
		}

		if ((!ShowSelectText) || (style == 2)) return;

		if ((curGridAlig == 0) || (curGridAlig == 1))
		{
			if (IntervalStart < curGridBhw.x()) IntervalStart = curGridBhw.x();
			if (IntervalEnd > sz.x()) IntervalEnd = sz.x();
		}
		else
		{ 
			if (IntervalStart < 0) IntervalStart = 0;
			if (IntervalEnd > sz.x()-curGridBhw.x()) IntervalEnd = sz.x()-curGridBhw.x();
		}
		
		double TextXpos = ((IntervalEnd-IntervalStart)/2)+IntervalStart;
		double TextYpos = 0;
		//рисование текста
		if ((curGridAlig == 0) || (curGridAlig == 2))	
		{	
			if (style == 0) TextYpos = FromY+fnt.height();
			if (style == 2) TextYpos = FromY+fnt.height();//TODO:
			if (style == 1) TextYpos = ToY-fnt.height()*2;
			if (ToY-FromY < fnt.height()+fnt.height()/2) TextYpos -= fnt.height()+fnt.height()/2;
			if (TextYpos < curGridBhw.y()+(fnt.height()*2)) TextYpos = curGridBhw.y()+(fnt.height()*2);
			if (TextYpos > sz.y()-5) TextYpos = sz.y()-5;
		}
		else
		{	
			if (style == 0) TextYpos = ToY-5;
			if (style == 2) TextYpos = ToY-5;//TODO:
			if (style == 1) TextYpos = FromY+fnt.height()*2+8;
			if (ToY-FromY < fnt.height()+fnt.height()/2) TextYpos += fnt.height()+fnt.height()/2;
			if (TextYpos > sz.y()-curGridBhw.y()-fnt.height()-1) TextYpos = sz.y()-curGridBhw.y()-fnt.height()-1;
			if (TextYpos < fnt.height()) TextYpos = fnt.height();
		}
		
		int SpaceVal = 0;
		if (IntervalEnd-IntervalStart > fnt.width(OutText)+15)
		{
			TextXpos -= (fnt.width(OutText)/2);
			SpaceVal = 4;
		} else OutText = "";
	
		if (ToX-FromX < 20) return;
		//рисование Линии со стрелками
		double LineYPos = TextYpos-fnt.height()/2+1;
		//рисование направляющих если значение выскачило за пределы квадрата
		if (!points.contains(QPointF(FromX,LineYPos)))
		{
			QPen pen(settings->TextDecorate_AdditionLinesColor);
			pen.setStyle(Qt::DashLine);
			painter->setPen(pen);
			painter->drawLine(QLineF(FromX,ToY,FromX,LineYPos));
			painter->drawLine(QLineF(ToX,ToY,ToX,LineYPos));
		}
		
		QRectF LineRect(QPointF(TextXpos-SpaceVal,LineYPos-fnt.height()/2),QPointF(TextXpos+SpaceVal+fnt.width(OutText)-1,LineYPos+fnt.height()/2+1));
		
		//цвет линий
		if (style == 0) painter->setPen(settings->TextDecorate_BaseLineColor0);
		if (style == 1) painter->setPen(settings->TextDecorate_BaseLineColor1);
		if (style == 2) painter->setPen(settings->TextDecorate_BaseLineColor2);
		painter->drawLine(QLineF(FromX,LineYPos,LineRect.x(),LineYPos));
		painter->drawLine(QLineF(LineRect.right(),LineYPos,ToX,LineYPos));
		painter->drawLine(QLineF(FromX,LineYPos,FromX+4,LineYPos-2));
		painter->drawLine(QLineF(FromX,LineYPos,FromX+4,LineYPos+2));
		painter->drawLine(QLineF(ToX-1,LineYPos,ToX-4,LineYPos-2));
		painter->drawLine(QLineF(ToX-1,LineYPos,ToX-4,LineYPos+2));
		if (!OutText.isEmpty())
		{
			//цвет фона
			if (style == 0) painter->setBrush(settings->TextDecorate_BaseFillColor0);
			if (style == 1) painter->setBrush(settings->TextDecorate_BaseFillColor1);
			if (style == 2) painter->setBrush(settings->TextDecorate_BaseFillColor2);
			painter->drawRect(LineRect);
			painter->setPen(settings->TextDecorate_BaseTextColor);//цвет текста
			painter->drawText(TextXpos,TextYpos,OutText);
		}
	}
}

bool Q_MG_MouseCursor::FigureIsVisible( const QRectF& points )
{
	QRectF screenRect;
	switch (curGridAlig)
	{
	case 0: screenRect = QRectF(curGridBhw,QPointF(sz));break;
	case 1: screenRect = QRectF(QPointF(curGridBhw.x(),0),QPointF(sz.x(),sz.y()-curGridBhw.y()));break;
	case 2: screenRect = QRectF(QPointF(0,curGridBhw.y()),QPointF(sz.x()-curGridBhw.x(),sz.y()));break;
	case 3: screenRect = QRectF(QPointF(0,0),QPointF(sz-curGridBhw));break;
	}
	return points.intersects(screenRect);
}

void Q_MG_MouseCursor::ClearAllSelections()
{
	OnSelection = false;
	bool callSignal = false;
	if (VAxisSelList.count() > 0) {VAxisSelList.clear();callSignal = true;}
	if (HAxisSelList.count() > 0) {HAxisSelList.clear();callSignal = true;}
	if (RectSelList.count() > 0) {RectSelList.clear();callSignal = true;}
	zoomOnButtonList_VAxisSel.clear();
	zoomOnButtonList_HAxisSel.clear();
	zoomOnButtonList_RectSel.clear();

	if (callSignal)
	{
		emit selectionCleared();
		ReDraw();
	}
}

QList<QPointF> Q_MG_MouseCursor::getHVSelected( bool Vert )
{
	if (!Vert)
	{
		QList<QPointF> HAxisSelRetList;
		for (int a = 0;a < HAxisSelList.count();a++)
		{
			QPointF tt (ApplyInterpretSum(HAxisSelList[a].x(),false),ApplyInterpretSum(HAxisSelList[a].y(),false));
			HAxisSelRetList.append(tt);
		}
		return HAxisSelRetList;
	}
	else
	{
		QList<QPointF> VAxisSelRetList;
		for (int a = 0;a < VAxisSelList.count();a++)
		{
			QPointF tt (ApplyInterpretSum(VAxisSelList[a].x(),true),ApplyInterpretSum(VAxisSelList[a].y(),true));
			VAxisSelRetList.append(tt);
		}
		return VAxisSelRetList;
	}
}

QList<QRectF> Q_MG_MouseCursor::getRectSelected()
{
	QList<QRectF> RectSelRetList;
	for (int a = 0;a < RectSelList.count();a++)
	{
		QPointF pp1 (ApplyInterpretSum(RectSelList[a].left(),false), ApplyInterpretSum(RectSelList[a].top(),true) );
		QPointF pp2 (ApplyInterpretSum(RectSelList[a].right(),false), ApplyInterpretSum(RectSelList[a].bottom(),true) );

		RectSelRetList.append(QRectF(pp1,pp2));
	}
	return RectSelRetList;
}

void Q_MG_MouseCursor::DrawMarkers( QPainter *painter )
{
	if (!OnMarkerMoove) UnderMouseMarkerIndex = -1;
	if ((!showVMarkers) && (!showHMarkers)) return;
	QPen oldPen = painter->pen();
	QPen markPen = settings->Marker_Color;//цвет маркера
	QPen markPenGlow = settings->Marker_GlowColor;//цвет свечения
	double prevVal;

	if (showVMarkers)
	{
		if (NeedSortVertMark)
		{
			sortedVertMark = VertMark;
			if (showMarkersDeff) qSort(sortedVertMark.begin(), sortedVertMark.end());
			NeedSortVertMark = false;
		}
		for (int i = 0;i < sortedVertMark.count();i++)
		{
			double posX = GetPixelByValue(sortedVertMark[i],false);
			QRectF markFig = QRectF(QPointF(posX-2,-1),QPointF(posX+2,sz.y()+1));
			//рисование подписи к маркеру и маркера
			if (FigureIsVisible(markFig))
			{
				QPointF startEndRect;
				painter->setPen(markPen);
				if (showMarkersText)
				{
					QFontMetrics fnt(painter->font());
					QString OutText = Interpret(sortedVertMark[i],false,false);
					double TextW_2 = fnt.width(OutText)/2;
					double TextYpos;
					if ((curGridAlig == 0) || (curGridAlig == 2)) TextYpos = sz.y()-7;
					else TextYpos = fnt.height()+3;
					QRectF LabelRect(QPointF(posX-TextW_2-4,TextYpos-fnt.height()),QPointF(posX+TextW_2+3,TextYpos+3));
					painter->setBrush(settings->Marker_TextFillColor);//цвет фона
					painter->drawRect(LabelRect);
					painter->setPen(settings->Marker_TextColor);//цвет текста
					painter->drawText(posX-TextW_2,TextYpos,OutText);
					startEndRect = QPointF(LabelRect.y(),LabelRect.bottom());
				}
				else startEndRect = QPointF(-1,-1);

				if (MarkerGlowEffect)
				{
					painter->setPen(markPenGlow);
					painter->drawLine(QLineF(posX-1,-1,posX-1,startEndRect.x()));
					painter->drawLine(QLineF(posX-1,startEndRect.y(),posX-1,sz.y()+1));
					painter->drawLine(QLineF(posX+1,-1,posX+1,startEndRect.x()));
					painter->drawLine(QLineF(posX+1,startEndRect.y(),posX+1,sz.y()+1));
				}
				painter->setPen(markPen);
				painter->drawLine(QLineF(posX,-1,posX,startEndRect.x()));
				painter->drawLine(QLineF(posX,startEndRect.y(),posX,sz.y()+1));

				if ((markFig.contains(QPointF(CursorPosX,CursorPosY))) && (!OnSelection) && (!OnMarkerMoove))
				{	UnderMouseMarkerIndex = VertMark.indexOf(sortedVertMark[i]);UnderMouseMarkerIndexIsVert = true;	}
			} //if (FigureIsVisible(markFig))
			
			//разметка ширины между маркерами
			if (showMarkersDeff)
			{
				if (i == 0) {prevVal = posX;continue;}
				markFig = QRectF(QPointF(prevVal,-1),QPointF(posX,sz.y()+1));
				if (FigureIsVisible(markFig)) TextDecorate(painter,markFig,QPointF(sortedVertMark[i-1],sortedVertMark[i]),false,1);
			}
			prevVal = posX;
		}
	}


	if (showHMarkers)
	{
		if (NeedSortHorMark)
		{
			sortedHorMark = HorMark;
			if (showMarkersDeff) qSort(sortedHorMark.begin(), sortedHorMark.end());
			NeedSortHorMark = false;
		}
		for (int i = 0;i < sortedHorMark.count();i++)
		{
			double posY = GetPixelByValue(sortedHorMark[i],true);
			QRectF markFig = QRectF(QPointF(-1,posY-2),QPointF(sz.x()+1,posY+2));
			//рисование подписи к маркеру
			if (FigureIsVisible(markFig))
			{
				QPointF startEndRect;
				painter->setPen(markPen);
				if (showMarkersText)
				{
					QFontMetrics fnt(painter->font());
					QString OutText = Interpret(sortedHorMark[i],true,false);
					double TextH_2= fnt.height()/2;
					double TextXpos;
					if ((curGridAlig == 0) || (curGridAlig == 1)) TextXpos = sz.x()-fnt.width(OutText)-23;
					else TextXpos = 20;
					QRectF LabelRect(QPointF(TextXpos-3,posY-TextH_2),QPointF(TextXpos+fnt.width(OutText)+2,posY+TextH_2));
					painter->setBrush(settings->Marker_TextFillColor);//цвет фона
					painter->drawRect(LabelRect);
					painter->setPen(settings->Marker_TextColor);//цвет текста
					painter->drawText(TextXpos,posY+TextH_2-1,OutText);
					startEndRect = QPointF(LabelRect.x(),LabelRect.right());
				}
				else startEndRect = QPointF(-1,-1);
				
				if (MarkerGlowEffect)
				{
					painter->setPen(markPenGlow);
					painter->drawLine(QLineF(-1,posY-1,startEndRect.x(),posY-1));
					painter->drawLine(QLineF(startEndRect.y(),posY-1,sz.x()+1,posY-1));
					painter->drawLine(QLineF(-1,posY+1,startEndRect.x(),posY+1));
					painter->drawLine(QLineF(startEndRect.y(),posY+1,sz.x()+1,posY+1));
				}
				painter->setPen(markPen);
				painter->drawLine(QLineF(-1,posY,startEndRect.x(),posY));
				painter->drawLine(QLineF(startEndRect.y(),posY,sz.x()+1,posY));

				if ((markFig.contains(QPointF(CursorPosX,CursorPosY))) && (!OnSelection) && (!OnMarkerMoove))
				{	UnderMouseMarkerIndex = HorMark.indexOf(sortedHorMark[i]);UnderMouseMarkerIndexIsVert = false;	}
			}//if (FigureIsVisible(markFig))

			//разметка ширины между маркерами
			if (showMarkersDeff)
			{
				if (i == 0) {prevVal = posY;continue;}
				markFig = QRectF(QPointF(-1,prevVal),QPointF(sz.x()+1,posY));
				if (FigureIsVisible(markFig)) TextDecorate(painter,markFig,QPointF(sortedHorMark[i-1],sortedHorMark[i]),true,1);
			}
			prevVal = posY;
		}
	}
	painter->setPen(oldPen);
}

void Q_MG_MouseCursor::addMarker( double Val,bool isVert )
{
	if (isVert)
	{
		Val -= HorInterpretSum;
		VertMark.append(Val);NeedSortVertMark = true;
		emit markersChanged(VertMark,isVert);
	}
	else
	{
		Val -= VertInterpretSum;
		HorMark.append(Val);NeedSortHorMark = true;
		emit markersChanged(HorMark,isVert);
	}
	ReDraw();
}

QList<double> Q_MG_MouseCursor::getMarkers( bool isVert )
{
	if (isVert)
	{
		QList<double> VertRetList;
		for (int a = 0;a < VertMark.count();a++)
			VertRetList.append(ApplyInterpretSum(VertMark[a],false));
		return VertRetList;
	}
	else
	{
		QList<double> RetList;
		for (int a = 0;a < HorMark.count();a++)
			RetList.append(ApplyInterpretSum(HorMark[a],true));
		return RetList;
	}
}

void Q_MG_MouseCursor::ClearAllMarkers( bool isVert )
{
	if (isVert)
	{
		sortedVertMark.clear();
		bool some_ch = !VertMark.isEmpty();
		VertMark.clear();
		if (some_ch)
			emit markersChanged(VertMark,isVert);
	}
	else
	{
		sortedHorMark.clear();
		bool some_ch = !HorMark.isEmpty();
		HorMark.clear();
		if (some_ch)
			emit markersChanged(HorMark,isVert);
	}
	ReDraw();
}

void Q_MG_MouseCursor::getCommand( QString ToObj,QString CommandName,QVariant params )
{
	if ((ToObj != objectName()) && (ToObj != "mg_ALL")) return;
	if (CommandName == "gridChanged")
	{
		Q_MG_BaseClass* bcl = static_cast<Q_MG_BaseClass*>(sender());
		CopyMatrix(bcl);
	}
}

double Q_MG_MouseCursor::Correct_ValueByStep_or_PixelByGrid( double val,double pixel,bool isVertical )
{
	if (EnableByStepSelection)
	{
		double retVal = 0;
		if (isVertical)
		{
			__int64 tt = qRound64((val-MinTransPoint.y())/stepValY);
			retVal = (tt * stepValY) - MinTransPoint.y();
		}
		else
		{
			__int64 tt = qRound64((val-MinTransPoint.x())/stepValX);
			retVal = (tt * stepValX) - MinTransPoint.x();
		}
		return retVal;
	}
	
	return val;
}

void Q_MG_MouseCursor::DrawStrangeCross(QPainter *painter)
{
	if (!showStrangeCross) return;
	double posX = GetPixelByValue(StrangeCrossValX,false);
	double posY = GetPixelByValue(StrangeCrossValY,true);

	QRectF VertLine = QRectF(QPointF(posX-2,-1),QPointF(posX+2,sz.y()+1));
	if (FigureIsVisible(VertLine))
	{
		QPointF startEndRect;
		if (showStrangeCrossVertText)
		{
			QFontMetrics fnt(painter->font());
			QString OutText = Interpret(StrangeCrossValX,false,false);
			double TextW_2 = fnt.width(OutText)/2;
			double TextYpos;
			if ((curGridAlig == 0) || (curGridAlig == 2)) TextYpos = sz.y()-7;
			else TextYpos = fnt.height()+3;
			QRectF LabelRect(QPointF(posX-TextW_2-4,TextYpos-fnt.height()),QPointF(posX+TextW_2+3,TextYpos+3));
			painter->setPen(settings->StrangeCross_LineColor);
			painter->setBrush(settings->StrangeCross_TextFillColor);//цвет фона
			painter->drawRect(LabelRect);
			painter->setPen(settings->StrangeCross_TextColor);//цвет текста
			painter->drawText(posX-TextW_2,TextYpos,OutText);
			startEndRect = QPointF(LabelRect.y(),LabelRect.bottom());
		}
		else startEndRect = QPointF(-1,-1);

		painter->setPen(settings->StrangeCross_LineColor);
		painter->drawLine(QLineF(posX,-1,posX,startEndRect.x()));
		painter->drawLine(QLineF(posX,startEndRect.y(),posX,sz.y()+1));
	}

	QRectF HorLine =  QRectF(QPointF(-1,posY-2),QPointF(sz.x()+1,posY+2));
	if (FigureIsVisible(HorLine))
	{
		QPointF startEndRect;
		if (showStrangeCrossHorText)
		{
			QFontMetrics fnt(painter->font());
			QString OutText = Interpret(StrangeCrossValY,true,false);
			double TextH_2= fnt.height()/2;
			double TextXpos;
			if ((curGridAlig == 0) || (curGridAlig == 1)) TextXpos = sz.x()-fnt.width(OutText)-23;
			else TextXpos = 20;
			QRectF LabelRect(QPointF(TextXpos-3,posY-TextH_2),QPointF(TextXpos+fnt.width(OutText)+2,posY+TextH_2));
			painter->setPen(settings->StrangeCross_LineColor);
			painter->setBrush(settings->StrangeCross_TextFillColor);//цвет фона
			painter->drawRect(LabelRect);
			painter->setPen(settings->StrangeCross_TextColor);//цвет текста
			painter->drawText(TextXpos,posY+TextH_2-1,OutText);
			startEndRect = QPointF(LabelRect.x(),LabelRect.right());
		}
		else startEndRect = QPointF(-1,-1);

		painter->setPen(settings->StrangeCross_LineColor);
		painter->drawLine(QLineF(-1,posY,startEndRect.x(),posY));
		painter->drawLine(QLineF(startEndRect.y(),posY,sz.x()+1,posY));

	}

}

void Q_MG_MouseCursor::SetMarkers( QList<double> incList,bool isVertical,bool noSignal/* = false*/)
{
	if (isVertical)
	{
		VertMark.clear();
		for (int a = 0;a < incList.count();a++) VertMark.append(incList[a]-HorInterpretSum);
		NeedSortVertMark = true;
		if (!noSignal) emit markersChanged(VertMark,isVertical);
	}
	else
	{
		HorMark.clear();
		for (int a = 0;a < incList.count();a++) HorMark.append(incList[a]-VertInterpretSum);
		NeedSortHorMark = true;
		if (!noSignal) emit markersChanged(HorMark,isVertical);
	}
	ReDraw();
}

void Q_MG_MouseCursor::UpdateMarker( int index, double val,bool isVertical )
{
	if (isVertical)
	{
		val -= HorInterpretSum;
		if (index >= VertMark.count())  VertMark.append(val);
		VertMark.replace(index,val);
		NeedSortVertMark = true;
		emit markersChanged(VertMark,isVertical);
	}
	else
	{
		val -= VertInterpretSum;
		if (index >= HorMark.count())  HorMark.append(val);
		HorMark.replace(index,val);
		NeedSortHorMark = true;
		emit markersChanged(HorMark,isVertical);
	}
	ReDraw();
}


void Q_MG_MouseCursor::SetSelection( double x1,double y1,double x2,double y2, bool noSignal /*= false*/)
{
	if (!multi_select_enable_)
	{
		VAxisSelList.clear();
		HAxisSelList.clear();
		RectSelList.clear();
	}

	bool SomeF = false;
	if (((x1 == 0) && (x2 == 0)) || (CurSelectionType == 2))
	{
		y1 -= VertInterpretSum;
		y2 -= VertInterpretSum;
		VAxisSelList.append(QPointF(y1,y2));
		SomeF = true;
	}

	if (((y1 == 0) && (y2 == 0) && (!SomeF)) || (CurSelectionType == 1))
	{
		x1 -= HorInterpretSum;
		x2 -= HorInterpretSum;
		HAxisSelList.append(QPointF(x1,x2));
		SomeF = true;
	}

	if ((!SomeF) || (CurSelectionType == 3))
	{
		x1 -= HorInterpretSum;
		x2 -= VertInterpretSum;
		y1 -= HorInterpretSum;
		y2 -= VertInterpretSum;
		RectSelList.append(QRectF(QPointF(x1,y1),QPointF(x2,y2)));
	}
	if (!noSignal) emit selectedPixelChange(x1,y1,x2,y2);
	ReDraw();
}

void Q_MG_MouseCursor::RemoveLastSelection()
{
	if (CurSelectionType == 2)
		if (HasVertSelection())	VAxisSelList.removeLast();
	if ((CurSelectionType == 1))
		if (HasHorSelection())	HAxisSelList.removeLast();
	if (CurSelectionType == 3)
		if (HasRectSelection()) RectSelList.removeLast();
}

bool Q_MG_MouseCursor::HasSelection() const
{
	if (!VAxisSelList.isEmpty())	return true;
	if (!HAxisSelList.isEmpty())	return true;
	if (!RectSelList.isEmpty())		return true;
	return false;
}

bool Q_MG_MouseCursor::HasRectSelection() const
{
	return !RectSelList.isEmpty();
}

bool Q_MG_MouseCursor::HasHorSelection() const
{
	return !HAxisSelList.isEmpty();
}

bool Q_MG_MouseCursor::HasVertSelection() const
{
	return !VAxisSelList.isEmpty();
}

void Q_MG_MouseCursor::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	emit DoubleClicked(GetValueByPixel(event->pos().x(),false),GetValueByPixel(event->pos().y(),true));
	event->accept();
}

//подписи
void Q_MG_MouseCursor::SetFredomLabel( double valX_b,QString text,QString name,bool immediately_update /*= false*/, QColor label_color )
{
	if (!labels_manager_) return;
    labels_manager_->addLabel(valX_b,text,name,immediately_update,label_color);
	ReDraw();
}

void Q_MG_MouseCursor::ClearAllFredomLabels()
{
	if (!labels_manager_) return;
	labels_manager_->clearLabels();
	ReDraw();
}

void Q_MG_MouseCursor::DrawFredomLabels(QPainter* painter)
{
	if (!labels_manager_) return;
	labels_manager_->redraw();
}

void Q_MG_MouseCursor::ChangeLabelPos( QString name,double new_pos, bool immediately_update /*= false*/ )
{
	if (!labels_manager_) return;
	labels_manager_->changeLabelPosition(name,new_pos,immediately_update);
}

void Q_MG_MouseCursor::ChangeLabelText( QString name,QString new_text, bool immediately_update /*= false*/ )
{
	if (!labels_manager_) return;
	labels_manager_->changeLabelText(name,new_text,immediately_update);
}

void Q_MG_MouseCursor::SetLabelsUnionText( QString union_text )
{
	if (!labels_manager_) return;
	labels_manager_->setUnionsText(union_text);
}

void Q_MG_MouseCursor::SetLabelsUnionShowHint( bool show_hint )
{
	if (!labels_manager_) return;
	labels_manager_->setUnionsShowHints(show_hint);
}

void Q_MG_MouseCursor::RemoveLabel( QString name, bool immediately_update /*= false*/ )
{
	if (!labels_manager_) return;
	labels_manager_->removeLabel(name,immediately_update);
}

void Q_MG_MouseCursor::SetMultySelectEnabled( bool enable )
{
	multi_select_enable_ = enable;
}

bool Q_MG_MouseCursor::IsMultySelectEnabled() const
{
	return multi_select_enable_;
}

bool Q_MG_MouseCursor::IsButtonCtrlClickIsSingle() const
{
	return button_ctrl_click_is_single_;
}

void Q_MG_MouseCursor::SetButtonCtrlClickIsSingle(bool is_single)
{
	button_ctrl_click_is_single_ = is_single;
}

void Q_MG_MouseCursor::StartSelectionDeselection(QGraphicsSceneMouseEvent * event)
{
	//старт выделения
	if (!OnMarkerMoove)
	{
		if (event->buttons() == Qt::LeftButton) is_deselect_ = false;
		else is_deselect_ = true;

		if (!is_deselect_)
		{
			if ((!multi_select_enable_) || (CurSelectionType == 0)) ClearAllSelections();
			else
				if (button_ctrl_click_is_single_)
				{
					if (event->modifiers() & Qt::ControlModifier)
						ClearAllSelections();
				}
				else
				{
					if (!(event->modifiers() & Qt::ControlModifier))
						ClearAllSelections();
				}
		}
			

		if (CurSelectionType == 1)
		{
			double valX = GetValueByPixel(event->pos().x(),false);
			valX = Correct_ValueByStep_or_PixelByGrid(valX,event->pos().x(),false);
			CurMoveSel.setX(valX);
		}
		if (CurSelectionType == 2)
		{
			double valY = GetValueByPixel(event->pos().y(),true);
			valY = Correct_ValueByStep_or_PixelByGrid(valY,event->pos().y(),true);
			CurMoveSel.setX(valY);
		}
		if (CurSelectionType == 3)
		{
			double valX = GetValueByPixel(event->pos().x(),false);
			valX = Correct_ValueByStep_or_PixelByGrid(valX,event->pos().x(),false);
			double valY = GetValueByPixel(event->pos().y(),true);
			valY = Correct_ValueByStep_or_PixelByGrid(valY,event->pos().y(),true);
			CurMoveSel.setTopLeft(QPointF(valX,valY));
		}

		selection_deselection_started_ = true;
	}	
}

bool Q_MG_MouseCursor::CalcAddSelectionPoint( QList<QPointF> & list, QPointF selectRectPoint )
{

	bool retVal = false;
    QRectF SelectRect(QPointF(selectRectPoint.x(),0),QPointF(selectRectPoint.y(),1));

    bool anyIntersect = false;
	bool wasFullAddIndex = false;//было ли полное добавление

    for (int z = 0;z < list.count();z++)//1
	{

		if (z < 0 ) z = 0;
		QPointF sel = list[z];
        QRectF selFigure(QPointF(sel.x(),0),QPointF(sel.y(),1));
		if (selFigure.intersects(SelectRect))
		{
            anyIntersect = true;
            QRectF inter = selFigure.intersected(SelectRect);
            if (inter == selFigure)
            {
                // +[   ]+
                if (!wasFullAddIndex)
                {
                    list.replace(z,QPointF(SelectRect.left(),SelectRect.right()));wasFullAddIndex = true;
                    retVal = true;
                }
                else
                {
                    if (SelectRect != inter)
                    {
                        list.removeAt(z);
                        retVal = true;
                        z--;
                    }
                }
                continue;
            }
            if (inter.right() == selFigure.right())
            {
                // [   ]+
                z -= list.removeAll(sel);
                z -= list.removeAll(QPointF(SelectRect.left(),SelectRect.right()));
                SelectRect.setLeft(sel.x());
                wasFullAddIndex = true;
                list.append(QPointF(SelectRect.left(),SelectRect.right()));
                retVal = true;
                continue;
            }
            if (inter.left() == selFigure.left())
            {
                // +[   ]
                z -= list.removeAll(sel);
                z -= list.removeAll(QPointF(SelectRect.left(),SelectRect.right()));
                SelectRect.setRight(sel.y());
                wasFullAddIndex = true;
                list.append(QPointF(SelectRect.left(),SelectRect.right()));
                retVal = true;
                continue;
            }

			//[ - ] continue;
		}
	}//for
	if (!anyIntersect)
	{
		list.append(selectRectPoint);
		retVal = true;
	}

	return retVal;
}

bool Q_MG_MouseCursor::CalcAddSelectRect( QList<QRectF> & list, QRectF SelectRect )
{

	bool retVal = false;//someChange
	bool wasFullAddIndex = false;
	bool intersect = false;
	for (int z = 0;z < list.count();z++)//1
	{
		QRectF sel = list[z];
		if (sel.intersects(SelectRect))
		{
			QRectF inter = sel.intersected(SelectRect);
			if (inter == sel)
			{
				intersect = true;
				// +[   ]+
				if (!wasFullAddIndex)
				{
					list.replace(z,SelectRect);wasFullAddIndex = true;
					retVal = true;
				}
				else
				{
					if (SelectRect != inter)
					{
						list.removeAt(z);
						retVal = true;
						z--;
					}
				}
				continue;
			}

			if (inter == SelectRect)
			{
				intersect = true;
				// [ + ]
				continue;
            }
		}	
	}
	if (!intersect)
	{
		list.append(SelectRect);
		retVal = true;
	}
	return retVal;
}

bool Q_MG_MouseCursor::CalcDeselectRect( QList<QRectF> & list, QRectF unSelectRect )
{
	bool retVal = false;//someChange
	for (int z = 0;z < list.count();z++)//1
	{
		QRectF sel = list[z];
		if (sel.intersects(unSelectRect))
		{
			QRectF inter = sel.intersected(unSelectRect);
			if (inter == sel)
			{
				// -[---]-
				list.removeAt(z);
				retVal = true;
				z--;
				continue;
			}
		}	
	}
	return retVal;
}

bool Q_MG_MouseCursor::CalcDeselectPoint( QList<QPointF> & list, QPointF unSelectRectPoint, bool multySelect )
{
	bool retVal = false;//someChange
	QRectF unSelectRect(QPointF(unSelectRectPoint.x(),0),QPointF(unSelectRectPoint.y(),1));
	for (int z = 0;z < list.count();z++)//1
	{
		QPointF sel = list[z];
		QRectF selFigure(QPointF(sel.x(),0),QPointF(sel.y(),1));
		if (selFigure.intersects(unSelectRect))
		{
			QRectF inter = selFigure.intersected(unSelectRect);
			if (inter == selFigure)
			{
				// -[---]-
				list.removeAt(z);
				retVal = true;
				z--;
				continue;
			}
			if (inter.right() == selFigure.right())
			{
				// [  -]
				sel.setY(inter.left());
				list.replace(z,sel);
				retVal = true;
				continue;
			}
			if (inter.left() == selFigure.left())
			{
				// [-  ]
				sel.setX(inter.right());
				list.replace(z,sel);
				retVal = true;
				continue;
			}

			// [ - ]
			if (multySelect)
			{
				QPoint selNew(inter.right(),sel.y());
				sel.setY(inter.left());
				list.replace(z,sel);
				list.insert(z,selNew);
				retVal = true;
				z++;
			}
		}	
	}
	return retVal;
}

void Q_MG_MouseCursor::RemoveMarker( int index, bool isVert, bool need_change_signal /*= true*/ )
{
	if (index < 0) return;
	if (isVert)
	{
		if (index >= VertMark.size()) return;
		sortedVertMark.clear();NeedSortVertMark = true;
		VertMark.removeAt(index);
		if (need_change_signal)
			emit markersChanged(VertMark,isVert);
	}
	else
	{
		if (index >= HorMark.size()) return;
		sortedHorMark.clear();NeedSortHorMark = true;
		HorMark.removeAt(index);
		if (need_change_signal)
			emit markersChanged(HorMark,isVert);
	}
	if (UnderMouseMarkerIndex == index)
		UnderMouseMarkerIndex = -1;
	ReDraw();
}




//конец подписи

//////////////////////////////////////////////////////////////////////////
//		Q_MG_Material													//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

void debugOut( QString a )
{

#ifdef SHOW_DEBUG_INFO
	qDebug() << a;
#endif // SHOW_DEBUG_INFO

}

Q_MG_RenderThreadClass::Q_MG_RenderThreadClass(QObject *parent): QThread(parent),
	thread_status_(0),
	isPaused_(false), useFilter_(0)
{

}

Q_MG_RenderThreadClass::~Q_MG_RenderThreadClass()
{
	FinishHim_wait();
}

void Q_MG_RenderThreadClass::run()
{
#ifdef OMEGA
    CrThreadAutoInstallHelper crash_rpt_helper;
#endif // OMEGA
	forever
	{
		debugOut("try to start render");
		{
			QMutexLocker locker(&render_mutex_);
			debugOut("start render");
			RenderData();
			debugOut("finish render");
		}
		
        bool not_need_pause = false;
        {
            QReadLocker locker(&thread_status_lock_);
            if (thread_status_ == 2) return;
			if (thread_status_ == 1) SwitchToThread();
            not_need_pause = (thread_status_ == 0);
        }
		
		if (not_need_pause)
		{
			int el_in_Q = ElementsInQueue();
			debugOut("ElementsInQueue "+QString::number(el_in_Q));
			if (el_in_Q > 0) continue;

			QMutex mtx;
			QMutexLocker mlock(&mtx);
			debugOut("Thread_finishWork");
			emit RenderFinished();

			thread_sleep_status_ = true;
			Thread_finishWork.wait(&mtx);
            {
                QReadLocker locker(&thread_status_lock_);
                if (thread_status_ == 2) return;
            }

			thread_sleep_status_ = false;
			debugOut("Thread_startWork");
		}
	}
}


void Q_MG_RenderThreadClass::Start_Wake()
{
    {
        QWriteLocker locker(&thread_status_lock_);
        if (thread_status_ != 2)
            thread_status_ = 0;
    }
    if (!isRunning()) {	start();return;	}// QThread::TimeCriticalPriority
	Thread_finishWork.wakeAll();
}



void Q_MG_RenderThreadClass::FinishHim_wait()
{
	if (!isRunning()) return;

    {
        QWriteLocker locker(&thread_status_lock_);
        thread_status_ = 2;
    }

	Start_Wake();
	wait();
}

bool Q_MG_RenderThreadClass::NeedBreaked()
{
    QReadLocker locker(&thread_status_lock_);
	return (thread_status_ == 1) || (thread_status_ == 2);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Q_MG_Material::Q_MG_Material( QGraphicsItem *parent, int _ElementH, int _ElementW, QImage::Format ElementsImageFormat, bool use_multy_thread_render, QPointer<Q_MG_RenderThreadClass> in_renderThread) : Q_MG_BaseClass(parent)
{
	setAcceptHoverEvents(false);
	setAcceptedMouseButtons(Qt::NoButton);
	//setAcceptedMouseButtons(Qt::MidButton | Qt::RightButton | Qt::LeftButton);
	setFlag(QGraphicsItem::ItemIsFocusable,false);
	setFlag(QGraphicsItem::ItemIsMovable,false);
	setFlag(QGraphicsItem::ItemIsSelectable,false);
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);

	setObjectName("mg_Material");

	FirstPaint = true;

	MaterialSize = QSizeF(300,300);

	ElementH = _ElementH;
	ElementW = _ElementW;
	ElementsImageFormat_ = ElementsImageFormat;
	NeedMaterialUpdateAfterMoov = false;

	Hor_kf_inc = 1;
	Vert_kf_inc = 1;

	//для многопоточности
	UseMultiThread = use_multy_thread_render;
	if (in_renderThread == NULL) UseMultiThread = false;
	if (UseMultiThread)
	{
		RenderThread = in_renderThread;
		connect(RenderThread,SIGNAL(RenderFinished()),this,SLOT(RenderFinishedSlot()));
		connect(RenderThread,SIGNAL(ElementReady(Q_MG_MaterialElement*,bool)),this,SLOT(ElementReady(Q_MG_MaterialElement*,bool)));//пока оставляю все параметры, потом после теста посмотрим пригодятся они или нет, ибо хз как там будут евенты приходить

		RenderThread->setFilter([&](Q_MG_MaterialElement* el)->bool
		{
			return ElementIsOnScreen(el);
		});

	}
	
	on_Scale_or_MinMaxChange();//считаем размер материала
}

void Q_MG_Material::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	//MG_Print("Paint Material");
	if (FirstPaint) {on_Move();FirstPaint = false;}
	//painter->fillRect(0,0,MaterialSize.width(),MaterialSize.height(),QColor(0,0,150));
	//qDebug() << MaterialSize.height();
}

void Q_MG_Material::AllMaterialReInit()
{
	ClearEl();
	ExtMaterialReInit();
}

void Q_MG_Material::on_Move()
{
	//MG_Print("on_Move");
	double MaxXVal_pix = GetPixelByValueForTrans(Trans,kf,MaxTransPoint.x(),false);
	double MinXVal_pix = GetPixelByValueForTrans(Trans,kf,MinTransPoint.x(),false);
	double MaxYVal_pix = GetPixelByValueForTrans(Trans,kf,MaxTransPoint.y(),true);
	double MinYVal_pix = GetPixelByValueForTrans(Trans,kf,MinTransPoint.y(),true);
	if (InverseHAxis)
	{	double z = MaxXVal_pix;MaxXVal_pix = MinXVal_pix;MinXVal_pix = z; }
	if (InverseVAxis)
	{	double z = MaxYVal_pix;MaxYVal_pix = MinYVal_pix;MinYVal_pix = z; }
	
	setPos(MinXVal_pix,MinYVal_pix);
	if (NeedMaterialUpdateAfterMoov) {AllMaterialReInit();}
	NeedMaterialUpdateAfterMoov = false;
    ViewPortChange();
}

void Q_MG_Material::on_Resize()
{
	//MG_Print("on_Resize");
	if (!NeedMaterialUpdateAfterMoov) on_Move();return;
	if (MaterialSize.height() < sz.y()-(GridTopStep+GridBottomStep)) MaterialSize.setHeight(sz.y()-(GridTopStep+GridBottomStep));
	if (MaterialSize.width() < sz.x()-(GridLeftStep+GridRightStep)) MaterialSize.setWidth(sz.x()-(GridLeftStep+GridRightStep));
    if (!NeedMaterialUpdateAfterMoov) on_Move();
}

void Q_MG_Material::on_Scale_or_MinMaxChange()
{
	//вычисляет размер MaterialSize исходя из BaseClass
//    qDebug() << "on_Scale_or_MinMaxChange";
	double MaxXVal_pix = GetPixelByValueForTrans(Trans,kf,MaxTransPoint.x(),false);
	double MinXVal_pix = GetPixelByValueForTrans(Trans,kf,MinTransPoint.x(),false);
	double MaxYVal_pix = GetPixelByValueForTrans(Trans,kf,MaxTransPoint.y(),true);
	double MinYVal_pix = GetPixelByValueForTrans(Trans,kf,MinTransPoint.y(),true);
	if (InverseHAxis)
	{	double z = MaxXVal_pix;MaxXVal_pix = MinXVal_pix;MinXVal_pix = z; }
	if (InverseVAxis)
	{	double z = MaxYVal_pix;MaxYVal_pix = MinYVal_pix;MinYVal_pix = z; }

	MaterialSize.setWidth(MaxXVal_pix-MinXVal_pix);
	MaterialSize.setHeight(MaxYVal_pix-MinYVal_pix);
    
	NeedMaterialUpdateAfterMoov = true;
	on_Resize();
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);//обновляем Кэш
}

QPointF Q_MG_Material::GetPixelByValueOnMaterial( double Val1,double Val2 )
{
	QPointF retVal;
	retVal.setX(GetPixelByValue(Val1,false)-pos().x());
	retVal.setY(GetPixelByValue(Val2,true)-pos().y());
	return retVal;
}

QPointF Q_MG_Material::GetValueByPixelOnMaterial( double PixelX,double PixelY )
{
	QPointF retVal;
	retVal.setX(GetValueByPixel(PixelX+pos().x(),false));
	retVal.setY(GetValueByPixel(PixelY+pos().y(),true));
	return retVal;
}

void Q_MG_Material::ViewPortChange()
{
	double StartPixelX = pos().x()*-1+GridLeftStep; 
	double EndPixelX = pos().x()*-1+(sz.x()-1)-GridRightStep;
	double StartPixelY = pos().y()*-1+GridTopStep;
	double EndPixelY = pos().y()*-1+(sz.y()-1)-GridBottomStep;//*/

	/*double StartPixelX = pos().x()*-1;
	double EndPixelX = pos().x()*-1+(sz.x()-1);
	double StartPixelY = pos().y()*-1;
	double EndPixelY = pos().y()*-1+(sz.y()-1);//*/
	
	double a,b,c,d;
	GetViewPort(a,b,c,d);
	emit ViewPortChanged(a,b,c,d);

	qreal key_kf_x = kf.x()*Hor_kf_inc;
	qreal key_kf_y = kf.y()*Vert_kf_inc;
// 	unsigned __int64 round_key_kf = qRound64(key_kf_x);

	if (ElList.AvailRead() > 3000)
	{
		if (UseMultiThread)
		{
			RenderThread->ClearQueue();
			RemoveAllOffScreenEl();
			SelfClear();
			RenderThread->addData(ElList);
			sendCommand("mg_InfoWidget","SetProgress",1);
		}
		else RemoveAllOffScreenEl();
	}
	

	//необходимо выяснить какие элементы необходимо рисовать
	unsigned __int64 byXStart = StartPixelX/ElementW;
	unsigned __int64 byXEnd = (EndPixelX/ElementW)+1;
	unsigned __int64 byYStart = (unsigned __int64) (StartPixelY / (double)ElementH);
	unsigned __int64 byYEnd = (EndPixelY/ElementH)+1;

	unsigned __int64  InLineEl = MaterialSize.width()/ElementW;
	InLineEl++;

	if (!PrepareForViewPortChange(key_kf_x, key_kf_y,StartPixelX,EndPixelX,StartPixelY,EndPixelY)) return;

	if (UseMultiThread)
	{
		Storage<Q_MG_MaterialElement*> newElements;
		auto searchOnlyTo = ElList.AvailRead();
		for (unsigned __int64 i = byYStart;i < byYEnd;i++)
		{
			for (unsigned __int64 h = byXStart;h < byXEnd;h++)
			{
				unsigned __int64 ItemIndex = i*InLineEl+h;
				bool Found = false;
				for (size_t g = 0;g < searchOnlyTo;g++)
					if (ElList[g]->ElementIndex_64 == ItemIndex)
					{ Found = true;break; }
					if (Found) continue;

					//надо создавать и размещать
					Q_MG_MaterialElement* NewEl = new Q_MG_MaterialElement(ElementW,ElementH,ElementsImageFormat_,this);
					NewEl->setVisible(false);
					NewEl->setZValue(1);
					NewEl->setPos(h*ElementW,i*ElementH);
					NewEl->ElementIndex_64 = ItemIndex;
					NewEl->elX = h;
					NewEl->elY = i;

					ElList.PrepareForWrite(1);
					ElList[ElList.AvailRead()] = NewEl;
					ElList.WriteComplete(1);
					//добавление в очередь на рендер
					newElements.PrepareForWrite(1);
					newElements[newElements.AvailRead()] = NewEl;
					newElements.WriteComplete(1);
			}
		}
		int CurProgressPerc = RenderThread->GetCurrentProgressPercent();
		if ((CurProgressPerc == 0) || (CurProgressPerc == 100)) CurProgressPerc = 1;
		if (newElements.AvailRead() > 0) sendCommand("mg_InfoWidget","SetProgress",CurProgressPerc);
		RenderThread->addData(newElements);
	}
	else
	{
		Q_MG_MaterialElement** Elmass = ElList.PrepareForWrite(150);
		unsigned __int64 appendedEl = 0;
		for (unsigned __int64 i = byYStart;i < byYEnd;i++)
		{
			for (unsigned __int64 h = byXStart;h < byXEnd;h++)
			{
				unsigned __int64 ItemIndex = i*InLineEl+h;
				bool Found = false;
				for (int g=0;g < (int)ElList.AvailRead();g++)
					if (ElList[g]->ElementIndex_64 == ItemIndex)
					{ Found = true;break; }
				if (Found) continue;
			
				//надо создавать и размещать
				Q_MG_MaterialElement* NewEl = new Q_MG_MaterialElement(ElementW,ElementH,ElementsImageFormat_,this);
				NewEl->setZValue(1);
				NewEl->setPos(h*ElementW,i*ElementH);
				NewEl->ElementIndex_64 = ItemIndex;
				NewEl->elX = h;
				NewEl->elY = i;

				if (ElList.AvailWrite() <= 0) {Elmass = ElList.PrepareForWrite(150);appendedEl = 0;}
                if(Elmass)
                {
                    Elmass[appendedEl] = NewEl;
                    ElList.WriteComplete(1);
                }
				appendedEl++;
				bool NeedHide = ExtViewPortChange(NewEl,h,i);
						
				if (NeedHide) NewEl->setVisible(false);
			}
		}
		
		AfterViewPortChange();
	}//if (UseMultyThread)
	

	return;
}

void Q_MG_Material::ClearEl()
{
	if (UseMultiThread) RenderThread->ClearQueue();
	
	for (size_t i = 0;i < ElList.AvailRead();i++) 
	{
		scene()->removeItem(ElList[i]);
		delete ElList[i];
	}
	ElList.Clear();
}


void Q_MG_Material::getCommand( QString ToObj,QString CommandName,QVariant params )
{
	if ((ToObj != objectName()) && (ToObj != "mg_ALL")) return;
	if (CommandName == "gridChanged")
	{
		Q_MG_BaseClass* bcl = static_cast<Q_MG_BaseClass*>(sender());
		CopyMatrix(bcl);
	}
}

void Q_MG_Material::RenderFinishedSlot()
{
	sendCommand("mg_InfoWidget","SetProgress",100);
	AfterViewPortChange();
}

void Q_MG_Material::ElementReady(Q_MG_MaterialElement* el,  bool ElementIsEmpty)
{
	//проверяем че это за херня и есть ли она в очереди (защита от того что если сигнал не успел дойти когда пользователь агрессивно тащит и очердь чиститься)
	bool founded = false;
	for (int i = 0;i < (int)ElList.AvailRead();i++)
		if (el == ElList[i]) {founded = true;break;}
	
	sendCommand("mg_InfoWidget","SetProgress",RenderThread->GetCurrentProgressPercent());
	if (!founded) return;
	if ((!ElementIsEmpty) && (el->elementIsReady_))
		el->setVisible(true);
}

bool Q_MG_Material::ElementIsOnScreen(const Q_MG_MaterialElement* el )
{
	QRectF screenRect;
	QPointF matP = GetValueByPixelOnMaterial(el->pos().x(),el->pos().y());
	double ScreenX = GetPixelByValue(matP.x(),false);
	double ScreenY = GetPixelByValue(matP.y(),true);
	
	QRectF elRect(ScreenX ,ScreenY ,ElementW,ElementH);
	screenRect = QRectF(QPointF(0,0),QPointF(sz));
	return elRect.intersects(screenRect);
}

void Q_MG_Material::RemoveAllOffScreenEl()
{
	//соритруем чтобы offscrin-ы были в конце массива
	//if (UseMultiThread)	if (!RenderThread->isSleep()) return;//!!!! тоже под вопросом надо ли оно тут

	std::sort(&ElList.data()[0],&ElList.data()[ElList.AvailRead()],

		[&](const Q_MG_MaterialElement* a, const Q_MG_MaterialElement* b) -> bool
		{
			bool A = ElementIsOnScreen(a);
			bool B = ElementIsOnScreen(b);
			double cya = a->elY;
			double cyb = b->elY;
			if (A && B) return (cya < cyb);
			if (A) cya *= -1;
			if (B) cyb *= -1;
			return (cya < cyb);
		}
	);

	size_t LastCount = ElList.AvailRead();
	for (size_t i = 0;i < ElList.AvailRead();i++)
	{
		if (ElementIsOnScreen(ElList.data()[i])) LastCount--;
		else
		{
			scene()->removeItem(ElList[i]);
			delete ElList[i];
		}
	}
	ElList.RemoveLast(LastCount);
}

void Q_MG_Material::AfterViewPortChange()
{
	ExtAfterViewPortChange();
}

void Q_MG_Material::GetViewPort( double& value_startX, double& value_startY,double& value_endX, double& value_endY )
{
	double StartPixelX = pos().x()*-1; 
	double EndPixelX = pos().x()*-1+(sz.x()-1);
	double StartPixelY = pos().y()*-1;
	double EndPixelY = pos().y()*-1+(sz.y()-1);
	QPointF startP = GetValueByPixelOnMaterial(StartPixelX+GridLeftStep,StartPixelY+GridTopStep);
	QPointF endP = GetValueByPixelOnMaterial(EndPixelX-GridRightStep,EndPixelY-GridBottomStep);
	value_startX = startP.x();
	value_startY = startP.y();
	value_endX = endP.x();
	value_endY  = endP.y();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
Q_MG_MaterialElement::Q_MG_MaterialElement( int _szX,int _szY, QImage::Format ElementFormat, QGraphicsItem *parent /*= NULL*/ ) : QGraphicsObject(parent)
{
	setAcceptHoverEvents(false);
	setAcceptedMouseButtons(Qt::NoButton);
	setFlag(QGraphicsItem::ItemIsFocusable,false);
	setFlag(QGraphicsItem::ItemIsMovable,false);
	setFlag(QGraphicsItem::ItemIsSelectable,false);
	szX = _szX;szY = _szY;
	elementIsReady_ = false;
	img = new QImage(szX,szY,ElementFormat);
	img->fill(0); //не убирать - надо для спектра
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

void Q_MG_MaterialElement::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	
	//ImgMtx.lock();
	//qDebug() << ElementIndex_64;
	//QVector<uint> color;color << qRgb(0,0,0) << qRgb(255,20,20);
	//img->setColorTable(color);
	painter->drawImage(0,0,*img);
	//painter->setPen(QColor(255,0,0,50));painter->setBrush(Qt::NoBrush);painter->drawRect(0,0,szX-1,szY-1);
	//ImgMtx.unlock();
	//painter->drawText(1,10,QString::number(ElementIndex_64,'f',0));
}

//////////////////////////////////////////////////////////////////////////
// Utils
//////////////////////////////////////////////////////////////////////////

QString FormatMSecs( quint64 MSecs )
{
	QTime time(0, 0, 0);
	time = time.addMSecs(MSecs);

	if((time.hour() == 0) && (time.minute() == 0))
		return QString("%1.%2").arg(((int)time.second()), 2, 10, QChar('0')).arg(((int)time.msec()), 3, 10, QChar('0'));

	if(time.hour() == 0)
		return QString("%1:%2.%3").arg(time.minute(), 2, 10, QChar('0')).arg(time.second(), 2, 10, QChar('0')).arg(time.msec(), 3, 10, QChar('0'));
	else
		return QString("%1:%2:%3.%4").arg(time.hour(), 2, 10, QChar('0')).arg(time.minute(), 2, 10, QChar('0')).arg(time.second(), 2, 10, QChar('0')).arg(time.msec(), 3, 10, QChar('0'));
}

QString FormatSecs( quint64 secs )
{
	quint64 days = secs/86400;
	int temp = secs - (days*86400);
	QTime tmp(0,0,0);
	tmp = tmp.addSecs(temp);
	
	QString result;
	if (days > 0) result = QString::number(days)+QObject::tr("д.");
	result += tmp.toString("hh:mm:ss");
	return result;	
}

QString FormatNanSecs( double NanSecs, double begin, double End, QString &outformat, bool showFullTime )
{
	if (begin > End) { double t =0; t = begin;begin = End;End = t; }
	if (begin < 0) begin = 0;
	if ((NanSecs < 0) || (End < 0)) { outformat = "";return "<0"; }

	QString retVal;

	MG_NanoTimeStruct val(NanSecs);
	MG_NanoTimeStruct beg_end(End - begin);
	MG_NanoTimeStruct beg(begin);
	MG_NanoTimeStruct end(End);

	if (showFullTime)
	{
		retVal = QString::number(val.h)+":"+QString::number(val.m)+":"+QString::number(val.s)+"."+QString::number(val.ms)+"."+QString::number(val.mks)+"."+QString::number(val.ns);
		return retVal;
	}
	
	if (beg_end.h >= 1)
	{
		outformat = QObject::tr("ч:м");
		retVal = QString::number(val.h)+":"+QString::number(val.m);
	}
	else
		if (beg_end.m >= 1)
		{
			outformat = QObject::tr("м:c");
			retVal = MG_NanoTimeStruct::toStr(val.m,2)+":"+MG_NanoTimeStruct::toStr(val.s,2);
		}
		else
			if (beg_end.s >= 1)
			{
				outformat = QObject::tr("c.мс");
				retVal = MG_NanoTimeStruct::toStr(val.s,2)+"."+MG_NanoTimeStruct::toStr(val.ms,3);
			}
			else
				if (beg_end.ms >= 1)
				{
					outformat = QObject::tr("мс.мкс");
					retVal = MG_NanoTimeStruct::toStr(val.ms,3)+"."+MG_NanoTimeStruct::toStr(val.mks,3);
				}
				else
					if (beg_end.mks >= 1)
					{
						outformat = QObject::tr("мкс.нс");
						retVal = MG_NanoTimeStruct::toStr(val.mks,3)+"."+MG_NanoTimeStruct::toStr(val.ns,3);
					}
					else
					{
						outformat = QObject::tr("нс");
						retVal = MG_NanoTimeStruct::toStr(val.ns,3);
					}
		
	return retVal;
}


QVector<double> MarkInterval( double aStart, double aEnd, double WidthPix, double MinSpacePix, QVector<double>* Subdivision, double SubdivRate, double* OutStep )
{
	QVector<double> res;
	if(WidthPix == 0)return res;
	Q_ASSERT(WidthPix > 0);
	Q_ASSERT(aStart >= 0);
	Q_ASSERT(aEnd > 0);
	if(WidthPix < MinSpacePix)return res;
	double Start = qMin(aStart, aEnd);
	double End = qMax(aStart, aEnd);
	double Length = End-Start;
	double Scale = Length/WidthPix;
	double FirstSign;
	QString Text = QString("%1").arg(MinSpacePix*Scale, 50, 'f', 30, '0');

	bool Found = false;
	for(int i = 0; i < Text.size(); i++)
	{
		if(!Found)
		{
			if((Text[i] > '0') && (Text[i] <= '9'))
			{
				Found = true;
				Text[i] = '1';
			}
		}
		else
			if((Text[i] >= '0') && (Text[i] <= '9'))
				Text[i] = '0';
	}

	FirstSign = Text.toDouble()*10.0;

	double MinSpaceS = FirstSign;
	while(MinSpacePix*Scale < MinSpaceS*0.5)MinSpaceS *= 0.5;
	Q_ASSERT(MinSpaceS > 0.0);

	QString Start1 = QString("%1").arg(Start, 50, 'f', 30, '0');
	QString Start2 = QString("%1").arg(Start+MinSpaceS, 50, 'f', 30, '0');

	Found = false;
	for(int i = 0; i < Start1.size(); i++)
	{
		if(!Found)
		{
			if((Start1[i] != Start2[i]) && (Start2[i] != '0'))
			{
				Found = true;
				Start1[i] = '0';
			}
		}
		else
			if((Start1[i] >= '0') && (Start1[i] <= '9'))
				Start1[i] = '0';
	}

	double Current = Start1.toDouble();
	bool first_adequate_number = false;
	double SecondStart = 0;
	
	if(Current >= Start)
	{
		if (!first_adequate_number)
		{
			first_adequate_number = true;
			SecondStart = Current;
		}
		res.append(Current);
	}

    {
        bool res_size_less_10000 = res.size() < 10000;  //  cache
        while((Current <= End) && res_size_less_10000)
        {
            Current += MinSpaceS;
            if(Current >= Start)
            {
                if (!first_adequate_number)
                {
                    first_adequate_number = true;
                    SecondStart = Current - MinSpaceS;
                }
                res.append(Current);
                res_size_less_10000 = res.size() < 10000;
            }
        }
        Q_ASSERT(first_adequate_number);
    }

	if(Subdivision)
	{
		double Current = SecondStart;//Start1.toDouble();
		int DivRate = 1.0/SubdivRate;
		int Num = 0;

		while((Current <= End) && (Subdivision->size() < 10000))
		{
			if(Num != 0)
				if(Current >= Start)
					Subdivision->append(Current);
			Current += MinSpaceS*SubdivRate;
			Num = ++Num % DivRate;
		}
	}

	if(OutStep) *OutStep = MinSpaceS;
	return res;
}


//////////////////////////////////////////////////////////////////////////
//		Q_MG_InfoWidget
//////////////////////////////////////////////////////////////////////////
Q_MG_InfoWidget::Q_MG_InfoWidget( Q_MG_Style* sett, QGraphicsItem *parent /*= NULL*/ ) : Q_MG_BaseClass(parent)
{
	setAcceptHoverEvents(true);
	//setAcceptedMouseButtons(Qt::LeftButton);
	setAcceptedMouseButtons(Qt::MidButton | Qt::RightButton | Qt::LeftButton);
	setFlag(QGraphicsItem::ItemIsFocusable,false);
	setFlag(QGraphicsItem::ItemIsMovable,true);
	setFlag(QGraphicsItem::ItemIsSelectable,false);
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	szX = 100;szY = 100;
	setPos(20000,-1000);
	setObjectName("mg_InfoWidget");
	setCursor(QCursor(Qt::OpenHandCursor));
	
	settings = sett;

	TextItem = new QGraphicsTextItem(this);
	TextItem->setAcceptHoverEvents(false);
	TextItem->setAcceptedMouseButtons(Qt::NoButton);
	TextItem->setFlag(QGraphicsItem::ItemIsFocusable,false);
	TextItem->setFlag(QGraphicsItem::ItemIsMovable,false);
	TextItem->setFlag(QGraphicsItem::ItemIsSelectable,false);
	TextItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	TextItem->setPlainText(tr("Нет данных"));
	TextItem->setPos(7,7);
	TextItem->setDefaultTextColor(settings->InfoWidget_Text_Color);//
	QFont fnt = TextItem->font();
	//fnt.setFamily("Century Gothic");//Century Gothic  Impact
	fnt.setPointSize(10);
	TextItem->setFont(fnt);
	PrintTextMode = 0;

	conLT = QPixmap("://Q_MultiGraphics/Resources/conLT.png");
	conRT = QPixmap("://Q_MultiGraphics/Resources/conRT.png");
	conLB = QPixmap("://Q_MultiGraphics/Resources/conLB.png");
	conRB =QPixmap("://Q_MultiGraphics/Resources/conRB.png");

	DockedTo.X1Col = false;
	DockedTo.X2Col = false;
	DockedTo.Y1Col = false;
	DockedTo.Y2Col = false;

	ShowList.append(&UserData);

	ProgressBarValue = 0;
	ShowProgressBar = true;

}

void Q_MG_InfoWidget::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
 	painter->setRenderHint(QPainter::Antialiasing,false);
	painter->setRenderHint(QPainter::TextAntialiasing,false);

	QLinearGradient BackGrad(0.0, 0.0, 0.0, szY);
	BackGrad.setColorAt(0.0, settings->InfoWidget_Back_BeginColor);
	BackGrad.setColorAt(0.3, settings->InfoWidget_Back_MidColor);
	BackGrad.setColorAt(0.7, settings->InfoWidget_Back_MidColor);
	BackGrad.setColorAt(1.0, settings->InfoWidget_Back_BeginColor);
	
	painter->setPen(settings->InfoWidget_Border_Color);
	painter->setBrush(BackGrad);
	painter->drawRect(2,2,szX-4,szY-4);

	painter->drawPixmap(0,0,conLT);
	painter->drawPixmap(szX-4,0,conRT);
	painter->drawPixmap(0,szY-4,conLB);
	painter->drawPixmap(szX-4,szY-4,conRB);

	if (NeedShowProgress())
	{
		double w100 = szX-12;
		double ProgPixelVal = qRound64((w100-1)*ProgressBarValue/100.0);
		painter->setPen(settings->InfoWidget_ProgressBar_FullFillColor);
		painter->setBrush(settings->InfoWidget_ProgressBar_EmptyFillColor);
		painter->drawRect(6,szY-13,w100,8);
		painter->setPen(settings->InfoWidget_ProgressBar_HightColor);
		painter->setBrush(settings->InfoWidget_ProgressBar_FullFillColor);
		painter->drawRect(7,szY-12,ProgPixelVal,6);
	}

}

void Q_MG_InfoWidget::getCommand( QString ToObj,QString CommandName,QVariant params )
{
	if ((ToObj != objectName()) && (ToObj != "mg_ALL")) return;
	if (CommandName == "gridChanged")
	{
		Q_MG_BaseClass* bcl = static_cast<Q_MG_BaseClass*>(sender());
		CopyMatrix(bcl);
		calcGeometry();
	}
	if (CommandName == "PrintText")
	{
		TextItem->setPlainText(params.toString());
		if (params.toString().isEmpty()) setPrintTextMode(0);
		else
		{
			if ((!isVisible()) && (isEnabled())) show();
			calcGeometry();
			update();
		}
	}

	if (CommandName == "AddText")
	{
		TextItem->setPlainText(TextItem->toPlainText()+'\n'+params.toString());
		if ((!isVisible()) && (isEnabled())) show();
		calcGeometry();
		update();
	}

	if (CommandName == "SetProgress")
	{
		SetProgress(params.toDouble());
	}

}

void Q_MG_InfoWidget::calcGeometry(bool onMove)
{
	szX = TextItem->boundingRect().width()+TextItem->pos().x()*2;
	szY = TextItem->boundingRect().height()+TextItem->pos().y()*2;
	if (TextItem->toPlainText().isEmpty()) szY = 0;

	if (NeedShowProgress())
	{
		if (szX < 200) szX = 200;
		if (szY < 20) szY = 18;
		else szY+=5;
	}

	QPoint mySz = sz;
	mySz += QPoint(-2,-2);

	QPointF Bhw;
	Bhw.setX(curGridBhw.x()+2);
	Bhw.setY(curGridBhw.y());

	QRectF screenRect;//рект сцены без бордюра 
	switch (curGridAlig)
	{
		case 0: screenRect = QRectF(Bhw,QPointF(mySz));break;
		case 1: screenRect = QRectF(QPointF(Bhw.x(),0),QPointF(mySz.x(),mySz.y()-Bhw.y()));break;
		case 2: screenRect = QRectF(QPointF(0,Bhw.y()),QPointF(mySz.x()-Bhw.x(),mySz.y()));break;
		case 3: screenRect = QRectF(QPointF(0,0),QPointF(mySz-Bhw));break;
	}

	if (onMove)
	{
		DockedTo.X1Col = false;
		DockedTo.X2Col = false;
		DockedTo.Y1Col = false;
		DockedTo.Y2Col = false;
	}

	if ((!onMove) || (!screenRect.contains(QRectF(pos().x(),pos().y(),szX,szY))))
	{
		int RazXLeft = pos().x()-screenRect.x();
		int RazXRight = screenRect.topRight().x()-(pos().x()+szX);
// 		int totX = qMin(RazXLeft,RazXRight);

		int RazYTop = pos().y()-screenRect.y();
		int RazYBotom = screenRect.bottomLeft().y()-(pos().y()+szY);
// 		int totY = qMin(RazYTop,RazYBotom);
 		
		if ((RazXLeft <= 0) || (DockedTo.X1Col))
		{	setX(screenRect.x());DockedTo.X1Col = true;	}
		if ((RazXRight <= 0) || (DockedTo.X2Col))
		{	setX(screenRect.topRight().x()-szX);	DockedTo.X2Col = true;	}
		if ((RazYTop <= 0) || (DockedTo.Y1Col))
		{	setY(screenRect.y());DockedTo.Y1Col = true; }
		if ((RazYBotom <= 0) || (DockedTo.Y2Col))
		{	setY(screenRect.bottomLeft().y()-szY);DockedTo.Y2Col = true; }
	}
}

void Q_MG_InfoWidget::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	if (event->button() == Qt::LeftButton)
	{
		setCursor(QCursor(Qt::ClosedHandCursor));
	}
	QGraphicsItem::mousePressEvent(event);
}

void Q_MG_InfoWidget::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	if (event->button() == Qt::LeftButton)
	{
		setCursor(QCursor(Qt::OpenHandCursor));
	}
	QGraphicsItem::mouseReleaseEvent(event);
}

void Q_MG_InfoWidget::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsItem::mouseMoveEvent(event);
	calcGeometry(true);
	ErrasedbackGround->update();
}

void Q_MG_InfoWidget::setUserText( QString str,bool isHtml )
{
	PrintTextMode = 1;
	UserText = str;
	UserTextIsHtml = isHtml;
	if (UserTextIsHtml) TextItem->setHtml(UserText);
	else TextItem->setPlainText(UserText);
	
	if (UserText.isEmpty()) {hide();PrintTextMode = 0;}
	else if ((!isVisible()) && (isEnabled())) show();

	calcGeometry();
	update();
}

void Q_MG_InfoWidget::on_Resize()
{
	calcGeometry();
}

void Q_MG_InfoWidget::on_Scale()
{
	calcGeometry();
}

void Q_MG_InfoWidget::PrepareAndUpdateStandartText()
{
	if (PrintTextMode == 1) return;
	QString text;
	bool SameAdded = false;
	
	//необходимы для правильной интерпритации
	double VerStartNumV = (Trans.y()*kf.y())+VertInterpretSum;
	double VerEndNumV = ((Trans.y()+sz.y())*kf.y())+VertInterpretSum;
	double HorStartNumV = (Trans.x()*kf.x())+HorInterpretSum;
	double HorEndNumV = ( (Trans.x()+sz.x())*kf.x() )+HorInterpretSum;
	
	for (int a=0;a<ShowList.count();a++)
	{
		QVector<InfoWidgetData>* vect = ShowList[a];
		for (int z=0;z<vect->count();z++)
		{	
			if ((vect->at(z).show) && (vect->at(z).valid))
			{
				if (SameAdded) text += "\n";
				text += vect->at(z).name+": ";
				
				QString tmpPtr;
				QString interpStr;
				if (!vect->at(z).needInterpretate) interpStr = QString::number(vect->at(z).value,'f',0);
				else
				{
					if (vect->at(z).singleInterpretate) interpStr = Interpret(vect->at(z).value,vect->at(z).vertical,vect->at(z).virtualVar);
					else
					{
						if (vect->at(z).vertical)
							interpStr = Interpret(vect->at(z).value,vect->at(z).vertical,vect->at(z).virtualVar,NULL,Vert_InterpretAddLabel,VerStartNumV,VerEndNumV,true);
						else
							interpStr = Interpret(vect->at(z).value,vect->at(z).vertical,vect->at(z).virtualVar,NULL,Hor_InterpretAddLabel,HorStartNumV,HorEndNumV,true);
					}
				}
				text += interpStr;
				
				SameAdded = true;
			}
		}
	}
	TextItem->setPlainText(text);//*/

	if ((!SameAdded) && (!UserText.isEmpty()))
	{
		setUserText(UserText,UserTextIsHtml);
		PrintTextMode = 0;
		return;
	}
	
	if ((!SameAdded) && (!NeedShowProgress())) hide();
	else if ((!isVisible()) && (isEnabled())) show();

	calcGeometry();
	update();
}

void Q_MG_InfoWidget::setPrintTextMode( int _PrintTextMode )
{
	PrintTextMode = _PrintTextMode;
	if (PrintTextMode == 1) setUserText(UserText,UserTextIsHtml);
	else PrepareAndUpdateStandartText();
}

void Q_MG_InfoWidget::SetValid( QVector<InfoWidgetData>& vect,bool a )
{
	for (int z=0;z<vect.count();z++)
	{	vect[z].valid = a;	}
}

void Q_MG_InfoWidget::SetShow( QVector<InfoWidgetData>& vect,bool a )
{
	for (int z=0;z<vect.count();z++)
	{	vect[z].show = a;	}
}

void Q_MG_InfoWidget::setUserData( QVector<InfoWidgetData> _UserData )
{
	UserData = _UserData;
	PrepareAndUpdateStandartText();
}

void Q_MG_InfoWidget::setShowOptionsForStarndartLabels( QString name /*= ""*/, bool showparam /*= false*/ )
{
	QVector<InfoWidgetData> tmpvar;
	tmpvar = UserData;

	if (name.isEmpty()) SetShow(tmpvar,showparam);
	else
	{
		for (int i = 0;i < tmpvar.count();i++)
			if (tmpvar[i].name==name)	{	tmpvar[i].show = showparam;break;	}
	}

	UserData = tmpvar;
}

void Q_MG_InfoWidget::SetShowProgress( bool value )
{
	ShowProgressBar = value;
	PrepareAndUpdateStandartText();
}

void Q_MG_InfoWidget::SetProgress( double a)
{
	bool NeedCalc = false;
	if ((ProgressBarValue <= 0) || (ProgressBarValue >= 100)) NeedCalc = true;
	ProgressBarValue = a;
	if ((ProgressBarValue <= 0) || (ProgressBarValue >= 100))
	{
		PrepareAndUpdateStandartText();
		return;	
	}

	if ((!isVisible()) && (isEnabled()))
		show();
	if (NeedCalc) calcGeometry();
	update();
}

bool Q_MG_InfoWidget::ShowProgress()
{
	return ShowProgressBar;
}

bool Q_MG_InfoWidget::NeedShowProgress()
{
	if ((ShowProgressBar) && (ProgressBarValue > 0) && (ProgressBarValue < 100)) return true;
	return false;
}

double Q_MG_InfoWidget::GetProgress()
{
	return ProgressBarValue;
}
