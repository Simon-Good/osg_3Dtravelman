#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <osgViewer/Viewer>
class RenderThread : public QThread
{
	Q_OBJECT

public:
	RenderThread():QThread(),viewerPtr(0){}
	virtual ~RenderThread(){
		if(viewerPtr)
			viewerPtr->setDone(true);wait();
	}
	osgViewer::Viewer* viewerPtr;
private:
	virtual void run(){
		if(viewerPtr){
			while(!viewerPtr->done())
				viewerPtr->frame();
		}
	}
};

#endif // RENDERTHREAD_H
