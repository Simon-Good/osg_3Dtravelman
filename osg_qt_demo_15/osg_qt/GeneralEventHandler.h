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
#include <map>
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
	void setDBMap(vector<map<string, string>*>* dbmap);
	void setTempData(vector<map<string, string>*>* dbmap);
protected:
	GeneralEventHandler():curSwtIndex(0),frameCount(0){curSwt = NULL;dbMap = NULL;}
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
	vector<map<string, string>*>* dbMap;
	int curSwtIndex;
	unsigned int frameCount;
	char* timestr;
	time_t rawtime;
};
