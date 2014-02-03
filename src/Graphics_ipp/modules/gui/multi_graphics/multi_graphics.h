#ifndef Q_MULTIGRAPHICS_H
#define Q_MULTIGRAPHICS_H

#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QReadWriteLock>
#include <QCache>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QHash>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QPixmapCache>
#include <QColor>
#include <QDesktopWidget>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QGraphicsObject>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QFontMetrics>
#include <QPointer>
#include <QMenu>
#include <QSettings>
#include <QWaitCondition>
#include <Windows.h>
#include <ipps.h>
#include <functional>
#include <QMutex>
#include "../../core/storage_inl.h"
#include "../../gui/utilities/push_button_item.h"
#include "../../gui/multi_graphics/labels/no_visual_labels_manager.h"

//������ ������ ��� ����������� ����� � ������/
#ifdef _DEBUG
#define MG_EnableDebugOutput
#endif


struct MG_NanoTimeStruct
{
	quint64 h;
	quint64 m;
	quint64 s;
	quint64 ms;
	quint64 mks;
	quint64 ns;
	MG_NanoTimeStruct(quint64 NanSecs)
	{
		h = NanSecs/3600000000000;
		NanSecs -= (h * 3600000000000);
		m = NanSecs / 60000000000;
		NanSecs -= (m * 60000000000);
		s = NanSecs / 1000000000;
		NanSecs -= (s * 1000000000);
		ms =  NanSecs / 1000000;
		NanSecs -= (ms * 1000000);
		mks =  NanSecs / 1000;
		NanSecs -= (mks * 1000);
		ns = NanSecs;
	}
	static QString toStr(quint64 num, int par)
	{
		QString retV = QString::number(num);
		int g = par - retV.size();
		if (g == 0) return retV;
		if (g == 1) retV = "0"+retV;
		if (g == 2) retV = "00"+retV;
		return retV;
	}
};

QString FormatSecs(quint64 secs);
QString FormatMSecs(quint64 MSecs);
QString FormatNanSecs(double NanSecs, double begin, double end, QString &outformat, bool showFullTime);
QVector<double> MarkInterval(double aStart, double aEnd, double WidthPix, double MinSpacePix, QVector<double>* Subdivision, double SubdivRate, double* OutStep = NULL);

class Q_MG_Grid;
class Q_MG_Back_Grid;
class Q_MG_MouseCursor;
class Q_MG_Material;
class Q_MG_InfoWidget;
class Q_MG_Style;

class Q_MultiGraphics : public QGraphicsView
{
	Q_OBJECT

public:
	Q_MultiGraphics(QWidget *parent);
	~Q_MultiGraphics();

	Q_MG_Back_Grid* GridBack_Layer;
	Q_MG_Grid* Grid_Layer;
	Q_MG_MouseCursor* MouseLayer;
	Q_MG_InfoWidget* InfoLayer;
	QVector<Q_MG_Material*> Materials_LayersList;
	bool SpectrumMode;//���������� ��� �������������� �������� ���� �� ������ ��� �����, ������������� ������ � wheelEvent

	void addMaterial(Q_MG_Material* mat);
	void wheelEvent ( QWheelEvent * event );
	void resizeEvent ( QResizeEvent * event );
	void AllCopyMatrixByGridPublic() {AllCopyMatrixByGrid();}
	

	//�������
	void	ShowMarkers();
	void	HideMarkers();
	void	SetMoveAllMarkers();
	void	SetMoveOneMarker();
	int		MarkersCount(bool isVertical) const;
	bool	MarkersVisible(bool isVertical) const;
	bool	MoveAllMarkers() const;

	
	//�������� �����
	void	SetStrangeCross(double, double);
	void	SetShowStrangeCross(bool value,bool horLabel,bool vertLabel);

	
	//�������
	void	ClearAllSelections();
	
	
	//ZOOM
	void ScaleOnSteps(int steps, bool byX,bool byY);
	void ZoomOutFull(bool x = true, bool y = true);
	void SetViewportForValues(double LeftVal,double TopVal,double RightVal,double BottomVal);//� ���������������� (�����������) ������
	void ZoomToSelection(int selIndex = 0);//selIndex - ���� ���� ������������ �� �������� ��������� ��� ������ ������� ������, ������� �� CurSelectionType
	void SetDirectKf(QPointF newKf);
	bool isSomeZoomIsOnMax(bool &X, bool& Y);
	bool EnableHorZoom;//���. ���� Zoom
	bool EnableVertZoom;
	double VertStep;//���� ������ �� 1-�� ������ �������� ����, �� ��������� 2
	double HorStep;



	QPointer<Q_MultiGraphics>  RelationBaseClass_;//��������� �� ����� ������� relation
	
