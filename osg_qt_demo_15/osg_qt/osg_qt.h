#ifndef OSG_QT_H
#define OSG_QT_H

#include <QtGui/QMainWindow>
#include "ui_osg_qt.h"

class osg_qt : public QMainWindow
{
	Q_OBJECT

public:
	osg_qt(QWidget *parent = 0, Qt::WFlags flags = 0);
	~osg_qt();

private:
	Ui::osg_qtClass ui;
};

#endif // OSG_QT_H
