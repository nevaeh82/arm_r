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

//убрать дефайн или перегрузить вывод в релизе/
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
class Q_MG_SelectionAreas;
class Q_MG_Material;
class Q_MG_InfoWidget;
class Q_MG_Style;

class Q_MultiGraphics : public QGraphicsView
{
	Q_OBJECT

public:
	Q_MultiGraphics(QWidget *parent,QSettings* settings);
	~Q_MultiGraphics();

	Q_MG_Back_Grid* GridBack_Layer;
	Q_MG_Grid* Grid_Layer;
	Q_MG_MouseCursor* MouseLayer;
	Q_MG_SelectionAreas*	DetectedLayer;
	Q_MG_InfoWidget* InfoLayer;
	QVector<Q_MG_Material*> Materials_LayersList;
	bool SpectrumMode;//необходимо для переназначения колесика мыши на другой тип зуума, испоользуется только в wheelEvent

	void addMaterial(Q_MG_Material* mat);
	void wheelEvent ( QWheelEvent * event );
	void resizeEvent ( QResizeEvent * event );
	void AllCopyMatrixByGridPublic() {AllCopyMatrixByGrid();}
	

	//маркеры
	void	ShowMarkers();
	void	HideMarkers();
	void	SetMoveAllMarkers();
	void	SetMoveOneMarker();
	int		MarkersCount(bool isVertical) const;
	bool	MarkersVisible(bool isVertical) const;
	bool	MoveAllMarkers() const;

	
	//страшный крест
	void	SetStrangeCross(double, double);
	void	SetShowStrangeCross(bool value,bool horLabel,bool vertLabel);

	
	//выборка
	void	ClearAllSelections();
	
	
	//ZOOM
	void ScaleOnSteps(int steps, bool byX,bool byY);
	void ZoomOutFull(bool x = true, bool y = true);
	void SetViewportForValues(double LeftVal,double TopVal,double RightVal,double BottomVal);//в пользовательских (виртуальных) бубнах
	void ZoomToSelection(int selIndex = 0);//selIndex - если есть мультиселект то говорить конкретно для какого индекса зумить, зависит от CurSelectionType
	void SetDirectKf(QPointF newKf);
	bool isSomeZoomIsOnMax(bool &X, bool& Y);
	bool EnableHorZoom;//вкл. выкл Zoom
	bool EnableVertZoom;
	double VertStep;//шаги скейла по 1-му крутку колесика мыши, по умолчанию 2
	double HorStep;



	QPointer<Q_MultiGraphics>  RelationBaseClass_;//указатель на класс который relation
	
	void keyPressEvent ( QKeyEvent * event );//временные тестовые
	void keyReleaseEvent ( QKeyEvent * event );//временные тестовые


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
	
	//слот для ContextMenu
	void on_wantContextMenu();
	void on_menuAboutToShow();
	void on_menuAboutToHide();

public slots:
	

};


//////////////////////////////////////////////////////////////////////////
//ДАЛЕЕ ИДУТ СЛУЖЕБНЫЕ КЛАССЫ НЕОБХОДИМЫЕ ДЛЯ РАБОТЫ
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
	friend Q_MG_SelectionAreas;
	friend Q_MultiGraphics;
	friend Q_MG_InfoWidget;
	Q_OBJECT