	void keyPressEvent ( QKeyEvent * event );//��������� ��������
	void keyReleaseEvent ( QKeyEvent * event );//��������� ��������


	//ContextMenu
	QMenu* contextMenu() const {return contextMenu_;}
	void setContextMenu(QMenu* menu);


	//wallpaper
	void setWallPaper(QGraphicsPixmapItem* wallp);
	void removeWallPaper();
	void posWallPaper();

private:


	QGraphicsScene* MainScene;
	void PrepareScene();
	Q_MG_Style* MainSettings;

	void AllCopyMatrixByGrid();

	QGraphicsPixmapItem* wallp_;
	//ContextMenu
	QPointer<QMenu> contextMenu_;


	DISALLOW_COPY_AND_ASSIGN(Q_MultiGraphics);

protected:
    void scrollContentsBy(int, int);

public slots:
	void ScreenDragByMouse(QPointF pf);
	void ScreenScaleByMidMouse(QPointF pf, QPointF MousePos);
	
	//���� ��� ContextMenu
	void on_wantContextMenu();
	void on_menuAboutToShow();
	void on_menuAboutToHide();

public slots:
	

};


//////////////////////////////////////////////////////////////////////////
//����� ���� ��������� ������ ����������� ��� ������
//////////////////////////////////////////////////////////////////////////

struct str_MouseState
{
	bool Left;bool Right;bool Middle;
	str_MouseState()
	{	Left = false;Right = false;Middle = false;	}
	void reset()
	{	Left = false;Right = false;Middle = false;	}
};

struct str_Collision
{
	bool X1Col;bool X2Col;bool Y1Col;bool Y2Col;
	str_Collision()
	{ X1Col = false;X2Col = false;Y1Col = false;Y2Col = false; }
};

class Q_MG_BaseClass :public QGraphicsObject
{
	friend Q_MG_Grid;
	friend Q_MG_Material;
	friend Q_MG_MouseCursor;
	friend Q_MultiGraphics;
	friend Q_MG_InfoWidget;
	Q_OBJECT
public:
	Q_MG_BaseClass(QGraphicsItem *parent = NULL);
	virtual ~Q_MG_BaseClass() {};

	void CopyMatrix(Q_MG_BaseClass* src);//�������� ������� ������� �� ������, ����� �� ����������� ��������, ��� ������������ InterpretType �� ����������

	inline void ResizeEvent(QPoint _sz, bool MinMaxChanged = false);//�������� ��� �������� �����
	inline void MoveEvent(QPointF _Trans,bool NeedCalcKf = false);//�����������, ������������� ����� Trans (Trans = _Trans) � ��������	(NeedCalcKf - ��������� ���� ������������ Scale-��)
	void MoveEvent_ValueOnCenter(QPointF Value);//������������� ������, ������ ��������� ����� ������� ����� �������� ��������� �� �������� ������ �� �������� �� TransformOnCenter
	void MoveEvent_ValueOnPixel(double valX, double valY, double PixelX, double PixelY);
	QPointF ScaleEvent(QPointF scaleKf, QPointF scalePos);//��������� ����� kf (kf+scaleKf), � ����� ���� ���� �� ������ � ������� ������� ��������� ���������� Kf ���, ���������� ��������� kf
	QPointF setKf(QPointF inc_kf);
	void ScaleAllToScreen(bool x,bool y);//������������ ����� ���������� ��� ������� ��� ��������� �� ������

	void SetViewportForValues(double LeftVal,double TopVal,double RightVal,double BottomVal, int mode);//� "��������" ������, mode: 0-������������ ��� ���, 1-������ 0X 2-������ 0Y

	void setMinAndMaxTransPoint( QPointF _MaxTransPoint,QPointF  _MinTransPoint);//� "��������" ������

	QPointF getCurTrans();//���������� ���������������� ���������
	QPointF getCurKf();//���������� �������

	bool Release_ScaleIncrementType;//true �������� ������� �� ���, false ��������� � �������� ���
	bool InverseVAxis;//������������� �������� ����
	bool InverseHAxis;
	bool LockHorMove;//���� ���������� ����� ���������� �������� �������� �� ����������� (���������� ��� ��� ���������� ����� �����)
	bool LockVerMove;
	QPointF MaximumZoom;

	virtual void on_Resize() {};//���������� ����� ���������� ResizeEvent
	virtual void on_Move() {};//���������� ����� ���������� MoveEvent
	virtual void on_MinMaxChage() {};//���������� ����� ���������� setMinAndMaxTransPoint
	virtual void on_Scale() {};
	virtual void on_Scale_or_MinMaxChange() {};//�������������� ������

