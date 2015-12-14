#include "WelcomePage.h"
#include <iostream>
#include<QStyleFactory>
using namespace std;
WelcomePage::WelcomePage(QWidget *parent)
	: QDialog(parent)
{
	
	QString stylesheet;
	stylesheet +=QString(
		"QWidget{background:transparent;}"
		"QProgressBar::chunk { background-color:rgb(255,143,89);width:10px;margin:0.5px}"//  rgb(8,221,229) #FF8F59  #FFDAC8 	
		"QProgressBar{text-align:right;border:transparent}"
		
		);
	pb = new QProgressBar(this);
	pb->setStyle(QStyleFactory::create("Cleanlooks"));
	pb->setFixedWidth(580);//750
	pb->setFixedHeight(13);
	pb->setStyleSheet(stylesheet);
	pb->setTextVisible(false);
	
	setFixedWidth(850);
	setFixedHeight(600);
	QPixmap pixmap = QPixmap("./background2.bmp").scaled(this->size());
	QPalette palette(this->palette());
	palette.setBrush(QPalette::Background, QBrush(pixmap));
	setPalette(palette);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	setWindowTitle("»¶Ó­");//»¶Ó­
    setWindowModality(Qt::WindowModal);
	

	pb->setGeometry(145,470,20,20);
	show();
}

WelcomePage::~WelcomePage()
{

}

void WelcomePage::setValue(int val){
	pb->setValue(val);
	if(val == 100){
		QElapsedTimer t;
		t.start();
		while(t.elapsed() < 1500)
			QCoreApplication::processEvents();
		close();
	}
}
