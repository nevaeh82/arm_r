#ifndef Q_MG_SPECTRUMINTERFACE_H
#define Q_MG_SPECTRUMINTERFACE_H

#include "../../../modules/gui/multi_graphics/multi_graphics.h"
#include <QWidget>

class SpectrumMaterial;
class QVBoxLayout;

class Q_MG_SpectrumInterface : public QWidget
{
	Q_OBJECT


public:

	Q_MG_SpectrumInterface(QWidget *parent = NULL, QString wallPaperFileName = ":/Q_MultiGraphics/Resources/WallPaperSpec.png");//������� ������ � ������� ����� �������� ������ ����� ������ Setup

	~Q_MG_SpectrumInterface();

	//функция активации (создания) спектра
    bool Setup(bool is_complex, double bandwidth_hz, QString vertical_dial_label, const float* spectrum_1, size_t items_count_1, const float* spectrum_2 = NULL, size_t items_count_2 = 0, bool inline_memory_1 = false, bool inline_memory_2 = false, float minv_ = 0.0, float maxv_ = 0.0);
	//выставить новый спектр без пересоздания объектов спектра, не меняет viewPort 
	//функция должна быть вызвана после функции Setup в которой инициализуется движок
	//вызов до Setup возвращает false и нихрена не делает, размерность массива (items_count) не должна быть меньше выставленного в Setup,
	//NOTE: проверить что спектрум инициализирован можно функцией isActivated()
    bool PermanentDataSetup(const float* spectrum_1,const float* spectrum_2 = NULL, float minv_ = 0, float maxv_ = 0);
	void Reset();

	//������ � viewPort
	void	SetAlign(int inc_alig)	{	align = inc_alig;	}//0 - ��������, 1 - ������� ������ ������ ����������, 2 - ������ ������ ������ ����������, 3 - ��� ������� ������. ������ ����������� ����� ���������� ������ Setup.

	void	SetHorizontalLabel(QString label);
	void	SetZeroFrequencyHz(double valX);//аналог SetVirtualOffsetX
	double	ZeroFrequencyHz() const { return MainGraf->Grid_Layer->HorInterpretSum;}
	void	SetVirtualOffsetY(double valY);
	void	SetVirtualOffsetXY(double valX,double valY);//функция не перегружена, она быстрее, если надо ставить сразу 2 значения
	void	GetCurrentViewport( double &start_hz, double &start_Y, double &end_hz, double &end_Y);
	void	SetScaleStepByHorizontal(double hor_wheel_step);//установка шага зуума
	void	SetScaleStepByVertical(double vert_wheel_step);
	void	ZoomInX();//увеличивает. уменьшает текущий Zoom на 1 шаг 
	void	ZoomOutX();
	void	ZoomInY();//увеличивает. уменьшает текущий Zoom на 1 шаг 
	void	ZoomOutY();
	void	ZoomToSelection();

	void	SetViewport(double LeftVal,double TopVal,double RightVal,double BottomVal);//����������������
	void	ZoomOutFull();//������� ���� ������ �� ����� (������������ ZoomOut)
	void	SetZoomOutMaxOnDataSet(bool enabled = true) {		ZoomOutMaxOnDataSet = enabled;		}//�� ���������=false �������������� �����  ZoomOutFull() ����� Setup ��� SetupNewData
	void	SetAutoscaleY(bool enabled = true) {	autoscaleY_	= enabled;	}

	void	SetScaleY_1to1();
	void	MoveScreenByPixels(QPoint pixels,bool emit_viewPortChangeSignal = true);
    void    SetSpectrumVisible(int spectr, bool visible);
	//////////////////////////