	////
	virtual QString Interpret(double val,  bool isVertical,bool isVirtual, QString* out = NULL,QString In = QString(),double start = 0,double end = 0,bool startEndisVirtual = true);//������� ����������������� �������� �������� ��� ����������� �� ������
	int Vert_RoundValue;//��� ���������� �������� �������� �� ����� �� ��������� = 0 (����.)
	int Hor_RoundValue;
	bool EnableInterpretAddLabel;
	QString Vert_InterpretAddLabel;//���������� ������ ��� �������������, ����������� ��� EnableInterpretAddLabel
	QString Hor_InterpretAddLabel;
	bool EnableKiloMath;//
	bool EnableHorTimerConvertor;
	bool EnableVertTimerConvertor;
	bool InterpretShowFullTime;
	double HorTimerKf;//������ FormatMSec ( val / HorTimerKf )
	double VertTimerKf;//������ FormatMSec ( val / HorTimerKf )

	/////
	double ApplyInterpretSum(double val,bool isVertical) const;
	double DisApplyInterpretSum(double val,bool isVertical) const;
	double VertInterpretSum;//���������� ������������� ���������
	double HorInterpretSum;//���������� ������������� ��������

	//��� �������� Grid ����� �����������
	bool GridChanged(QPointF Hw,int aligment);

	void Reset();

	double GetValueByPixelPublic(double pixel,bool isVertical);
	double GetPixelByValuePublic(double val,bool isVertical);

	void setFixedScaleList(QList<double> FixedList);

	bool isSomeZoomIsOnMax(bool &X, bool& Y);//������ ������ ��� , "����� ���� ���� (�� X ��� Y) ��������� ����������� ��� ��-��?"


	str_Collision CheckCollisionForTrans(QPointF trans,QPointF _kf);//��������� ��������� � ������ ������� ��� ��������� trans � _kf
	double GetTransByValueOnPixel(double Value, double pointOnScreen, bool isVertical);
	double GetPixelByValueForTrans(QPointF tans, QPointF _kf,double val,bool isVertical);
	double GetPixelByValue(double val,bool isVertical);//������� ������������� ��������� c ������ Trans � �������� ����
	double GetValueByPixel(double pixel,bool isVertical);

protected:
	virtual void ReDraw () = 0;//���������� ��� Move � Resize ����� ���� ������������ ������
	QPoint sz;//������� ������ � ������ ����� (���� ���������)
	QPointF Trans;//����� �� ������� �������� ����� �� ���������� ������� (� ��������)
	QPointF UserTrans;// -/- ����������������
	QPointF kf;//��������� , ������� ������ � �������� //screenX = x*kf.x() screenY = y*kf.y()
	QPointF MinimumKf;//���������� ��������� ����������, �������������� ������ ��� �������

	QPointF MaxTransPoint;//������������ �������� � ���������
	QPointF MinTransPoint;//������������ ��������

	Q_MG_Style* settings;


	//��������� �����, �������� �� ������� ����������� �� ���������� ����� GridChanged
	double GridTopStep;
	double GridLeftStep;
	double GridRightStep;
	double GridBottomStep;
	QPointF curGridBhw;int curGridAlig;

	//������������� �����
	bool EnableFixedScale;
	QList<double> FixedScaleList;//���������� ������ ���� ������������ � ������� ���������� �.�. 1,2,3,3.5,4, ������ � ��� ����� ��������
	

public slots:
	virtual void getCommand(QString ToObj,QString CommandName,QVariant params) = 0;

	//�������� ��� ������������, ���������� �� Squish
	double getCurKf_SQ_X()		{	return kf.x();		};
	double getCurKf_SQ_Y()		{	return kf.y();		};
	double getCurTrans_SQ_X()	{	return Trans.x();	};
	double getCurTrans_SQ_Y()	{	return Trans.y();	};


signals:
	void sendCommand(QString ToObj,QString CommandName,QVariant params);

private:
	DISALLOW_COPY_AND_ASSIGN(Q_MG_BaseClass);
};
#include <QDebug>
//////////////////////////////////////////////////////////////////////////
//������� �� ������ ����� Align � BorderHW
class Q_MG_MouseCursor : public Q_MG_BaseClass
{
	Q_OBJECT
public:
	Q_MG_MouseCursor(QGraphicsItem *parent = NULL);
	~Q_MG_MouseCursor() {ClearAllSelections();delete SizeAllCur;};

    virtual void ReDraw () {update();};

	// �������� � ���������� "�����������" �����

