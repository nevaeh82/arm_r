#include "StdAfx.h"
#include "push_button_item.h"

using namespace gui;

PushButtonItem::PushButtonItem( QGraphicsItem* parent, QString fileName_InPixmap,QString fileName_OutPixmap, QString fileName_PressPixmap ) : QGraphicsObject(parent)
{
	pixIn.load(fileName_InPixmap);
	pixOut.load(fileName_OutPixmap);
	pixPress.load(fileName_PressPixmap);
	leftButtonPressed = false;
	setAcceptHoverEvents(true);
	setCacheMode(QGraphicsItem::ItemCoordinateCache);
	setAcceptedMouseButtons(Qt::LeftButton);
	underMouse_ = false;
}

PushButtonItem::~PushButtonItem()
{	}

QRectF PushButtonItem::boundingRect() const
{
	QRectF retVal(0,0,pixIn.width(),pixIn.height());
	return retVal;
}

void PushButtonItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
	if (underMouse_)
	{
		if (leftButtonPressed) painter->drawPixmap(0,0,pixPress);
		else painter->drawPixmap(0,0,pixIn);
	}
	else painter->drawPixmap(0,0,pixOut);
}

void PushButtonItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	underMouse_ = true;
	update();
	
}

void PushButtonItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	underMouse_ = false;
	update();
	
}

void PushButtonItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	leftButtonPressed = true;
	update();
}

void PushButtonItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	leftButtonPressed = false;
	update();
	if ((event->pos().x() < boundingRect().width()) && (event->pos().y() < boundingRect().height())) emit button_clicked();
}

//////////////////////////////////////////////////////////////////////////