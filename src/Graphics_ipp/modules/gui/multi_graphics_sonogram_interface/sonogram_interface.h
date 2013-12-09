#ifndef SONOGRAM_INTERFACE_H
#define SONOGRAM_INTERFACE_H

#include <QGraphicsPixmapItem>
#include "../../../modules/gui/multi_graphics/multi_graphics.h"
#include "sonogram_material.h"
#include "../../../modules/filesystem/filesystem.h"
#include <QVBoxLayout>
#include <QWidget>

class QFile;
class QSettings;




class SonogramInterface : public QWidget
{
	Q_OBJECT

public:
    typedef SonogramMaterial::FileType  FileType;

	SonogramInterface(QWidget *parent = NULL,QSettings* settings = NULL);
	~SonogramInterface();

    bool Setup(const FileType&, int fft_Order, QByteArray color_level_data = QByteArray());
	void Reset();

	//��������� ������ ������� ������� �� �������������� �����
	QByteArray GetColorLevels();

	//������� ���� ������ �� ����� (������������ ZoomOut)
	void ZoomOutFull();
	void SetZoomOutMaxOnDataSet(bool enabled = true) {		ZoomOutMaxOnDataSet = enabled;		};

	void SetVirtualOffsetX(double valX);
	
	//////////////////////
	//���� ���������
	//////////////////////
	//��������� ����������������� ������ � �������������� ����
	void SetUsetTextToInfo(QString txt, bool IsHtmlFormat = false);
	//��� ��������� ��������: 0 - ��������, ���� ����� �� ����������������, � ���� � ��� ����� �� ������, 1 - ������ ������ ����������������(��������������� ��������� ��� SetUsetTextToInfo)
	void SetInfoWidgetPrintTextMode(int _PrintTextMode);
	//���������� ��������� ���� � �����������
	void SetVisibleInfoWidget(bool isVisible = true);
	//���������� ��������� �������� � ���� � �����������, ��� �������� ������� ����� - �������� ��� ������
	void SetShowOptionsForStarndartLabels(QString name = "", bool showparam = false);
	/////////////////////////

	//����� �����������
	void setRelationStatus(bool enable) {	haveRelations_ = enable;}//�� ���� �������� ���� ������������ ����� ComponentsRelation, ��� ��� ������ �����������


	//ContextMenu
	QMenu* contextMenu() { return MainGraf->contextMenu();}
	void  setContextMenu(QMenu* menu) {contextMenu_ = menu;MainGraf->setContextMenu(contextMenu_);}

	
	void	GetCurrentViewport(double &start_hz, double &start_smpl, double &end_hz, double &end_smpl);
	double  GetCurrentBandwidth_hz()	{	return bandwidth_hz_;	}
	


	//��������� (��� � ������)
	void				GetSelection(double &start_x, double &start_y,  double &end_x, double &end_y) const; //������ ������ ���������� (�� �������� ���� ��������� � ������ � ������), ������������ �� ������������, � ������ ������������� ��� ������������� ��������� ��� �� ������� �� ������������� ��������� (��������� ���) ������������ 0
	bool				IsSomethingSelected() const;//  ���������� true ���� ���-���� ��������
	int					curSelectionType();// 0 - ����������, 1 - �������������� ||, 2 - ������������ =, 3 - ��������� []
	void				SetCurSelectionType(int type);// 0 - ����������, 1 - �������������� ||, 2 - ������������ =, 3 - ��������� []
	bool				HasRectSelection() const;
	bool				HasHorSelection() const;
	bool				HasVertSelection() const;
	void				RemoveLastSelection();
	QList<QPointF>		GetAllHorSelections() const;
	QList<QPointF>		GetAllVertSelections() const;
	QList<QRectF> 		GetAllRectSelections() const;
	void				SetMultySelectEnabled(bool enable);
	void				SetButtonCtrlClickIsSingle(bool is_single = true);//�� ��������� � ������� Ctrl �������� ������������� ��������� (false), � ���������� ����� ��� �������� �� ������, ��� Ctrl multy � Ctrl single
	//////////////////////////////////////////////////////////////////////////



	Q_MultiGraphics* getBaseEngineClass() { return MainGraf;};
		