	//�������
	void			addMarker(double Val,bool isVert);//��������� ������ � "�����������" ������
	void			ClearAllMarkers(bool isVert);//������� ��� ������� 
	void			RemoveMarker(int index, bool isVert, bool need_change_signal = true);//����� ����� ������� ��� ������� Ctrl � ������ ������� ���� ���� �� �������
	QList<double>	getMarkers(bool isVert);//�������� ������ �������� � "�����������" ������
	void			SetMarkers(QList<double>,bool isVertical,bool noSignal = false);//� "�����������" ������
	void			UpdateMarker(int index, double val,bool isVertical);//� "�����������" ������
	bool			showVMarkers;//���������� �� �������
	bool			showHMarkers;//���������� �� �������
	bool			mutiMarkerMoove;//���������� ������� ������
	bool			MarkerGlowEffect;//��������� ��������� � ��������
	bool			showMarkersText;//���������� �� ������� �� ��������
	bool			showMarkersDeff;//���������� �� ������� ����� ���������
	bool			showMarkerCentralParam;//���������� �� ������� � ������ �� ����� ������ �������
	//////////////////////////////////////////////////////////////////////////

	//���������
	void			SetSelection(double,double,double,double, bool noSignal = false);//�� ��������  CurSelectionType, � ����������� ������
	void			updateCursorPosition(QPointF pos);//��������� ������� ������� (���������� ��-�� ������������ ����, ���� �������� paintEvent � hover ��� �� ������) 
	bool			HasSelection() const;//�� ����
	bool			HasRectSelection() const;
	bool			HasHorSelection() const;
	bool			HasVertSelection() const;
	void			ClearAllSelections();//������� ��� �������
	QList<QPointF>	getHVSelected(bool Vert);//�������� �� ��� ��������� ������������� � "�����������" ������
	QList<QRectF>	getRectSelected();//�������� �� ��� ��������� ������������� � "�����������" ������
	void			RemoveLastSelection();//�� ��������  CurSelectionType
	void			SetMultySelectEnabled(bool enable);//true, ������ ����� ����������� ������ � multy select, �.�. ������ Ctrl �� ����� �������������� ��. SetButtonAltClickIsSingle
	bool			IsMultySelectEnabled() const;
	void			SetButtonCtrlClickIsSingle(bool is_single = true);//�� ��������� � ������� Ctrl �������� ������������� ��������� (false), � ���������� ����� ��� �������� �� ������, ��� Ctrl multy � Ctrl single
	bool			IsButtonCtrlClickIsSingle() const;
	bool			EnableByStepSelection;//��� ���������
	double			stepValX;//��� ���������
	double			stepValY;//��� ���������
	bool			ShowSelectText;//�������� �� ������� � ��������� ��� ���������
	int				CurSelectionType;//��� ����������������� ���������, 0 - ����������, 1 - �������������� ||, 2 - ������������ =, 3 - ��������� []
	//////////////////////////////////////////////////////////////////////////

	//�������
	void			ClearAllFredomLabels();
	void			ChangeLabelText(QString name,QString new_text, bool immediately_update = false);
	void			ChangeLabelPos(QString name,double new_pos, bool immediately_update = false);
	void			SetFredomLabel(double valX_b,QString text,QString name = QString(),bool immediately_update = false, QColor label_color  = QColor(100,250,250));
	void			SetLabelsUnionText(QString union_text);
	void			SetLabelsUnionShowHint(bool show_hint);
	void			RemoveLabel(QString name, bool immediately_update = false);
	//////////////////////////////////////////////////////////////////////////

	bool	DrawTextDecorateMidLine;//�������� �� ���������� ����� ������ ��� �������� ��� ���������	
	bool	showBigGrafCursor;//�������� �� ������� ����������� �������
	bool	showMiniGrafCursor;//�������� �� ����� ����������� ������� (����� �������� �������, ����� �� ��������)
	bool	showVValues;//���������� �������� �� ������� ��������� ����
	bool	showHValues;
	bool	GlowEffect;//��������� ������ ��������� � �������
	bool	UseMouseCursorJump;//������������� �� ���� � ������ ������ ��� ���������� ����� � ��������
		
	
	//�����
	bool	showStrangeCrossHorText;
	bool	showStrangeCrossVertText;
	bool	showStrangeCross;
	double	StrangeCrossValX;
	double	StrangeCrossValY;


private:

