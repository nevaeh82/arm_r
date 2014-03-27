#include "stdafx.h"
#include "grid_graphics_item.h"
#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QScrollBar>
#include <tbb\tbb.h>

namespace gui
{
//#define ENABLE_TIMERS_OUTPUT

GridGraphicsItem::GridGraphicsItem(QGraphicsScene *parent): QGraphicsObject(nullptr)
{
	setAcceptedMouseButtons(Qt::NoButton);
	setFlag(QGraphicsItem::ItemIsFocusable,false);
	setFlag(QGraphicsItem::ItemIsMovable,false);
	setFlag(QGraphicsItem::ItemIsSelectable,false);
	setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	grid_color_ = QColor(0,0,0).rgba();
	gridStepX_ = 10;
	gridStepY_ = 10;
	elSize_ = QSize(100,100);
	grid_image_ = QImage(elSize_,QImage::Format_RGB32);
	scanLineFill_ = nullptr;
	reCalc_scanLineFill_();
	connect(parent,SIGNAL(sceneRectChanged(QRectF)),this,SLOT(sceneRectChanged(QRectF)));
	parent->addItem(this);
	noMoreThenScene = true;
	corner_ = Qt::TopLeftCorner;
	x_offset_ = 0;
	y_offset_ = 0;
}

GridGraphicsItem::~GridGraphicsItem()
{
	delete[] scanLineFill_;
}

void GridGraphicsItem::setGridItemSize(QSize gridSize, bool call_redraw /*= true*/, bool force_call_redraw_if_image_geom_ch /*= false*/)
{
	//проверка на то чтобы слой не был больше чем сцена
	if (noMoreThenScene)
	{
		if (scene() != nullptr)
		{
			QGraphicsView* gv = getGraphicsView();
			if (gv == nullptr) return;
			QPoint pixelSz = gv->mapFromScene(scene()->sceneRect().width(),scene()->sceneRect().height());
			if (pixelSz.y() > 0)
				if (gridSize.height() > pixelSz.y()) gridSize.setHeight(pixelSz.y());	
			if (pixelSz.x() > 0)
				if (gridSize.width()  > pixelSz.x()) gridSize.setWidth(pixelSz.x());
		}
	}
	
	if (elSize_ != gridSize)
	{
		elSize_ = gridSize;
		grid_image_ = QImage(elSize_,QImage::Format_RGB32);
		reCalc_scanLineFill_();
		if (force_call_redraw_if_image_geom_ch ) call_redraw = true;
	}
	if (call_redraw) ReDrawGrid();
}

void GridGraphicsItem::setGridColor( QColor color,  bool call_redraw /*= true*/ )
{
	if (grid_color_ == color.rgba()) return;
	grid_color_ = color.rgba();
	reCalc_scanLineFill_();
	if (call_redraw) ReDrawGrid();
}

void GridGraphicsItem::setGridSteps( int x_step,int y_step, bool call_redraw /*= true */)
{
	if (((gridStepX_ == x_step) && (gridStepY_ == y_step)) || ((x_step <= 0) && (y_step <= 0))) return;
	if (x_step > 0) gridStepX_ = x_step;
	if (y_step > 0) gridStepY_ = y_step;
	if (call_redraw) ReDrawGrid();
}

void GridGraphicsItem::getGridSteps(int &x_step,int &y_step) const
{
	x_step = gridStepX_;
	y_step = gridStepY_;
}

void GridGraphicsItem::ReDrawGrid()
{
	if (!isVisible()) return;

	#ifdef ENABLE_TIMERS_OUTPUT
		tbb::tick_count t0 = tbb::tick_count::now();
	#endif
	
	QGraphicsView* gv = getGraphicsView();
	if (gv == nullptr) return;
	QPoint pixelsFromStart;
	
	if (corner_ == Qt::TopLeftCorner)
		pixelsFromStart = gv->mapFromScene(0,0);
	if (corner_ == Qt::TopRightCorner)
		pixelsFromStart = gv->mapFromScene(scene()->width(),0);
	if (corner_ == Qt::BottomLeftCorner)
		pixelsFromStart = gv->mapFromScene(0,scene()->height());
	if (corner_ == Qt::BottomRightCorner)
		pixelsFromStart = gv->mapFromScene(scene()->width(),scene()->height());

	int pixel_offset_x = qAbs(pixelsFromStart.x() % gridStepX_);
	int pixel_offset_y = qAbs(pixelsFromStart.y() % gridStepY_);
	Q_ASSERT_X(pixel_offset_x < gridStepX_,"ReDrawGrid","parametrs error pixel_offset_x < gridStepX_");
	Q_ASSERT_X(pixel_offset_y < gridStepY_,"ReDrawGrid","parametrs error pixel_offset_y < gridStepY_");

	//qDebug() << "pixel_offset_x " << pixel_offset_x;
	int massSize = grid_image_.bytesPerLine()/4;
	if (massSize <= 0) 
	{	
		Q_ASSERT_X(false,"ReDrawGrid","Grid not inited. Call setGridSize first.");
		return;
	}
	uint *scanLineEmpty = new uint[massSize];
	memset(scanLineEmpty,0,grid_image_.bytesPerLine());
	
	if ((corner_ == Qt::TopLeftCorner) || (corner_ == Qt::BottomLeftCorner))
	{
		for (int i = 1; i <= massSize/gridStepX_;i++)
			scanLineEmpty[(i*gridStepX_-1)-pixel_offset_x] = grid_color_;
	}
	else //((corner_ == Qt::TopRightCorner) || (corner_ == Qt::BottomRightCorner))
	{
		for (int i = (massSize/gridStepX_); i >= 0;i--)
		{
			int massIndex = (i*gridStepX_)+pixel_offset_x;
			if (massIndex >= massSize) continue;
			scanLineEmpty[massIndex] = grid_color_;
		}
	}
	
	
	tbb::parallel_for( tbb::blocked_range<int>(0,elSize_.height()), [=](const tbb::blocked_range<int>& r)
	{
		for(int i = r.begin();i < r.end();i++)
		{
			if ((corner_ == Qt::TopLeftCorner) || (corner_ == Qt::TopRightCorner))
			{
				if ( (i+1+pixel_offset_y)%gridStepY_ == 0 )
				{
					memcpy(grid_image_.scanLine(i),scanLineFill_,grid_image_.bytesPerLine());
					continue;
				}
			}
			else//((corner_ == Qt::BottomLeftCorner) || (corner_ == Qt::BottomRightCorner))
			{
				if ( (i-pixel_offset_y)%gridStepY_ == 0 )
				{
					memcpy(grid_image_.scanLine(i),scanLineFill_,grid_image_.bytesPerLine());
					continue;
				}
			}	
			memcpy(grid_image_.scanLine(i),scanLineEmpty,grid_image_.bytesPerLine());
		}
	});

	delete[] scanLineEmpty;
	
	#ifdef ENABLE_TIMERS_OUTPUT
		tbb::tick_count t1 = tbb::tick_count::now();
		qDebug() << "Grid prepare time=" << (t1-t0).seconds()*1000 << " msecs";
	#endif

	update();
}

void GridGraphicsItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
	#ifdef ENABLE_TIMERS_OUTPUT
		tbb::tick_count t0 = tbb::tick_count::now();
	#endif
	