	//сетка и бордюр
	bool	ShowHorizGrid();
	void	SetShowHorizGrid(bool value = true);		//скрывается разметка (сетка)
	bool	ShowVerticalGrid();
	void	SetShowVerticalGrid(bool value = true);
	bool	ShowLevelBar();							
	void	SetShowLevelBar(bool value = true);		//показывать ли верт. линейку, выключит и сетку и подписи указателя
	bool	ShowHorizontalBar();			
	void	SetShowHorizontalBar(bool value = true);	//показывать ли горизонт. линейку, выключит и сетку и подписи указателя
	bool	ShowHorGridBorder();
	void	SetShowHorGridBorder(bool value = true);	//подписи и разметка остаются скрывается только бордюр
	bool	ShowVertGridBorder();
	void	SetShowVertGridBorder(bool value = true);	//подписи и разметка остаются скрывается только бордюр
	/////////////////////////

	
	//маркеры
	int		MarkersCount() const;
	bool	MarkersVisible() const;
	QList<double> GetMarkers() const;
	bool	MoveAll() const;
	void	ShowMarkers();
	void	HideMarkers();
	void	SetMoveAllMarkers();
	void	SetMoveOneMarker();
	void	SetMarkers(QVector<double>,bool,bool noSignal = false);
	void	UpdateMarker(int, double,bool);
	void	ClearAllMarkers();
	////////////////////////////////////

	
	//ОКНО СООБЩЕНИЙ
	void	SetUserTextToInfo(QString txt, bool IsHtmlFormat = false);//установка пользовательского текста в информационное окно
	void	SetInfoWidgetPrintTextMode(int _PrintTextMode);//вид выводимых занчений: 0 - стандарт, если пусто то пользовательские, а если и там пусто то скрыть, 1 - всегда только пользовательские(устанавливается автоматом при SetUserTextToInfo)
	void	SetVisibleInfoWidget(bool isVisible = true);//установить видимость окна с сообщениями
	void	SetShowOptionsForStarndartLabels(QString name = "", bool showparam = false);//установить видимость надписей в окне с сообщениями, при передпче пустого имени - скроется вся группа
	/////////////////////////

	
	//свободные подписи
	void	ClearAllLabels();
	void	SetLabel(double valX_b, QString text, QString name = QString(), bool immediately_update = false, QColor label_color = QColor(100,250,250));
	void	ChangeLabelText(QString name, QString new_text, bool immediately_update = false);
	void	ChangeLabelPos(QString name, double new_pos, bool immediately_update = false);
	void	SetLabelsUnionsText(QString union_text);
	void	SetLabelsUnionsShowHint(bool show_hint);
	void	RemoveLabel(QString name, bool immediately_update = false);
	///////////////////////

		
	//страшный крест
	double	CrossHz() const;	
	double	CrossPix() const;
	void	SetCross(double, double);
	void	SetShowCross(bool value);
	void	SetShowCrossHorizontalText(bool visible);
	void	SetShowCrossVerticalText(bool visible);
	//////////////////////



	//выделение
	void				SetMultySelectEnabled(bool enable);
	void				SetButtonCtrlClickIsSingle(bool is_single = true);//по умолчанию с зажатым Ctrl делается множественное выделение (false), с установкой флага все меняется на оборот, без Ctrl multy с Ctrl single
	double				SelStartHz() const;
	double				SelEndHz() const;
	bool				HasSelection() const; // == IsSomethingSelected() , просто перегружена
	bool				IsSomethingSelected() const;
	bool				HasRectSelection() const;
	bool				HasHorSelection() const;
	bool				HasVertSelection() const;
	void				RemoveLastSelection();
	void				ClearAllSelections(bool noSignal = false);
	QList<QPointF>		GetAllHorSelections() const;
	QList<QPointF>		GetAllVertSelections() const;
	QList<QRectF> 		GetAllRectSelections() const;
	void				SetSelection( double x1,double y1,double x2,double y2, bool noSignal = false);
	///////////////////////////////

	/// my
	void				SetDetectedAreas(int mode, double x1,double y1,double x2,double y2, bool noSignal = false);
	void				ClearAllDetectedAreas();
	//связь компонентов
	void setRelationStatus(bool enable) {	haveRelations_ = enable;}//не надо вызывать если использовать класс ComponentsRelation, сам все сделает необходимое
	

