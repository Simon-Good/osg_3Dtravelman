#include "MapItem.h"


MapItem::MapItem(const QPixmap& pixmap, QGraphicsItem *parent, QGraphicsScene *scene)
	:QGraphicsPixmapItem(pixmap, parent, scene)
{
	setAcceptedMouseButtons(Qt::LeftButton);
	setAcceptHoverEvents(true);
	setZValue(-2);
}

void MapItem::hoverEnterEvent(QGraphicsSceneHoverEvent*){
	setZValue(1);
}

void MapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent*){
	setZValue(-2);
}