public:
	Q_MG_BaseClass(QGraphicsItem *parent = NULL);
	virtual ~Q_MG_BaseClass() {};

	void CopyMatrix(Q_MG_BaseClass* src);//копирует базовую матрицу из класса, чтобы не производить расчетов, вид предствления InterpretType не копируется

	inline void ResizeEvent(QPoint _sz, bool MinMaxChanged = false);//вызывать при ресайезе сцены
	inline void MoveEvent(QPointF _Trans,bool NeedCalcKf = false);//перемещение, устанавливает новый Trans (Trans = _Trans) в пикселях	(NeedCalcKf - ставиться если инициировано Scale-ом)
	void MoveEvent_ValueOnCenter(QPointF Value);//перегруженная фнкция, ставит трансформ таким образом чтобы значение оказалось по середине экрана не зависимо от TransformOnCenter
	void MoveEvent_ValueOnPixel(double valX, double valY, double PixelX, double PixelY);
	QPointF ScaleEvent(QPointF scaleKf, QPointF scalePos);//вычисляет новый kf (kf+scaleKf), в сучае если зуум не влазит в границы области просмотра изменяется Kf сам, возвращает полченный kf
	QPointF setKf(QPointF inc_kf);
	void ScaleAllToScreen(bool x,bool y);//устанавливет такой коофициент при котором все находится на экране

	void SetViewportForValues(double LeftVal,double TopVal,double RightVal,double BottomVal, int mode);//в "реальных" бубнах, mode: 0-просчитываем все оси, 1-только 0X 2-только 0Y

	void setMinAndMaxTransPoint( QPointF _MaxTransPoint,QPointF  _MinTransPoint);//в "реальных" бубнах

	QPointF getCurTrans();//возвращает пользовательский трансформ
	QPointF getCurKf();//возвращает текущий

	bool Release_ScaleIncrementType;//true умножать текущий на шаг, false плюсовать к текущему шаг
	bool InverseVAxis;//устанавливают инверсию осей
	bool InverseHAxis;
	bool LockHorMove;//флаг включается когда необходимо залочить движение по горизонтали (включается сам при достижении конца зумма)
	bool LockVerMove;
	QPointF MaximumZoom;

	virtual void on_Resize() {};//вызывеатся после выполнения ResizeEvent
	virtual void on_Move() {};//вызывеатся после выполнения MoveEvent
	virtual void on_MinMaxChage() {};//вызывеатся после выполнения setMinAndMaxTransPoint
	virtual void on_Scale() {};
	virtual void on_Scale_or_MinMaxChange() {};//исключительный случай

	////
	virtual QString Interpret(double val,  bool isVertical,bool isVirtual, QString* out = NULL,QString In = QString(),double start = 0,double end = 0,bool startEndisVirtual = true);//функция интерпритирования цифровых значений для отображения на экране
	int Vert_RoundValue;//тип округдения цифровых значений на шкале по умолчанию = 0 (выкл.)
	int Hor_RoundValue;
	bool EnableInterpretAddLabel;
	QString Vert_InterpretAddLabel;//добавочная запись при интерпритации, добавляется при EnableInterpretAddLabel
	QString Hor_InterpretAddLabel;
	bool EnableKiloMath;//
	bool EnableHorTimerConvertor;
	bool EnableVertTimerConvertor;
	bool InterpretShowFullTime;
	double HorTimerKf;//посути FormatMSec ( val / HorTimerKf )
	double VertTimerKf;//посути FormatMSec ( val / HorTimerKf )

	/////
	double ApplyInterpretSum(double val,bool isVertical) const;
	double DisApplyInterpretSum(double val,bool isVertical) const;
	double VertInterpretSum;//коофициент интерпритации вертикаль
	double HorInterpretSum;//коофициент интерпритации горизонт

	//это вызывает Grid после перерисовки
	bool GridChanged(QPointF Hw,int aligment);

	void Reset();

	double GetValueByPixelPublic(double pixel,bool isVertical);
	double GetPixelByValuePublic(double val,bool isVertical);

	void setFixedScaleList(QList<double> FixedList);

	bool isSomeZoomIsOnMax(bool &X, bool& Y);//вопрос звучит так , "какой нить зуум (по X или Y) приближен максимально или че-ли?"


	str_Collision CheckCollisionForTrans(QPointF trans,QPointF _kf);//проверяет перехлест с краями области для заданного trans и _kf
	double GetTransByValueOnPixel(double Value, double pointOnScreen, bool isVertical);
	double GetPixelByValueForTrans(QPointF tans, QPointF _kf,double val,bool isVertical);
	double GetPixelByValue(double val,bool isVertical);//функции пребразования координат c учетом Trans и инверсии осей
	double GetValueByPixel(double pixel,bool isVertical);

