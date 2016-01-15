#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QDialog>
#include <QProgressBar>
#include <QBoxLayout>
#include <QLabel>
#include <QPalette>
#include <QGridLayout>
#include <QElapsedTimer>
#include <QCoreApplication>
#include "Global.h"
class WelcomePage : public QDialog
{
	Q_OBJECT

public:
	WelcomePage(QWidget *parent = 0);
	~WelcomePage();
	void setValue(int val);
private:
	QProgressBar* pb;
};

#endif // PROGRESSWIDGET_H
