#pragma once
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QBoxLayout>
#include <osg/Vec3>
#include "TravelManipulator.h"

class CameraSetting: public QDialog
{
	Q_OBJECT
public:
	CameraSetting(QWidget *parent = 0);
	~CameraSetting(void);
	QHBoxLayout* vecLayout(QString type, QLineEdit* lnEdit1, QLineEdit* lnEdit2, QLineEdit* lnEdit3);
	QHBoxLayout* stepLayout(QString step1, QString step2, QLineEdit* lnEdit1, QLineEdit* lnEdit2);
	void showWithContext();
	void showWithContext(CameraContext* cc);
public slots:
	void updateContext();
protected:
	CameraContext m_cc;
	QLineEdit* xpos_lnEdt;
	QLineEdit* ypos_lnEdt;
	QLineEdit* zpos_lnEdt;
	QLineEdit* xrot_lnEdt;
	QLineEdit* yrot_lnEdt;
	QLineEdit* zrot_lnEdt;
	QLineEdit* transSpeed_lnEdt;
	QLineEdit* rotSpeed_lnEdt;
	QPushButton* ok_btn;
	QPushButton* cancle_btn;
	CameraContext* cc;
};