	QRectF boundingRect() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
	virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
	virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );

	double CursorPosX;
	double CursorPosY;
	bool HideCursor;
	QCursor* SizeAllCur;
	QDesktopWidget Desktop;
	bool CursorJastReDroped;


	//���������
	QRectF CurMoveSel;
	bool OnSelection;
	bool selection_deselection_started_;
	bool multi_select_enable_;//�������� �� ������������� �����
	bool button_ctrl_click_is_single_;
	bool is_deselect_;
	QList<QPointF> VAxisSelList;//�������� � ���������, ������� ������ ������
	QList<QPointF> HAxisSelList;
	QList<QRectF> RectSelList;
	struct SelectionButtonPanel
	{
		std::shared_ptr<gui::PushButtonItem> zoom_on_button_;
		std::shared_ptr<gui::PushButtonItem> close_button_;
		SelectionButtonPanel(QGraphicsItem* parent)
		{
			zoom_on_button_ = std::make_shared<gui::PushButtonItem>(parent,"d:\\zoom_B_in.png","d:\\zoom_B_out.png","d:\\zoom_B_press.png");
		}
	};

	void StartSelectionDeselection(QGraphicsSceneMouseEvent * event);
	bool CalcAddSelectionPoint(QList<QPointF> & list, QPointF selectRectPoint );
	bool CalcAddSelectRect(QList<QRectF> & list, QRectF SelectRect);
	bool CalcDeselectPoint(QList<QPointF> & list, QPointF unSelectRectPoint, bool multySelect);
	bool CalcDeselectRect(QList<QRectF> & list, QRectF unSelectRect);

	//�� ����������� ������� ������� � 
	QList<SelectionButtonPanel> zoomOnButtonList_VAxisSel;//������ ������ ��� �������
	QList<SelectionButtonPanel> zoomOnButtonList_HAxisSel;
	QList<SelectionButtonPanel> zoomOnButtonList_RectSel;
	//////////////////////////////////////////////////////////////////////////

	inline void DrawSelections(QPainter *painter);
	inline void TextDecorate(QPainter *painter,const QRectF& points, const QPointF& value, bool isVertical,int style = 0);
	inline bool FigureIsVisible(const QRectF& points);//������ �� ������ ���� �� ������ � ������ ��������, � ��������

	//�������
	QPixmap VerCurPix;
	QPixmap HorCurPix;
	QList<double> HorMark;
	QList<double> VertMark;
	inline void DrawMarkers(QPainter *painter);//������ ������� � ����� ��������� ��������� �� ������ ��� �������� � ��������� �������������� UnderMouseMarkerIndex
	int UnderMouseMarkerIndex;//������ ������� ��� �������� (��� ������� ����� ���� < 0 �� ������ �� �����)
	bool UnderMouseMarkerIndexIsVert;//� ����� ������� ������ ����������
	bool OnMarkerMoove;
	bool NeedSortHorMark;
	bool NeedSortVertMark;
	QList<double> sortedHorMark;
	QList<double> sortedVertMark;

	//������������� �������� ��������� � �������� �� �������� �����\���� ���������
	inline double Correct_ValueByStep_or_PixelByGrid(double val,double pixel,bool isVertical);

	//����� �� �� ����� �����
	int screenStartPosScaleX;
	int screenStartPosScaleY;
	QPointF sceneStartPosScale;

	void DrawStrangeCross(QPainter *painter);

	//������� ����������� ���� ��� ����� ������������ ����
	qint64 deltaMoveX;
	qint64 deltaMoveY;
	
	//���������� ��� COntextMenu ����� ������ �� ������
	bool IgnoreNextHoverEvent;

	//��������� ������� �� ������
	void DrawFredomLabels(QPainter *painter);
	//�����
	QPointer<NoVisualLabelsManager> labels_manager_;

	//////////////////////////////////////////////////////////////////////////

	DISALLOW_COPY_AND_ASSIGN(Q_MG_MouseCursor);

signals:
	//��� ������� � "��������" ������
	void dragScreen(QPointF dragPoint);//� �������� �� ������
	void dragScreenFinish();//�������� ������ ���������, �������� ���� ����� ��� ��������� ������ �� 1 ������� (�� daltaMooveX (Y) )
	void scaleScreen(QPointF dragPoint, QPointF MousePos);//� �������� �� ������
	
	void selectionCleared();
	void selectedPixelChange(double x1, double y1,double x2,double y2);
	void selectionFinished(double x1, double y1,double x2,double y2);
    void selectionFinishedRedLine(double y); ///emit when choose threshold
	void deselectFinished();//�������� ����� deselect ���� �������� - ���������� ����� ��������� ��������� ��������� - ����� ���������� ��� ������
	
	void markerMooved(int index, double value,bool isVertical);
	void markersChanged(QList<double> Mark,bool isVertical);
	
	void DoubleClicked(double,double);
	void StrangeCrossMoved();
	
	void wantContextMenu();
	

public slots:
	void getCommand(QString ToObj,QString CommandName,QVariant params);

};