protected:
	virtual void ReDraw () = 0;//вызывается при Move и Resize когда надо перерисовать объект
	QPoint sz;//текущая ширина и высота сцены (окна просмотра)
	QPointF Trans;//точка на сколько пикселей сдвиг по глобальной системе (в пикселях)
	QPointF UserTrans;// -/- пользовательский
	QPointF kf;//коофицент , сколько вешать в пикселях //screenX = x*kf.x() screenY = y*kf.y()
	QPointF MinimumKf;//минимально возможный коофициент, рассчитывается только при ресайзе

	QPointF MaxTransPoint;//ограничители размеров в значениях
	QPointF MinTransPoint;//ограничители размеров

	Q_MG_Style* settings;


	//параметры сетки, меняется по команде вызываеемой из подготовки сетки GridChanged
	double GridTopStep;
	double GridLeftStep;
	double GridRightStep;
	double GridBottomStep;
	QPointF curGridBhw;int curGridAlig;

	//фиксированный скейл
	bool EnableFixedScale;
	QList<double> FixedScaleList;//желательно должен быть отсортирован в порядке увеличения т.е. 1,2,3,3.5,4, однако и без этого работает
	

public slots:
	virtual void getCommand(QString ToObj,QString CommandName,QVariant params) = 0;

	//функуции для тестирования, вызываются из Squish
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
//ТРЕБУЕТ ОТ КЛАССА СЕТКИ Align и BorderHW
class Q_MG_MouseCursor : public Q_MG_BaseClass
{
	Q_OBJECT
public:
	Q_MG_MouseCursor(QGraphicsItem *parent = NULL);
	~Q_MG_MouseCursor() {ClearAllSelections();delete SizeAllCur;};

    virtual void ReDraw () {update();};

	// приниают и возвращают "виртуальные" бубны

	//маркеры
	void			addMarker(double Val,bool isVert);//добавляет маркер в "виртуальных" бубнах
	void			ClearAllMarkers(bool isVert);//очищает все маркеры 
	void			RemoveMarker(int index, bool isVert, bool need_change_signal = true);//также можно удалять при зажатом Ctrl и правой кнопкой мыши клик по маркеру
	QList<double>	getMarkers(bool isVert);//получить список маркеров в "виртуальных" бубнах
	void			SetMarkers(QList<double>,bool isVertical,bool noSignal = false);//в "виртуальных" бубнах
	void			UpdateMarker(int index, double val,bool isVertical);//в "виртуальных" бубнах
	bool			showVMarkers;//показывать ли маркеры
	bool			showHMarkers;//показывать ли маркеры
	bool			mutiMarkerMoove;//перемещать маркеры кучкой
	bool			MarkerGlowEffect;//применять подсветку к маркерам
	bool			showMarkersText;//показывать ли подписи на меркерах
	bool			showMarkersDeff;//показывать ли разницу между маркерами
	bool			showMarkerCentralParam;//показывать ли подписи к центру во время показа разницы
	//////////////////////////////////////////////////////////////////////////

	//выделение
	void			SetSelection(double,double,double,double, bool noSignal = false);//по текущему  CurSelectionType, в виртуальных бубнах
	void			updateCursorPosition(QPointF pos);//обновляет позицию курсора (вызывается из-за контекстного меню, оибо приходит paintEvent а hover еще не пришел) 
	bool			HasSelection() const;//по всем
	bool			HasRectSelection() const;
	bool			HasHorSelection() const;
	bool			HasVertSelection() const;
	void			ClearAllSelections();//очищает всю выборку
	QList<QPointF>	getHVSelected(bool Vert);//получить то что выделенно пользователем в "виртуальных" бубнах
	QList<QRectF>	getRectSelected();//получить то что выделенно пользователем в "виртуальных" бубнах
	void			RemoveLastSelection();//по текущему  CurSelectionType
	void			SetMultySelectEnabled(bool enable);//true, только фллаг возможности работы с multy select, т.е. кнопка Ctrl не будет игнорироваться см. SetButtonAltClickIsSingle
	bool			IsMultySelectEnabled() const;
	void			SetButtonCtrlClickIsSingle(bool is_single = true);//по умолчанию с зажатым Ctrl делается множественное выделение (false), с установкой флага все меняется на оборот, без Ctrl multy с Ctrl single
	bool			IsButtonCtrlClickIsSingle() const;
	bool			EnableByStepSelection;//шаг выделения
	double			stepValX;//шаг выделения
	double			stepValY;//шаг выделения
	bool			ShowSelectText;//Рисовать ли подписи и стрелочки при выделении
	int				CurSelectionType;//тип пользовательского выделения, 0 - недоступна, 1 - горизонтальная ||, 2 - вертикальная =, 3 - свободная []
	//////////////////////////////////////////////////////////////////////////

