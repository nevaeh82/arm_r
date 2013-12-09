#ifndef PUSH_BUTTON_ITEM_H
#define PUSH_BUTTON_ITEM_H

#include <QGraphicsObject>

namespace gui
{

class PushButtonItem: public QGraphicsObject
{
	Q_OBJECT
public:
	PushButtonItem(QGraphicsItem* parent, QString fileName_InPixmap,QString fileName_OutPixmap,QString fileName_PressPixmap);
	virtual ~PushButtonItem();
private:
	QRectF			boundingRect() const;
	void			paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	QPixmap			pixOut;
	QPixmap			pixIn;
	QPixmap			pixPress;
	bool			leftButtonPressed;

	virtual void	hoverEnterEvent		( QGraphicsSceneHoverEvent * event );
	virtual void	hoverLeaveEvent		( QGraphicsSceneHoverEvent * event );

	virtual void	mousePressEvent		( QGraphicsSceneMouseEvent * event );
	virtual void	mouseReleaseEvent	( QGraphicsSceneMouseEvent * event );

	bool underMouse_;

signals:
	void button_clicked();
};

}//namespace gui
#endif // PUSH_BUTTON_ITEM_H
