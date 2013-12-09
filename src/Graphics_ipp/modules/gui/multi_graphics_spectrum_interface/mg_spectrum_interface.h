#ifndef Q_MG_SPECTRUMINTERFACE_H
#define Q_MG_SPECTRUMINTERFACE_H

#include "../../../modules/gui/multi_graphics/multi_graphics.h"
#include <QWidget>

class QSettings;
class SpectrumMaterial;
class QVBoxLayout;

class Q_MG_SpectrumInterface : public QWidget
{
	Q_OBJECT


public:
	Q_MG_SpectrumInterface(QWidget *parent = NULL, QSettings* settings = NULL, QString wallPaperFileName = ":/Q_MultiGraphics/Resources/WallPaperSpec.png");//������� ������ � ������� ����� �������� ������ ����� ������ Setup
	~Q_MG_SpectrumInterface();

	//������� ��������� (��������) �������
    bool Setup(bool is_complex, double bandwidth_hz, QString vertical_dial_label, const float* spectrum_1, size_t items_count_1, const float* spectrum_2 = NULL, size_t items_count_2 = 0, bool inline_memory_1 = false, bool inline_memory_2 = false, float minv_ = 0.0, float maxv_ = 0.0);
	//��������� ����� ������ ��� ������������ �������� �������, �� ������ viewPort 
	//������� ������ ���� ������� ����� ������� Setup � ������� �������������� ������
	//����� �� Setup ���������� false � ������� �� ������, ����������� ������� (items_count) �� ������ ���� ������ ������������� � Setup,
	//NOTE: ��������� ��� �������� ��������������� ����� �������� isActivated()
    bool PermanentDataSetup(const float* spectrum_1,const float* spectrum_2 = NULL, float minv_ = 0, float maxv_ = 0);
	void Reset();

	
	//������ � viewPort
	void	SetAlign(int inc_alig)	{	align = inc_alig;	};//0 - ��������, 1 - ������� ������ ������ ����������, 2 - ������ ������ ������ ����������, 3 - ��� ������� ������. ������ ����������� ����� ���������� ������ Setup.
	void	SetHorizontalLabel(QString label);
	void	SetZeroFrequencyHz(double valX);//������ SetVirtualOffsetX
	double	ZeroFrequencyHz() const { return MainGraf->Grid_Layer->HorInterpretSum;}
	void	SetVirtualOffsetY(double valY);
	void	SetVirtualOffsetXY(double valX,double valY);//������� �� �����������, ��� �������, ���� ���� ������� ����� 2 ��������
	void	GetCurrentViewport( double &start_hz, double &start_Y, double &end_hz, double &end_Y);
	void	SetScaleStepByHorizontal(double hor_wheel_step);//��������� ���� �����
	void	SetScaleStepByVertical(double vert_wheel_step);
	void	ZoomInX();//�����������. ��������� ������� Zoom �� 1 ��� 
	void	ZoomOutX();
	void	ZoomInY();//�����������. ��������� ������� Zoom �� 1 ��� 
	void	ZoomOutY();
	void	ZoomToSelection();
	void	SetViewport(double LeftVal,double TopVal,double RightVal,double BottomVal);//����������������
	void	ZoomOutFull();//������� ���� ������ �� ����� (������������ ZoomOut)
	void	SetZoomOutMaxOnDataSet(bool enabled = true) {		ZoomOutMaxOnDataSet = enabled;		};//�� ���������=false �������������� �����  ZoomOutFull() ����� Setup ��� SetupNewData
	void	SetAutoscaleY(bool enabled = true) {	autoscaleY_	= enabled;	};
	void	SetScaleY_1to1();
	void	MoveScreenByPixels(QPoint pixels,bool emit_viewPortChangeSignal = true);
    void    SetSpectrumVisible(int spectr, bool visible);
	//////////////////////////


