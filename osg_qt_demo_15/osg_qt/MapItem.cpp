#include "MapItem.h"


MapItem::MapItem(const QPixmap& pixmap, QGraphicsItem *parent, QGraphicsScene *scene)
	:QGraphicsPixmapItem(pixmap, parent, scene)
{
	setAcceptedMouseButtons(Qt::LeftButton);
	setAcceptHoverEvents(true);
	
	timeLine.setDuration(150);
	timeLine.setFrameRange(0, 150);

	connect(&timeLine, SIGNAL(frameChanged(int)), this, SLOT(setFrame(int)));
	connect(&timeLine, SIGNAL(finished()), this, SLOT(updateItemPosition()));
	adjust();
}

void MapItem::hoverEnterEvent(QGraphicsSceneHoverEvent*){
	timeLine.setDirection(QTimeLine::Forward);
	if(z != 1.0){
		z = 1.0;
		updateItemPosition();
	}

	if(timeLine.state() == QTimeLine::NotRunning)
		timeLine.start();
}

void MapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent*){
	timeLine.setDirection(QTimeLine::Backward);
	if(z != 0.0)
		z = 1.0;

	if(timeLine.state() == QTimeLine::NotRunning)
		timeLine.start();
}

void MapItem::setFrame(int frame){
	adjust();
	QPointF center = boundingRect().center();
	translate(center.x(), center.y());
	scale(1+frame/1350.0, 1+frame/1350.0);
	translate(-center.x(), -center.y());
}

void MapItem::adjust(){
	QMatrix matrix;
	matrix.scale(150/boundingRect().width(), 120/boundingRect().height());
	setMatrix(matrix);
}

void MapItem::updateItemPosition(){
	setZValue(z);
}