	//подписи
	void			ClearAllFredomLabels();
	void			ChangeLabelText(QString name,QString new_text, bool immediately_update = false);
	void			ChangeLabelPos(QString name,double new_pos, bool immediately_update = false);
	void			SetFredomLabel(double valX_b,QString text,QString name = QString(),bool immediately_update = false, QColor label_color  = QColor(100,250,250));
	void			SetLabelsUnionText(QString union_text);
	void			SetLabelsUnionShowHint(bool show_hint);
	void			RemoveLabel(QString name, bool immediately_update = false);
	//////////////////////////////////////////////////////////////////////////

	bool	DrawTextDecorateMidLine;//рисовать ли пунктирную линию центра при маркерах или выделении	
	bool	showBigGrafCursor;//рисовать ли большое перекрестие курсора
	bool	showMiniGrafCursor;//рисовать ли малое перекрестие курсора (когда рисуется большое, малое не рисуется)
	bool	showVValues;//показывать значения по которым указатель мыши
	bool	showHValues;
	bool	GlowEffect;//применять эффект подсветки к курсору
	bool	UseMouseCursorJump;//перепрыгивает ли мышь в начало экрана при достижения конца и наоборот
		
	
	//крест
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


	//выделения
	QRectF CurMoveSel;
	bool OnSelection;
	bool selection_deselection_started_;
	bool multi_select_enable_;//вызможен ли множественный выбор
	bool button_ctrl_click_is_single_;
	bool is_deselect_;
	QList<QPointF> VAxisSelList;//хранится в значениях, меньшее всегда первое
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

	//Не реализовано имеется ссылска с 
	QList<SelectionButtonPanel> zoomOnButtonList_VAxisSel;//список кнопок для выборки
	QList<SelectionButtonPanel> zoomOnButtonList_HAxisSel;
	QList<SelectionButtonPanel> zoomOnButtonList_RectSel;
	//////////////////////////////////////////////////////////////////////////

	inline void DrawSelections(QPainter *painter);
	inline void TextDecorate(QPainter *painter,const QRectF& points, const QPointF& value, bool isVertical,int style = 0);
	inline bool FigureIsVisible(const QRectF& points);//видима ли фигура ваще на экране с учетом алиджина, в пикселях

	//маркеры
	QPixmap VerCurPix;
	QPixmap HorCurPix;
	QList<double> HorMark;
	QList<double> VertMark;
	inline void DrawMarkers(QPainter *painter);//рисует маркеры и также проверяет находится ли маркер под курсором и заполняет соответственно UnderMouseMarkerIndex
	int UnderMouseMarkerIndex;//индекс маркера под курсором (тот который тянем если < 0 то ничего не тянем)
	bool UnderMouseMarkerIndexIsVert;//в каком массиве индекс собственно
	bool OnMarkerMoove;
	bool NeedSortHorMark;
	bool NeedSortVertMark;
	QList<double> sortedHorMark;
	QList<double> sortedVertMark;

	//корректировка значений выделений и маркеров по заданной сетке\шагу выделения
	inline double Correct_ValueByStep_or_PixelByGrid(double val,double pixel,bool isVertical);

	//скейл по ср копке мышке
	int screenStartPosScaleX;
	int screenStartPosScaleY;
	QPointF sceneStartPosScale;

	void DrawStrangeCross(QPainter *painter);

	//счетчик перемещения мыши для выова контекстного меню
	qint64 deltaMoveX;
	qint64 deltaMoveY;
	
	//необходимо для COntextMenu чтобы курсор не прыгал
	bool IgnoreNextHoverEvent;

	//свободные подписи на экране
	void DrawFredomLabels(QPainter *painter);
	//новое
	QPointer<NoVisualLabelsManager> labels_manager_;

	//////////////////////////////////////////////////////////////////////////

