#include "RenderThread.h"
RenderThread::~RenderThread(){
	if(viewerPtr)
		viewerPtr->setDone(true);wait();
}

void RenderThread::run(){
	if(viewerPtr){
		while(!viewerPtr->done()){
			viewerPtr->frame();
			if(additional == true){
				if(loadswt != NULL && rootswt != NULL && loadindex != -1){
					rootswt->insertChild(loadindex, loadswt, false);
					loadswt = NULL;
					emit loadSwtFinished();
				}
				additional = false;
			}
		}
	}
}

void RenderThread::loadNewSwt(osg::Switch* swt, int index){
	additional = true;
	loadswt = swt;
	loadindex = index;
}