#include <QApplication>
#include <QTextCodec>
#include <QWaitCondition>
#include <locale.h>
#include "mainwindow.h"
#include "CameraSetting.h"
#include "WelcomePage.h"

int main(int argc, char** argv){
	QApplication app(argc, argv);
	QString sPath = app.applicationDirPath();
	sPath += QString("/plugins");
	app.addLibraryPath(sPath);
	QTextCodec *codec = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);

	setlocale(LC_ALL, ".936");
	WelcomePage* wp = new WelcomePage();
	MainWindow *mwd = new MainWindow(wp);
	wp->setValue(100);
	mwd->showMaximized();
	return app.exec();
}