//////////////////////////////////////////////////////////////////////////
class Q_MG_Back_Grid : public  QGraphicsItem
{
friend Q_MG_Grid;
friend Q_MultiGraphics;
public:
	Q_MG_Back_Grid(QGraphicsItem *parent = NULL);
	~Q_MG_Back_Grid() {};
	QRectF boundingRect() const { return QRectF(0,0,sz.x()+1,sz.y()+1); };
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
	void ReDraw() { setCacheMode(QGraphicsItem::DeviceCoordinateCache);update(); }
	
	inline void ClearAll();

private:
	QPoint sz;
	
	//��� ��� ���� - � ������� ����� ��� ������� �������� ���� ������� ��� ��������
	QVector<qreal> XSubDiv;
	QVector<qreal> YSubDiv;
	QVector<qreal> XDiv;
	QVector<qreal> YDiv;
	double HightLightX;
	double HightLightY;
	Q_MG_Style* settings;

	DISALLOW_COPY_AND_ASSIGN(Q_MG_Back_Grid);
};

class Q_MG_Grid : public Q_MG_BaseClass
{
	Q_OBJECT
public:
	Q_MG_Grid(Q_MG_Back_Grid* _GridBack_Layer,QGraphicsItem *parent = NULL);
	~Q_MG_Grid() {};

	QRectF boundingRect() const { return QRectF(0,0,sz.x()+1,sz.y()+1); };
	QPainterPath shape() const;
	virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );//������ ����� �� ������, ����� �� �������
	virtual QPointF getBorderHightWidth();//��������� ������ � ������ ������������� �������, ���������� �� Q_MG_MouseCursor

	
	int flag_GridAligment;// 0 - LT 1 - LB 2 - RT 3 - RB // ��������� ��� Q_MG_MouseCursor		0	2
																					//			1	3

	bool flag_DrawVAxis;//�������� �� ��� � �����
	bool flag_DrawHAxis;

	bool DrawVSubDiv;//�������� �� ���. �������
	bool DrawHSubDiv;

	bool DrawVBaseLine;
	bool DrawHBaseLine;

	bool DrawVGridBorders;//�������� ������� �� ����������
	bool DrawHGridBorders;

	double HightLightVVal;//����� �������� ������������ �� ��������� �������������� 0 0, ���� �������� ��� �� ����� ��� �� ��������������
	double HightLightHVal;
	
	QString LabelVAxis;//������� � ������� ����������� ������������� (���������)
	QString LabelHAxis;

	double HGridNumberStep;//������� ��� �����, ������ ���� maxTextWidth += HGridNumberStep, ��������� ������� 50
	double VGridNumberStep;//������� ��� ����� = 25 ���� ��� ����.

	void ReDraw ()
	{
		int errCode = MarkIntervalAndCalcBorderSizes();
		if (errCode == 1)
		{
			sendCommand("mg_InfoWidget","PrintText",QObject::tr("�������� ������� ������� �� ������� ������������ �������"));
		}

		if (errCode == 2)
		{
			return;
		}

		update();
	};

	
	int MarkIntervalAndCalcBorderSizes();//���������� ��� ������ 0-��� ���������, 1 - ������ ��� �������� ����������, 2-������� ���� ����� ����, ������� �������� ����� ������
	QVector<double> IntervalRetValV;
	QVector<double> IntervalRetValH;
	QVector<double> SubDivRetValV;
	QVector<double> SubDivRetValH;

private:

	QString printLabelVAxis;//������� � ������� ������� ����������
	QString printLabelHAxis;//������� � ������� ������� ����������

	qreal BorderW;//������ ������������� �������, ������������ �� ����� ���������
	qreal BorderH;//������ -/-
		
	Q_MG_Back_Grid* GridBack_Layer;

	inline void DrawStandartGrid(QPainter* pnt1);
	QVector<double> MyMarkInterval( double start,double end,double WidthPix, double MinSpacePix, QVector<double>* Subdivision, double SubdivRate, bool isVertical);

	DISALLOW_COPY_AND_ASSIGN(Q_MG_Grid);

public slots:
	void getCommand(QString ToObj,QString CommandName,QVariant params);
};

//////////////////////////////////////////////////////////////////////////
struct InfoWidgetData
{
	bool show;
	bool valid;
	bool vertical;
	bool needInterpretate;
	bool singleInterpretate;
	bool virtualVar;
	double value;
	QString name;
	InfoWidgetData()	{	virtualVar = false;singleInterpretate = false;needInterpretate = true;show = true;valid = false;vertical = false;value = 0;	}
	void setValueAndValid(double val,bool isVertical, bool _needInterpretate = true, bool _singleInterpretate = false, bool is_virtual = false)	{	virtualVar = is_virtual;needInterpretate = _needInterpretate;singleInterpretate = _singleInterpretate;value = val;valid = true;vertical = isVertical;	}
};


