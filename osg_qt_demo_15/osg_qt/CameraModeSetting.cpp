#include "CameraModeSetting.h"

CameraModeSetting::CameraModeSetting(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("�ӵ�ģʽ����"));
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	this->options = new QGroupBox;
	this->fly_rdbtn = new QRadioButton(tr("���ģʽ"));
	this->lowslide_rdbtn = new QRadioButton(tr("����ģʽ"));
	lowslide_rdbtn->setChecked(true);
	QVBoxLayout* option_layout = new QVBoxLayout;
	option_layout->addWidget(lowslide_rdbtn);
	option_layout->addWidget(fly_rdbtn);
	options->setLayout(option_layout);

	QVBoxLayout* button_layout = new QVBoxLayout;
	QPushButton* ok_btn = new QPushButton(tr("ȷ��"));
	QPushButton* cancel_btn = new QPushButton(tr("ȡ��"));
	connect(cancel_btn, SIGNAL(clicked()), this, SLOT(close()));
	connect(ok_btn, SIGNAL(clicked()), this, SLOT(setCameraMode()));
	button_layout->addWidget(cancel_btn);
	button_layout->addWidget(ok_btn);

	QHBoxLayout* main_layout = new QHBoxLayout;
	main_layout->addWidget(options);
	main_layout->addLayout(button_layout);
	setLayout(main_layout);
}

CameraModeSetting::~CameraModeSetting()
{

}

void CameraModeSetting::setCameraMode(){

	close();
}
