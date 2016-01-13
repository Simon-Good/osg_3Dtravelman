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
#include "Global.h"

using namespace std;
class GeneralEventHandler :public QObject, public osgGA::GUIEventHandler
{
	Q_OBJECT
public:
	static GeneralEventHandler* Instance();
	void infoEnable(bool enable);
	void setCurrentScene(osg::Switch*, int);
	void setDBMap(vector<map<string, string, MyCompRule>*>* dbmap);
protected:
	GeneralEventHandler():curSwtIndex(0),frameCount(0){curSwt = NULL;dbMap = NULL;}
	~GeneralEventHandler(){}
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	
signals:
	void selectedPosition(float x, float y, float radius);
	void resetDoshow();
private:
	static GeneralEventHandler* instance;
	static bool infoenable;
	osg::Switch* curSwt;
	vector<map<string, string, MyCompRule>*>* dbMap;
	int curSwtIndex;
	unsigned int frameCount;
	char* timestr;
	time_t rawtime;
};