class Q_MG_InfoWidget : public Q_MG_BaseClass
{
	friend Q_MultiGraphics;
	Q_OBJECT
public:

	Q_MG_InfoWidget(Q_MG_Style* sett,QGraphicsItem *parent = NULL);
	~Q_MG_InfoWidget() {};


	void PrepareAndUpdateStandartText();
	void setUserData(QVector<InfoWidgetData> _UserData);//
	void setShowOptionsForStarndartLabels(QString name = "", bool showparam = false);
	QGraphicsItem* ErrasedbackGround;

	void SetValid(QVector<InfoWidgetData>& vect,bool a);
	void SetShow(QVector<InfoWidgetData>& vect,bool a);

	QVector<InfoWidgetData> UserData;//������ ��������� �������� ������������

private:
	QRectF boundingRect() const { return QRectF(0,0,szX+1,szY+1); };
	virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	void ReDraw () { };


	QString UserText;bool UserTextIsHtml;
	int PrintTextMode;

	QGraphicsTextItem* TextItem;
	QPixmap conLT,conRT,conLB,conRB;

	void on_Resize();
	void on_Scale();

	int szX;
	int szY;

	void calcGeometry(bool onMove= false);
	str_Collision DockedTo;
	
	QVector< QVector<InfoWidgetData>* > ShowList;

	//�������� ���
	double ProgressBarValue;
	bool ShowProgressBar;
	bool NeedShowProgress();

	DISALLOW_COPY_AND_ASSIGN(Q_MG_InfoWidget);

public slots:

	void setUserText(QString str,bool isHtml = false);//��������� ����������������� ������ ���� PrintTextMode = 1 �������� �������������
	void setPrintTextMode(int _PrintTextMode);//��� ��������� ��������: 0 - ��������,1 - ����������������
	void getCommand(QString ToObj,QString CommandName,QVariant params);

	void SetProgress(double);//����� �������� �� 0 �� 100, ���������� � ���� ����, ���� �� 0 ��� 100 ��������
	double GetProgress();//���������� ������� ��������
	bool ShowProgress();
	void SetShowProgress(bool value);

};


//////////////////////////////////////////////////////////////////////////



class Q_MG_MaterialElement : public  QGraphicsObject
{
friend Q_MG_Material;
public:
	Q_MG_MaterialElement(int _szX,int _szY, QImage::Format ElementFormat,QGraphicsItem *parent = NULL);
	~Q_MG_MaterialElement()
	{
		delete img;img = NULL;
	};
		
	virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
	QRectF boundingRect() const { return QRectF(0,0,szX,szY); };

	unsigned __int64 ElementIndex_64;
	int szX;int szY;
	unsigned __int64 elX;
	unsigned __int64 elY;

	bool elementIsReady_;

	QImage* img;

private:
	DISALLOW_COPY_AND_ASSIGN(Q_MG_MaterialElement);
};



//#define SHOW_DEBUG_INFO
void debugOut(QString a);





class Q_MG_RenderThreadClass : public QThread
{
	Q_OBJECT

public:
	Q_MG_RenderThreadClass( QObject *parent = NULL);
	virtual ~Q_MG_RenderThreadClass();


	template<class T>
	void MakePauseAndDo(T t)
	{
		if (isPaused_) return;

        {
            QReadLocker locker(&thread_status_lock_);
            if (thread_status_ == 2) return;// ����� ���� ����� �� ������ ������ �����
        }

        {
            QWriteLocker locker(&thread_status_lock_);
            thread_status_ = 1;
        }

		debugOut("try set pause");
		{
			QMutexLocker locker(&render_mutex_);
			debugOut("pause sets, execute function");

			isPaused_ = true;
			t();
			isPaused_ = false;

			debugOut("unpause");
		}
	}
	template<class fT>
	void setFilter(fT t)
	{	filter_func_ = t; 	}
	bool isFiltered()		{return useFilter_;};
	void turnOnFilter()		{useFilter_ = true;};
	void turnOffFilter()	{useFilter_ = false;};


	Q_MG_RenderThreadClass* GetThreadPointer() { return this; };
    
    int thread_status()                     { QReadLocker   locker(&thread_status_lock_); return thread_status_;                                }
    void set_thread_status(int new_status)  { QWriteLocker  locker(&thread_status_lock_); if(thread_status_ != 2) thread_status_ = new_status;  }

	bool isSleep() {	return thread_sleep_status_;	};

