#pragma once
#include <QtCore>
#include <QtGui/QGraphicsPixmapItem>
#include <osg/Vec3>
class MapItem: public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	MapItem(const QPixmap &pixmap, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);
	void adjust();
	~MapItem(void){}

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
private slots:
	void setFrame(int Frame);
	void updateItemPosition();
private:
	QTimeLine timeLine;
	double z;
};

