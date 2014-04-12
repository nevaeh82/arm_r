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

	//получение данных уровней палитры по установленному файлу
	QByteArray GetColorLevels();

	//скейлит весь график на экран (максимальный ZoomOut)
	void ZoomOutFull();
	void SetZoomOutMaxOnDataSet(bool enabled = true) {		ZoomOutMaxOnDataSet = enabled;		};

	void SetVirtualOffsetX(double valX);
	
	//////////////////////
	//ОКНО СООБЩЕНИЙ
	//////////////////////
	//установка пользовательского текста в информационное окно
	void SetUsetTextToInfo(QString txt, bool IsHtmlFormat = false);
	//вид выводимых занчений: 0 - стандарт, если пусто то пользовательские, а если и там пусто то скрыть, 1 - всегда только пользовательские(устанавливается автоматом при SetUsetTextToInfo)
	void SetInfoWidgetPrintTextMode(int _PrintTextMode);
	//установить видимость окна с сообщениями
	void SetVisibleInfoWidget(bool isVisible = true);
	//установить видимость надписей в окне с сообщениями, при передаче пустого имени - скроется вся группа
	void SetShowOptionsForStarndartLabels(QString name = "", bool showparam = false);
	/////////////////////////

	//связь компонентов
	void setRelationStatus(bool enable) {	haveRelations_ = enable;}//не надо вызывать если использовать класс ComponentsRelation, сам все сделае необходимое


	//ContextMenu
	QMenu* contextMenu() { return MainGraf->contextMenu();}
	void  setContextMenu(QMenu* menu) {contextMenu_ = menu;MainGraf->setContextMenu(contextMenu_);}

	
	void	GetCurrentViewport(double &start_hz, double &start_smpl, double &end_hz, double &end_smpl);
	double  GetCurrentBandwidth_hz()	{	return bandwidth_hz_;	}
	


	//выделение (еще в слотах)
	void				GetSelection(double &start_x, double &start_y,  double &end_x, double &end_y) const; //внерет первый попавшийся (по текущему типу выделения и первый в списке), мультиселект не поддерживает, в случае горизонтально или вертикального выделения оси по которым не производились измерения (выделенно вся) возвращается 0
	bool				IsSomethingSelected() const;//  возвращает true если что-либо выделено
	int					curSelectionType();// 0 - недоступна, 1 - горизонтальная ||, 2 - вертикальная =, 3 - свободная []
	void				SetCurSelectionType(int type);// 0 - недоступна, 1 - горизонтальная ||, 2 - вертикальная =, 3 - свободная []
	bool				HasRectSelection() const;
	bool				HasHorSelection() const;
	bool				HasVertSelection() const;
	void				RemoveLastSelection();
	QList<QPointF>		GetAllHorSelections() const;
	QList<QPointF>		GetAllVertSelections() const;
	QList<QRectF> 		GetAllRectSelections() const;
	void				SetMultySelectEnabled(bool enable);
	void				SetButtonCtrlClickIsSingle(bool is_single = true);//по умолчанию с зажатым Ctrl делается множественное выделение (false), с установкой флага все меняется на оборот, без Ctrl multy с Ctrl single
	//////////////////////////////////////////////////////////////////////////



	Q_MultiGraphics* getBaseEngineClass() { return MainGraf;};
		
	void	MoveScreenByPixels(QPoint pixels,bool emit_viewPortChangeSignal = true);

public slots:
	//группа программного изменения viewPorta
	void ZoomToSelection();
	void SetViewport( double start_hz, double start_smpl, double end_hz, double end_smpl);//X в "виртуальных" бубнах, Y в "реальных"
	
	//выделение
	void ClearAllSelections(bool noSignal = false);
	void SetSelection( double x1,double y1,double x2,double y2, bool noSignal = false);
	
	
	//маркеры
	void	ShowMarkers();
	void	HideMarkers();
	void	SetMoveAllMarkers();
	void	SetMoveOneMarker();
	void	SetMarkers(QVector<double>,bool,bool noSignal = false);
	void	UpdateMarker(int, double,bool);
	void	ClearAllMarkers();
	//////////////////////////////////////////////////////////////////////////

signals:
	//группа сигналов касающихся выделения 
	void SelectionCleared();//очистилось выделение
	void DeselectFinished();
	void HorizontalSelectionChanged(double start, double end);//в данный момент происходит выделение (только горизонтальное)
	void VerticalSelectionChanged(double start, double end);//в данный момент происходит выделение (только вертикальное)
	void SelectionChangedTo(double x1, double y1, double x2, double y2);//в данный момент происходит выделение (включает себя горизотальное, вертикальное и квадратное) , ось времени Y в отсчетах
	void selectionFinished(double x1, double y1, double x2, double y2);//ось времени Y в отсчетах

	void MarkerMoved(int, double,bool);//индекс маркера и ГЦ (если двигается любой маркер и к каждому)
	void MarkersChanged(QVector<double>,bool);//состояние маркеров было изменено, в любом случае, (передвинули , почистили, добавили)

	void viewPortChanged( double start_hz, double start_smpl, double end_hz, double end_smpl);//изменен ViewPort
	void dragScreenFinish();//таскание экрана завершено, эмитится если экран был перемещен хотябы на 1 пиксель, кроме того могло быть вызвано ContextMenu, если экран был перемещен на не более 2 пикселей в обеих плоскостях

	void SonogramScreenSpectrumsChanged(FFTLine_32f mean_spectrum, FFTLine max_spectrum);

	
	//сигнал по завершению рассчета палитры, несет данные о цветовых уровнях необходимые для сохранения и последующей передачи в класс
	void CalcPallateFinished(QByteArray color_level_data);

//////////////////////////////////////////////////////////////////////////
// PRIVATE
//////////////////////////////////////////////////////////////////////////
private:
	SIZE_T				maximum_ram_of_system_;
	SonogramMaterial * material_1;//поддерживает пока что только 1 материал, в дальнейшем можно сделать либо списком либо тупо 2 (по мере постановки задачи и выделения времени)
	SonogramGraphicsRenderThread* sonorenderThread;
	double bandwidth_hz_;

	Q_MultiGraphics* MainGraf;

	bool ZoomOutMaxOnDataSet;
	bool SonogramActivated;

	bool emitSignalOnProgrammChangeOffViewPort_;//отладочная, не используется пользователями в нормальном режиме всегда = false
	bool wasProgrammChangeOffViewPort_;

	void InitMainGraf();

	QSettings* settings_;//необходим для переустановки при функции Reset()
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