	//����� � ������
	bool	ShowHorizGrid();
	void	SetShowHorizGrid(bool value = true);		//���������� �������� (�����)
	bool	ShowVerticalGrid();
	void	SetShowVerticalGrid(bool value = true);
	bool	ShowLevelBar();							
	void	SetShowLevelBar(bool value = true);		//���������� �� ����. �������, �������� � ����� � ������� ���������
	bool	ShowHorizontalBar();			
	void	SetShowHorizontalBar(bool value = true);	//���������� �� ��������. �������, �������� � ����� � ������� ���������
	bool	ShowHorGridBorder();
	void	SetShowHorGridBorder(bool value = true);	//������� � �������� �������� ���������� ������ ������
	bool	ShowVertGridBorder();
	void	SetShowVertGridBorder(bool value = true);	//������� � �������� �������� ���������� ������ ������
	/////////////////////////

	
	//�������
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

	
	//���� ���������
	void	SetUserTextToInfo(QString txt, bool IsHtmlFormat = false);//��������� ����������������� ������ � �������������� ����
	void	SetInfoWidgetPrintTextMode(int _PrintTextMode);//��� ��������� ��������: 0 - ��������, ���� ����� �� ����������������, � ���� � ��� ����� �� ������, 1 - ������ ������ ����������������(��������������� ��������� ��� SetUserTextToInfo)
	void	SetVisibleInfoWidget(bool isVisible = true);//���������� ��������� ���� � �����������
	void	SetShowOptionsForStarndartLabels(QString name = "", bool showparam = false);//���������� ��������� �������� � ���� � �����������, ��� �������� ������� ����� - �������� ��� ������
	/////////////////////////

	
	//��������� �������
	void	ClearAllLabels();
	void	SetLabel(double valX_b, QString text, QString name = QString(), bool immediately_update = false, QColor label_color = QColor(100,250,250));
	void	ChangeLabelText(QString name, QString new_text, bool immediately_update = false);
	void	ChangeLabelPos(QString name, double new_pos, bool immediately_update = false);
	void	SetLabelsUnionsText(QString union_text);
	void	SetLabelsUnionsShowHint(bool show_hint);
	void	RemoveLabel(QString name, bool immediately_update = false);
	///////////////////////

		
	//�������� �����
	double	CrossHz() const;	
	double	CrossPix() const;
	void	SetCross(double, double);
	void	SetShowCross(bool value);
	void	SetShowCrossHorizontalText(bool visible);
	void	SetShowCrossVerticalText(bool visible);
	//////////////////////



	//���������
	void				SetMultySelectEnabled(bool enable);
	void				SetButtonCtrlClickIsSingle(bool is_single = true);//�� ��������� � ������� Ctrl �������� ������������� ��������� (false), � ���������� ����� ��� �������� �� ������, ��� Ctrl multy � Ctrl single
	double				SelStartHz() const;
	double				SelEndHz() const;
	bool				HasSelection() const; // == IsSomethingSelected() , ������ �����������
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

	
	//����� �����������
	void setRelationStatus(bool enable) {	haveRelations_ = enable;}//�� ���� �������� ���� ������������ ����� ComponentsRelation, ��� ��� ������� �����������
	

	//contextMenu
	QMenu*	contextMenu() { return MainGraf->contextMenu();}
	void	setContextMenu(QMenu* menu) { contextMenu_ = menu;MainGraf->setContextMenu(contextMenu_);}
	////////////////////////////



	//������
	bool isActivated() { return spectrumActivated_;}
	double BandWidth()	const	{ return bandwidth_hz_; }
	bool FilledSpectrum() const;
	Q_MultiGraphics* getBaseEngineClass() { return MainGraf;};


    /// set min value for horizontal axis
    void SetStartAndFinishHz(QPointF start, QPointF finish);
	
public slots:
		
	void SetFillSpectrum();//TODO:������
	void SetLinesOnlySpectrum();//TODO:������

	void SetProgress(double);//����� �������� �� 0 �� 100, ���������� � ���� ����, ���� �� 0 ��� 100 ��������
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
	
	void CrossMoved();//����� ���������

	void viewPortChanged( double start_hz, double start_Y, double end_hz, double end_Y);//������� ViewPort

	void MarkerMoved(int, double,bool);//������ ������� � �� (���� ��������� ����� ������ � � �������)
	void MarkersChanged(QVector<double>,bool);//��������� �������� ���� ��������, � ����� ������, (����������� , ���������, ��������)

	void DoubleClicked(double, double);//�� � ��

	void SpectrumActivated();//�������� ����� ������ ���� ��������

	





private:
	SpectrumMaterial* material_1;//������������ ���� ��� ������ 1 ��������, � ���������� ����� ������� ���� ������� ���� ���� 2 (�� ���� ���������� ������ � ��������� �������)
	SpectrumMaterial* material_2;
	
	Q_MultiGraphics* MainGraf;


	QSettings* settings;
	QVBoxLayout* MainLayout;

	bool skip_signal_on_selection_change_;
	bool skip_signal_on_selection_clear_;

	bool autoscaleY_;
	bool ZoomOutMaxOnDataSet;
	bool spectrumActivated_;
	int align;

	bool is_complex_;//��� �������� � SetNewData(QByteArray);
	double bandwidth_hz_;
	void InitMainGraf();

	bool haveRelations_;
	bool emitSignalOnProgrammChangeOffViewPort_;//����������, �� ������������ �������������� � ���������� ������ ������ = false
	bool wasProgrammChangeOffViewPort_;

	QGraphicsPixmapItem wallPaper;
	unsigned int TimerId;
	void timerEvent(QTimerEvent * ev);

	void InitSomeHiddenProperties();

	QPointer<QMenu> contextMenu_;

	DISALLOW_COPY_AND_ASSIGN(Q_MG_SpectrumInterface);

	QString HorLabel;

	bool eventFilter(QObject*, QEvent*);//��� ��������� wasProgrammChangeOffViewPort_ �� ����� Resize � ����������
	

private slots:
	void on_selectedPixelChangeSpectrum(double x1, double y1, double x2, double y2);//��������� �������� �����
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