	DISALLOW_COPY_AND_ASSIGN(Q_MG_MouseCursor);

signals:
	//все сигналы в "реальных" бубнах
	void dragScreen(QPointF dragPoint);//в пикселях на экране
	void dragScreenFinish();//таскание экрана завершено, эмитится если экран был перемещен хотябы на 1 пиксель (по daltaMooveX (Y) )
	void scaleScreen(QPointF dragPoint, QPointF MousePos);//в пикселях на экране
	
	void selectionCleared();
	void selectedPixelChange(double x1, double y1,double x2,double y2);
	void selectionFinished(double x1, double y1,double x2,double y2);
    void selectionFinishedRedLine(double y); ///emit when choose threshold
	void deselectFinished();//эмитится когда deselect чето натворил - определить какие конкретно изменения произошли - проще перечитать все данные
	
	void markerMooved(int index, double value,bool isVertical);
	void markersChanged(QList<double> Mark,bool isVertical);
	
	void DoubleClicked(double,double);
	void StrangeCrossMoved();
	
	void wantContextMenu();
	

public slots:
	void getCommand(QString ToObj,QString CommandName,QVariant params);

};

//ТРЕБУЕТ ОТ КЛАССА СЕТКИ Align и BorderHW
class Q_MG_SelectionAreas : public Q_MG_BaseClass
{
	Q_OBJECT
public:
	Q_MG_SelectionAreas(QGraphicsItem *parent = NULL);
	~Q_MG_SelectionAreas() {/*ClearAllSelections();delete SizeAllCur;*/};

	virtual void ReDraw () {update();};

	void			SetSelection(double,double,double,double, bool noSignal = false);//по текущему  CurSelectionType, в виртуальных бубнах
//	bool			HasSelection() const;//по всем
//	bool			HasRectSelection() const;
//	bool			HasHorSelection() const;
//	bool			HasVertSelection() const;

	void			ClearAllSelections();//очищает всю выборку


private:
	QList<QPointF> HAxisSelList;

	inline void DrawSelections(QPainter *painter);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

	QRectF boundingRect() const {return QRectF();};
	virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event ){};
	virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ){};
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event ){};
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ){};
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event ){};
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event ){};
	virtual void on_Resize() {};//вызывеатся после выполнения ResizeEvent
	virtual void on_Move() {};//вызывеатся после выполнения MoveEvent
	virtual void on_MinMaxChage() {};//вызывеатся после выполнения setMinAndMaxTransPoint
	virtual void on_Scale() {};
	virtual void on_Scale_or_MinMaxChange() {};//исключительный случай

	virtual QString Interpret(double val,  bool isVertical,bool isVirtual, QString* out = NULL,QString In = QString(),double start = 0,double end = 0,bool startEndisVirtual = true){ return QString();};//функция интерпритирования цифровых значений для отображения на экране

public slots:
	virtual void getCommand(QString ToObj,QString CommandName,QVariant params) {};


signals:
	//все сигналы в "реальных" бубнах

	void selectionCleared();
	void selectionFinished(double x1, double y1,double x2,double y2);

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
	
	//все что ниже - в пиксеях здесь нет никаких значений тупо пиксели где рисовать
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
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );//нажали чисто на бордюр, далее не пускает
	virtual QPointF getBorderHightWidth();//вовращает ширину и высоту отрисованного бордюра, вызывается из Q_MG_MouseCursor

	
	int flag_GridAligment;// 0 - LT 1 - LB 2 - RT 3 - RB // требуется для Q_MG_MouseCursor		0	2
																					//			1	3

	bool flag_DrawVAxis;//рисовать ли ось в целом
	bool flag_DrawHAxis;

	bool DrawVSubDiv;//рисовать ли доп. деления
	bool DrawHSubDiv;

	bool DrawVBaseLine;
	bool DrawHBaseLine;

	bool DrawVGridBorders;//рисовать бордюры со значениями
	bool DrawHGridBorders;

	double HightLightVVal;//какие значения подсвечивать по умолчанию подсвечивается 0 0, если значения нет на сетке оно не подсвечивается
	double HightLightHVal;
	
	QString LabelVAxis;//подписи к шакалам запрошенные пользователем (оригиналы)
	QString LabelHAxis;

	double HGridNumberStep;//базовый шаг шкалы, расчет идет maxTextWidth += HGridNumberStep, результат минимум 50
	double VGridNumberStep;//базовый шаг шкалы = 25 идет как есть.

	void ReDraw ()
	{
		int errCode = MarkIntervalAndCalcBorderSizes();
		if (errCode == 1)
		{
			sendCommand("mg_InfoWidget","PrintText",QObject::tr("Значения графика выходят за границы отображаемой области"));
		}

		if (errCode == 2)
		{
			return;
		}

		update();
	};

	
	int MarkIntervalAndCalcBorderSizes();//возвращает код ошибки 0-все нормально, 1 - ошибка при расчетах интервалов, 2-размеры окна очень малы, вообщем рисовать сетку ненадо
	QVector<double> IntervalRetValV;
	QVector<double> IntervalRetValH;
	QVector<double> SubDivRetValV;
	QVector<double> SubDivRetValH;

