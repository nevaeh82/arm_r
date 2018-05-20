#ifndef GRID_GRAPHICS_ITEM_H
#define GRID_GRAPHICS_ITEM_H

#include <QGraphicsObject>

namespace gui
{

class GridGraphicsItem : public QGraphicsObject
{
	Q_OBJECT

public:
	GridGraphicsItem(QGraphicsScene *parent);
	~GridGraphicsItem();

	QRectF	boundingRect () const {return QRectF(QPoint(0,0),elSize_);}
	
	void	setGridColor(QColor color,  bool call_redraw = true);
	void	setGridSteps(int x_step,int y_step, bool call_redraw = true);//���� �������� ���������� < 1 - �� �� ������ �������. setGridSteps(0,15); - ��������� Y ������� X ��� ���������
	void	getGridSteps(int &out_x_step,int &out_y_step) const;
	void	setGridMargins(int x_offset,int y_offset,  bool call_redraw = true);//��������� ������������ ���� ���������� �� ���� �����//�� ������������
	void	setStartConer(Qt::Corner corner,  bool call_redraw = true);//����� ���� ����� ������������ ��� �������, �� ��������� TopLeft
	void	setFlag_noMoreThenScene(bool enable,  bool call_redraw = true);//���� ���������� �� �� ��� ����� �� ����� ��� ������ �����, ���� false ����� ����� �� ���� viewPort, ��-��������� true
	bool	getFlag_noMoreThenScene() const;

	void	callMeOnViewPortScroll(bool call_redraw = true);
	void	callMeOnViewPortResizeOrScale(bool call_redraw = true);
	
	void	forceReDraw();//���������������� ������� ��������������� ����������
private:
	QSize elSize_;
	QImage grid_image_;
	uint grid_color_;
	int gridStepX_;
	int gridStepY_;
	int x_offset_;
	int y_offset_;
	Qt::Corner corner_;
	uint *scanLineFill_;
	bool noMoreThenScene;
	QGraphicsView* getGraphicsView() const;
	QSize	getGraphicsViewSize() const;
	void	setGridItemSize(QSize gridSize, bool call_redraw = true, bool force_call_redraw_if_image_geom_ch = false);
	void	reCalc_scanLineFill_();
	void	ReDrawGrid();
	void	paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

private slots:
	void sceneRectChanged(QRectF rect);
};

}//namespace gui

#endif // GRID_GRAPHICS_ITEM_H