	virtual void	RenderData()										=0;
	virtual void	addData(Storage<Q_MG_MaterialElement*>& in_list)	=0;
	virtual void	ClearQueue()										=0;
	virtual int		ElementsInQueue()			{return 0;}
	virtual int		GetCurrentProgressPercent() {return 100;};
	

signals:
	void ElementReady(Q_MG_MaterialElement*, bool);
	void RenderFinished();


protected:
	void	Start_Wake();		//����� WaitContion, ���� ����� ����,������� ���� �����
	void	FinishHim_wait();	//������������ ������ ��� ������ ���������
	bool	NeedBreaked();		//���������� true ���� ��������� ���� �� ������ pause ��� finish
	bool UseFilterIfOn(Q_MG_MaterialElement* el)
	{	if (useFilter_) return filter_func_(el);else return true;	}

private:
	//�����������
	void	run();
	std::function<bool(Q_MG_MaterialElement*)>  filter_func_;
	//void	setPauseFlag();		//������ "�� ��������" ���� �����
	bool	isPaused_;

    QReadWriteLock  thread_status_lock_;
	int             thread_status_;
	QWaitCondition	Thread_finishWork;	
	QMutex render_mutex_;	
	QAtomicInt useFilter_;
	QAtomicInt thread_sleep_status_;

private:
	DISALLOW_COPY_AND_ASSIGN(Q_MG_RenderThreadClass);
};


class Q_MG_Material : public Q_MG_BaseClass
{
friend Q_MG_MaterialElement;
friend Q_MultiGraphics;
	Q_OBJECT
public:
	Q_MG_Material(QGraphicsItem *parent = NULL, int _ElementH = 100, int _ElementW = 100,QImage::Format ElementsImageFormat = QImage::Format_ARGB32_Premultiplied, bool use_multi_thread_render = false, QPointer<Q_MG_RenderThreadClass> in_renderThread = NULL);
	virtual ~Q_MG_Material() { };

	virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
	QRectF boundingRect() const { return QRectF(0,0,MaterialSize.width(),MaterialSize.height()); };

	QPointF GetPixelByValueOnMaterial(double Val1,double Val2);
	QPointF GetValueByPixelOnMaterial(double PixelX,double PixelY);
	double fast_GetYPixelByValue(double Val1) {return GetPixelByValue(Val1,true)-pos().y();};
	void ReDraw() { }; //�� ���� ��������� ���������� ������� ��� ����������� ���� ��� ��������� �������

	double Hor_kf_inc;
	double Vert_kf_inc;

	virtual void SelfClear() {};//����� ����� ��������� �� �����, � ��� ������� 90% �� ��� ��� ������, ���� �����������, ��������� ��� ������ ��������
	virtual void ExtMaterialReInit() = 0;//����� ���������� ����� �����
	virtual bool ExtViewPortChange(Q_MG_MaterialElement* NewEl,unsigned __int64 elXpos,unsigned __int64 elYpos) = 0;
	virtual bool PrepareForViewPortChange(qreal key_kf_x,qreal key_kf_y,double StartPixelX, double EndPixelX, double StartPixelY,double EndPixelY) = 0;
	virtual void ExtAfterViewPortChange() = 0;
	
	bool ElementIsOnScreen(const Q_MG_MaterialElement* el);
	void GetViewPort(double& value_startX, double& value_startY,double& value_endX, double& value_endY);

	int MaterialID;//�������� �������� ��� ���������� � ������, ���� ������ ��� ����������� ����� ������ ��������

signals:
	void ViewPortChanged(double value_startX, double value_startY,double value_endX, double value_endY);//� �������� ��������� (���� �������)

protected:

	void AllMaterialReInit();//������������� ���������� ��� ������ ��������� �����, ��������: ��������� ��������, ��������� ����� ������
	bool FirstPaint;
	QSizeF MaterialSize;

	virtual void on_Move();
	virtual void on_Resize();
	virtual void on_Scale_or_MinMaxChange();

	void ViewPortChange();
	void AfterViewPortChange();

	bool NeedMaterialUpdateAfterMoov;

	QImage::Format ElementsImageFormat_;
	
	void ClearEl();
	void RemoveAllOffScreenEl();//�������� ��� �������� ����������� �� �������, ������� � ������ �������
	
	Storage<Q_MG_MaterialElement*> ElList;
	int ElementH;
	int ElementW;
	
	//��� ���������������
	bool UseMultiThread;
	QPointer<Q_MG_RenderThreadClass> RenderThread;

	
private:
	DISALLOW_COPY_AND_ASSIGN(Q_MG_Material);

public slots:
	void getCommand(QString ToObj,QString CommandName,QVariant params);

	void RenderFinishedSlot();
	void ElementReady(Q_MG_MaterialElement* el, bool ElementIsEmpty);
};


#endif // Q_MULTIGRAPHICS_H


