#pragma once
#include <osgViewer/Viewer>
#include <osgManipulator/Dragger>
#include <osg/Node>
#include <osgDB/ReadFile>
#include <osg/Group>
#include <osgGA/GUIEventAdapter>
#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <string>
#include <time.h>
#include <iostream>
#include "TextPanel.h"

using namespace std;

class GeneralEventHandler :public QObject, public osgGA::GUIEventHandler
{
	Q_OBJECT
public:
	static GeneralEventHandler* Instance(QWidget* parent = 0);
	void infoEnable(bool enable);
	void setCurrentScene(osg::Switch*, int);
protected:
	GeneralEventHandler(){curSwt = NULL;}
	~GeneralEventHandler(){}
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	
signals:
	void selectedPosition(osg::Vec3 pos, float radius);
	void resetDoshow();
private:
	static GeneralEventHandler* instance;
	static QWidget* mparent;
	static bool infoenable;
	osg::Switch* curSwt;
	int curSwtIndex;
	char* timestr;
	time_t rawtime;
};
