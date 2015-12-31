#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <osgViewer/Viewer>
#include <iostream>

using namespace std;
class RenderThread : public QThread
{
	Q_OBJECT

public:
	RenderThread( QObject * parent = 0):QThread(parent),
		viewerPtr(0), additional(false),loadswt(NULL), rootswt(NULL),loadindex(-1){}
	virtual ~RenderThread();
	osgViewer::Viewer* viewerPtr;
	osg::Switch* loadswt;
	osg::Switch* rootswt;
	bool additional;
	int loadindex;
public slots:
	void loadNewSwt(osg::Switch* swt, int index);

signals:
	void loadSwtFinished();
private:
	virtual void run();
};

#endif // RENDERTHREAD_H
