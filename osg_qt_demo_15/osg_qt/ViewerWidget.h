#pragma once
#include <QTimer>
#include <QBoxLayout>
#include <QWidget>
#include <osgDB/Readfile>
#include <osg/MatrixTransform>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgQt/GraphicsWindowQt>
#include "TravelManipulator.h"
#include "GeneralEventHandler.h"
#include "TextPanel.h"
#include "RenderThread.h"
#include <vector>
#include <string>
#include <boost/thread/thread.hpp>
//#include "RenderThread.h"
using namespace std;
class ViewerWidget:public QWidget//, public osgViewer::CompositeViewer
{
	Q_OBJECT
public:
	ViewerWidget(QWidget* parent = 0);
	~ViewerWidget(){}

	//osg::ref_ptr<osgViewer::Viewer> generateMainView(osgQt::GraphicsWindowQt* );
	//void createTraits(osg::GraphicsContext::Traits* traits, string name, int x, int y, int H, int W);
	osg::Camera* createCamera( int x, int y, int w, int h );
	void reloadModel(int index);
	void loadModels(int size);
	void loadModleThread(int modelnum);

	CameraContext getCameraContext(int index);
	void textInfoSwt(int index, bool showTxt);

	vector<RangeNode>* getKeepOutBorder(int modelindex);
	vector<RangeNode>* getKeepInBorder(int modelindex);
	vector<map<string, string>*>* generateDBMap(int index);

	//virtual void paintEvent(QPaintEvent* event);
signals:
	void modelLoadFinished();

public:
	osg::ref_ptr<osg::Group> root;
	osg::ref_ptr<osg::Switch> swt;
	osg::Node* threadNode;
	osg::Switch* threadSwt;
	unsigned int currentIndex;
protected:
	//QTimer _timer;
	osg::ref_ptr<osgViewer::Viewer> mainView;
	osgQt::GraphicsWindowQt* qgw;
	boost::thread thread;
	vector<CameraContext> cameraContextList;
	QWidget* mparent;
	bool loadFinished;
	RenderThread rThread;
};
