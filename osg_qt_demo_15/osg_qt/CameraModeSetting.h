#ifndef CAMERAMODESETTING_H
#define CAMERAMODESETTING_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <osg/Vec3>
#include "TravelManipulator.h"

class CameraModeSetting : public QDialog
{
	Q_OBJECT

public:
	CameraModeSetting(QWidget *parent = 0);
	~CameraModeSetting();
public slots:
	void setCameraMode();
private:
	QGroupBox* options;
	QRadioButton* fly_rdbtn;
	QRadioButton* lowslide_rdbtn;
};

#endif // CAMERAMODESETTING_H