	void	MoveScreenByPixels(QPoint pixels,bool emit_viewPortChangeSignal = true);

public slots:
	//������ ������������ ��������� viewPorta
	void ZoomToSelection();
	void SetViewport( double start_hz, double start_smpl, double end_hz, double end_smpl);//X � "�����������" ������, Y � "��������"
	
	//���������
	void ClearAllSelections(bool noSignal = false);
	void SetSelection( double x1,double y1,double x2,double y2, bool noSignal = false);
	
	
	//�������
	void	ShowMarkers();
	void	HideMarkers();
	void	SetMoveAllMarkers();
	void	SetMoveOneMarker();
	void	SetMarkers(QVector<double>,bool,bool noSignal = false);
	void	UpdateMarker(int, double,bool);
	void	ClearAllMarkers();
	//////////////////////////////////////////////////////////////////////////

signals:
	//������ �������� ���������� ��������� 
	void SelectionCleared();//���������� ���������
	void DeselectFinished();
	void HorizontalSelectionChanged(double start, double end);//� ������ ������ ���������� ��������� (������ ��������������)
	void VerticalSelectionChanged(double start, double end);//� ������ ������ ���������� ��������� (������ ������������)
	void SelectionChangedTo(double x1, double y1, double x2, double y2);//� ������ ������ ���������� ��������� (�������� ���� �������������, ������������ � ����������) , ��� ������� Y � ��������
	void selectionFinished(double x1, double y1, double x2, double y2);//��� ������� Y � ��������

	void MarkerMoved(int, double,bool);//������ ������� � �� (���� ��������� ����� ������ � � �������)
	void MarkersChanged(QVector<double>,bool);//��������� �������� ���� ��������, � ����� ������, (����������� , ���������, ��������)

	void viewPortChanged( double start_hz, double start_smpl, double end_hz, double end_smpl);//������� ViewPort
	void dragScreenFinish();//�������� ������ ���������, �������� ���� ����� ��� ��������� ������ �� 1 �������, ����� ���� ����� ���� ������� ContextMenu, ���� ����� ��� ��������� �� �� ����� 2 �������� � ����� ����������

	void SonogramScreenSpectrumsChanged(FFTLine_32f mean_spectrum, FFTLine max_spectrum);

	
	//������ �� ���������� �������� �������, ����� ������ � �������� ������� ����������� ��� ���������� � ����������� �������� � �����
	void CalcPallateFinished(QByteArray color_level_data);

//////////////////////////////////////////////////////////////////////////
// PRIVATE
//////////////////////////////////////////////////////////////////////////
private:
	SIZE_T				maximum_ram_of_system_;
	SonogramMaterial * material_1;//������������ ���� ��� ������ 1 ��������, � ���������� ����� ������� ���� ������� ���� ���� 2 (�� ���� ���������� ������ � ��������� �������)
	SonogramGraphicsRenderThread* sonorenderThread;
	double bandwidth_hz_;

	Q_MultiGraphics* MainGraf;

	bool ZoomOutMaxOnDataSet;
	bool SonogramActivated;

	bool emitSignalOnProgrammChangeOffViewPort_;//����������, �� ������������ �������������� � ���������� ������ ������ = false
	bool wasProgrammChangeOffViewPort_;

	void InitMainGraf();

	QSettings* settings_;//��������� ��� ������������� ��� ������� Reset()
	QVBoxLayout* MainLayout;//  -//- 

	QGraphicsPixmapItem wallPaper;
	unsigned int TimerId;
	void timerEvent(QTimerEvent * ev);

	bool skip_signal_on_selection_clear_;
	bool skip_signal_on_selection_change_;

	bool haveRelations_;

	QPointer<QMenu> contextMenu_;


	bool eventFilter( QObject* obj, QEvent* ev);
	

	DISALLOW_COPY_AND_ASSIGN(SonogramInterface);

private slots:
	void on_selectedPixelChangeSonogram(double x1, double y1, double x2, double y2);
	void on_selectionFinished(double x1, double y1, double x2, double y2);
	void on_ViewPortChanged(double value_startX, double value_startY,double value_endX, double value_endY);
	void on_selectionCleared();

	
	
	void on_markerMoved(int, double,bool);
	void on_markersChanged(QList<double>,bool);
};

#endif // SONOGRAM_INTERFACE_H