	painter->drawImage(0,0,grid_image_);
	
	#ifdef ENABLE_TIMERS_OUTPUT
		tbb::tick_count t1 = tbb::tick_count::now();
		qDebug() << "Grid draw time=" << (t1-t0).seconds()*1000 << " msecs";

		//отладочный бордюр сетки
		QRectF rr = boundingRect();
		rr.setWidth(rr.width()-1);rr.setHeight(rr.height()-1);
		painter->setPen(QColor(0,0,255));
		painter->drawRect(rr);
		rr.setX(1);rr.setY(1);
		rr.setWidth(rr.width()-1);rr.setHeight(rr.height()-1);
		painter->setPen(QColor(255,255,0));
		painter->drawRect(rr);
		rr.setX(2);rr.setY(2);
		rr.setWidth(rr.width()-1);rr.setHeight(rr.height()-1);
		painter->setPen(QColor(0,0,255));
		painter->drawRect(rr);//*/
	#endif
}

void GridGraphicsItem::setGridMargins( int x_offset,int y_offset,  bool call_redraw /*= true*/ )
{
	if ((x_offset == x_offset_) && (y_offset == y_offset_)) return;
	x_offset_ = x_offset;
	y_offset_ = y_offset;
	if (call_redraw) ReDrawGrid();
}

void GridGraphicsItem::reCalc_scanLineFill_()
{
	int massSize = grid_image_.bytesPerLine()/4;
	delete[] scanLineFill_;
	scanLineFill_ = new uint [massSize];
	for (int i = 0; i < massSize;i++) scanLineFill_[i] = grid_color_;
}

QGraphicsView* GridGraphicsItem::getGraphicsView() const
{
	if (scene() == nullptr) return nullptr;
	if (scene()->views().count() == 0) return nullptr;
	return scene()->views().at(0);
}

void GridGraphicsItem::callMeOnViewPortScroll(bool call_redraw /*= true*/)
{
	QGraphicsView* gv = getGraphicsView();
	if (gv == nullptr) return;
	QPointF pp = gv->mapToScene(0,0);
	setPos(pp);
	if (call_redraw) ReDrawGrid();
}

void GridGraphicsItem::callMeOnViewPortResizeOrScale(bool call_redraw /*= true*/)
{
	QGraphicsView* gv = getGraphicsView();
	if (gv == nullptr) return;
	QPointF pp = gv->mapToScene(0,0);
	setPos(pp);
	setGridItemSize(getGraphicsViewSize(), call_redraw);
}

void GridGraphicsItem::sceneRectChanged(QRectF)
{
	if (!noMoreThenScene) return;
	setGridItemSize(getGraphicsViewSize(),false,true);
}

void GridGraphicsItem::setStartConer( Qt::Corner corner,  bool call_redraw /*= true*/ )
{
	if (corner_ == corner) return;
	corner_ = corner;
	if (call_redraw) ReDrawGrid();
}

void GridGraphicsItem::setFlag_noMoreThenScene( bool enable, bool call_redraw /*= true*/ )
{
	if (noMoreThenScene == enable) return;
	noMoreThenScene = enable;
	callMeOnViewPortResizeOrScale(call_redraw);
}

bool GridGraphicsItem::getFlag_noMoreThenScene() const
{
	return noMoreThenScene;
}

void GridGraphicsItem::forceReDraw()
{
	ReDrawGrid();
}

QSize GridGraphicsItem::getGraphicsViewSize() const
{
	QSize retVal(0,0);
	QGraphicsView* gv = getGraphicsView();
	if (gv == nullptr) return retVal;
	QSize fullSize = gv->size();
	retVal = fullSize;
	//if (gv->verticalScrollBar()->isVisible()) retVal = fullSize - QSize(gv->verticalScrollBar()->width(),0);
	//if (gv->horizontalScrollBar()->isVisible()) retVal = retVal - QSize(0,gv->horizontalScrollBar()->height());
	return retVal;
}

#undef ENABLE_TIMERS_OUTPUT

}//namespace gui