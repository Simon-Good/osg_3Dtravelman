#include "CameraSetting.h"
#include "ViewerWidget.h"

CameraSetting::CameraSetting(QWidget *parent):QDialog(parent)
{
	setWindowTitle(tr("视点参数设置"));
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	QVBoxLayout* main_layout = new QVBoxLayout;
	xpos_lnEdt = new QLineEdit;
	ypos_lnEdt = new QLineEdit;
	zpos_lnEdt = new QLineEdit;
	xrot_lnEdt = new QLineEdit;
	yrot_lnEdt = new QLineEdit;
	zrot_lnEdt = new QLineEdit;
	transSpeed_lnEdt = new QLineEdit;
	rotSpeed_lnEdt = new QLineEdit;
	main_layout->addLayout(stepLayout(tr("移动步长："),tr("旋转步长："), transSpeed_lnEdt, rotSpeed_lnEdt));
	main_layout->addLayout(vecLayout(tr("位置设置："), xpos_lnEdt, ypos_lnEdt, zpos_lnEdt));
	main_layout->addLayout(vecLayout(tr("旋转设置："), xrot_lnEdt, yrot_lnEdt, zrot_lnEdt));
	QHBoxLayout* btn_layout = new QHBoxLayout;
	btn_layout->addStretch();
	ok_btn = new QPushButton(tr("确定"));
	cancle_btn = new QPushButton(tr("取消"));
	btn_layout->addWidget(ok_btn);
	btn_layout->addWidget(cancle_btn);
	main_layout->addLayout(btn_layout);
	setLayout(main_layout);
	connect(cancle_btn, SIGNAL(clicked()), this, SLOT(close()));
	connect(ok_btn, SIGNAL(clicked()), this, SLOT(updateContext()));
}


CameraSetting::~CameraSetting(void)
{
}

QHBoxLayout* CameraSetting::vecLayout(QString type, QLineEdit* lnEdt1, QLineEdit* lnEdit2, QLineEdit* lnEdit3){
	QHBoxLayout* type_layout = new QHBoxLayout;
	QLabel* type_lbl = new QLabel(type);
	QLabel* xtype_lbl = new QLabel(tr("x:"));
	QLabel* ytype_lbl = new QLabel(tr("y:"));
	QLabel* ztype_lbl = new QLabel(tr("z:"));

	type_layout->addWidget(type_lbl);
	type_layout->addStretch(2);
	type_layout->addWidget(xtype_lbl);
	type_layout->addWidget(lnEdt1);
	type_layout->addStretch(2);
	type_layout->addWidget(ytype_lbl);
	type_layout->addWidget(lnEdit2);
	type_layout->addStretch(2);
	type_layout->addWidget(ztype_lbl);
	type_layout->addWidget(lnEdit3);
	type_layout->addStretch(2);
	return type_layout;
}

QHBoxLayout* CameraSetting::stepLayout(QString step1, QString step2, QLineEdit* lnEdt1, QLineEdit* lnEdt2){
	QHBoxLayout* step_layout = new QHBoxLayout;
	QLabel* step_lbl1 = new QLabel(step1);
	step_layout->addWidget(step_lbl1);
	step_layout->addWidget(lnEdt1);

	QLabel* step_lbl2 = new QLabel(step2);
	step_layout->addWidget(step_lbl2);
	step_layout->addWidget(lnEdt2);
	return step_layout;
}

void CameraSetting::showWithContext(){
	//CameraContext cc = TravelManipulator::Instance()->getCameraContext();
	cc = TravelManipulator::Instance()->getCameraContext();
	showWithContext(cc);
}

void CameraSetting::updateContext(){
	//CameraContext cc;
	cc->m_fAngle = rotSpeed_lnEdt->text().toFloat();
	cc->m_fMoveSpeed = transSpeed_lnEdt->text().toFloat();
	cc->m_vPosition = osg::Vec3(xpos_lnEdt->text().toFloat(),
							   ypos_lnEdt->text().toFloat(),
							   zpos_lnEdt->text().toFloat());
	cc->m_vRotation = osg::Vec3(xrot_lnEdt->text().toFloat(),
							   yrot_lnEdt->text().toFloat(),
							   zrot_lnEdt->text().toFloat());
	TravelManipulator::Instance()->setCameraContext(cc);
	close();
}

void CameraSetting::showWithContext(CameraContext* cc){
	QString text;
	transSpeed_lnEdt->setText(text.setNum(cc->m_fMoveSpeed));
	rotSpeed_lnEdt->setText(text.setNum(cc->m_fAngle));
	xpos_lnEdt->setText(text.setNum(cc->m_vPosition.x()));
	ypos_lnEdt->setText(text.setNum(cc->m_vPosition.y()));
	zpos_lnEdt->setText(text.setNum(cc->m_vPosition.z()));
	xrot_lnEdt->setText(text.setNum(cc->m_vRotation.x()));
	yrot_lnEdt->setText(text.setNum(cc->m_vRotation.y()));
	zrot_lnEdt->setText(text.setNum(cc->m_vRotation.z()));
	show();
}