	//contextMenu
	QMenu*	contextMenu() { return MainGraf->contextMenu();}
	void	setContextMenu(QMenu* menu) { contextMenu_ = menu;MainGraf->setContextMenu(contextMenu_);}
	////////////////////////////



	//разное
	bool isActivated() { return spectrumActivated_;}
	double BandWidth()	const	{ return bandwidth_hz_; }
	bool FilledSpectrum() const;
	Q_MultiGraphics* getBaseEngineClass() { return MainGraf;}


    /// set min value for horizontal axis
    void SetStartAndFinishHz(QPointF start, QPointF finish);
	
public slots:
		
	void SetFillSpectrum();//TODO:залить
	void SetLinesOnlySpectrum();//TODO:отлить

	void SetProgress(double);//некий прогресс от 0 до 100, отображать в инфо окне, если не 0 или 100 показать
	bool ShowProgress();
	void SetShowProgress(bool value);

    ///change selection type
    void slotCurSelectionChanged(int);
	
signals:
	void HorizontalSelectionChanged(double start, double end);
	void VerticalSelectionChanged(double start, double end);
	void SelectionCleared();
	void SelectionChangedTo(double x1, double y1, double x2, double y2);
	void SelectionFinished(double x1, double y1, double x2, double y2);
    void selectionFinishedRedLine(double y); ///emit when choose threshold
	void DeselectFinished();
	
	void CrossMoved();//крест двигается

	void viewPortChanged( double start_hz, double start_Y, double end_hz, double end_Y);//изменен ViewPort

	void MarkerMoved(int, double,bool);//индекс маркера и ГЦ (если двигается любой маркер и к каждому)
	void MarkersChanged(QVector<double>,bool);//состояние маркеров было изменено, в любом случае, (передвинули , почистили, добавили)

	void DoubleClicked(double, double);//ГЦ и ДБ

	void SpectrumActivated();//эмитится когда спектр стал активным

	void plotReady();


private:
	SpectrumMaterial* material_1;//поддерживает пока что только 1 материал, в дальнейшем можно сделать либо списком либо тупо 2 (по мере постановки задачи и выделения времени)
	SpectrumMaterial* material_2;
	
	Q_MultiGraphics* MainGraf;

	QVBoxLayout* MainLayout;

	bool skip_signal_on_selection_change_;
	bool skip_signal_on_selection_clear_;

	bool autoscaleY_;
	bool ZoomOutMaxOnDataSet;
	bool spectrumActivated_;
	int align;

	bool is_complex_;//для передачи в SetNewData(QByteArray);
	double bandwidth_hz_;
	void InitMainGraf();

	bool haveRelations_;
	bool emitSignalOnProgrammChangeOffViewPort_;//отладочная, не используется пользователями в нормальном режиме всегда = false
	bool wasProgrammChangeOffViewPort_;

	QGraphicsPixmapItem wallPaper;
	unsigned int TimerId;
	void timerEvent(QTimerEvent * ev);

	void InitSomeHiddenProperties();

	QPointer<QMenu> contextMenu_;

	DISALLOW_COPY_AND_ASSIGN(Q_MG_SpectrumInterface);

	QString HorLabel;

	bool eventFilter(QObject*, QEvent*);//для установки wasProgrammChangeOffViewPort_ во время Resize в отношениях
	

private slots:
	void on_selectedPixelChangeSpectrum(double x1, double y1, double x2, double y2);//принимает реальные бубны
	void on_selectionCleared();
	void on_selectionFinished(double x1, double y1, double x2, double y2);
    void on_selectionFinishedRedLine(double y);

	void on_markerMoved(int, double,bool);
	void on_markersChanged(QList<double>,bool);
	
	void on_doubleClicked(double, double);
	void on_ViewPortChanged(double value_startX, double value_startY,double value_endX, double value_endY);


private:
    bool _material2_visible;

};

#endif // Q_MG_SPECTRUMINTERFACE_H