private:

	QString printLabelVAxis;//подписи к шакалам реальни печатаемые
	QString printLabelHAxis;//подписи к шакалам реальни печатаемые

	qreal BorderW;//ширина вертикального бордюра, определяется во время рисования
	qreal BorderH;//высота -/-
		
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

	QVector<InfoWidgetData> UserData;//список выводимых значений пользователя

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

	//прогресс бар
	double ProgressBarValue;
	bool ShowProgressBar;
	bool NeedShowProgress();

	DISALLOW_COPY_AND_ASSIGN(Q_MG_InfoWidget);

public slots:

	void setUserText(QString str,bool isHtml = false);//установка пользовательского текста флаг PrintTextMode = 1 ставится автоматически
	void setPrintTextMode(int _PrintTextMode);//вид выводимых занчений: 0 - стандарт,1 - пользовательские
	void getCommand(QString ToObj,QString CommandName,QVariant params);

	void SetProgress(double);//некий прогресс от 0 до 100, отображать в инфо окне, если не 0 или 100 показать
	double GetProgress();//возвращает текущий прогресс
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
            if (thread_status_ == 2) return;// чтобы флаг убить не снялся флагом паузы
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
	void	Start_Wake();		//будит WaitContion, если поток спал,снимает флаг паузы
	void	FinishHim_wait();	//возвращается только при полной остановке
	bool	NeedBreaked();		//Возвращает true если поставлен один из флагов pause или finish
	bool UseFilterIfOn(Q_MG_MaterialElement* el)
	{	if (useFilter_) return filter_func_(el);else return true;	}

private:
	//упрпавление
	void	run();
	std::function<bool(Q_MG_MaterialElement*)>  filter_func_;
	//void	setPauseFlag();		//ставит "по правилам" флаг паузы
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
	void ReDraw() { }; //на слое материала обновление ненужно оно обновляется само при изменении позиции

	double Hor_kf_inc;
	double Vert_kf_inc;

	virtual void SelfClear() {};//когда дохуя элементов на сцене, и как правило 90% из них уже ненадо, надо почиститься, случается при долгом таскании
	virtual void ExtMaterialReInit() = 0;//когда происходит любой скейл
	virtual bool ExtViewPortChange(Q_MG_MaterialElement* NewEl,unsigned __int64 elXpos,unsigned __int64 elYpos) = 0;
	virtual bool PrepareForViewPortChange(qreal key_kf_x,qreal key_kf_y,double StartPixelX, double EndPixelX, double StartPixelY,double EndPixelY) = 0;
	virtual void ExtAfterViewPortChange() = 0;
	
	bool ElementIsOnScreen(const Q_MG_MaterialElement* el);
	void GetViewPort(double& value_startX, double& value_startY,double& value_endX, double& value_endY);

	int MaterialID;//выдается единожды при добавленни в список, пока только для определения каким цветом рисовать

signals:
	void ViewPortChanged(double value_startX, double value_startY,double value_endX, double value_endY);//в реальных значениях (надо виртить)

protected:

	void AllMaterialReInit();//автоматически вызывается при полном изменении сцены, вслучаях: изменение масштаба, установка новых данных
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
	void RemoveAllOffScreenEl();//очистить все элементы находящихся за экраном, включая и список рендера
	
	Storage<Q_MG_MaterialElement*> ElList;
	int ElementH;
	int ElementW;
	
	//для многопоточности
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


